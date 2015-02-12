#include <boost/algorithm/string.hpp>
#include <iostream>
#include <vector>

int main(void)
{

  std::string dttm("2004-03-14 15:15:19");
  std::vector<std::string> slist; 

  boost::algorithm::iter_split(slist, dttm, boost::algorithm::nth_finder(":", 1));

  for(size_t index=0; index < slist.size(); ++index)
    std::cout << slist[index] << std::endl;

  return 0;
}
