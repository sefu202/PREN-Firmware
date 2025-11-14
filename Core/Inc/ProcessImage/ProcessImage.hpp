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

#include "IProcessImage.hpp"

namespace ProcessImage {

class ProcessImage : public IProcessImage{
public:
    void setXTarget(uint32_t) override;
    uint32_t getXTarget() override;
    void setYTarget(uint32_t) override;
    uint32_t getYTarget() override;
    void setZTarget(uint32_t) override;
    uint32_t getZTarget() override;
    void setMagnet(bool) override;
    bool getMagnet() override;
    void setRotDelta(uint32_t) override;
    uint32_t getRotDelta() override;
    void setShootConfetti(bool) override;
    bool getShootConfetti() override;
    void setLed(RGB rgb) override;
    RGB getLed(void) override;

private:

    
    bool m_confetti;
    RGB m_led;
};


}