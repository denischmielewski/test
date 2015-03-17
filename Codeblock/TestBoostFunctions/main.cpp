#include <boost/function.hpp>
#include <iostream>

struct world
{
  int hello(std::ostream &os)
  {
    os << "Hello, world!" << std::endl;
    return 1;
  }
};

int main()
{
  boost::function<int (world*, std::ostream&)> f = &world::hello;
  world w;
  f(&w, boost::ref(std::cout));
}
