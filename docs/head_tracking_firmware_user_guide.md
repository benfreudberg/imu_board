# Head Tracking firmware user guide

* This head-tracker is designed to be used with the OpenTrack head tracking software available here: https://sourceforge.net/projects/opentrack.mirror/  
Please install this first.

* When you plug in the device, it will appear to your computer as a USB gamepad. You can test its function by opening the `Set up usb game controllers` tool in windows. You should see an active signal on the X, Y, and Z rotation signals as you rotate the device.

* Copy [imu_headtracker.ini](../other/imu_headtracker.ini) into `/Documents/opentrack-2.3`

* Launch OpenTrack and select `imu_headtracker.ini` as your profile. `Input` should say `Joystick input`. Click the hammer and select the correct device if it's not selected already. Click `Start` and you should see the little octopus guy moving around as you rotate the device.

* There are a lot of settings you can play around with, but first you should take care of
  * <font size="5"> [calibration](./calibration.md)
  * [mounting](./mounting.md) </font>


* After you've mounted it and are ready to go, sit with your head in a neutral position and press B0 (refer to the [mounting document](./mounting.md) for button labels) - this will set your yaw zero point. It is probably best not to use the OpenTrack zeroing function, but you can try it if you want.

* If you power up the device while it is near a source of magnetic interference, it will take some time for the readings to drift back to correct values. You can either wait it out or power cycle the device when it is clear of magnetic interference. This is also a problem in the much less likely scenario that it is undergoing significant acceleration when it is turned on.
