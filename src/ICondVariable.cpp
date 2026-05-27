#include "ICondVariable.hpp"


void ConditionVariable::wait(Mutex &mutex)
{
    _condVar.wait(mutex);
}

void ConditionVariable::signal()
{
    _condVar.notify_one();
}

void ConditionVariable::broadcast()
{
    _condVar.notify_all();
}
