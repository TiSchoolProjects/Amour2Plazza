#pragma once

#include <atomic>
#include <thread>

template <typename T>

class Thread {
private:
  std::thread _thread;
  std::atomic<bool> _active{false};
  T _function;

public:
  Thread(T func) : _function(std::move(func)) {}

  ~Thread() { stopThread(); }

  void startThread() {
    if (_active.load())
      return;
    _active = true;
    _thread = std::thread([this]() {
      _function();
      _active = false;
    });
  }
  void stopThread() {
    if (_thread.joinable())
      _thread.join();
    _active = false;
  }
};
