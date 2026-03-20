/**
 * @file ProcessImage.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Process Image implementation
 * @version 0.1
 * @date 2026-03-10
 * 
 * @copyright Copyright (c) 2026
 * 
 */

 #include "ProcessImage/ProcessImage.hpp"

namespace ProcessImage {


ProcessImage::ProcessImage(LinearAxis& xAxis, LinearAxis& yAxis, LinearAxis& zAxis) 
  : m_xAxis(xAxis), m_yAxis(yAxis), m_zAxis(zAxis) {

}

bool ProcessImage::setXTarget(uint32_t target) {
    if (target > m_xAxis.getLength()) {
        return false;
    }

    m_xAxis.moveTo(target);
    return true;
}

uint32_t ProcessImage::getXTarget() {
    // todo remove
    return 0;
}

bool ProcessImage::setYTarget(uint32_t target) {
    if (target > m_yAxis.getLength()) {
        return false;
    }

    m_yAxis.moveTo(target);
    return true;
}

uint32_t ProcessImage::getYTarget() {
    // todo remove
    return 0;
}

bool ProcessImage::setZTarget(uint32_t target) {
    if (target > m_zAxis.getLength()) {
        return false;
    }

    m_zAxis.moveTo(target);
    return true;
}

uint32_t ProcessImage::getZTarget() {
    // todo remove
    return 0;
}

bool ProcessImage::setVacuum(bool vacuum) {
    m_vacuum = vacuum;
    return true;
}

bool ProcessImage::getVacuum() {
    return m_vacuum;
}

bool ProcessImage::setRot(int32_t) {
    return false;
}

uint32_t ProcessImage::getRotDelta() {
    // todo
    return 0;
}

bool ProcessImage::setShootConfetti(bool shootConfetti) {
    m_shootConfetti = shootConfetti;
    return true;
}

bool ProcessImage::getShootConfetti() {
    return m_shootConfetti;
}

bool ProcessImage::setLed(RGB rgb) {
    m_led = rgb;
    return true;
}

RGB ProcessImage::getLed(void) {
    return m_led;
}


void ProcessImage::update() {
    // todo reset shootconfetti
}

void ProcessImage::setEstop(bool estop) {
    m_estop = estop;

    if (estop) {
        m_vacuum = false;   // reset vacuum on estop
        m_shootConfetti = false; // reset confetti cannon on estop
    }
}

bool ProcessImage::getEstop() {
    return m_estop;
}

void ProcessImage::setStartButton(bool startButton) {
    m_startButton = startButton;
}

bool ProcessImage::getStartButton() { 
    return m_startButton;
}

void ProcessImage::setLimitSwitches(uint8_t limSw) {
    m_limSw = limSw;
}

uint8_t ProcessImage::getLimitSwitches() {
    return m_limSw;
}

bool ProcessImage::getVacuumState() {
    return getVacuum();
}

uint32_t ProcessImage::getXPosition() {
    return m_xAxis.getCurrentPosition();
}

uint32_t ProcessImage::getYPosition() {
    return m_yAxis.getCurrentPosition();
}

uint32_t ProcessImage::getZPosition() { 
    return m_zAxis.getCurrentPosition();
}

int32_t ProcessImage::getRotPosition() {
    // todo
    return 0;
}

bool ProcessImage::isInitialized() { 
    return m_xAxis.isInitialized() && m_yAxis.isInitialized() && m_zAxis.isInitialized();
}

}