/**
 * @file ProtocolServer.cpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Implementation of protocol server for Object placement machine
 * @version 0.1
 * @date 2025-11-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Comm/ProtocolServer.hpp"
#include <algorithm> // min
#include <cassert>

namespace Comm {
/*

ProtocolServer::ProtocolServer(uint16_t port) 
 : tcpserver(port), m_port(m_port){
    tcpserver.receive(m_buffer.data(), m_buffer.size());
}

void ProtocolServer::update(){
    if (tcpserver.getState() != Comm::TcpServer::State::LISTEN || tcpserver.getState() != Comm::TcpServer::State::ACCEPTED){
        tcpserver.listen(m_port);

        LOG_WARN("Reopen Tcp Server");
    }

    const uint16_t receivedBytes = tcpserver.getReceivedBytes();
    uint16_t readIdx = 0;
    while(readIdx < receivedBytes) {

        auto len = m_unfinishedProtocol.getLenght();
        if (len) {
            const uint8_t bytesToRead = std::min(len - m_unfinishedProtocol.getFillLevel(), receivedBytes - readIdx);
            m_unfinishedProtocol.appendData(m_buffer.data() + readIdx, bytesToRead);
            readIdx += bytesToRead;
        }
        else if (m_unfinishedProtocol.getFillLevel() == 1) {
            m_unfinishedProtocol.setLength(m_buffer[readIdx++]);
        }
        else if (m_unfinishedProtocol.getFillLevel() == 0){
            m_unfinishedProtocol.setOpcode(m_buffer[readIdx++]);
        }
        else {
            assert(0);
            // bad
        }

        len = m_unfinishedProtocol.getLenght();

        // Protocol is full and can be processed further
        if (m_unfinishedProtocol.getFillLevel() == len && len) {
            // todo
            m_unfinishedProtocol.serialize(); m_unfinishedProtocol.getLenght();


        }

        assert(readIdx <= receivedBytes);
    }

    // If data has been received and processed, prepare to receive new data
    if (readIdx > 0) {
        tcpserver.receive(m_buffer.data(), m_buffer.size());
    }
}*/

}