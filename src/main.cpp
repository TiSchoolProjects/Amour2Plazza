
#include "Plazza.hpp"
#include "shell.hpp"

int main(int ac, char **argv) {
  if (ac != 4) {
    std::cerr << "./plazza [cooking time] [number of cooks] [time to refill]"
              << std::endl;
    return 84;
  }
  Shell shell(ac, argv);
  shell.run();
  std::cout << "Nothing done yet" << std::endl;
  return 0;
}
