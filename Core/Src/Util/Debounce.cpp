#include <cstdint>
#include "Util/Debounce.hpp"
#include "stm32f7xx_hal.h"


bool Debounce::operator()(bool current)
{
    return (*this)(current, debounceTime);
}

bool Debounce::operator()(bool current, uint32_t debounceTime)
{
    uint32_t now = HAL_GetTick();

    switch (stateFsm) {
        case State::LowReady: {
            state = false;

            if (current) // rising edge
            {
                stateFsm = State::HighLocked;
                lockStart = now;
            }
            break;
        }

        case State::HighLocked: {
            state = true;

            if ((now - lockStart) >= debounceTime)
            {
                stateFsm = State::HighReady;
            }
            break;
        }

        case State::HighReady: {
            state = true;

            if (!current) // falling edge
            {
                stateFsm = State::LowLocked;
                lockStart = now;
            }
            break;
        }

        case State::LowLocked: {
            state = false;

            if ((now - lockStart) >= debounceTime)
            {
                stateFsm = State::LowReady;
            }
            break;
        }
    }

    lastInput = current;
    return state;
}

Debounce::operator bool() const
{
    return state;
}