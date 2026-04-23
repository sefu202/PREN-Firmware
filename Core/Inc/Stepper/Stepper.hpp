/**
 * @file Stepper.hpp
 * @author Josef Aschwanden (aschwandenjosef@stud.hslu.ch)
 * @brief Stepper motor control using step/dir interface with interrupts
 * @version 0.1
 * @date 2025-11-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include <cstdint>
#include "stm32f7xx_hal.h"

#define MAX_NUM_STEPPERS 6
#define STEPPER_IRQ TIM8_BRK_TIM12_IRQn
#define STEPPER_IRQ_TICKRATE 40000  // [hz] set accoring to configuration
#define STEPPER_MAX_SPEED STEPPER_IRQ_TICKRATE / STEPPER_MIN_TICKS_BETWEEN_STEP
#define STEPPER_MIN_TICKS_BETWEEN_STEP 8    // needs to be atleast 2, put higher for better speed control for high speeds

#if STEPPER_MIN_TICKS_BETWEEN_STEP < 2
#error "STEPPER_MIN_TICKS_BETWEEN_STEP needs to be atleast 2"
#endif

namespace Stepper{

class Stepper {
public:

    struct PinConfig{
        GPIO_TypeDef *stepGpio = nullptr;
        uint16_t stepPin = 0;
        GPIO_TypeDef *dirGpio = nullptr;
        uint16_t dirPin = 0;
    };

    /**
     * @brief Construct a new Stepper object
     * 
     * @param pinConfig Pin configuration of stepper motor
     */
    Stepper(PinConfig pinConfig);

    /**
     * @brief adds new steps that should be done
     * 
     * @param steps 
     */
    void step(int32_t steps);

    /**
     * @brief sets step speed in steps / second
     * @param speed
     */
    void setSpeed(uint16_t speed);

    /**
     * @brief Get remaining steps
     * 
     * @return int32_t remaining steps and direction
     */
    int32_t getRemainingSteps() const;

    /**
     * @brief Clears all remaining steps
     * 
     */
    void resetSteps();

    /**
     * @brief Interrupt service routine, call periodically
     */
    static void ISR();

private:

    static volatile PinConfig s_stepperPinConfig[MAX_NUM_STEPPERS];
    static volatile int32_t s_remainingSteps[MAX_NUM_STEPPERS];      // remaining steps and direction
    static volatile uint16_t s_currentTickBetweenSteps[MAX_NUM_STEPPERS];   // for speed control
    static volatile uint16_t s_tickBetweenSteps[MAX_NUM_STEPPERS];   // for speed control

    uint8_t m_stepperId;

};

}

