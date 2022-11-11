#include <vector>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

class Example
{
private:
  uint64_t first;
  uint64_t second;

public:
  Example(uint64_t one, uint64_t two) : first(one), second(two) {}
  ~Example() {}

  friend std::ostream &operator<<(std::ostream &o, const Example &e)
  {
    return o << "{" << e.first << ", " << e.second << "} ";
  }
};

int main()
{
  std::vector<Example> someNumbers = {{1, 2}, {3, 4}, {5, 6}};
  std::cout<<&someNumbers<<std::endl;
  
  while (true)
  {
    std::cout << "{ ";
    std::for_each(someNumbers.begin(), someNumbers.end(), [](Example &e)
                  { std::cout << e; });

    std::cout << "}" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}