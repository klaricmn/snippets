#include <iostream>

#include <cgi/core/exceptions/exceptions.hpp>
#include <cgi/virtual/VirtualOverlayCreator.hpp>

#include <ogr_feature.h>
#include <ogr_api.h>
#include <ogrsf_frmts.h>

#include <gdal_alg.h>

void producePixelMappedVRT(const char* inputFilename, const char* outputFilename)
{
    cgi::VirtualOverlayCreator voc(inputFilename);
    std::map<unsigned char, unsigned char> lut;
    lut[0] = 2;
    cgi::ComplexSourceOptions<unsigned char, unsigned char> cso;
    cso.addLookupTable(lut);
    voc.addComplexSourcedOverlay(inputFilename, cso);
    voc.writeToFile(outputFilename);
}

std::tr1::shared_ptr<OGRDataSource> createTmpOgrDataSource()
{
    OGRRegisterAll();
    OGRSFDriver *poDriver =  OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName( "ESRI Shapefile" );
    CGI_THROW_IF(NULL == poDriver, std::runtime_error);

    OGRDataSource *poDS = poDriver->CreateDataSource( "out.shp", NULL );
    CGI_THROW_IF(NULL == poDS, std::runtime_error);

    return std::tr1::shared_ptr<OGRDataSource>( poDS, OGRDataSource::DestroyDataSource );
}

OGRLayer* createPolygonLayer(std::tr1::shared_ptr<OGRDataSource>& ds, const std::string& name)
{

    OGRLayer *poLayer;
    poLayer = ds->CreateLayer( name.c_str(), NULL, wkbPolygon, NULL );

    OGRFieldDefn oField( "Name", OFTString );
    oField.SetWidth(32);
    poLayer->CreateField( &oField );
    OGRFieldDefn oFieldV( "Value", OFTInteger );
    poLayer->CreateField( &oFieldV );

    return poLayer;
}

int main(int argc, char** argv)
{
    if(3 != argc)
    {
        std::cerr << "ERROR: Usage: " << argv[0] << " <input filename> <output file>" << std::endl;
        return 1;
    }

    producePixelMappedVRT(argv[1], argv[2]);


    std::tr1::shared_ptr<GDALDataset> dataset = std::tr1::shared_ptr<GDALDataset>(static_cast<GDALDataset*>(GDALOpen(argv[2], GA_ReadOnly)), GDALClose);
    CGI_THROW_IF(NULL==dataset.get(), std::runtime_error);

    GDALRasterBand* band = dataset->GetRasterBand(1);
    CGI_THROW_IF(NULL==band, std::runtime_error);

    GDALRasterBand* mask = NULL;
    if (band->GetMaskFlags() & GMF_PER_DATASET)
        mask = band->GetMaskBand();

    std::tr1::shared_ptr<OGRDataSource> poDS = createTmpOgrDataSource();

    OGRLayer *olayer = createPolygonLayer(poDS, "newlayer");

    // NOTE: Not a typo.  The option name really does start with 8
    char **papszOptions = NULL;
    papszOptions = CSLAddString(papszOptions, "8CONNECTED=8");

    const int valueField = 1; // NOTE: Counts from right to left using zero-based indexing, but be mindful that drivers like 'ESRI Shapefile', but the coords at the end.
    const CPLErr err = GDALPolygonize(band, mask, olayer, valueField, papszOptions, NULL, NULL);
    CGI_THROW_IF(CE_None != err, std::runtime_error);

    return 0;
}
