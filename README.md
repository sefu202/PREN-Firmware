# PREN-Firmware

Firmware for the PREN (Projekt Robotik und Embedded Systems) Object Placement Machine, developed for the STM32F746/756 microcontroller.

## Description

This firmware controls a robotic object placement system featuring:
- 5-axis stepper motor control (X, Y, Z, Z-twin, C-rotation)
- Vacuum-based object pickup system
- Ethernet communication via LWIP stack
- Safety systems including emergency stop and limit switches
- RGB LED indicators
- Optional confetti cannon for celebrations

The system communicates with a Python-based control unit over TCP/IP for coordinated object placement operations.

## Features

### Motion Control
- **Linear Axes**: X, Y, Z axes with configurable acceleration, speed limits, and length
- **Rotary Axis**: C-axis for object rotation
- **Stepper Motors**: 5 stepper motors with microstepping support
- **Limit Switches**: 8 limit switches for position safety
- **Emergency Stop**: Emergency stop functionality

### Actuators
- **Vacuum System**: Pump and valve control for object pickup/release
- **Confetti Cannon**: Optional celebratory feature

### Communication
- **TCP Server**: Protocol-based communication on port 36769
- **Process Image**: Real-time data exchange with control unit
- **Python Interface**: ControlUnit.py for high-level control

### Safety & Monitoring
- **Debounced Inputs**: All digital inputs are debounced
- **RGB LEDs**: Status indication via PWM-controlled LEDs
- **Heartbeat**: LED blinking for system health monitoring

## Requirements

### Hardware
- STM32F756 microcontroller
- 5 stepper motor drivers (configured for 1/4 or 1/16 microstepping)
- Ethernet interface
- Limit switches and emergency stop button
- Vacuum pump and solenoid valve
- RGB LEDs (optional)
- Confetti cannon (optional)

### Software
- **Toolchain**: arm-none-eabi-gcc (GCC ARM Embedded)
- **Build System**: CMake 3.22+
- **Flashing Tool**: OpenOCD
- **ST-Link Programmer**: For flashing via JTAG/SWD
- **STM32CubeMX**: Version 1.15 (used to generate project files)

### Development Environment
- Linux/Windows/macOS
- VS Code with CMake Tools extension (recommended)
- Python 3.x for control unit interface

## Building

### Prerequisites
Ensure you have the ARM GCC toolchain 14.3 rel1 installed and in your PATH:

```bash

# Or download from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

### Build Steps
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd PREN-Firmware
   ```

2. Use STM32CubeMX 6.15 to generate the external code

3. Build the project using CMake

The compiled ELF file will be located at `build/PREN-Firmware`.

## Usage

### Hardware Setup
1. Connect stepper motors to the configured pins
2. Wire limit switches to digital inputs DI3-DI10
3. Connect emergency stop to DI1
4. Configure Ethernet settings in LWIP configuration
5. Connect vacuum pump and valve to DO1 and DO2
6. Optional: Connect RGB LEDs to PWM outputs

### Software Operation
1. Power on the system
2. The firmware initializes stepper motors and network
3. Use the Python ControlUnit.py script to connect and control the machine:
   ```python
   from ControlUnit import ControlUnit

   cu = ControlUnit()
   cu.connect()

   # Move axes
   cu.setXTarget(1000)
   cu.setYTarget(2000)

   # Control vacuum
   cu.setVacuum(True)

   # Check status
   pi = cu.getProcessImage()
   print(f"X Position: {pi.xPositionSteps}")
   ```
- Note: The methods are asynchronous. To wait for a operation to be finished, the process image needs to be polled.

### Network Configuration (configured in LWIP settings)
- **IP Address**: 10.0.1.1
- **Port**: 36769
- **Protocol**: Custom binary protocol

## Architecture

### Core Components
- **Main Loop**: `application.cpp` - Main control logic
- **Process Image**: `ProcessImage.hpp/cpp` - Data exchange interface
- **Communication**: `ProtocolServer.hpp/cpp` - TCP server implementation
- **Motion Control**: `Stepper.hpp/cpp`, `LinearAxis.hpp/cpp` - Motor control

### Directory Structure (after code generation)
```
Core/
├── Inc/           # Header files
├── Src/           # Source files
│   ├── Actuators/ # Vacuum, confetti cannon
│   ├── Comm/      # Network communication
│   ├── LinearAxis/# Axis control
│   ├── ProcessImage/# Data interface
│   ├── Stepper/   # Stepper motor control
│   └── Util/      # Utilities (debounce, PWM)
Drivers/           # STM32 HAL drivers
LWIP/             # Lightweight IP stack
Middlewares/      # Third-party middleware
```

## Configuration

### Stepper Motor Settings
Modify stepper configurations in `application.cpp`:
- Microstepping mode (M0/M1 pins)
- Acceleration and speed limits
- Axis lengths and initialization

### Network Settings
Configure LWIP in STM32CubeMX

### Pin Assignments
GPIO pin assignments are configured via STM32CubeMX (PREN-Firmware.ioc)


## Authors

- Josef Aschwanden (josef.aschwanden@stud.hslu.ch)

## Acknowledgments

- Based on STM32CubeMX generated code
- Uses LWIP lightweight TCP/IP stack
- Developed as part of PREN project