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


    // X-Axis (Stepper 1)
    Stepper::Stepper::PinConfig step1PinConfig;
    step1PinConfig.stepGpio = STEP1_STEP_GPIO_Port;
    step1PinConfig.dirGpio = STEP1_DIR_GPIO_Port;
    step1PinConfig.stepPin = STEP1_STEP_Pin;
    step1PinConfig.dirPin = STEP1_DIR_Pin;
    Stepper::Stepper step1(step1PinConfig);

    // Y-Axis (Stepper 2)
    Stepper::Stepper::PinConfig step2PinConfig;
    /*step2PinConfig.stepGpio = STEP2_STEP_GPIO_Port;
    step2PinConfig.dirGpio = STEP2_DIR_GPIO_Port;
    step2PinConfig.stepPin = STEP2_STEP_Pin;
    step2PinConfig.dirPin = STEP2_DIR_Pin;*/
    Stepper::Stepper step2(step2PinConfig);

    // Z-Axis (Stepper 3)
    Stepper::Stepper::PinConfig step3PinConfig;
    /*step3PinConfig.stepGpio = STEP3_STEP_GPIO_Port;
    step3PinConfig.dirGpio = STEP3_DIR_GPIO_Port;
    step3PinConfig.stepPin = STEP3_STEP_Pin;
    step3PinConfig.dirPin = STEP3_DIR_Pin;*/
    Stepper::Stepper step3(step3PinConfig);

    // Rotation-Axis (Stepper 4)
    Stepper::Stepper::PinConfig step4PinConfig;
    /*step4PinConfig.stepGpio = STEP4_STEP_GPIO_Port;
    step4PinConfig.dirGpio = STEP4_DIR_GPIO_Port;
    step4PinConfig.stepPin = STEP4_STEP_Pin;
    step4PinConfig.dirPin = STEP4_DIR_Pin;*/
    Stepper::Stepper step4(step4PinConfig);

    HAL_GPIO_WritePin(STEP1_EN_GPIO_Port, STEP1_EN_Pin, GPIO_PIN_SET);
    /*HAL_GPIO_WritePin(STEP2_EN_GPIO_Port, STEP2_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(STEP3_EN_GPIO_Port, STEP3_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(STEP4_EN_GPIO_Port, STEP4_EN_Pin, GPIO_PIN_SET);*/

    HAL_GPIO_WritePin(STEP1_M0_GPIO_Port, STEP1_M0_Pin, GPIO_PIN_SET);  // half step
    // HAL_GPIO_WritePin(STEP1_M1_GPIO_Port, STEP1_M1_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(STEP1_M2_GPIO_Port, STEP1_M2_Pin, GPIO_PIN_SET);

    //HAL_GPIO_WritePin(STEP2_M0_GPIO_Port, STEP2_M0_Pin, GPIO_PIN_SET);  // half step
    // HAL_GPIO_WritePin(STEP2_M1_GPIO_Port, STEP2_M1_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(STEP2_M2_GPIO_Port, STEP2_M2_Pin, GPIO_PIN_SET);

    //HAL_GPIO_WritePin(STEP3_M0_GPIO_Port, STEP3_M0_Pin, GPIO_PIN_SET);  // half step
    // HAL_GPIO_WritePin(STEP3_M1_GPIO_Port, STEP3_M1_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(STEP3_M2_GPIO_Port, STEP3_M2_Pin, GPIO_PIN_SET);

    //HAL_GPIO_WritePin(STEP4_M0_GPIO_Port, STEP4_M0_Pin, GPIO_PIN_SET);  // half step
    // HAL_GPIO_WritePin(STEP4_M1_GPIO_Port, STEP4_M1_Pin, GPIO_PIN_SET);
    //HAL_GPIO_WritePin(STEP4_M2_GPIO_Port, STEP4_M2_Pin, GPIO_PIN_SET);

    LinearAxis xAxis  (step1, 50, 1250, 50, 1000000);  // a, maxSpeed, initSpeed, length
    LinearAxis yAxis  (step2, 50, 1250, 50, 1000000);  // a, maxSpeed, initSpeed, length
    LinearAxis zAxis  (step3, 50, 1250, 50, 1000000);  // a, maxSpeed, initSpeed, length
    xAxis.init();
    yAxis.init();
    zAxis.init();


    // Process Image
    ProcessImage::ProcessImage processImage(xAxis, yAxis, zAxis);
    
    // Communication
    Comm::ProtocolServer server(36769, processImage);


    Debounce limSw[6];


    while(1) {

        MX_LWIP_Process();

        server.update();

        bool usrBtn = HAL_GPIO_ReadPin(USER_BTN_GPIO_Port, USER_BTN_Pin) == GPIO_PIN_SET ? true : false;
        bool estop = usrBtn;
        processImage.setEstop(estop);

        limSw[0](!HAL_GPIO_ReadPin(LIM_SW_X_LOW_GPIO_Port, LIM_SW_X_LOW_Pin));
        limSw[1](!HAL_GPIO_ReadPin(LIM_SW_X_HIGH_GPIO_Port, LIM_SW_X_HIGH_Pin));

        uint8_t limSwBF = 0;
        for (uint8_t i = 0; i < 6; i++) {
            limSwBF |= (limSw[i] << i);
        }

        processImage.setLimitSwitches(limSwBF);
        processImage.setStartButton(false);

        xAxis.update(limSw[0], limSw[1]);
        yAxis.update(limSw[2], limSw[3]);
        zAxis.update(limSw[4], limSw[5]);

        if (estop){
            xAxis.estop();
            yAxis.estop();
            zAxis.estop();
        }

        processImage.update();

        HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
    }
    return 0;
}