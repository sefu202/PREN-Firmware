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
    bool prev = false;
    bool rising = false;

    bool operator()(bool current);

    operator bool() const;
};