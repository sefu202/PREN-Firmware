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
#include <array> 

namespace Comm {

class ProtocolServer {
public:

    /**
     * @brief Constructor, opens new protocol server
     * @param port tcp port
     */
    ProtocolServer(uint16_t port);

    /**
     * @brief Periodic update of the server
     * 
     */
    void update();

private:

    TcpServer tcpserver;

    uint16_t m_port;

    std::array<uint8_t, 1024> m_buffer;

    Protocol m_unfinishedProtocol;

};

}