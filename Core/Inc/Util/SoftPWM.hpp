/**
 * @file SoftPWM.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Interrupt based software Pulse width modulation
 * @version 0.1
 * @date 2026-03-27
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#pragma once

#include <cstdint>
#include "main.h"


#define MAX_NUM_PWM_PINS 4
#define PWM_IRQ TIM8_BRK_TIM12_IRQn
#define PWM_PERIOD 256


class SoftPWM {
public:

    struct PinConfig{
        GPIO_TypeDef *gpio = nullptr;
        uint16_t pin = 0;
    };

    /**
     * @brief Constructor
     * 
     * @param pinConfig pin configuration of pwm pin
     */
    SoftPWM(PinConfig pinConfig);

    /**
     * @brief Sets PWM value
     * 
     * @param setPoint value to set, duty cycle is calculated by D =  setPoint / PWM_PERIOD
     */
    void set(uint32_t setPoint);

    /**
     * @brief Interrupt service routine, call periodically
     * 
     */
    static void ISR();

private:

    static volatile PinConfig s_pwmPinConfig[MAX_NUM_PWM_PINS];
    static volatile uint32_t s_count;
    static volatile uint32_t s_setPoint[MAX_NUM_PWM_PINS];

    uint8_t m_pwmID = 0;
};