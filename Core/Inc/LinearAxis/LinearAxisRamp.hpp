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

public:

    /**
     * @brief Constructor
     * 
     * @param a max acceleration [100 * steps / s^2]
     * @param maxSpeed max speed [steps / s]
     */
    LinearAxisRamp(uint16_t a, uint16_t maxSpeed);

    /**
     * @brief get speed for ramp
     * @param distance absolute distance to set point
     * 
     * @return uint16_t speed in steps / second
     */
    uint16_t getSpeed(uint32_t distance);

    /**
     * @brief Resets ramp to 0
     */
    void reset();

private:

    uint16_t posRampSpeed();
    uint16_t negRampSpeed(uint32_t distance);

    uint32_t m_lastTick = 0;
    uint16_t m_lastSpeed = 0;
    uint16_t m_a;   // 100 * steps / s^2
    uint16_t m_maxSpeed;
};