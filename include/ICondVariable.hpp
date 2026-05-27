#pragma once
#include <condition_variable>
#include "Mutex.hpp"

class IConditionVariable 
{
    public :
        virtual ~IConditionVariable() = default;
        virtual void wait(Mutex &mutex) = 0;
        virtual void signal() = 0;
        virtual void broadcast() = 0;
        
};

class ConditionVariable : public IConditionVariable
{
    public:
        ConditionVariable() = default;
        ~ConditionVariable() = default;
        void wait(Mutex &mutex) override;
        void signal() override;
        void broadcast() override;
    private:
        std::condition_variable_any _condVar;
};
