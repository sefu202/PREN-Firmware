/**
 * @file ProcessImage.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Process Image class
 * @version 0.1
 * @date 2025-11-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "IProcessImage.hpp"

namespace ProcessImage {

class ProcessImage : public IProcessImage{
public:
    bool setXTarget(uint32_t) override;
    uint32_t getXTarget() override;
    bool setYTarget(uint32_t) override;
    uint32_t getYTarget() override;
    bool setZTarget(uint32_t) override;
    uint32_t getZTarget() override;
    bool setVacuum(bool) override;
    bool getVacuum() override;
    bool setRot(int32_t) override;
    uint32_t getRotDelta() override;
    bool setShootConfetti(bool) override;
    bool getShootConfetti() override;
    bool setLed(RGB rgb) override;
    RGB getLed(void) override;


    bool getEstop() override;
    bool getStartButton() override;
    uint8_t getLimitSwitches() override;
    bool getVacuumState() override;
    uint32_t getXPosition() override;
    uint32_t getYPosition() override;
    uint32_t getZPosition() override;
    int32_t getRotPosition() override;
    bool isInitialized() override;


    void update();

private:

    
    bool m_confetti;
    RGB m_led;
};


}