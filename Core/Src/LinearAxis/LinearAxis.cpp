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
#include <algorithm> // min, max



LinearAxis::LinearAxis(Stepper::Stepper& stepper, uint16_t maxA, uint16_t maxSpeed, uint16_t initSpeed, uint32_t length) 
  : m_stepper(stepper), 
    m_ramp(maxA, maxSpeed), 
    m_initSpeed(initSpeed),
    m_length(length), 
    m_lowLimitSwitchEdgePos(true),
    m_highLimitSwitchEdgePos(true) {

}

void LinearAxis::moveTo(uint32_t setPoint) {
    if (m_initialized) {
        m_stepper.setSpeed(0);  // make sure no steps are done after reading position
        int32_t positionCurrent = getCurrentPosition();
        m_stepper.resetSteps();
        
        m_positionSetPoint = std::min(std::max(setPoint, 0ul), m_length);
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
    if (!m_initialized) {
        m_stepper.resetSteps();
        m_stepper.step(INT32_MIN);
        m_positionSetPoint = 0;
    }
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
        m_initialized = true;
        m_positionSetPoint = std::max(m_positionSetPoint, 0l);
        m_stepper.resetSteps();
        m_stepper.step(m_positionSetPoint);
    }

    // Check if axis is already on limit switch at start:
    // if yes (uncertain initialisation, position is ambigious) -> back of again and detect edge (certain position)
    if (lowLimitSwitch && !m_initialized) {
        m_uncertainInitialized = true;
        m_stepper.resetSteps();
        m_stepper.step(200);
    }
    else if (m_uncertainInitialized && !m_initialized) {
        m_stepper.resetSteps();
        m_stepper.step(INT32_MIN);
        m_uncertainInitialized = false;
    }

    if (m_highLimitSwitchEdgePos(highLimitSwitch)) {
        m_highLimitSwitchPositionMeasured = getCurrentPosition();
        m_positionSetPoint = std::min(m_positionSetPoint, (int32_t)m_length);
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