/**
 * @file LinearAxis.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Linear axis implementation
 * @version 0.1
 * @date 2025-12-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "LinearAxis/LinearAxis.hpp"



LinearAxis::LinearAxis(Stepper::Stepper& stepper) : m_stepper(stepper) {

}

void LinearAxis::moveTo(uint32_t setPoint) {
    m_stepper.setSpeed(0);
    m_positionCurrent = m_positionSetPoint - m_stepper.getRemainingSteps();
    m_stepper.resetSteps();
    m_positionSetPoint = setPoint;
    m_stepper.step(m_positionSetPoint - m_positionCurrent);
    m_stepper.setSpeed(255); // todo
}