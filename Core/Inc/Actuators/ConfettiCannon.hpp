/**
 * @file ConfettiCannon.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Confetti Cannon Subsystem
 * @version 0.1
 * @date 2026-03-25
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <cstdint>
#include "Util/EdgePos.hpp"
#include "main.h"

#define CONFETTI_CANNON_IGNITE_TIME 5000 // [ms]

class ConfettiCannon {

public: 

    /**
     * @brief Periodic update of cc subsystem
     * 
     * @param shoot positive edge to shoot cannon, cannon gets ignited on positive edge
     * @param estop block confetti cannon
     */
    void update(bool shoot, bool estop);

    /**
     * @brief Output bit of confetti cannon
     * 
     * @return true ignite cc
     * @return false dont ignite cc
     */
    bool oIgnite() const;

private:

    bool m_ignite = false;
    EdgePos m_shootPosEdge;
    uint32_t m_igniteTick = 0;

};