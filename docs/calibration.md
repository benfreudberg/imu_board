# Calibration

There are two calibration procedures, one for the accelerometer and gyroscope, and one for the magnetometer. Refer to the game-die-face description in the [mounting document](./mounting.md) for references below and for button labels.

## Calibrating the Gyroscope and Accelerometer

For all steps, it is important that the device is as still as possible. Exact orientation is less important.

1. Put the device flat on a table, press and hold B2 and then press B1. Then release B2. You'll see that the green LED is now doing a short flash periodically. You have now captured your gyroscope calibration data and the first of your `6` sets of accelerometer data.
1. Put the device in a new orientation such that a different face is pointing up. I do this by pressing the board against the side of a table. Press B1. You'll now see a slightly longer flash on the green LED.
1. Repeat `step 2` 4 more times until you have done it once for all 6 faces pointing up. The order does not matter, but you must not repeat any faces. (For face 6, you can press it against the underside of a table.)
1. After the final face, the calculations for calibration will be performed and the new calibration values will be saved to non-volatile memory. They will be loaded each time the device is powered on.

If you do a bad job (too much motion, repeating a face, etc.), the device will reject the calibration and all the LEDs will start flashing. At this point, you must power cycle the device and try again. If you are in the middle of the process and know you've made a mistake, you can power cycle and start again without finishing. No values are saved until you complete the whole process successfully.

## Calibrating the Magnetometer

The magnetometer is sensitive to any sources of magnetic fields and must be kept away from any such sources, *especially* during calibration. Some objects you think are safe may not be, for example, one of my wooden desks causes inaccurate readings if I put the device too close. It is best to keep the device at least `1 foot` away from any other objects besides your own hands during magnetometer calibration. It is ok to calibrate after mounting if you were careful about mounting location.

1. Hold the device with one of the 6 faces pointed up. Press and hold B1 and then press B2. Then release B1. Wait for the yellow LED to flash rapidly. Then rotate the device around the vertical axis until the yellow LED stops flashing rapidly. It should take one full rotation. Try to keep the device as level as possible during this rotation and only rotate in one direction. Which direction does not matter, but don't change in the middle of one face's worth of data. You have now captured your first of `6` sets of magnetometer data.
1. Rotate the device so that a different face is pointed up. Press B2 and wait for the yellow LED to flash rapidly. Rotate around the vertical axis again. *Note, it is always the vertical axis with respect to the world, not the device.*
1. Repeat `step 2` 4 more times until you have done it once for all 6 faces pointing up. The order does not matter, but you must not repeat any faces.
1. After the final face, the calculations for calibration will be performed and the new calibration values will be saved to non-volatile memory. They will be loaded each time the device is powered on.

If you do a bad job (not keeping it level while rotating, repeating a face, getting too close to a strong source of interference, etc.), the device will reject the calibration and all the LEDs will start flashing. At this point, you must power cycle the device and try again. If you are in the middle of the process and know you've made a mistake, you can power cycle and start again without finishing. No values are saved until you complete the whole process successfully.
