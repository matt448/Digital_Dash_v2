#!/bin/bash

# This script creates links for init scripts that need to be started at boot

# Make sure only root can run our script
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi


cd /etc/init.d && ln -s /home/ubuntu/Digital_Dash_v2/etc/init.d/tft_update tft_update
cd /etc/init.d && ln -s /home/ubuntu/Digital_Dash_v2/etc/init.d/arduino_serial_read arduino_serial_read

cd /etc/rc2.d && ln -s ../init.d/arduino_serial_read S90arduino_serial_read
cd /etc/rc2.d && ln -s ../init.d/tft_update S99tft_update

cd /etc/rc3.d && ln -s ../init.d/arduino_serial_read S90arduino_serial_read
cd /etc/rc3.d && ln -s ../init.d/tft_update S99tft_update

cd /etc/rc4.d && ln -s ../init.d/arduino_serial_read S90arduino_serial_read
cd /etc/rc4.d && ln -s ../init.d/tft_update S99tft_update

cd /etc/rc5.d && ln -s ../init.d/arduino_serial_read S90arduino_serial_read
cd /etc/rc5.d && ln -s ../init.d/tft_update S99tft_update
