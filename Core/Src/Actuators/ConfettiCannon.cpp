/**
 * @file ConfettiCannon.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Confetti cannon implementation
 * @version 0.1
 * @date 2026-03-25
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "Actuators/ConfettiCannon.hpp"


void ConfettiCannon::update(bool shoot, bool estop) {
    m_shootPosEdge(shoot);

    if (m_shootPosEdge && !estop) {
        m_igniteTick = HAL_GetTick();
        m_ignite = true;
    }

    if (m_ignite && HAL_GetTick() < (m_igniteTick + CONFETTI_CANNON_IGNITE_TIME)) {
        m_ignite = true;
    }
    else {
        m_ignite = false;
    }
}

bool ConfettiCannon::oIgnite() const {
    return m_ignite;
}