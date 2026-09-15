# Autonomous Obstacle-Avoidance Car

A fully custom-built RC car featuring a 3D-printed chassis (including custom fasteners), designed in Onshape and controlled by an Arduino Uno. The car supports both manual IR remote driving and autonomous obstacle avoidance via an ultrasonic distance sensor.

## Features

- **IR Remote Control** — directional driving (forward / backward / left / right) plus a live emergency stop that interrupts any running motor command mid-execution
- **Ultrasonic Obstacle Detection (HC-SR04)** — automatically halts forward motion when an object is detected within a configurable minimum distance
- **Dual H-Bridge Motor Control** with independent per-motor PWM, correcting a ~20% speed discrepancy between the two drive motors that was causing veering
- **Configurable Smooth Stopping** — toggle between a gradual PWM ramp-down ("slow stop") and immediate braking
- **Rear-Wheel-Drive Layout** — redesigned from the original layout after part constraints, improving weight distribution and structural support from the printed wheels
- Full system wiring documented in a **KiCad schematic**, covering sensor, motor driver, and power/ground distribution

## Hardware

- Arduino Uno
- HC-SR04 Ultrasonic Sensor
- IR Receiver + Remote
- Dual-motor H-Bridge Driver
- Custom 3D-printed chassis (Onshape CAD, custom fasteners)

## Tools

- **Onshape** — chassis and mechanical design
- **KiCad** — circuit schematic and wiring documentation
- **Arduino IDE / C++** — firmware
- **IRremote** library — IR signal decoding

## Pin Mapping

| Component            | Pin(s)         |
|-----------------------|----------------|
| Ultrasonic Trig       | 12             |
| Ultrasonic Echo       | 13             |
| IR Receiver           | 2              |
| Motor 1 Enable (PWM)  | 5              |
| Motor 1 IN1 / IN2     | 6 / 7          |
| Motor 2 IN3 / IN4     | 8 / 9          |
| Motor 2 Enable (PWM)  | 10             |

## Controls

| Button        | Action                        |
|---------------|-------------------------------|
| UP            | Drive forward                 |
| DOWN          | Drive backward                |
| LEFT (3)      | Turn left                     |
| RIGHT (2)     | Turn right                    |
| STOP          | Emergency stop mid-motion     |
| 4             | Enable slow (ramped) stop     |
| 5             | Disable slow stop (hard brake)|
| 1             | Print current distance reading|

## How It Works

The car listens for IR remote signals in the main loop and dispatches drive commands accordingly. While driving forward, it continuously polls the ultrasonic sensor and halts automatically if an obstacle comes within the minimum safe distance. All drive commands run in a time-boxed loop that checks for a STOP signal on every iteration, allowing the remote to interrupt movement instantly. Per-motor PWM values are tuned independently to compensate for mechanical differences between the two drive motors.

## Future Improvements

- Integrate an **MPU6050** IMU for more accurate turning and orientation tracking
- Use **EEPROM** to store and run presaved routes
- Replace polling-based IR decode with interrupt-driven handling
- Add PID-based speed correction instead of fixed PWM offsets
- Expand obstacle avoidance to include automatic turning/rerouting instead of a hard stop
