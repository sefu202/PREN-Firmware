/**
 * @file callbacks.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Interrupt callbacks implementation
 * @version 0.1
 * @date 2025-11-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "stm32f7xx.h"
#include "main.h"
#include "Stepper/Stepper.hpp"
#include "Util/SoftPWM.hpp"

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim->Instance == TIM12) {
        
        //HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin,GPIO_PIN_SET);
        Stepper::Stepper::ISR();
        SoftPWM::ISR();
        
        //HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin,GPIO_PIN_RESET);
    }
}