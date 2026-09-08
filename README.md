# ECE 3 Line-Following Robot

A UCLA ECE 3 team course project that implemented feedback control on a provided differential-drive robot. The robot followed a marked course, handled gradual turns, detected transverse bars, executed a U-turn, and stopped after returning to the starting point.

<!-- Project photo will be added at assets/ece3-robot.jpg. -->

## Key results

- Followed the assigned route through gradual curves using an eight-sensor infrared reflectance array.
- Detected transverse bars from the combined sensor response.
- Executed a pivot-style U-turn at the first bar and stopped after reaching the bar again on the return trip.
- Improved tracking behavior through repeated on-track tuning and module-by-module debugging.

## Control approach

1. Calibrate and normalize the eight infrared sensor readings.
2. Apply asymmetric sensor weights to estimate lateral line-position error.
3. Calculate proportional and derivative corrections from the current and previous errors.
4. Adjust the left and right motor PWM commands around a base speed.
5. Constrain both motor commands to the valid PWM range of 0–255.

The archived final controller uses proportional and derivative terms. An integral term was not active in this version.

## Testing and debugging

The team tuned the controller empirically by running the robot on the physical track, observing tracking error and oscillation, and adjusting the control response. When the robot became unstable or lost the line, we disabled and tested software sections independently to isolate the problem before reintegrating the full behavior.

## My contribution

This was a collaborative project without a rigid division of software responsibilities. I participated in discussions of the control logic, repeated track testing, gain tuning, and module-level debugging. The course supplied the robot as a fully assembled hardware platform, so the project focused on sensing, feedback control, testing, and behavioral integration rather than mechanical construction.

## Repository contents

- `src/ece3_line_follower.ino` — archived Arduino-style control program.
- `assets/ece3-robot.jpg` — photo of the course robot.

## Notes

The program depends on the UCLA ECE 3 course library (`ECE3.h`) and the corresponding course robot platform. The source file was reconstructed from a text copy retained after the course; identifier formatting lost during copy and paste was normalized without intentionally changing the control behavior.

