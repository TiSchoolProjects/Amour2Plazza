#include "Kitchen.hpp"

Kitchen::Kitchen(double cookingTime, int numberOfCooks, int timeToRefillMS, int id) : _ipc(true)
{
    _cookingTime = cookingTime;
    _numberOfCooks = numberOfCooks;
    _timeToRefillMS = timeToRefillMS;
    _nbOfPizzas = 0;
    _isActive = true;
    _id = id;

    for (size_t i = 0; i < 8; i++) {
        Ingredients ing = static_cast<Ingredients>(i);
        _stock.push_back({ing, 5});
    }
    for (size_t i = 0; i < numberOfCooks; i++) {
        _cooks.push_back(std::make_unique<Thread<std::function<void()>>>([this]() {
            this->cookFct();
        }));
    }
    for (auto &cook : _cooks) {
        cook->startThread();
    }
}

Kitchen::~Kitchen()
{
    for (auto &cook : _cooks) {
        cook->stopThread();
    }
}

void Kitchen::cookFct()
{
    std::pair<PizzaType, PizzaSize> currentOrder;
    double bakeTime = 0.0;

    while(_isActive) {
        _mutex.lock();
        if (!_orders.empty()) {
            currentOrder = _orders.front();
            _orders.pop();
            _mutex.unlock();
            switch (currentOrder.first){
                case Regina :
                    bakeTime = 2.0;
                    break;
                case Margarita : 
                    bakeTime = 1.0;
                    break;
                case Americana :
                    bakeTime = 2.0;
                    break;
                case Fantasia :
                    bakeTime = 4.0;
                    break;
            }
            double total = bakeTime * _cookingTime;
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(total * 1000)));
            _ipc.sendPizzaToReception(currentOrder.first, currentOrder.second, _id);
        } else {
            _mutex.unlock();
        }
    }
}

void Kitchen::run()
{
    auto lastActivity = std::chrono::steady_clock::now();
    auto lastRefill = std::chrono::steady_clock::now();

    while (_isActive) {
        auto now = std::chrono::steady_clock::now();
        auto timeSinceRefill = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRefill).count();

        if (timeSinceRefill >= _timeToRefillMS) {
            _mutex.lock();
            for (auto &ingredient : _stock){
                if (ingredient.second = 5)
                    continue;
                ingredient.second++;
            }
            _mutex.unlock();
            lastRefill = now;
        }
        auto timeSinceActivity = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastActivity).count();
        if (timeSinceActivity >= 5 && _orders.empty()) {
            _isActive = false;
            std::cout << "Kitchen ID: " << _id << "closed." << std::endl;
            break;
        }
        std::string msgOrder = _ipc.receive(NEWPIZZA + _id);
        if (!msgOrder.empty()) {
            std::stringstream ss(msgOrder);
            int type;
            int size;
            ss >> type >> size;
            _mutex.lock();
            std::pair<PizzaType, PizzaSize> newPizza;
            newPizza.first = static_cast<PizzaType>(type);
            newPizza.second = static_cast<PizzaSize>(size);
            _orders.push(newPizza);
            _mutex.unlock();
            lastActivity = std::chrono::steady_clock::now();
        } else
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
