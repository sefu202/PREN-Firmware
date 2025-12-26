/**
 * @file StepperMusicPlayer.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Play some music on your stepper motors
 * @version 0.1
 * @date 2025-12-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Stepper/StepperMusicPlayer.hpp"

StepperMusicPlayer::StepperMusicPlayer(Stepper::Stepper& step) : m_step(step){

}
void StepperMusicPlayer::play() {
    const uint16_t speed[] = {
        330, 0, 330, 0, 330,
        0,
        330, 0, 330, 0, 330,
        0,
        330, 0, 392, 262, 294, 330,
        0,
        349, 0, 349, 0, 349, 0, 349,
        0,
        349, 330, 330, 330,
        0,
        330, 294, 294, 330, 294, 392
    };


    const uint16_t duration[] = {
        250, 50, 250, 50, 500,
        150,
        250, 50, 250, 50, 500,
        150,
        250, 50, 250, 250, 250, 1000,
        150,
        250, 50, 250, 50, 250, 50, 500,
        150,
        250, 250, 250, 250,
        150,
        250, 250, 500, 250, 1000
    };

    uint8_t notes = sizeof(speed) / sizeof(speed[0]);

    m_step.resetSteps();
    m_step.step(UINT32_MAX/10);
    for (uint16_t i = 0; i < notes; i++) {
        m_step.setSpeed(speed[i]);
        HAL_Delay(duration[i]);
    }
    m_step.resetSteps();
}