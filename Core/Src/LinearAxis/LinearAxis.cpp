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



LinearAxis::LinearAxis(Stepper::Stepper& stepper, uint16_t maxA, uint16_t maxSpeed, uint16_t initSpeed, uint32_t length) 
  : m_stepper(stepper), 
    m_ramp(maxA, maxSpeed), 
    m_initSpeed(initSpeed),
    m_length(length) {

}

void LinearAxis::moveTo(uint32_t setPoint) {
    if (m_initialized) {
        m_stepper.setSpeed(0);  // make sure no steps are done after reading position
        int32_t positionCurrent = getCurrentPosition();
        m_stepper.resetSteps();
        m_positionSetPoint = setPoint;
        m_stepper.step(m_positionSetPoint - positionCurrent);
    }
}


uint32_t LinearAxis::getCurrentPosition() const {
    if (m_initialized) {
        return std::max(m_positionSetPoint - m_stepper.getRemainingSteps(), (int32_t)0);
    }
    else {
        return 0;
    }
}

void LinearAxis::init() {
    m_stepper.step(INT32_MIN);
    m_positionSetPoint = 0;
}

void LinearAxis::update(bool lowLimitSwitch, bool highLimitSwitch) {
    if (m_initialized && !lowLimitSwitch && !highLimitSwitch) {
        m_stepper.setSpeed(m_ramp.getSpeed(std::abs(m_stepper.getRemainingSteps())));
    }
    else {
        m_stepper.setSpeed(m_initSpeed);
        m_ramp.reset();
    }

    if (m_lowLimitSwitchEdgePos(lowLimitSwitch)) {
        m_stepper.resetSteps();
        m_stepper.step(m_positionSetPoint);
        m_initialized = true;
    }

    if (m_highLimitSwitchEdgePos(highLimitSwitch)) {
        m_highLimitSwitchPositionMeasured = getCurrentPosition();
        m_stepper.resetSteps();
        m_stepper.step(m_positionSetPoint - m_length);
    }
}

void LinearAxis::estop() {
    m_stepper.setSpeed(0);
    m_stepper.resetSteps();
    m_initialized = false;  // assume steps have been lost
    m_positionSetPoint = 0;
}


bool LinearAxis::isInitialized() const{
    return m_initialized;
}

uint32_t LinearAxis::getLength() const {
    return m_length;
}