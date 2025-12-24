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
#include "LinearAxis/LinearAxisRamp.hpp"


class LinearAxis {
public:
    /**
     * @brief Constructor for linear axis
     * 
     * @param stepper stepper motor reference
     * @param max_a max acceleration in 100 * steps / s^2
     * @param max_speed max speed in steps / s
     */
    LinearAxis(Stepper::Stepper& stepper, uint16_t max_a, uint16_t max_speed);

    /**
     * @brief Sets position to move to
     * 
     * @param setPoint position to move to
     */
    void moveTo(uint32_t setPoint);

    /**
     * @brief Returns current positon on axis
     * 
     * @return uint32_t current position
     */
    uint32_t getCurrentPosition() const;

    //void init();

    /**
     * @brief Updates linear axis
     */
    void update();


    //void stop();

private:
    Stepper::Stepper& m_stepper;   // stepper motor reference
    LinearAxisRamp m_ramp;
    bool m_initialized = false;
    int32_t m_positionSetPoint = 0;
};


