#pragma once

class Shell {
private:
  double _cookingTime = 0.f;
  size_t _numberOfCooks = 0;
  size_t _timeToRefillMs = 0;

public:
  Shell(int ac, char **argv);
  void run(void);
};