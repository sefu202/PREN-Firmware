/**
 * @file LinearAxisRamp.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Ramp for linear axis
 * @version 0.1
 * @date 2025-12-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include <cstdint>


class LinearAxisRamp {

    /**
     * @brief get speed for ramp
     * 
     * @return uint16_t speed in ticks / second
     */
    uint16_t getSpeed();

private:

    uint16_t posRampSpeed();
    uint16_t negRampSpeed();

    uint32_t m_lastTick;
    uint16_t m_lastSpeed;
    uint16_t m_maxSpeed;
    uint16_t m_a;   // 1000 * ticks / s^2
    uint32_t m_distance;    // absolute distance
};