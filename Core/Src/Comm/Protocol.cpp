/**
 * @file Protocol.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Protocol container implementation
 * @version 0.1
 * @date 2025-11-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "Comm/Protocol.hpp"
#include <cstring> // memcpy
#include <cassert>

namespace Comm {

const uint8_t *Protocol::serialize() const {
    return m_data.data();
}

void Protocol::appendData(const uint8_t *data, uint8_t len) {
    if (len < (m_data.size() - m_lenFilled)) {
        std::memcpy(m_data.data() + m_lenFilled, data, len);
        m_lenFilled += len;
    }
    else {
        assert(0); // not good
    }
}

void Protocol::appendByte(const uint8_t byte) {
    if (1 < (m_data.size() - m_lenFilled)) {
        m_data[m_lenFilled++] = byte;
    }
    else {
        assert(0);
    }
}

uint8_t Protocol::getOpcode() const {
    return m_data[0];
}
void Protocol::setOpcode(uint8_t opcode) {
    m_data[0] = opcode;
    if (m_lenFilled == 0) {
        m_lenFilled = 1;
    }
}

uint8_t Protocol::getLength() const {
    return (m_lenFilled >= 2) ? m_data[1] : 0;
}

void Protocol::setLength(uint8_t len) {
    m_data[1] = len;

    if (m_lenFilled < 2) {
        m_lenFilled = 2;
    }
}


const uint8_t* Protocol::getData() const {
    return m_data.data()+2;
}

uint8_t Protocol::getFillLevel() const {
    return m_lenFilled;
}

void Protocol::clear() {
    m_lenFilled = 0;
    m_data[0] = 0;
    m_data[1] = 0;
}


}
 
 
 
 
