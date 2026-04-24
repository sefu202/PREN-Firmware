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
#include "Actuators/Vacuum.hpp"
#include "Actuators/ConfettiCannon.hpp"
#include "Util/SoftPWM.hpp"

using namespace Stepper;


void readDI(std::array<bool, 13>& DI) {
    DI[ 0] = HAL_GPIO_ReadPin(USER_BTN_GPIO_Port, USER_BTN_Pin) == GPIO_PIN_SET ? true : false;
    DI[ 1] = !(HAL_GPIO_ReadPin(DI1_GPIO_Port, DI1_Pin) == GPIO_PIN_SET ? true : false);
    DI[ 2] = !(HAL_GPIO_ReadPin(DI2_GPIO_Port, DI2_Pin) == GPIO_PIN_SET ? true : false);
    DI[ 3] = !(HAL_GPIO_ReadPin(DI3_GPIO_Port, DI3_Pin) == GPIO_PIN_SET ? true : false);
    DI[ 4] = !(HAL_GPIO_ReadPin(DI4_GPIO_Port, DI4_Pin) == GPIO_PIN_SET ? true : false);
    DI[ 5] = !(HAL_GPIO_ReadPin(DI5_GPIO_Port, DI5_Pin) == GPIO_PIN_SET ? true : false);
    DI[ 6] = !(HAL_GPIO_ReadPin(DI6_GPIO_Port, DI6_Pin) == GPIO_PIN_SET ? true : false);
    DI[ 7] = !(HAL_GPIO_ReadPin(DI7_GPIO_Port, DI7_Pin) == GPIO_PIN_SET ? true : false);
    DI[ 8] = !(HAL_GPIO_ReadPin(DI8_GPIO_Port, DI8_Pin) == GPIO_PIN_SET ? true : false);
    DI[ 9] = !(HAL_GPIO_ReadPin(DI9_GPIO_Port, DI9_Pin) == GPIO_PIN_SET ? true : false);
    DI[10] = !(HAL_GPIO_ReadPin(DI10_GPIO_Port, DI10_Pin) == GPIO_PIN_SET ? true : false);
    DI[11] = !(HAL_GPIO_ReadPin(DI11_GPIO_Port, DI11_Pin) == GPIO_PIN_SET ? true : false);
    DI[12] = !(HAL_GPIO_ReadPin(DI12_GPIO_Port, DI12_Pin) == GPIO_PIN_SET ? true : false);
}

void writeDO(const std::array<bool, 13>& DO) {
    HAL_GPIO_WritePin( DO1_GPIO_Port,  DO1_Pin, DO[ 1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin( DO2_GPIO_Port,  DO2_Pin, DO[ 2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin( DO3_GPIO_Port,  DO3_Pin, DO[ 3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin( DO4_GPIO_Port,  DO4_Pin, DO[ 4] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    /* Written by PWM!
    HAL_GPIO_WritePin( DO5_GPIO_Port,  DO5_Pin, DO[ 5] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin( DO6_GPIO_Port,  DO6_Pin, DO[ 6] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin( DO7_GPIO_Port,  DO7_Pin, DO[ 7] ? GPIO_PIN_SET : GPIO_PIN_RESET);*/
    HAL_GPIO_WritePin( DO8_GPIO_Port,  DO8_Pin, DO[ 8] ? GPIO_PIN_SET : GPIO_PIN_RESET); 
    /* Written by PWM */
    /*HAL_GPIO_WritePin( DO9_GPIO_Port,  DO9_Pin, DO[ 9] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DO10_GPIO_Port, DO10_Pin, DO[10] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DO11_GPIO_Port, DO11_Pin, DO[11] ? GPIO_PIN_SET : GPIO_PIN_RESET); */ 
    HAL_GPIO_WritePin(DO12_GPIO_Port, DO12_Pin, DO[12] ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

extern "C" int application(void){


    // X-Axis (Stepper 1)
    Stepper::Stepper::PinConfig step1PinConfig;
    step1PinConfig.stepGpio = STEP1_STEP_GPIO_Port;
    step1PinConfig.stepPin = STEP1_STEP_Pin;
    step1PinConfig.dirGpio = STEP1_DIR_GPIO_Port;
    step1PinConfig.dirPin = STEP1_DIR_Pin;
    Stepper::Stepper step1(step1PinConfig);

    // Y-Axis (Stepper 2)
    Stepper::Stepper::PinConfig step2PinConfig;
    step2PinConfig.stepGpio = STEP2_STEP_GPIO_Port;
    step2PinConfig.stepPin = STEP2_STEP_Pin;
    step2PinConfig.dirGpio = STEP2_DIR_GPIO_Port;
    step2PinConfig.dirPin = STEP2_DIR_Pin;
    Stepper::Stepper step2(step2PinConfig);

    // Z-Axis (Stepper 3)
    Stepper::Stepper::PinConfig step3PinConfig;
    step3PinConfig.stepGpio = STEP3_STEP_GPIO_Port;
    step3PinConfig.stepPin = STEP3_STEP_Pin;
    step3PinConfig.dirGpio = STEP3_DIR_GPIO_Port;
    step3PinConfig.dirPin = STEP3_DIR_Pin;
    Stepper::Stepper step3(step3PinConfig);

    // Twin Z-Axis (Stepper 4)
    Stepper::Stepper::PinConfig step4PinConfig;
    step4PinConfig.stepGpio = STEP4_STEP_GPIO_Port;
    step4PinConfig.stepPin = STEP4_STEP_Pin;
    step4PinConfig.dirGpio = STEP4_DIR_GPIO_Port;
    step4PinConfig.dirPin = STEP4_DIR_Pin;
    Stepper::Stepper step4(step4PinConfig);

    // C-Axis (Stepper 5)
    Stepper::Stepper::PinConfig step5PinConfig;
    step5PinConfig.stepGpio = STEP5_STEP_GPIO_Port;
    step5PinConfig.stepPin = STEP5_STEP_Pin;
    step5PinConfig.dirGpio = STEP5_DIR_GPIO_Port;
    step5PinConfig.dirPin = STEP5_DIR_Pin;
    Stepper::Stepper step5(step5PinConfig);


    // Set stepper modes
    HAL_GPIO_WritePin(STEP1_M0_GPIO_Port, STEP1_M0_Pin, GPIO_PIN_SET);  // 1/4 step
    HAL_GPIO_WritePin(STEP1_M1_GPIO_Port, STEP1_M1_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(STEP2_M0_GPIO_Port, STEP2_M0_Pin, GPIO_PIN_SET);  // 1/4 step
    HAL_GPIO_WritePin(STEP2_M1_GPIO_Port, STEP2_M1_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(STEP3_M0_GPIO_Port, STEP3_M0_Pin, GPIO_PIN_SET);  // 1/4 step
    HAL_GPIO_WritePin(STEP3_M1_GPIO_Port, STEP3_M1_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(STEP4_M0_GPIO_Port, STEP4_M0_Pin, GPIO_PIN_SET);  // 1/4 step
    HAL_GPIO_WritePin(STEP4_M1_GPIO_Port, STEP4_M1_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(STEP5_M0_GPIO_Port, STEP5_M0_Pin, GPIO_PIN_SET);  // 1/16 step
    HAL_GPIO_WritePin(STEP5_M1_GPIO_Port, STEP5_M1_Pin, GPIO_PIN_RESET);

    // m0 set, m1 reset: 200 steps / r
    // m0 reset, m1 set: 400 steps / r

    LinearAxis xAxis        (step1, 50, 5000, 250,   10000);  // a, maxSpeed, initSpeed, length
    LinearAxis yAxis        (step2, 50, 5000, 250,   10000);  // a, maxSpeed, initSpeed, length
    LinearAxis zAxis        (step3, 50, 1250, 250, 1000000);  // a, maxSpeed, initSpeed, length
    LinearAxis zAxisTwin    (step4, 50, 1250, 250, 1000000);  // a, maxSpeed, initSpeed, length
    xAxis.init();
    yAxis.init();
    zAxis.init();
    zAxisTwin.init();

    Stepper::Stepper& cAxis = step5;
    cAxis.setSpeed(5000);



    // Process Image
    ProcessImage::ProcessImage processImage(xAxis, yAxis, zAxis, zAxisTwin, cAxis);
    
    // Communication
    Comm::ProtocolServer server(36769, processImage);

    std::array<bool,13> DI; // DI[0] is user button on nucleo!
    std::array<bool,13> DO; // DO[0] unused!
    std::array<Debounce, 8> limSw;
    EdgePos eStopReleasedEdge;
    Debounce btnStart;

    // Vacuum Subsystem
    Vacuum vacuum;

    // Confetti Cannon Subsystem
    ConfettiCannon cannon;

    // RGB
    SoftPWM::PinConfig ledRedConfig = {
        .gpio = DO5_GPIO_Port,
        .pin = DO5_Pin
    };
    SoftPWM::PinConfig ledGreenConfig = {
        .gpio = DO6_GPIO_Port,
        .pin = DO6_Pin
    };
    SoftPWM::PinConfig ledBlueConfig = {
        .gpio = DO7_GPIO_Port,
        .pin = DO7_Pin
    };
    SoftPWM::PinConfig ledRedConfig2 = {
        .gpio = DO9_GPIO_Port,
        .pin = DO9_Pin
    };
    SoftPWM::PinConfig ledGreenConfig2 = {
        .gpio = DO10_GPIO_Port,
        .pin = DO10_Pin
    };
    SoftPWM::PinConfig ledBlueConfig2 = {
        .gpio = DO11_GPIO_Port,
        .pin = DO11_Pin
    };
    SoftPWM ledRed(ledRedConfig);
    SoftPWM ledGreen(ledGreenConfig);
    SoftPWM ledBlue(ledBlueConfig);
    SoftPWM ledRed2(ledRedConfig2);
    SoftPWM ledGreen2(ledGreenConfig2);
    SoftPWM ledBlue2(ledBlueConfig2);

    HAL_Delay(500);

    while(1) {

        MX_LWIP_Process();

        server.update();
        readDI(DI);

        const bool estop = !DI[1] || DI[0];  // ESTOP or Nucleo User Button
        processImage.setEstop(estop);

        btnStart(DI[2]);
        processImage.setStartButton(btnStart);

        limSw[0](!DI[ 3]);
        limSw[1](!DI[ 4]);
        limSw[2](!DI[ 5]);
        limSw[3](!DI[ 6]);
        limSw[4](!DI[ 7]);
        limSw[5](!DI[ 8]);
        limSw[6](!DI[ 9]);
        limSw[7](!DI[10]);

        uint8_t limSwBF = 0;
        for (uint8_t i = 0; i < 8; i++) {
            limSwBF |= (limSw[i] << i);
        }

        processImage.setLimitSwitches(limSwBF);

        xAxis.update(limSw[0], limSw[1]);
        yAxis.update(limSw[2], limSw[3]);
        zAxis.update(limSw[4], limSw[5]);
        zAxisTwin.update(false,false);

        // Emergency Stop
        if (estop){
            xAxis.estop();
            yAxis.estop();
            zAxis.estop();
            zAxisTwin.estop();
        }
        HAL_GPIO_WritePin(STEP1_ENABLE_GPIO_Port, STEP1_ENABLE_Pin, estop ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(STEP2_ENABLE_GPIO_Port, STEP2_ENABLE_Pin, estop ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(STEP3_ENABLE_GPIO_Port, STEP3_ENABLE_Pin, true ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(STEP4_ENABLE_GPIO_Port, STEP4_ENABLE_Pin, true ? GPIO_PIN_RESET : GPIO_PIN_SET);
        HAL_GPIO_WritePin(STEP5_ENABLE_GPIO_Port, STEP5_ENABLE_Pin, estop ? GPIO_PIN_RESET : GPIO_PIN_SET);

        // Initialize after E-Stop
        eStopReleasedEdge(!estop);
        if (eStopReleasedEdge) {
            xAxis.init();
            yAxis.init();
            zAxis.init();
            zAxisTwin.init();
        }

        // Vacuum System
        vacuum.update(processImage.getVacuum(), estop);
        cannon.update(processImage.getShootConfetti(), estop);

        // RGB LED
        ledRed.set(processImage.getLed().r);
        ledGreen.set(processImage.getLed().g);
        ledBlue.set(processImage.getLed().b);
        ledRed2.set(processImage.getLed().r);
        ledGreen2.set(processImage.getLed().g);
        ledBlue2.set(processImage.getLed().b);


        DO[1] = vacuum.oRunPump();   // Vacuum Pump
        DO[2] = vacuum.oEnableValve();   // Valve
        DO[3] = cannon.oIgnite(); // confetti cannon
        // DO[5] = red
        //DO[6] = green
        //DO[7] = blue
        // DO[9] = red2
        //DO[10] = green2
        //DO[11] = blue2


        writeDO(DO);

        processImage.update();
    }
    return 0;
}