#include "Ipc.hpp"
#include <sstream>
#include <string>

Ipc::Ipc(bool reception) : _reception(reception) {
  _key = ftok("/tmp", 67);
  if (_key == -1)
    throw std::runtime_error("ftok failed");
  _msgid = msgget(_key, 0666 | IPC_CREAT);
  if (_msgid == -1)
    throw std::runtime_error("msgget failed");
}

int Ipc::send(std::string msg, long type) {
  ipcMsg to_send;

  to_send._msgType = type;
  msg.copy(to_send._msg, sizeof(to_send._msg) - 1);
  to_send._msg[std::min(sizeof(to_send._msg) - 1, msg.size())] = '\0';

  std::cout << "IPC: Sending -> " << to_send._msg << std::endl;
  if (msgsnd(_msgid, &to_send, sizeof(to_send._msg), 0) == -1)
    throw std::runtime_error("msgsnd failed");
  return 0;
}

std::string Ipc::receive(long type) {
  ipcMsg received;
  if (msgrcv(_msgid, &received, sizeof(received._msg), type, 0) == -1)
    throw std::runtime_error("msgrcv failed");
  std::cout << "IPC: getting -> " << received._msg << std::endl;
  return received._msg;
}

int Ipc::sendPizza(PizzaType type, PizzaSize size, int kitchenId) {
  std::string msg = std::to_string(type) + " " + std::to_string(size);
  return send(msg, NEWPIZZA + kitchenId);
}

std::pair<PizzaType, PizzaSize> Ipc::receivePizza(int kitchenId) {
  int type;
  int size;
  std::string msg = receive(NEWPIZZA + kitchenId);
  std::stringstream ss(msg);

  ss >> type >> size;
  return {static_cast<PizzaType>(type), static_cast<PizzaSize>(size)};
}

Ipc &Ipc::operator<<(const ipcMsg &msg) {
  send(msg._msg, msg._msgType);
  return *this;
}

Ipc &Ipc::operator>>(const ipcMsg &msg) {
  receive(msg._msgType);
  return *this;
}

Ipc::~Ipc() {
  if (_reception)
    msgctl(_msgid, IPC_RMID, NULL);
}
