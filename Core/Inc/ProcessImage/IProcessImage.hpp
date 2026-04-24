/**
 * @file IProcessImage.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Process Image interface
 * @version 0.1
 * @date 2025-11-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include <cstdint>
#include "RGB.hpp"

namespace ProcessImage {

class IProcessImage{
public:
    virtual bool setXTarget(uint32_t) = 0;
    virtual bool setYTarget(uint32_t) = 0;
    virtual bool setZTarget(uint32_t) = 0;
    virtual bool setVacuum(bool) = 0;
    virtual bool getVacuum() = 0;
    virtual bool setRot(int32_t) = 0;
    virtual bool setShootConfetti(bool) = 0;
    virtual bool getShootConfetti() = 0;
    virtual bool setLed(RGB rgb) = 0;
    virtual RGB getLed(void) = 0;


    virtual bool getEstop() = 0;
    virtual bool getStartButton() = 0;
    virtual uint8_t getLimitSwitches() = 0;
    virtual bool getVacuumState() = 0;
    virtual uint32_t getXPosition() = 0;
    virtual uint32_t getYPosition() = 0;
    virtual uint32_t getZPosition() = 0;
    virtual int32_t getRotPosition() = 0;
    virtual bool isInitialized() = 0;

};


}