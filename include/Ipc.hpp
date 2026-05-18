#pragma once

#include "EnumPlazza.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>
#define NEWPIZZA 1000
#define KITCHENRESP 2000
#define STATUSMSG 3000

struct ipcMsg {
  long _msgType;
  char _msg[100];
};

class Ipc {
private:
  int _msgid;
  key_t _key;
  bool _reception;

public:
  int send(std::string msg, long type);
  int sendPizza(PizzaType type, PizzaSize size, int kithenId);
  std::string receive(long type);
  std::pair<PizzaType, PizzaSize> receivePizza(int kithenId);
  Ipc(bool reception);
  ~Ipc();
};
