/**
 * @file LinearAxisRamp.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Linear axis ramp implementation
 * @version 0.1
 * @date 2025-12-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <cmath>
#include "stm32f7xx_hal.h"
#include "LinearAxis/LinearAxisRamp.hpp"


uint16_t LinearAxisRamp::getSpeed() {
    uint16_t speedPos = posRampSpeed();
    uint16_t speedNeg = negRampSpeed();
    m_lastSpeed = std::min(speedPos, std::min(speedNeg, m_maxSpeed));
    return m_lastSpeed;
}

uint16_t LinearAxisRamp::posRampSpeed() {
    uint32_t dt = HAL_GetTick() - m_lastTick;
    m_lastTick += dt;
    uint16_t speedPos = std::min(m_lastSpeed + m_a * dt, (uint32_t)UINT16_MAX);
    return speedPos;
}

uint16_t LinearAxisRamp::negRampSpeed() {
    return std::sqrt(2*m_a*1000*m_distance);
}