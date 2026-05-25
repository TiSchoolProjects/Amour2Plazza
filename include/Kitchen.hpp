#pragma once
#include "Reception.hpp"
#include "Thread.hpp"
#include <queue>
#include "Mutex.hpp"
#include <sstream>
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
        Kitchen(double cookingTime, int numberOfCooks, int timeToRefillMS, int id);
        ~Kitchen();
        void run();
    private:
        std::vector<std::pair<Ingredients, int>> _stock;
        int _numberOfCooks;
        std::vector<std::unique_ptr<Thread<std::function<void()>>>> _cooks;
        std::queue<std::pair<PizzaType, PizzaSize>> _orders;
        void cookFct();
        void takeIngredients(PizzaType type);
        bool canCook(PizzaType type);
        bool tryToCook(PizzaType type);
        Ipc _ipc;
        int _id;
        int _nbOfPizzas;
        Mutex _mutex;
        bool _isActive;
        int _timeToRefillMS;
        double _cookingTime;
};
