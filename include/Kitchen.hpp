#pragma once
#include "Reception.hpp"
#include "Thread.hpp"
#include <queue>
#include <functional>

enum Ingredients 
{
    dough,
    tomato,
    gruyere,
    ham,
    mushrooms,
    steak,
    eggPlant,
    goatCheese,
    chiefLove
};

class Kitchen 
{
    public:
        Kitchen(double cookingTime, int numberOfCooks, int timeToRefillMS);
        ~Kitchen();
        void run();
    private:
        std::vector<std::pair<Ingredients, int>> _stock;
        int _numberOfCooks;
        std::vector<std::unique_ptr<Thread<std::function<void()>>>> _cooks;
        std::queue<std::pair<PizzaType, PizzaSize>> _orders;
        Ipc _ipc;
        int _id;
        int _nbOfPizzas;
        int _timeToRefillMS;
        double _cookingTime;
};