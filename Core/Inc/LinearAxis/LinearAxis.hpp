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
#include "Util/EdgePos.hpp"


class LinearAxis {
public:
    /**
     * @brief Constructor for linear axis
     * 
     * @param stepper stepper motor reference
     * @param maxA max acceleration in 100 * steps / s^2
     * @param maxSpeed max speed in steps / s
     * @param initSpeed speed when initializing
     * @param lenght length of the linear axis in steps
     */
    LinearAxis(Stepper::Stepper& stepper, uint16_t maxA, uint16_t maxSpeed, uint16_t initSpeed, uint32_t length);

    /**
     * @brief Sets position to move to
     * 
     * @param setPoint position to move to
     * @note New setPoints should only be issued when previous movements are done, because when direction changes, the ramp will not be correct
     * @note Only works when axis is initialized, otherwise setPoint is ignored
     */
    void moveTo(uint32_t setPoint);

    /**
     * @brief Returns current positon on axis, when initialized
     * 
     * @return uint32_t current position
     */
    uint32_t getCurrentPosition() const;

    /**
     * @brief Starts initialisation of linear axis
     * 
     */
    void init();

    /**
     * @brief Updates linear axis
     */
    void update(bool lowLimitSwitch, bool highLimitSwitch);


    /**
     * @brief Stops linear axis immediatly, without respecting ramp (emergency stop)
     * @note steps may be lost, therefore the axis is considered uninitialized when called
     */
    void estop();

    /**
     * @brief Checks if axis is initialized
     * 
     * @return true axis is initialized
     * @return false axis is not initialized
     */
    bool isInitialized() const;

private:
    Stepper::Stepper& m_stepper;   // stepper motor reference
    LinearAxisRamp m_ramp;
    bool m_initialized = false;
    int32_t m_positionSetPoint = 0;
    uint16_t m_initSpeed;
    uint32_t m_highLimitSwitchPositionMeasured = 0; // measured position of high limit switch, set when high limit switch gets touched
    uint32_t m_length;
    EdgePos m_lowLimitSwitchEdgePos, m_highLimitSwitchEdgePos;
};


