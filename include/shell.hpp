#pragma once

#include <unordered_map>
#include <algorithm>

#include "EnumPlazza.hpp"
#include <string>
#include "Reception.hpp"

class Shell {
private:
  double _cookingTime = 0.f;
  size_t _numberOfCooks = 0;
  size_t _timeToRefillMs = 0;
  std::unique_ptr<Reception> _reception;
  const std::unordered_map<std::string, PizzaType> _pizzaTypeMap;
  const std::unordered_map<std::string, PizzaSize> _pizzaSizeMap;

public:
  Shell(int ac, char **argv);
  void run(void);
};