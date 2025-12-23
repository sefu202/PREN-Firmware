/**
 * @file Stepper.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Stm32 stepper implementation
 * @version 0.1
 * @date 2025-11-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Stepper/Stepper.hpp"
#include <cassert>
#include "main.h"

namespace Stepper{


Stepper::Stepper(PinConfig pinConfig) {
    assert(pinConfig.dirGpio != nullptr);
    assert(pinConfig.stepGpio != nullptr);

    // Find unused stepper
    uint8_t i = 0;
    for (i = 0; i < MAX_NUM_STEPPERS; i++) {
        if (s_stepperPinConfig[i].stepGpio == nullptr) {
            m_stepperId = i;
            break;
        }
    }

    // Check if there was an unused stepper, if yes, use it
    assert(i < MAX_NUM_STEPPERS);
    if (i < MAX_NUM_STEPPERS) {
        HAL_NVIC_DisableIRQ(STEPPER_IRQ);
        s_stepperPinConfig[m_stepperId].stepGpio = pinConfig.stepGpio;
        s_stepperPinConfig[m_stepperId].dirGpio = pinConfig.dirGpio;
        s_stepperPinConfig[m_stepperId].stepPin = pinConfig.stepPin;
        s_stepperPinConfig[m_stepperId].dirPin = pinConfig.dirPin;
        HAL_NVIC_EnableIRQ(STEPPER_IRQ);
    }
}

void Stepper::step(int32_t steps) {
    HAL_NVIC_DisableIRQ(STEPPER_IRQ);
    s_remainingSteps[m_stepperId] += steps;
    HAL_NVIC_EnableIRQ(STEPPER_IRQ);
}

void Stepper::setSpeed(uint16_t speed) {
    if (speed > STEPPER_MAX_SPEED) {
        speed = STEPPER_MAX_SPEED;
    }
    uint16_t ticks = 0;
    if (speed == 0) {
        ticks = 0;
    }
    else {
        ticks = STEPPER_MAX_SPEED * STEPPER_MIN_TICKS_BETWEEN_STEP / speed;
    }
    HAL_NVIC_DisableIRQ(STEPPER_IRQ);
    s_tickBetweenSteps[m_stepperId] = ticks; // todo
    HAL_NVIC_EnableIRQ(STEPPER_IRQ);
}





volatile Stepper::PinConfig Stepper::s_stepperPinConfig[MAX_NUM_STEPPERS];
volatile int32_t Stepper::s_remainingSteps[MAX_NUM_STEPPERS];      // remaining steps and direction
volatile uint16_t Stepper::s_currentTickBetweenSteps[MAX_NUM_STEPPERS];   // for speed control
volatile uint16_t Stepper::s_tickBetweenSteps[MAX_NUM_STEPPERS];   // for speed control

void Stepper::ISR(){



    for (uint8_t i = 0; i < MAX_NUM_STEPPERS; i++) {
        if (s_stepperPinConfig[i].stepGpio != nullptr) { // Stepper is configured
            if (s_currentTickBetweenSteps[i] == s_tickBetweenSteps[i]/2) {
                HAL_GPIO_WritePin(s_stepperPinConfig[i].stepGpio, s_stepperPinConfig[i].stepPin, GPIO_PIN_RESET);

                if (s_remainingSteps[i] < 0) {
                    HAL_GPIO_WritePin(s_stepperPinConfig[i].dirGpio, s_stepperPinConfig[i].dirPin, GPIO_PIN_SET);
                }
                else {
                    HAL_GPIO_WritePin(s_stepperPinConfig[i].dirGpio, s_stepperPinConfig[i].dirPin, GPIO_PIN_RESET);
                }
            }
            else if (s_currentTickBetweenSteps[i] >= s_tickBetweenSteps[i]) {
                s_currentTickBetweenSteps[i]  = 0;
                if (s_remainingSteps[i] > 0){
                    HAL_GPIO_WritePin(s_stepperPinConfig[i].stepGpio, s_stepperPinConfig[i].stepPin, GPIO_PIN_SET);
                    s_remainingSteps[i]--;
            
                } else if (s_remainingSteps[i] < 0) {
                    HAL_GPIO_WritePin(s_stepperPinConfig[i].stepGpio, s_stepperPinConfig[i].stepPin, GPIO_PIN_SET);
                    s_remainingSteps[i]++;
                }
            }
            s_currentTickBetweenSteps[i]++;
        }
    }

}
 
}
