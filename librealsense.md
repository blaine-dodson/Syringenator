
## Downloads

Update the system

`sudo apt update`

get the kernel headers so that we can compile new things

`sudo apt install raspberrypi-kernel-headers`

make sure that raspberrypi-kernel and raspberrypi-bootloader are at the latest versions

install git and other build tools

`sudo apt install git build-essential -y`

get the latest librealsense

`git clone --depth 1 https://github.com/IntelRealSense/librealsense.git`


Install Intel Realsense permission scripts located in librealsense source directory:

`sudo cp config/99-realsense-libusb.rules /etc/udev/rules.d/`
`sudo udevadm control --reload-rules && udevadm trigger`

get the source for the current kernel make sure version numbers match apt-cache

`wget https://github.com/raspberrypi/linux/archive/raspberrypi-kernel_1.20161215-1.tar.gz`

extract it

`tar -xzf raspberrypi-kernel_1.20161215-1.tar.gz`


## Kernel source patching

`LINUX_BRANCH=$(uname -r)


# Construct branch name from distribution codename {xenial,bionic,..} and kernel version
ubuntu_codename=`. /etc/os-release; echo ${UBUNTU_CODENAME/*, /}`
if [ -z "${ubuntu_codename}" ];
then
	# Trusty Tahr shall use xenial code base
	ubuntu_codename="xenial"
	retpoline_retrofit=1
fi
kernel_branch=$(choose_kernel_branch ${LINUX_BRANCH} ${ubuntu_codename})
kernel_name="ubuntu-${ubuntu_codename}-$kernel_branch"`



## Kernel Configuration

Load the kernel configuration module

`sudo modprobe configs`

get a copy of the current kernel configuration

`cp /proc/config.gz ./`

decompress it

`gunzip config.gz`

put the configuration in the source tree

`mv config linux-raspberrypi-kernel_1.20161215-1/.config`

In the kernel directory update the config

`make silentoldconfig`



