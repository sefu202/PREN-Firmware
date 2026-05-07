/**
 * @file MotionTimer.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief This class allows timing linear axises to reach their setpoint position at the same time
 * @version 0.1
 * @date 2026-05-07
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#include <cstdint>

#define MAX_TICKS_RECCOMEND 10000   // 10s

class MotionTimer {
public:
    MotionTimer(uint16_t a, uint16_t maxSpeed);

    /**
     * @brief Calculates the minimum needed time with no MotionTimer limitation
     * 
     * @param d distance to move
     * @return uint32_t time in ticks needed at minimum
     */
    uint32_t tMin(uint32_t d);


    void reccomendETATick(uint32_t etaTick);

    uint16_t vReccomend(uint32_t d);

    bool hasNewEtaTick();

private:
    uint16_t m_maxSpeed, m_vReccomend = UINT16_MAX;
    uint32_t m_a;
    uint32_t m_etaTick = 0;
    bool m_newEtaTick = false;

};