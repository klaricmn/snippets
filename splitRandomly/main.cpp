#include <cmath>
#include <time.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <sstream>

using namespace std;

#define MAX_LINE_LEN 10240
#define MAX_FILENAME_SIZE 256

///
/// Return a random number in the range [lb,ub]
///
int rand_n(int lb, int ub) {
  return (int) ( ((float)ub)*rand() / (RAND_MAX+1.0) );
}

///
/// Return a string for the file name based on the current file
/// number being processed
///
string getFilename(char* prefix, int currentNum, int numCharInNameCtr) {
  
  // variable to hold the variable part of the string
  string var = "";
 
  // loop through each position of the variable part of the string
  for(int i=0; i<numCharInNameCtr; ++i) {

    // get the character
    stringstream ss;
    ss << (char)('a'+(currentNum%26));

    // store the character on the front
    var = ss.str() + var;

    // chop of the num with a divide by 26
    currentNum /= 26;
  }

  // prepend the prefix on the front of the string
  stringstream tmp;
  tmp << prefix << var;
   return tmp.str();  
}

int main(int argc, char** argv) {

  if(argc < 4) {
    cerr << "USAGE: " << argv[0] << " <filename> <prefix> <num splits>" << endl;
    return -1;
  }

  

  //
  // setup some variables
  //
  char* lineBuffer = new char[MAX_LINE_LEN];
  vector<string> lines;
  int numSplits = atoi(argv[3]);
  char *prefix = argv[2];
  string splitFilename;
  FILE* splitFp;
  srand(time(NULL));

  //
  // calc the number of characters needed in the filename
  //
  if(numSplits <= 1) {
    cout << "ERROR: The number of splits must be greater than 1" << endl;
    return -2;
  }
  const int numCharInNameCtr = (int) ceil( log(numSplits) / log(26) );
  cout << "Info: Need " << numCharInNameCtr << " to represent " << numSplits << " files"  << endl;

  //
  // read the data
  //
  FILE *fp = fopen(argv[1], "r");
  int lineNum=0;
  while(NULL != fgets(lineBuffer, MAX_LINE_LEN, fp)) {
    if(strlen(lineBuffer) == (MAX_LINE_LEN-1)) {
      cerr << "ERROR: Line " << lineNum 
	   << " is too long.  Exiting...." << endl;
      return -3;
    }
    lines.push_back(string(lineBuffer));
    lineNum++;
  }
  fclose(fp);

  
  //
  // calc the number in each split
  //
  const int numInSplit = (int) ceil(lines.size() / (float)numSplits); 
  cout << "DEBUG: Num in each split: " << numInSplit << endl;
  
  int pos;
  for(int file=0; file<numSplits; file++) {
    splitFilename = getFilename(prefix, file, numCharInNameCtr);
    cout << "DEBUG: File " << file << " : " << splitFilename << endl;
    splitFp = fopen(splitFilename.c_str(), "w");
    if(splitFp == NULL) {
      cerr << "ERROR: Failed to open file " << splitFilename << endl;
    }


    for(int line=0; line<numInSplit; line++) {
      
      // if we've reached the end, quit
      if(lines.size() == 0) 
	break;
      
      // get a position, do something, then remove the line
      pos = rand_n(0, lines.size()-1);
      fwrite(lines[pos].c_str(), sizeof(char), lines[pos].size(), splitFp);
      lines.erase(lines.begin() + pos);
    }

    fclose(splitFp);
  }

  return 0;
}
