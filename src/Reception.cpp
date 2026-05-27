#include "Reception.hpp"
#include <memory>
#include <sstream>
#include <fstream>
#include <iostream>
#include <signal.h>

Reception::Reception(double cookingTime, size_t numberOfCooks, size_t timeToRefillMs) : _cookingTime(cookingTime), _numberOfCooks(numberOfCooks), _timeToRefillMs(timeToRefillMs), _nextKitchenId(0), _ipc(true)
{}

Reception::~Reception()
{
    for (const auto &kitchen : _kitchens) {
        kill(kitchen.pid, SIGTERM);
        waitpid(kitchen.pid, nullptr, 0);
    }
}

void Reception::handleOrder(const std::vector<std::pair<PizzaType, PizzaSize>> &pizzas)
{
    size_t maxCap = _numberOfCooks * 2;
    
    for (const auto &pizza : pizzas) {
        int best = -1;
        for (size_t i = 0; i < _kitchens.size(); i++) {
            if (_kitchens[i].activePizzas < maxCap){
                if (best == -1 || _kitchens[i].activePizzas < _kitchens[best].activePizzas) {
                    best = i;
                }
            }
        }
        if (best == -1){
            createKitchen();
            best = _kitchens.size() - 1;
        }
        _ipc.sendPizza(pizza.first, pizza.second, _kitchens[best].id);
        _kitchens[best].activePizzas++;
    }
}

void Reception::createKitchen() 
{
    _nextKitchenId++;
    int currentID = _nextKitchenId;
    pid_t pid = fork();

    if (pid == -1)
        throw std::runtime_error("Failed to create new kitchen");
    if (pid == 0) {
        try {
            Kitchen kitchen(_cookingTime, _numberOfCooks, _timeToRefillMs, currentID);
            kitchen.run();
        } catch (const std::exception &e) {
            std::cerr << "Error on kitchen " << currentID << " exception: " << e.what() << std::endl;
            exit(84);
        }
        exit(0);
    } else {
        _kitchens.push_back({pid, currentID, 0});
        std::cout << "[Reception] : new kitchen created id: " << currentID << std::endl;
    }
}

void Reception::update()
{
    pid_t exitedPid;
    int kitchenId;
    int pizzaType;
    int pizzaSize;

    while ((exitedPid = waitpid(-1, nullptr, WNOHANG)) > 0) {
        for (auto it = _kitchens.begin(); it != _kitchens.end(); ++it) {
            if (it->pid == exitedPid) {
                std::cout << "[Reception] kitchen id: " << it->id << " closed.\n";
                _kitchens.erase(it);
                break;
            }
        }
    }
    int msgid = msgget(ftok("/tmp", 67), 0666);
    if (msgid == -1)
        return;
    ipcMsg received;
    while (msgrcv(msgid, &received, sizeof(received._msg), KITCHENRESP, IPC_NOWAIT) != -1) {
        std::stringstream ss(received._msg);
        ss >> kitchenId >> pizzaType >> pizzaSize;
        logPizzaReady(static_cast<PizzaType>(pizzaType), static_cast<PizzaSize>(pizzaSize), kitchenId);
        for (auto &kitchen : _kitchens) {
            if (kitchen.id == kitchenId && kitchen.activePizzas > 0) {
                kitchen.activePizzas--;
                break;
            }   
        }
    }
}

void Reception::displayStatus()
{
    std::cout << "\n STATUS AMOUR2PLAZZA" << std::endl;
    if (_kitchens.empty()) {
        std::cout << "No kitchen in activity." << std::endl;
        return;
    }
    for (const auto &k : _kitchens) {
        std::cout << "Kitchen id " << k.id << "; pizzas:" << k.activePizzas << " / " << (_numberOfCooks * 2) <<std::endl;
        std::cout << "Stock of Ingredients :" << std::endl;
        _ipc.sendPizza(static_cast<PizzaType>(STATUS), static_cast<PizzaSize>(STATUS), k.id);
    }
    std::cout << "\n\n" << std::flush;
}

void Reception::logPizzaReady(PizzaType type, PizzaSize size, int kitchenId)
{
    std::ofstream file("plazza.log", std::ios::app);
    std::string typeStr = (type == Regina) ? "regina" : (type == Margarita) ? "margarita" : (type == Americana) ? "americana" : "fantasia";
    std::string sizeStr = (size == S) ? "S" : (size == M) ? "M" : (size == L) ? "L" : (size == XL) ? "XL" : "XXL";

    if (file.is_open()) {
        file << "[Pizza ready !] " << typeStr << " " << sizeStr << " cooked by the kitchen: " << kitchenId << std::endl;
    }
    std::cout << "[Reception] Pizza ready ! " << typeStr << " [" << sizeStr << "] kitchen id " << kitchenId << std::endl;
}
