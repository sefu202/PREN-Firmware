#include "application.h"
#include "main.h"
#include "lwip.h"
#include "gpio.h"
#include "Comm/UdpServer.hpp"
#include "Comm/TcpServer.hpp"
#include <cstring>
#include "Stepper/Stepper.hpp"

using namespace Stepper;

extern "C" int application(void){
    Comm::UdpServer udpserver;
    udpserver.udp_server_init();

    Comm::TcpServer tcpserver(30000);

    uint8_t buffer[1024];
    tcpserver.receive(buffer,sizeof(buffer));

    Stepper::Stepper::PinConfig step1PinConfig;
    step1PinConfig.stepGpio = STEP1_STEP_GPIO_Port;
    step1PinConfig.dirGpio = STEP1_DIR_GPIO_Port;
    step1PinConfig.stepPin = STEP1_STEP_Pin;
    step1PinConfig.dirPin = STEP1_DIR_Pin;
    Stepper::Stepper step1(step1PinConfig);

    HAL_GPIO_WritePin(STEP1_EN_GPIO_Port, STEP1_EN_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(STEP1_M0_GPIO_Port, STEP1_M0_Pin, GPIO_PIN_SET);
   // HAL_GPIO_WritePin(STEP1_M1_GPIO_Port, STEP1_M1_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(STEP1_M2_GPIO_Port, STEP1_M2_Pin, GPIO_PIN_SET);

    step1.step(100);


    uint8_t speed = 0;

    while(1) {
        if (tcpserver.getState() != Comm::TcpServer::State::LISTEN || tcpserver.getState() != Comm::TcpServer::State::ACCEPTED){
            tcpserver.listen(30000);
        }

        if (tcpserver.getReceivedBytes()) {
            tcpserver.write(buffer, tcpserver.getReceivedBytes());
            tcpserver.receive(buffer,sizeof(buffer));
        }

        step1.setSpeed(speed);
        step1.step(-10);
        MX_LWIP_Process();
    }
    return 0;
}