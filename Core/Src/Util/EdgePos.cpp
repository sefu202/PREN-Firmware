/**
 * @file EdgePos.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Edge pos implementation
 * @version 0.1
 * @date 2025-12-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include "Util/EdgePos.hpp"

bool EdgePos::operator()(bool current){
    if (!initialized) {
        prev = current;
        initialized = true;
    }
    rising = current && !prev;
    prev = current;
    return rising;
}

EdgePos::EdgePos(bool requireInit) {
    initialized = !requireInit;
}

EdgePos::operator bool() const {
    return rising;
}