# Helicopter Rig Control Project

## Project structure

The project is sorted into various subfolders.
- doc - Doxygen generated documentation
- lib - The libraries used.
- python - The python script and data files for the PID controller tuning.
- src - The source code.
- test - Various test programs for checking functionality.

## Source files
The files in src are sorted into various modules.
- buttons.c - Module operating button presses.
- flight_controller.c - Handles moving between flight modes.
- height.c - Module to acquire the current height.
- height_controller.c - PID controller for the main rotor.
- main.c - Initialisation code and entry point.
- oled_interface.c - A simple interface to the OLED library.
- pid.c - A generic PID controller.
- pwm.c - A PWM module to handle outputting power to the rotors.
- reset.c - Soft reset module.
- serial_interface.c - A interface to output serial data.
- switch.c - A module to handle the mode switch.
- yaw.c - A module to handle changes in yaw and detect reference yaw.
- yaw_controller.c - PID controller for the tail rotor.

## Contributers

Daniel van Wichen <dpv11@uclive.ac.nz>

Matthew Smit

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details