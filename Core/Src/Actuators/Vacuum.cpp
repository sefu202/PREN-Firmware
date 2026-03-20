/**
 * @file Vacuum.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Vacuum subystem implementation
 * @version 0.1
 * @date 2026-03-20
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "Actuators/Vacuum.hpp"
#include "main.h"


void Vacuum::update(bool enableVacuum, bool eStop) {
    enableVacuum = enableVacuum && !eStop;
    m_enableVacuumNegEdge(!enableVacuum);
    m_eStopPosEdge(eStop);

    if (enableVacuum) {
        m_runPump = true;
        m_enableValve = false;
    }
    else {

        if (m_enableVacuumNegEdge || m_eStopPosEdge) {
            m_enableValve = true;
            m_startValveTick = HAL_GetTick();
        }
        else if ((HAL_GetTick() - m_startValveTick) > VALVE_ENABLE_TIME) {
            m_enableValve = false;
        }

        m_runPump = false;
    }
}

bool Vacuum::oRunPump() const {
    return m_runPump;
}

bool Vacuum::oEnableValve() const {
    return m_enableValve;
}
