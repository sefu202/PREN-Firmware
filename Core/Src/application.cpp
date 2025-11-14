#include "application.h"
#include "main.h"
#include "lwip.h"
#include "gpio.h"
#include "Comm/UdpServer.hpp"
#include "Comm/TcpServer.hpp"
#include <cstring>

extern "C" int application(void){
    Comm::UdpServer udpserver;
    udpserver.udp_server_init();

    Comm::TcpServer tcpserver(30000);

    uint8_t buffer[1024];
    tcpserver.receive(buffer,sizeof(buffer));

    while(1) {
        if (tcpserver.getState() != Comm::TcpServer::State::LISTEN || tcpserver.getState() != Comm::TcpServer::State::ACCEPTED){
            tcpserver.listen(30000);
        }

        if (tcpserver.getReceivedBytes()) {
            tcpserver.write(buffer, tcpserver.getReceivedBytes());
            tcpserver.receive(buffer,sizeof(buffer));
        }

        MX_LWIP_Process();
    }
    return 0;
}