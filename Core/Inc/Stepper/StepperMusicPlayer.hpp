/**
 * @file StepperMusicPlayer.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Play some music on your stepper motors
 * @version 0.1
 * @date 2025-12-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include "Stepper/Stepper.hpp"



class StepperMusicPlayer {
public:
    StepperMusicPlayer(Stepper::Stepper& step);
    void play();

private: 
    Stepper::Stepper& m_step;
};