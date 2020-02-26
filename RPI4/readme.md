# Raspberry PI 4 Model B0 Baremetal Programming sandbox.

## About

Baremetal programming sandbox for RPI4.


## sample uart0

uart output sample. Using 14, 15 with input / output command console. Tada!

## sample v3d_power_on

![v3dpoweron](./Image/v3dpoweron.png)

V3D block power on sample for RPI4.

During cold boot and warm boot, the V3D AXI bus is not visible, and it becomes 0xDEADBEEF from V3D Registers.
RPI3 could solve this by sending the V3D powerDomain ON command to firmware, but RPI4 could not.

This is a sample that sets the ASB register and makes the V3D register visible with reference to the Raspbian Linux kernel.
ref:https://github.com/raspberrypi/linux/issues/3046
ref:https://github.com/raspberrypi/linux/blob/75f1d14cee8cfb1964cbda21a30cb030a632c3c2/drivers/soc/bcm/bcm2835-power.c#L153

Todo:adapt reboot command.


## Sample program LICENSE

MIT License
https://opensource.org/licenses/mit-license.php

## AStyle options

http://astyle.sourceforge.net/

astyle <sources> --indent=tab --style=linux --indent=force-tab --indent-after-parens --indent-col1-comments --pad-header --pad-oper

## Author

gyabo(aka yasai kumaashi)

