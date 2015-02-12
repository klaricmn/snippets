#include <iostream>

#include<boost/tokenizer.hpp>
#include<string>

int main(){
  using namespace std;
  using namespace boost;
  string s = "Low,1,2,3,5,7,11";
  tokenizer<escaped_list_separator<char> > tok(s);


  std::cout << (*tok.begin()) << std::endl;

  std::vector<std::string> vec(++tok.begin(), tok.end());

  for(size_t i=0; i<vec.size(); ++i)
    std::cout << vec[i] << std::endl;


}
