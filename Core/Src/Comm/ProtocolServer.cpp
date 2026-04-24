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


ProtocolServer::ProtocolServer(uint16_t port, ProcessImage::IProcessImage& processImage) 
 : tcpserver(port), m_port(port), processImage(processImage){
    tcpserver.receive(m_buffer.data(), m_buffer.size());
}

void ProtocolServer::update(){
    if (tcpserver.getState() != Comm::TcpServer::State::LISTEN && tcpserver.getState() != Comm::TcpServer::State::ACCEPTED){
        tcpserver.listen(m_port);

        // todo warn ("Reopen Tcp Server");
    }

    const uint16_t receivedBytes = tcpserver.getReceivedBytes();
    uint16_t readIdx = 0;
    while(readIdx < receivedBytes) {

        auto len = m_unfinishedProtocol.getLength();
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
            // bad todo warn
        }

        len = m_unfinishedProtocol.getLength();

        // Protocol is full and can be processed further
        if (m_unfinishedProtocol.getFillLevel() == len && len) {
            Protocol response;
            processRecvdProtocol(m_unfinishedProtocol, response);
            m_unfinishedProtocol.clear();
            tcpserver.write(response.serialize(), response.getLength());
        }

        assert(readIdx <= receivedBytes); // todo
        // todo add receive timeout to close dead connection
    }

    // If data has been received and processed, prepare to receive new data
    if (readIdx > 0) {
        tcpserver.receive(m_buffer.data(), m_buffer.size());
    }
}


void ProtocolServer::processRecvdProtocol(const Protocol& recvdProtocol, Protocol& responseProtocol) {
    const uint8_t len = recvdProtocol.getLength();
    const uint8_t opcode = recvdProtocol.getOpcode();
    responseProtocol.clear();

    uint32_t xTarget, yTarget, zTarget;
    bool enableVacuum;
    int32_t rot;
    RGB led;
    
    if (len) {
        switch(opcode) {
            case 0: // Echo

                // Create echo reply
                responseProtocol.appendData(recvdProtocol.serialize(), len);
                responseProtocol.setOpcode(0);
                responseProtocol.setLength(len);
                break;

            case 1: // GetState
                createStateReply(responseProtocol);
                break;

            case 2: // SetXTarget
                xTarget = processSetTargetProtocol(recvdProtocol);
                if (!processImage.setXTarget(xTarget)) {
                    createOtherReply(responseProtocol, 1); // error code 1: wrong data
                }
                else {
                    createOtherReply(responseProtocol, 0);
                }
                break;
                
            case 3: // SetYTarget
                yTarget = processSetTargetProtocol(recvdProtocol);
                if (!processImage.setYTarget(yTarget)) {
                    createOtherReply(responseProtocol, 1); // error code 1: wrong data
                }
                else {
                    createOtherReply(responseProtocol, 0);
                }

                break;

            case 4: // SetZTarget
                zTarget = processSetTargetProtocol(recvdProtocol);
                if (!processImage.setZTarget(zTarget)) {
                    createOtherReply(responseProtocol, 1); // error code 1: wrong data
                }
                else {
                    createOtherReply(responseProtocol, 0);
                }

                break;

            case 5: // EnableVacuum
                enableVacuum = processEnableVacuum(recvdProtocol);
                processImage.setVacuum(enableVacuum);
                createOtherReply(responseProtocol, 0);
                break;

            case 6: // SetRot
                rot = processSetRotProtocol(recvdProtocol);
                if (!processImage.setRot(rot)) {
                    createOtherReply(responseProtocol, 1); // error code 1: wrong data
                }
                else {
                    createOtherReply(responseProtocol, 0);
                }
                break;

            case 7: // ShootConfetti
                processImage.setShootConfetti(true);
                createOtherReply(responseProtocol, 0);
                break;

            case 8: // SetLed
                led = processSetLedProtocol(recvdProtocol);
                processImage.setLed(led);
                createOtherReply(responseProtocol, 0);
                break;

            default:
                // error code 2: wrong opcode
                createOtherReply(responseProtocol, 2);
                // todo close connection
                break;
        }
    }

}

uint32_t ProtocolServer::processSetTargetProtocol(const Protocol& recvdProtocol) {
    uint32_t ret = 0;
    if (recvdProtocol.getLength() == 6){
        const uint8_t *data = recvdProtocol.getData();
        for (uint8_t i = 0; i < 4; i++) {
            ret |= static_cast<uint32_t>(data[i]) << ((3-i)*8);
        }
    }
    else {
        ret = UINT32_MAX;
    }
    return ret;
}

bool ProtocolServer::processEnableVacuum(const Protocol& recvdProtocol) {
    bool ret = false;
    if (recvdProtocol.getLength() == 3){
        const uint8_t *data = recvdProtocol.getData();
        ret = !!(*data);
    }
    return ret;
}

int32_t ProtocolServer::processSetRotProtocol(const Protocol& recvdProtocol) {
    int32_t ret = 0;
    if (recvdProtocol.getLength() == 6){
        const uint8_t *data = recvdProtocol.getData();
        for (uint8_t i = 0; i < 4; i++) {
            ret |= static_cast<int32_t>(data[i]) << ((3-i)*8);
        }
    }
    return ret;
}

RGB ProtocolServer::processSetLedProtocol(const Protocol& recvdProtocol) {
    RGB ret = {0,0,0};

    if (recvdProtocol.getLength() == 5){
        const uint8_t *data = recvdProtocol.getData();
        ret.r = data[0];
        ret.g = data[1];
        ret.b = data[2];
    }
    return ret;
}


void ProtocolServer::createStateReply(Protocol& responseProtocol) {
    responseProtocol.setOpcode(1);
    responseProtocol.setLength(23);
    responseProtocol.appendByte(processImage.getEstop());
    responseProtocol.appendByte(processImage.getStartButton());
    responseProtocol.appendByte(processImage.getLimitSwitches());
    responseProtocol.appendByte(processImage.getVacuumState());

    const uint32_t xPosition = processImage.getXPosition();
    const uint32_t yPosition = processImage.getYPosition();
    const uint32_t zPosition = processImage.getZPosition();

    for (uint8_t i = 0; i < 4; i++) {
        responseProtocol.appendByte((xPosition >> ((3-i)*8)) & 0xff);
    }

    for (uint8_t i = 0; i < 4; i++) {
        responseProtocol.appendByte((yPosition >> ((3-i)*8)) & 0xff);
    }

    for (uint8_t i = 0; i < 4; i++) {
        responseProtocol.appendByte((zPosition >> ((3-i)*8)) & 0xff);
    }

    const int32_t rotPosition = processImage.getRotPosition();

    for (uint8_t i = 0; i < 4; i++) {
        responseProtocol.appendByte((rotPosition >> ((3-i)*8)) & 0xff);
    }

    responseProtocol.appendByte(processImage.isInitialized());
    
}


void ProtocolServer::createOtherReply(Protocol& responseProtocol, uint8_t state) {
    responseProtocol.setOpcode(255);
    responseProtocol.setLength(3);
    responseProtocol.appendByte(state);
}

}