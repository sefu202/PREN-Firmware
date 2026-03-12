#include "application.h"
#include "main.h"
#include "lwip.h"
#include "gpio.h"
//#include "Comm/TcpServer.hpp"
#include "Comm/ProtocolServer.hpp"
#include "ProcessImage/ProcessImage.hpp"
#include <cstring>
#include "Stepper/Stepper.hpp"
#include "Stepper/StepperMusicPlayer.hpp"
#include "LinearAxis/LinearAxis.hpp"
#include "Util/Debounce.hpp"

using namespace Stepper;

extern "C" int application(void){

    // Process Image
    ProcessImage::ProcessImage processImage;

    
    // Communication

    /*
    Comm::TcpServer tcpserver(30000);

    uint8_t buffer[1024];
    tcpserver.receive(buffer,sizeof(buffer)); */
    Comm::ProtocolServer server(36769, processImage);



    // X-Axis (Stepper 1)
    Stepper::Stepper::PinConfig step1PinConfig;
    step1PinConfig.stepGpio = STEP1_STEP_GPIO_Port;
    step1PinConfig.dirGpio = STEP1_DIR_GPIO_Port;
    step1PinConfig.stepPin = STEP1_STEP_Pin;
    step1PinConfig.dirPin = STEP1_DIR_Pin;
    Stepper::Stepper step1(step1PinConfig);

    HAL_GPIO_WritePin(STEP1_EN_GPIO_Port, STEP1_EN_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(STEP1_M0_GPIO_Port, STEP1_M0_Pin, GPIO_PIN_SET);  // half step
   // HAL_GPIO_WritePin(STEP1_M1_GPIO_Port, STEP1_M1_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(STEP1_M2_GPIO_Port, STEP1_M2_Pin, GPIO_PIN_SET);

    LinearAxis xAxis(step1, 50, 1250, 200, 1000000);    // a, maxSpeed, initSpeed, length
    xAxis.init();



    EdgePos initializedPos;
    Debounce limSwXLow, limSwXHigh;


    while(1) {
        /*if (tcpserver.getState() != Comm::TcpServer::State::LISTEN && tcpserver.getState() != Comm::TcpServer::State::ACCEPTED){
            tcpserver.listen(30000);
        }

        if (tcpserver.getReceivedBytes()) {
            tcpserver.write(buffer, tcpserver.getReceivedBytes());
            tcpserver.receive(buffer,sizeof(buffer));
        }*/
       

        MX_LWIP_Process();

        server.update();


        bool usrBtn = HAL_GPIO_ReadPin(USER_BTN_GPIO_Port, USER_BTN_Pin) == GPIO_PIN_SET ? true : false;

        limSwXLow(!HAL_GPIO_ReadPin(LIM_SW_X_LOW_GPIO_Port, LIM_SW_X_LOW_Pin));
        limSwXHigh(!HAL_GPIO_ReadPin(LIM_SW_X_HIGH_GPIO_Port, LIM_SW_X_HIGH_Pin));


        xAxis.update(limSwXLow, limSwXHigh);
        if (usrBtn){
            xAxis.estop();
        }

        HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
    }
    return 0;
}