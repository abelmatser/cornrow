# Cornrow
--- *the most sophisticated bluetooth audio receiver for linux*

Cornrow is a bluetooth audio daemon designed for embedded devices or low-powered boards like the Raspberry Pi. It accepts bluetooth audio sources and transforms your computer into a bluetooth speaker.

It is made for Debian based environments and compatible with Ubuntu 18.04 Bionic and Debian Stretch (and later). This means that this service runs as a dedicated user and can be cleanly installed and removed using Debian package management.

All this is in an early stage. However, i am willing to provide an easy out-of-the-box solution for anyone. So, your experience with this little daemon is very appreciated. Please leave your comments and issues using it. Thanks a lot.

The daemon has a built-in equalizer which can be remote controlled via this app:
[corocomo](https://play.google.com/store/apps/details?id=org.cornrow.corocomo)
![alt text](https://github.com/mincequi/cornrow/blob/master/data/screenshot_1.png)

## Installation (binary)
Compiled debian/ubuntu packages are available for [armhf](https://github.com/mincequi/cornrow/releases/download/v0.4.0/cornrowd_0.4.0_armhf.deb) and [amd64](https://github.com/mincequi/cornrow/releases/download/v0.4.0/cornrowd_0.4.0_amd64.deb).

Get dependencies first
```
sudo apt update
sudo apt install \
  gstreamer1.0-libav \
  gstreamer1.0-plugins-bad \
  gstreamer1.0-plugins-good \
  libgstreamermm-1.0 \
  libqt5bluetooth5 \
  libqt5network5 \
  libxxhash0
dpkg -i cornrowd__<version>_<your_arch>.deb
```

## Installation (from source)
Consider downloading the release tarballs. Master might be broken from time to time.

### For Debian users
```
sudo apt install \
  cmake extra-cmake-modules \
  libboost-dev \
  libgstreamermm-1.0-dev \
  libspdlog-dev \
  libxxhash-dev \
  gstreamer1.0-alsa gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad \
  qtconnectivity5-dev # get dependecies
wget https://github.com/mincequi/cornrow/archive/v0.4.0.tar.gz
tar xfvz cornrow-0.4.0.tar.gz
dpkg-buildpackage -us -uc                           # build unsigned debian package
sudo dpkg -i ../cornrowd_<version>_<your_arch>.deb  # install package
sudo systemctl start cornrowd.service               # start-up service. You should now be able to connect any bluetooth audio device.
sudo systemctl enable cornrowd.service              # start-up service on each reboot.
```

### Arbitrary distro
Get the dependencies. Basically, these are gstreamermm and qt5bluetooth.
```
git clone --recursive https://github.com/mincequi/cornrow
cd cornrow
mkdir build
cd build
cmake ..
make
```

## Known issues
### Raspberry Pi 3 Model B (BCM43143)
#### Firmware issues
Apparently, there are issues with on-board Bluetooth. Depending on the installed firmware i get stuttering Bluetooth audio playback. Additionally, there seem to be issues when operating in classic Bluetooth and Low Energy simultaneously.
Here is an incomplete list of working/non-working firmware versions (https://github.com/Hexxeh/rpi-firmware/commits/master). Use the rpi-update to flash your Raspberry Pi to the according version:
```shell
sudo rpi-update <commit-id>
```
Commit | Date | Working
--- | --- | ---
6aec73e | 2019-01-09 | OK
883750d | 2019-02-05 | OK
29952e8 | 2019-03-08 | NOK

#### UART issues
A lot of Bluetooth packets seem to be dropped when CPU usage is **low**. As soon as the system is put under load, bluetooth packet reception is a **lot** better. This seems to be related to core frequency scaling (https://www.raspberrypi.org/documentation/configuration/uart.md).

## To Do
* Remove dependencies
* ~~Add sophisticated logging~~
* ~~Implement volume control~~
* ~~Set discoverable/pairable timeout to forever~~
