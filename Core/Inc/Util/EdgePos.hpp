/**
 * @file EdgePos.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Positive edge detection
 * @version 0.1
 * @date 2025-12-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

struct EdgePos{

    /**
     * @brief Constructor
     * 
     * @param requireInit set requireInit to true to initialise the Object to the current state at the first scan
     * @note if requireInit is not set, setting current as true at the first cycle generates a Positive edge 
     */
    EdgePos(bool requireInit = false);

    bool prev = false;
    bool rising = false;
    bool initialized = false;

    bool operator()(bool current);

    operator bool() const;
};