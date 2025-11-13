#include "application.h"
#include "main.h"
#include "lwip.h"
#include "gpio.h"
#include "Comm/UdpServer.hpp"
#include "Comm/TcpServer.hpp"


extern "C" int application(void){
    Comm::UdpServer udpserver;
    udpserver.udp_server_init();

    Comm::TcpServer tcpserver(30000);

    while(1) {
        if (tcpserver.getState() != Comm::TcpServer::State::LISTEN || tcpserver.getState() != Comm::TcpServer::State::ACCEPTED){
            tcpserver.listen(30000);
        }
        MX_LWIP_Process();
    }
    return 0;
}