/**
 * @file Protocol.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Protocol container
 * @version 0.1
 * @date 2025-11-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

#include <array>
#include <cstdint>

namespace Comm {

class Protocol {
public:

    /**
     * @brief Serializes the protocol. Data must immediatly be used
     * (just returns a pointer to data because it is stored in serialized form)
     * 
     * @return uint8_t* pointer to data
     */
    const uint8_t *serialize()const;

    /**
     * @brief 
     * 
     * @param data 
     * @param len 
     */
    void appendData(const uint8_t *data, uint8_t len);


    /**
     * @brief appends a single byte to the protocol
     * 
     * @param byte byte to append
     */
    void appendByte(const uint8_t byte);

    /**
     * @brief Get the Opcode object
     * 
     * @return uint8_t 
     */
    uint8_t getOpcode() const;

    /**
     * @brief Set the Opcode
     * 
     * @param opcode opcode
     * 
     */
    void setOpcode(uint8_t opcode);

    /**
     * @brief Get the Length object
     * 
     * @return uint8_t 
     */
    uint8_t getLength() const;

    /**
     * @brief Get the Data
     * 
     * @return const uint8_t* pointer to data
     */
    const uint8_t* getData() const;

    /**
     * @brief Set the Length object
     * 
     * @param len 
     */
    void setLength(uint8_t len);

    uint8_t getFillLevel() const;

    /**
     * @brief Clears data
     * 
     */
    void clear();

private:

    std::array<uint8_t,256> m_data;
    uint8_t m_lenFilled = 0;
};



}