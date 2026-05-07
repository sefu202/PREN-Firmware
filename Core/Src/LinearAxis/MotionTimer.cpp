/**
 * @file MotionTimer.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief MotionTimer implementation
 * @version 0.1
 * @date 2026-05-07
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <cmath>
#include <stm32f7xx_hal.h>
#include "LinearAxis/MotionTimer.hpp"

MotionTimer::MotionTimer(uint16_t a, uint16_t maxSpeed) : m_maxSpeed(maxSpeed), m_a(a*100){

}

uint32_t MotionTimer::tMin(uint32_t d){
    uint64_t d_crit =
        ((uint64_t)m_maxSpeed * m_maxSpeed) / m_a;

    if (d >= d_crit) {
        return
            (2000ULL * m_maxSpeed) / m_a +
            (1000ULL * (d - d_crit)) / m_maxSpeed;
    }
    else {
        return std::sqrt((4000000ULL * d) / m_a);
    }
}


void MotionTimer::reccomendETATick(uint32_t etaTick) {
    m_etaTick = etaTick;
    m_newEtaTick = true;
}

uint16_t MotionTimer::vReccomend(uint32_t d) {
    m_newEtaTick = false;
    uint32_t tMs = m_etaTick - HAL_GetTick();

    if (tMs > MAX_TICKS_RECCOMEND) {
        return m_maxSpeed;
    }

    // absolute minimum physically possible time
    uint32_t tMinAbs =
        std::sqrt((4000000ULL * d) / m_a);

    if (tMs <= tMinAbs) {
        return m_maxSpeed;
    }

    uint64_t a  = m_a;
    uint64_t t  = tMs;
    uint64_t d64 = d;

    uint64_t discriminant =
        a * a * t * t
        - 4000000ULL * a * d64;

    uint32_t v =
        (
            a * t
            - std::sqrt(discriminant)
        ) / 2000ULL;

    if (v > m_maxSpeed) {
        v = m_maxSpeed;
    }

    m_etaTick = HAL_GetTick();

    return v;
}

bool MotionTimer::hasNewEtaTick() {

    bool b = m_newEtaTick; m_newEtaTick = false;
    return b;
}
