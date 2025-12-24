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
#include <cmath>
#include "LinearAxis/LinearAxis.hpp"



LinearAxis::LinearAxis(Stepper::Stepper& stepper, uint16_t max_a, uint16_t max_speed) 
  : m_stepper(stepper), 
    m_ramp(max_a, max_speed) {

}

void LinearAxis::moveTo(uint32_t setPoint) {
    m_stepper.setSpeed(0);
    int32_t positionCurrent = getCurrentPosition();
    m_stepper.resetSteps();
    m_positionSetPoint = setPoint;
    m_stepper.step(m_positionSetPoint - positionCurrent);
    m_stepper.setSpeed(0);
}


uint32_t LinearAxis::getCurrentPosition() const {
    return std::max(m_positionSetPoint - m_stepper.getRemainingSteps(), (int32_t)0);
}

void LinearAxis::update() {
   m_stepper.setSpeed(m_ramp.getSpeed(std::abs(m_stepper.getRemainingSteps())));
}