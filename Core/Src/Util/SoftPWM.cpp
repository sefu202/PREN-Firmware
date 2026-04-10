/**
 * @file SoftPWM.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Soft PWM implementation
 * @version 0.1
 * @date 2026-03-27
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "Util/SoftPWM.hpp"
#include <cassert>

SoftPWM::SoftPWM(PinConfig pinConfig) {
    assert(pinConfig.gpio != nullptr);
    assert(pinConfig.pin < 32);

    uint8_t i = 0; 
    for (i = 0; i < MAX_NUM_PWM_PINS; i++) {
        if (s_pwmPinConfig[i].gpio == nullptr) {
            m_pwmID = i;
            break;
        }
    }

    assert(i < MAX_NUM_PWM_PINS);
    if (i < MAX_NUM_PWM_PINS) {
        HAL_NVIC_DisableIRQ(PWM_IRQ);
        s_pwmPinConfig[i].gpio = pinConfig.gpio;
        s_pwmPinConfig[i].pin = pinConfig.pin;
        HAL_NVIC_EnableIRQ(PWM_IRQ);
    }
}

void SoftPWM::set(uint32_t setPoint) {
    s_setPoint[m_pwmID] = setPoint; // assume atomic copy
}

volatile SoftPWM::PinConfig SoftPWM::s_pwmPinConfig[MAX_NUM_PWM_PINS];
volatile uint32_t SoftPWM::s_count;
volatile uint32_t SoftPWM::s_setPoint[MAX_NUM_PWM_PINS];

void SoftPWM::ISR() {
    for (uint8_t i = 0; i < MAX_NUM_PWM_PINS; i++) {
        if (s_pwmPinConfig[i].gpio != nullptr) { // pwm pin is configured
            if (s_count >= s_setPoint[i]) {
                HAL_GPIO_WritePin(s_pwmPinConfig[i].gpio, s_pwmPinConfig[i].pin, GPIO_PIN_RESET);
            }
            else {
                HAL_GPIO_WritePin(s_pwmPinConfig[i].gpio, s_pwmPinConfig[i].pin, GPIO_PIN_SET);
            }
        }
    }

    s_count++; 
    if (s_count >= PWM_PERIOD) {
        s_count = 0;
    }
}
