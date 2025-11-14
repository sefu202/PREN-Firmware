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
#include "RGB.hpp"

namespace ProcessImage {

class IProcessImage{
public:
    virtual void setXTarget(uint32_t) = 0;
    virtual uint32_t getXTarget() = 0;
    virtual void setYTarget(uint32_t) = 0;
    virtual uint32_t getYTarget() = 0;
    virtual void setZTarget(uint32_t) = 0;
    virtual uint32_t getZTarget() = 0;
    virtual void setMagnet(bool) = 0;
    virtual bool getMagnet() = 0;
    virtual void setRotDelta(uint32_t) = 0;
    virtual uint32_t getRotDelta() = 0;
    virtual void setShootConfetti(bool) = 0;
    virtual bool getShootConfetti() = 0;
    virtual void setLed(RGB rgb) = 0;
    virtual RGB getLed(void) = 0;

};


}