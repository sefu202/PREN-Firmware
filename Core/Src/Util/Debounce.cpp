/**
 * @file Debounce.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief 
 * @version 0.1
 * @date 2025-12-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <cstdint>
#include "Util/Debounce.hpp"
#include "stm32f7xx_hal.h"

bool Debounce::operator()(bool current) {
    return (*this)(current, debounceTime);
}

bool Debounce::operator()(bool current, uint32_t debounceTime) {
    uint32_t now = HAL_GetTick();

    // Not locked -> accept first edge immediately
    if (tickFirstRisingEdge == 0 && tickFirstFallingEdge == 0)
    {
        if (current != lastInput)
        {
            state = current;

            if (current){
                tickFirstRisingEdge = now;
            }
            else {
                tickFirstFallingEdge = now;
            }
        }
    }
    else
    {
        // Lockout active -> check if debounce time expired
        uint32_t lockStart =
            tickFirstRisingEdge ? tickFirstRisingEdge
                                : tickFirstFallingEdge;

        if ((now - lockStart) >= debounceTime)
        {
            tickFirstRisingEdge  = 0;
            tickFirstFallingEdge = 0;
        }
    }

    lastInput = current;
    return state;
}

Debounce::operator bool() const {
    return state;
}