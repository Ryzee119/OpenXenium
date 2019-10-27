# Xenium-Tools XBE
![XeniumTools Mainmenu](https://github.com/Ryzee119/OpenXenium/blob/master/Images/xenium-tools_mainmenu.png?raw=true)
## Features
Xenium-Tools is a tool which allows various Xenium related things.
  * Dump the entire 2MB flash contents of a Xenium Device.
  * Write a raw 2MB flash dump to a Xenium Device. It must have the file name `flash.bin`.
  * Parse then write a XeniumOS update file. These are called `recovery.bin`. This feature only supports v2.3.1.
  * Toggle the RGB LED as a simple test.

Whilst these tools will work for a genuine Xenium and OpenXenium, it is only recommended to perform write commands to a Genuine only if it is non-functioning. This can bring back genuine Xeniums that have a corrupt flash for example.

## Limitations
  * Xenium-Tools does not check the validity of `flash.bin` or `recovery.bin` the user is responsible to ensure these are correct.
  * `recovery.bin` is  the name of the update files released by Team Xodus. These are normally used with the recovery switch on the Xenium to update the OS. Xenium-Tools can parse and write v2.3.1 only. The only downside is that this file does not contain the recovery parition. So if you only use this to write to your OpenXenium the recovery feature will not work.
  * If writing a blank OpenXenium or attempting to repair a corrupt Genuine Xenium, you need to boot your Xbox with a working modchip to actually open this program. At which point you need to hotspot the modchip.

## Building
 * Install [NXDK](https://github.com/XboxDev/nxdk) and its dependencies.
 * Install cmake.  i.e. `sudo apt-get install cmake`.
 * Make sure that the NXDK_DIR variable in the Makefile points to correct place.
 * Compile xenium-tools using `make` from within the xenium-tools directory.
 * This will generate an xbe which you need to transfer to your Xbox.

By Ryzee119
