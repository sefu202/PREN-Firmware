/**
 * @file LinearAxis.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Linear axis control with stepper motor
 * @version 0.1
 * @date 2025-12-24
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "Stepper/Stepper.hpp"


class LinearAxis {
public:
    /**
     * @brief Constructor for linear axis
     * 
     * @param stepper stepper motor reference
     */
    LinearAxis(Stepper::Stepper& stepper);

    /**
     * @brief Sets position to move to
     * 
     * @param setPoint position to move to
     */
    void moveTo(uint32_t setPoint);



private:
    Stepper::Stepper& m_stepper;   // stepper motor reference
    bool m_initialized = false;
    int32_t m_positionCurrent = 0;
    int32_t m_positionSetPoint = 0;
};


