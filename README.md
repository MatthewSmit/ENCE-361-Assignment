# Helicopter Rig Control Project

## Project structure

The project is sorted into various subfolders.
```
.
├── doc - Doxygen generated documentation
├── lib - Third party libraries.
│   └── libOrbitOled
├── python - Python script and data files for PID controller tuning.
│   └── data
├── src - The source code.
└── test - Various test programs for checking functionality.
```

## Source files

```
.
├── ...
├── src
│   ├── buttons.c - Buttons module with debouncing.
│   ├── flight_controller.c - Handles flight states and critical tasks.
│   ├── height.c - Module to acquire the current height.
│   ├── height_controller.c - PID controller for the main rotor.
│   ├── main.c - Initialisation code and entry point.
│   ├── oled_interface.c - A simple interface to the OLED library.
│   ├── pid.c - Generic PID controller module.
│   ├── pwm.c - Module handling PWM output to the rotors.
│   ├── reset.c - Soft reset module.
│   ├── serial_interface.c - A interface to output serial data.
│   ├── switch.c - Switch module with debouncing.
│   ├── yaw.c - Module to handle changes in yaw and detect reference yaw.
│   ├── yaw_controller.c - PID controller for the tail rotor.
│   └── ...
└── ...
```

## Contributers

Daniel van Wichen <dpv11@uclive.ac.nz>

Matthew Smit <mjs351@uclive.ac.nz>

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
