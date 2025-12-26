/**
 * @file Debounce.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Debounce for Input signals
 * @version 0.1
 * @date 2025-12-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

struct Debounce{
    uint32_t tickFirstRisingEdge  = 0;
    uint32_t tickFirstFallingEdge = 0;

    uint32_t debounceTime = 5;  // ms

private:
    bool state      = false;  // output state
    bool lastInput  = false;

public:
    bool operator()(bool current);

    bool operator()(bool current, uint32_t debounceTimeMs);

    operator bool() const;
};