#include <format>
#include <iostream>
#include <regex>
#include <string>

#include "shell.hpp"

Shell::Shell(__attribute_maybe_unused__ int ac,
             __attribute_maybe_unused__ char **argv) {
  try {
    _cookingTime = std::stod(argv[1]);
    _numberOfCooks = std::stoi(argv[2]);
    _timeToRefillMs = std::stoi(argv[3]);
    std::cout << std::format("- Time (double) : {}\n", _cookingTime);
    std::cout << std::format("- Number of Cooks (int)    : {}\n",
                             _numberOfCooks);
    std::cout << std::format("- Time to refill the kitchens (int)    : {}\n",
                             _timeToRefillMs);
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
}

void Shell::run() {
  std::string line;

  while (std::getline(std::cin, line)) {
    std::cout << line << std::endl;
  }
}