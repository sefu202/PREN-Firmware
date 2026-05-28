/**
 * @file LedFlash.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief LED Flashlight
 * @version 0.1
 * @date 2026-05-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "main.h"
#include <cstdint>


/**
 * @class LedFlash
 * @brief Controls a flashlight/LED with timeout protection.
 *
 * This class enables or disables a flashlight output and stores
 * the activation timestamp using HAL_GetTick(). The flashlight
 * automatically turns off after a configured timeout.
 */
class LedFlash
{
public:

    /**
     * @brief Constructor.
     * @param timeoutMs Timeout in milliseconds before automatic shutdown.
     */
    explicit LedFlash(uint32_t timeoutMs);

    /**
     * @brief Enable or disable the flashlight.
     *
     * When enabled, the current system tick is stored using HAL_GetTick().
     *
     * @param state true = ON, false = OFF
     */
    void enable(bool state);

    /**
     * @brief Periodic update function.
     *
     * Must be called regularly (e.g. main loop) to enforce timeout.
     */
    void update();

    /**
     * @brief returns state
     * 
     * @return true flash enabled
     * @return false flash disabled
     */
    bool o() const;

private:

    bool m_isEnabled;        /**< Current flashlight state */
    bool m_o = false;        /**< Output variable */
    uint32_t m_startTick;    /**< Timestamp when LED was enabled */
    uint32_t m_timeout;      /**< Auto-off timeout in milliseconds */
};
