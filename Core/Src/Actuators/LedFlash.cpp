#include "Actuators/LedFlash.hpp"
LedFlash::LedFlash(uint32_t timeoutMs)
    : m_isEnabled(false),
      m_o(false),
      m_startTick(0),
      m_timeout(timeoutMs) {}

void LedFlash::enable(bool state) {
    if (state) {
        if (!m_isEnabled) {
            m_startTick = HAL_GetTick();
            m_isEnabled = true;
            m_o = true;
        }
    } else {
        m_isEnabled = false;
        m_o = false;
    }
}

void LedFlash::update() {
    if (m_isEnabled) {
        if ((HAL_GetTick() - m_startTick) >= m_timeout) {
            m_o = false;
        }
    }
}

bool LedFlash::o() const {
    return m_o;
}