
#include "mc.h"

Context::Context() 
{
    setState(new AsleepState());
}

void Context::setState(State* state) 
{
    if (currentState)
        delete(currentState);
    state->setContext(this);
    currentState = state;
}

void Context::doEvents() 
{
    currentState->doEvents();
}
