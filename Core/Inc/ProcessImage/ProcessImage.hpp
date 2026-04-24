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
#include "LinearAxis/LinearAxis.hpp"
#include "Stepper/Stepper.hpp"

namespace ProcessImage {

class ProcessImage : public IProcessImage{
public:

    ProcessImage(LinearAxis& xAxis, LinearAxis& yAxis, LinearAxis& zAxis, LinearAxis& zAxisTwin, Stepper::Stepper& cAxis);

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

    void setEstop(bool estop);
    bool getEstop() override;
    void setStartButton(bool startButton);
    bool getStartButton() override;
    void setLimitSwitches(uint8_t limSw);
    uint8_t getLimitSwitches() override;
    bool getVacuumState() override;
    uint32_t getXPosition() override;
    uint32_t getYPosition() override;
    uint32_t getZPosition() override;
    int32_t getRotPosition() override;
    bool isInitialized() override;


    void update();

private:

    LinearAxis &m_xAxis, &m_yAxis, &m_zAxis, &m_zAxisTwin;
    Stepper::Stepper &m_cAxis;
    
    bool m_shootConfetti = false, m_vacuum = false, m_estop = false, m_startButton = false;
    uint8_t m_limSw = 0;
    RGB m_led;

    int32_t m_rotSetPoint = 0;
};


}