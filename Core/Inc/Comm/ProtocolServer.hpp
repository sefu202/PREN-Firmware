/**
 * @file ProtocolServer.hpp
 * @author Josef Aschwanden (josef.aschwanden@stud.hslu.ch)
 * @brief Server for Object Placement Machine Protocol
 * @version 0.1
 * @date 2025-11-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "Comm/TcpServer.hpp"
#include "Comm/Protocol.hpp"
#include "ProcessImage/IProcessImage.hpp"
#include <array> 

namespace Comm {

class ProtocolServer {
public:

    /**
     * @brief Constructor, opens new protocol server
     * @param port tcp port
     */
    ProtocolServer(uint16_t port, ProcessImage::IProcessImage& processImage);

    /**
     * @brief Periodic update of the server
     * 
     */
    void update();

private:

    /**
     * @brief Processes a received protocol and creates a response
     * 
     * @param recvdProtocol received protocol reference
     * @param responseProtocol response protocol out reference
     */
    void processRecvdProtocol(const Protocol& recvdProtocol, Protocol& responseProtocol);

    /**
     * @brief Processes a received SetTarget protocol
     * 
     * @param recvdProtocol received protocol reference
     * @return target coords in steps
     */
    uint32_t processSetTargetProtocol(const Protocol& recvdProtocol);


    /**
     * @brief Processes the enableVacuum protocol
     * 
     * @param recvdProtocol received protocol reference
     * @return true vacuum should be enabled
     * @return false vacuum should be disabled
     */
    bool processEnableVacuum(const Protocol& recvdProtocol);


    /**
     * @brief Processes the SetRot protocol
     * 
     * @param recvdProtocol received protocol reference
     * @return int32_t rotation in steps
     */
    int32_t processSetRotProtocol(const Protocol& recvdProtocol);


    /**
     * @brief Process the SetLed protocol
     * 
     * @param recvdProtocol received protocol reference
     * @return RGB led color and intensity
     */
    RGB processSetLedProtocol(const Protocol& recvdProtocol);


    /**
     * @brief Creates a state reply
     * 
     * @param responseProtocol protocol to fill
     */
    void createStateReply(Protocol& responseProtocol);


    /**
     * @brief Creates a other reply
     * 
     * @param responseProtocol protocol to fill
     * @param state state byte
     */
    void createOtherReply(Protocol& responseProtocol, uint8_t state);

    TcpServer tcpserver;
    uint16_t m_port;

    ProcessImage::IProcessImage& processImage;

    std::array<uint8_t, 1024> m_buffer;
    Protocol m_unfinishedProtocol;

};

}