/**
 * @file Vacuum.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Vacuum subystem
 * @version 0.1
 * @date 2026-03-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include "Util/EdgePos.hpp"
#include <cstdint>

#define VALVE_ENABLE_TIME 5000

class Vacuum{

public:
    /**
     * @brief Periodic update of vacuum subsystem
     * 
     * @param enableVacuum set true to enable vacuum
     * @param eStop e stop, disables pump and releases air pressure
     */
    void update(bool enableVacuum, bool eStop);

    bool oRunPump() const;

    bool oEnableValve() const;

private:

    bool m_runPump = false, m_enableValve = false;
    EdgePos m_enableVacuumNegEdge, m_eStopPosEdge;

    uint32_t m_startValveTick = 0;


};