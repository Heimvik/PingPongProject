# Installing GDB
Do the reccomended things in the lab manual as stated below
If avr-gcc, avr-libc or avrdude is not installed at Sanntidssalen, you can do this with the
command sudo apt-get install gcc-avr avr-libc avrdude. The toolchain for node to can
be set up with the following: Toolchain for linux
1. Call sudo apt install gcc-arm-none-eabi to install the compiler
2. Install prerequisites for a newer version of openocd with sudo apt install libusb-dev libusb-1.0-0-dev libusb-1.0-0
3. Open-jtag also requires sudo apt install libhidapi-dev libftdi-dev libftdi1-dev
4. Download openocd git clone https://git.code.sf.net/p/openocd/code openocd
5. Go into the folder cd openocd
6. Create the configure script with ./bootstrap
7. Create and go into a directory for the build mkdir build; cd build
8. Configure the build ../configure --enable-cmsis-dap --enable-openjtag --prefix=/opt/openocd
9. Make and install with make, then make install

Also create a udev rule for the Atmel-ICE device so that OpenOCD can access it without needing root privileges:
1. Create a new udev rule file:
sudo nano /etc/udev/rules.d/99-openocd.rules
2. Add the following line to the file (replace <user> with your username):
SUBSYSTEM=="usb", ATTRS{idVendor}=="03eb", ATTRS{idProduct}=="2141", MODE="0666", GROUP="plugdev", SYMLINK+="openocd/%k"
3. Save and exit the editor.
4. Reload udev rules:
bash
Kopier kode
sudo udevadm control --reload-rules
sudo udevadm trigger
Disconnect and reconnect the device.