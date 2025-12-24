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


LinearAxisRamp::LinearAxisRamp(uint16_t a, uint16_t maxSpeed) : m_a(a), m_maxSpeed(maxSpeed){
    reset();
}

uint16_t LinearAxisRamp::getSpeed(uint32_t distance) {
    uint16_t speedPos = posRampSpeed();
    uint16_t speedNeg = negRampSpeed(distance);
    m_lastSpeed = std::min(speedPos, std::min(speedNeg, m_maxSpeed));
    return m_lastSpeed;
}

uint16_t LinearAxisRamp::posRampSpeed() {
    uint32_t dt = HAL_GetTick() - m_lastTick;
    uint32_t dv = m_a * dt / 10;
    m_lastTick += dv * 10 / m_a;
    uint16_t speedPos = std::min(m_lastSpeed + dv, (uint32_t)UINT16_MAX);
    return speedPos;
}

uint16_t LinearAxisRamp::negRampSpeed(uint32_t distance) {
    return std::sqrt(2*m_a*100*distance);
}


void LinearAxisRamp::reset() {
    m_lastTick = HAL_GetTick();
    m_lastSpeed = 0;
}