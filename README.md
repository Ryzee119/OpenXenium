


# OpenXenium
![OpenXenium Assembled](https://github.com/Ryzee119/OpenXenium/blob/master/Images/openxenium2.jpg?raw=true)
## About this Repository
This is an open source recreation of an Original Xbox LPC memory/IO device. Generally known as a *modchip*.
The VHDL is 100% compatible with XeniumOS, which is a legal non-Microsoft based bootloader made by TeamXodus for the Original Xbox. This allows loading user BIOS binaries and some basic Xbox tools (EEPROM Backup/modification, Hard drive Rebuilding, Locking, Unlocking, FTP access).  
  
The VHDL written by Team Xodus was never released and the CPLD is read protected so it is not trivial to extract the bitstream.
My own VHDL is probably quite similar to what Team Xodus produced for the Xenium Modchip but I have independently reverse engineered the behavioral design on the CPLD. (There was no bypassing of the protection features of a genuine Xenium.) Therefore I have named this project **OpenXenium**. *An open source recreation of the Xenium CPLD released under GPL which documents the internal workings by means of open source VHDL of the Xenium Modchip*.

I do a quick run through of the build process here: https://youtu.be/P6YYViKby74

To manage your OpenXenium device's flash memory, the [Xenium-Tools](https://github.com/Ryzee119/Xenium-Tools) homebrew program has been moved (here){https://github.com/Ryzee119/Xenium-Tools}.

## Supported Features
The VHDL implements the following LPC transactions:
  * Memory Read/Writes (Translated to a parallel flash memory interface)
  * IO Read/Writes

From an Original Xbox perspective, this covers all requirements to make a custom LPC memory/IO peripheral for the console. Colloquially known as a *modchip* which can load custom BIOS files like Cromwell and its derivatives without modifying the onboard TSOP flash memory or softmodding. This supports IO control for accessories like LCD screens, LED controllers, external switches etc.

When used with XeniumOS the VHDL in this repo supports the following features:
  * Software controlled BIOS bank switching (any BIOS combinations that fit into 1mbyte of flash memory (4x256k, 1x512+2x256k, 2x512k, 1x1mbyte).
  * Instant boot to a chosen BIOS using the power button.
  * Boot the XeniumOS with eject button.
  * Ability to boot from the onboard BIOS (colloquially known as TSOP booting). This will completely disable OpenXenium and release D0/LFRAME(1.6) to boot the Xbox as if stock.
  * All the standard features in XeniumOS. (EEPROM tools, Hard drive tools, FTP/SMB Server etc.).
  * Three general purpose outputs. These are normally bitbanged as a SPI master (MOSI,CLK,CS) with the most common use an LCD, however could support any SPI peripheral in theory.
  * Two general purpose inputs. On a Xenium, these are I believe intended to be MISO lines to complement the SPI interface but could be used for any 3.3V digital logic.  
  * Three outputs are connected to an RGB led (Or an external user added RGB led).
  * Reserved memory on the flash chip for non-volatile storage of an EEPROM backup and XeniumOS settings.
  * If you bridge the two recovery pins on power up, it will attempt to boot the XeniumOS recovery BIOS if available. This functions the same as a genuine Xenium modchip.
  * I also simulate the LFRAME abort mechanism (*Ref Intel LPC Interface Spec Rev 1.1 Section 4.3.1.13*) so that this will work on a v1.6 Original Xbox. This aborts the LPC transaction to prevent the Xyclops responding to the MCPX LPC Memory Read requests during boot (and conflicting with an external LPC memory peripheral). This is generally accepted to be better than shorting LFRAME the ground constantly which some traditional *Modchips* do.

## XeniumOS BIOS
**The recommended way to get a copy of XeniumOS is to take a backup of your own Xenium modchip using `xenium-tools` provided in this repo.** It is also possible to parse the v2.3.1 XeniumOS update files released by Team Xodus to extract the neccessary data; however this will not contain the factory programmed recovery sector, but otherwise works in the same way.

This has been tested with XeniumOS 2.3.1 (Last release) and XeniumOS 2.3.1(Gold variant). The only way to obtain Gold OS is to dump the flash contents of your XeniumGold as the binary files were never distributed individually.

## Installation and Initial Setup
See [Installation.MD](https://github.com/Ryzee119/OpenXenium/blob/master/INSTALLATION.md)
## Licensing
OpenXenium is free and open source. Please respect the licenses available in their respective folders.
  *  Hardware is shared under the [CERN OHL version 1.2.](https://ohwr.org/cernohl).
  *  Firmware is shared under [GPLv3](https://www.gnu.org/licenses/quick-guide-gplv3.en.html).
  *  xenium-tools was made with [NXDK](https://github.com/XboxDev/nxdk) and is shared under [GPLv2](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html).
## References that helped me in this project
  * [Intel LPC Spec](https://www.intel.com/content/dam/www/program/design/us/en/documents/low-pin-count-interface-specification.pdf)
  *  [NXDK](https://github.com/XboxDev/nxdk)  
  * [AladdinLCD  Github Repo (CPLD LCD Driver)](https://github.com/Ryzee119/AladdinLCD)
  * [LPC Analyser Plugin](https://github.com/Ryzee119/LPCAnalyzer)  
  * [Deconstructing the Xbox Boot ROM](https://mborgerson.com/deconstructing-the-xbox-boot-rom/)

![Open Xenium Installed](https://github.com/Ryzee119/OpenXenium/blob/master/Images/20191018_212705.jpg?raw=true)

If you like my work please consider a small donation<br>
[![paypal](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=49HV7N8QH9KQ8&currency_code=AUD&source=url)<br>
By Ryzee119
