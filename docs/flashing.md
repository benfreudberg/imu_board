# Flashing a new binary to the device

If you've changed code and built a new binary, there are two ways to load it onto the device. If you have a ST-link V2 programmer like [this one](https://www.amazon.com/HiLetgo-Emulator-Downloader-Programmer-STM32F103C8T6/dp/B07SQV6VLZ/ref=sr_1_4?dchild=1&keywords=stlink+v2&qid=1607575790&sr=8-4), you can use it along with either of these pieces of free software:
* [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
* [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) (also good for editing, building, and debugging code)

If you don't have one, you can still load a binary via USB with the STM32CubeProgrammer software. I'm only going to discuss this method because I assume that if you're got an ST-link V2 programmer, you already know how to use it or can find out elsewhere.

1. While the device is *unplugged*, press and hold B0 (refer to the [mounting document](./mounting.md) for button labels).
1. While holding B0, plug in the device, then release B0. No LEDs will come on, that's OK.
1. Open the STM32CubeProgrammer software (it's OK if it was already open).
1. Click the `refresh button` under `USB configuration` - `Port` should be populated with `USB1`.
1. Click `Connect`.
1. Click `Open File` and then select the `.bin` binary file you want to load.
1. Click `Download`.
1. When it's done, click `Disconnect`.
1. Unplug the device, and then press B0 *while it is unplugged*.
1. Plug it back in and it will be running the new firmware.

The default firmware binary can be found [here](./../latest_binaries/headtracker_firmware.bin).
