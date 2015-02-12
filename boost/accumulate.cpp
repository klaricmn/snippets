#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

int main(void)
{
  std::vector<double> v;
  v.push_back(0.2);
  v.push_back(0.3);
  v.push_back(0.4);

  double total1 = std::accumulate(v.begin(), v.end(), 0);

  double total2 = std::accumulate(v.begin(), v.end(), 0.0);

  std::cout << "total1 = " << total1 << std::endl;
  std::cout << "total2 = " << total2 << std::endl;
  
  return 0;
}
