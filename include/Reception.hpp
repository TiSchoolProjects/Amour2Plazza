#pragma once

#include <vector>
#include <utility>
#include <string>
#include <unistd.h>
#include "Kitchen.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include "EnumPlazza.hpp"
#include "Ipc.hpp"

class Reception {
    public:
        Reception(double cookingTime, size_t numberOfCooks, size_t timeToRefillMs);
        ~Reception();
        void handleOrder(const std::vector<std::pair<PizzaType, PizzaSize>> &pizzas);
        void displayStatus();
        void update();

    private:
        struct KitchenInfo {
            pid_t pid;
            int id;
            size_t activePizzas;

        };
        double _cookingTime;
        size_t _numberOfCooks;
        size_t _timeToRefillMs;
        int _nextKitchenId;
        std::vector<KitchenInfo> _kitchens;
        Ipc _ipc;
        void createKitchen();
        void logPizzaReady(PizzaType type, PizzaSize size, int kitchenId);
};
