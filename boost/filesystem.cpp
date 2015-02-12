#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

//#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char** argv)
{
  const mode_t log_mode = ~(S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) & S_IRWXO;
  umask(log_mode);

  boost::filesystem::path p("foobar");
  boost::filesystem::create_directory(p);


  return 0;
}
