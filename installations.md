# Installations

# librealsense

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

~~~bash
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
~~~


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


# OpenCV
We used this [tutorial](https://www.pyimagesearch.com/2016/10/24/ubuntu-16-04-how-to-install-opencv/) with some modifications.

## Dependencies
The tutorial's atlas installation is insufficient resulting in:
```
-- Could NOT find Atlas (missing: Atlas_CLAPACK_INCLUDE_DIR)
```
Refering to [issue #10442](https://github.com/opencv/opencv/issues/10442) I did:
```
sudo apt install liblapacke-dev
```

## Python Virtual environment
I wanted to include the python virtual environment in the git repo so that it can be used by anyone. I am not sure if this is the prefered way to share virtual environments. We also won't lose it if the pi has to be rebuilt. so the .bashrc script reads:
```
export WORKON_HOME=$HOME/Desktop/Syringenator/src/pi/pyVirtEnv
source /usr/local/bin/virtualenvwrapper.sh
```

## cmake
the cmake step then needs to be modified to acommodate:
```
cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D INSTALL_PYTHON_EXAMPLES=ON \
    -D INSTALL_C_EXAMPLES=OFF \
    -D OPENCV_EXTRA_MODULES_PATH=~/Desktop/opencv_contrib-4.0.1/modules \
    -D PYTHON_EXECUTABLE=~/Desktop/Syringenator/src/pi/pyVirtEnv/syringenator/bin/python \
    -D BUILD_EXAMPLES=ON \
    -D WITH_OPENMP=ON ..
```

cmake reports:
```
-- Looking for ccache - not found
-- FP16 is not supported by C++ compiler
-- Found ZLIB: /usr/lib/arm-linux-gnueabihf/libz.so (found suitable version "1.2.8", minimum required is "1.2.3") 
-- Found ZLIB: /usr/lib/arm-linux-gnueabihf/libz.so (found version "1.2.8") 
-- Checking for module 'gstreamer-base-1.0'
--   No package 'gstreamer-base-1.0' found
-- Checking for module 'gstreamer-video-1.0'
--   No package 'gstreamer-video-1.0' found
-- Checking for module 'gstreamer-app-1.0'
--   No package 'gstreamer-app-1.0' found
-- Checking for module 'gstreamer-riff-1.0'
--   No package 'gstreamer-riff-1.0' found
-- Checking for module 'gstreamer-pbutils-1.0'
--   No package 'gstreamer-pbutils-1.0' found
-- Checking for module 'gstreamer-base-0.10'
--   No package 'gstreamer-base-0.10' found
-- Checking for module 'gstreamer-video-0.10'
--   No package 'gstreamer-video-0.10' found
-- Checking for module 'gstreamer-app-0.10'
--   No package 'gstreamer-app-0.10' found
-- Checking for module 'gstreamer-riff-0.10'
--   No package 'gstreamer-riff-0.10' found
-- Checking for module 'gstreamer-pbutils-0.10'
--   No package 'gstreamer-pbutils-0.10' found
-- Checking for module 'libdc1394-2'
--   No package 'libdc1394-2' found
-- Checking for module 'libdc1394'
--   No package 'libdc1394' found
-- Looking for linux/videodev2.h
-- Looking for linux/videodev2.h - found
-- Looking for sys/videoio.h
-- Looking for sys/videoio.h - not found
-- Checking for module 'libavresample'
--   No package 'libavresample' found
-- Could not find OpenBLAS lib. Turning OpenBLAS_FOUND off
-- Found Atlas: /usr/include  
-- Found Atlas (include: /usr/include, library: /usr/lib/libatlas.so)
-- LAPACK(Atlas): LAPACK_LIBRARIES: /usr/lib/liblapack.so;/usr/lib/libcblas.so;/usr/lib/libatlas.so
-- LAPACK(Atlas): Support is enabled.
-- Could NOT find JNI (missing: JAVA_INCLUDE_PATH JAVA_INCLUDE_PATH2 JAVA_AWT_INCLUDE_PATH) 
-- Could NOT find Pylint (missing: PYLINT_EXECUTABLE) 
-- Could NOT find Flake8 (missing: FLAKE8_EXECUTABLE) 
-- VTK is not found. Please set -DVTK_DIR in CMake to VTK build directory, or to VTK install subdirectory with VTKConfig.cmake file
-- OpenCV Python: during development append to PYTHONPATH: /home/big/Desktop/opencv-4.0.1/build/python_loader
-- Caffe:   NO
-- Protobuf:   NO
-- Glog:   NO
-- freetype2:   YES
-- harfbuzz:    YES
-- Could NOT find HDF5 (missing: HDF5_LIBRARIES HDF5_INCLUDE_DIRS) (found version "")
-- Module opencv_ovis disabled because OGRE3D was not found
-- No preference for use of exported gflags CMake configuration set, and no hints for include/library directories provided. Defaulting to preferring an installed/exported gflags CMake configuration if available.
-- Failed to find installed gflags CMake configuration, searching for gflags build directories exported with CMake.
-- Failed to find gflags - Failed to find an installed/exported CMake configuration for gflags, will perform search for installed gflags components.
-- Failed to find gflags - Could not find gflags include directory, set GFLAGS_INCLUDE_DIR to directory containing gflags/gflags.h
-- Failed to find glog - Could not find glog include directory, set GLOG_INCLUDE_DIR to directory containing glog/logging.h
-- Module opencv_sfm disabled because the following dependencies are not found: Eigen Glog/Gflags
-- Excluding from source files list: <BUILD>/modules/core/test/test_intrin128.sse2.cpp
-- Excluding from source files list: <BUILD>/modules/core/test/test_intrin128.sse3.cpp
-- Excluding from source files list: <BUILD>/modules/core/test/test_intrin128.ssse3.cpp
-- Excluding from source files list: <BUILD>/modules/core/test/test_intrin128.sse4_1.cpp
-- Excluding from source files list: <BUILD>/modules/core/test/test_intrin128.sse4_2.cpp
-- Excluding from source files list: <BUILD>/modules/core/test/test_intrin128.avx.cpp
-- Excluding from source files list: <BUILD>/modules/core/test/test_intrin128.fp16.cpp
-- Excluding from source files list: <BUILD>/modules/core/test/test_intrin128.avx2.cpp
-- Excluding from source files list: <BUILD>/modules/core/test/test_intrin256.avx2.cpp
-- Excluding from source files list: modules/imgproc/src/corner.avx.cpp
-- Excluding from source files list: modules/imgproc/src/filter.avx2.cpp
-- Excluding from source files list: modules/imgproc/src/imgwarp.avx2.cpp
-- Excluding from source files list: modules/imgproc/src/imgwarp.sse4_1.cpp
-- Excluding from source files list: modules/imgproc/src/resize.avx2.cpp
-- Excluding from source files list: modules/imgproc/src/resize.sse4_1.cpp
-- Excluding from source files list: <BUILD>/modules/dnn/layers/layers_common.avx.cpp
-- Excluding from source files list: <BUILD>/modules/dnn/layers/layers_common.avx2.cpp
-- Excluding from source files list: <BUILD>/modules/dnn/layers/layers_common.avx512_skx.cpp
-- freetype2:   YES
-- harfbuzz:    YES
-- Excluding from source files list: modules/features2d/src/fast.avx2.cpp
-- Checking for modules 'tesseract;lept'
--   No package 'tesseract' found
--   No package 'lept' found
-- Tesseract:   NO
-- Excluding from source files list: modules/calib3d/src/undistort.avx2.cpp
-- OpenCL samples are skipped: OpenCL SDK is required
-- 
-- General configuration for OpenCV 4.0.1 =====================================
--   Version control:               unknown
-- 
--   Extra modules:
--     Location (extra):            /home/big/Desktop/opencv_contrib-4.0.1/modules
--     Version control (extra):     unknown
-- 
--   Platform:
--     Timestamp:                   2019-02-14T22:20:14Z
--     Host:                        Linux 4.4.38-v7+ armv7l
--     CMake:                       3.13.3
--     CMake generator:             Unix Makefiles
--     CMake build tool:            /usr/bin/make
--     Configuration:               RELEASE
-- 
--   CPU/HW features:
--     Baseline:
--       requested:                 DETECT
--       disabled:                  VFPV3 NEON
-- 
--   C/C++:
--     Built as dynamic libs?:      YES
--     C++ Compiler:                /usr/bin/c++  (ver 5.5.0)
--     C++ flags (Release):         -fsigned-char -W -Wall -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -Wuninitialized -Winit-self -Wno-narrowing -Wno-delete-non-virtual-dtor -Wno-comment -fdiagnostics-show-option -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections  -mfp16-format=ieee -fvisibility=hidden -fvisibility-inlines-hidden -fopenmp -O3 -DNDEBUG  -DNDEBUG
--     C++ flags (Debug):           -fsigned-char -W -Wall -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -Wuninitialized -Winit-self -Wno-narrowing -Wno-delete-non-virtual-dtor -Wno-comment -fdiagnostics-show-option -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections  -mfp16-format=ieee -fvisibility=hidden -fvisibility-inlines-hidden -fopenmp -g  -O0 -DDEBUG -D_DEBUG
--     C Compiler:                  /usr/bin/cc
--     C flags (Release):           -fsigned-char -W -Wall -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -Wundef -Winit-self -Wpointer-arith -Wshadow -Wuninitialized -Winit-self -Wno-narrowing -Wno-comment -fdiagnostics-show-option -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections  -mfp16-format=ieee -fvisibility=hidden -fopenmp -O3 -DNDEBUG  -DNDEBUG
--     C flags (Debug):             -fsigned-char -W -Wall -Werror=return-type -Werror=non-virtual-dtor -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes -Wundef -Winit-self -Wpointer-arith -Wshadow -Wuninitialized -Winit-self -Wno-narrowing -Wno-comment -fdiagnostics-show-option -pthread -fomit-frame-pointer -ffunction-sections -fdata-sections  -mfp16-format=ieee -fvisibility=hidden -fopenmp -g  -O0 -DDEBUG -D_DEBUG
--     Linker flags (Release):      
--     Linker flags (Debug):        
--     ccache:                      NO
--     Precompiled headers:         YES
--     Extra dependencies:          dl m pthread rt
--     3rdparty dependencies:
-- 
--   OpenCV modules:
--     To be built:                 aruco bgsegm bioinspired calib3d ccalib core datasets dnn dnn_objdetect dpm face features2d flann freetype fuzzy gapi hfs highgui img_hash imgcodecs imgproc java_bindings_generator line_descriptor ml objdetect optflow phase_unwrapping photo plot python2 python_bindings_generator reg rgbd saliency shape stereo stitching structured_light superres surface_matching text tracking ts video videoio videostab xfeatures2d ximgproc xobjdetect xphoto
--     Disabled:                    world
--     Disabled by dependency:      -
--     Unavailable:                 cnn_3dobj cudaarithm cudabgsegm cudacodec cudafeatures2d cudafilters cudaimgproc cudalegacy cudaobjdetect cudaoptflow cudastereo cudawarping cudev cvv hdf java js matlab ovis python3 sfm viz
--     Applications:                tests perf_tests examples apps
--     Documentation:               NO
--     Non-free algorithms:         NO
-- 
--   GUI: 
--     GTK+:                        YES (ver 3.18.9)
--       GThread :                  YES (ver 2.48.2)
--       GtkGlExt:                  NO
--     VTK support:                 NO
-- 
--   Media I/O: 
--     ZLib:                        /usr/lib/arm-linux-gnueabihf/libz.so (ver 1.2.8)
--     JPEG:                        /usr/lib/arm-linux-gnueabihf/libjpeg.so (ver 80)
--     WEBP:                        build (ver encoder: 0x020e)
--     PNG:                         /usr/lib/arm-linux-gnueabihf/libpng.so (ver 1.2.54)
--     TIFF:                        /usr/lib/arm-linux-gnueabihf/libtiff.so (ver 42 / 4.0.6)
--     JPEG 2000:                   /usr/lib/arm-linux-gnueabihf/libjasper.so (ver 1.900.1)
--     OpenEXR:                     build (ver 1.7.1)
--     HDR:                         YES
--     SUNRASTER:                   YES
--     PXM:                         YES
--     PFM:                         YES
-- 
--   Video I/O:
--     DC1394:                      NO
--     FFMPEG:                      YES
--       avcodec:                   YES (ver 57.64.100)
--       avformat:                  YES (ver 57.56.100)
--       avutil:                    YES (ver 55.34.100)
--       swscale:                   YES (ver 4.2.100)
--       avresample:                NO
--     GStreamer:                   NO
--     v4l/v4l2:                    linux/videodev2.h
-- 
--   Parallel framework:            OpenMP
-- 
--   Trace:                         YES (built-in)
-- 
--   Other third-party libraries:
--     Lapack:                      YES (/usr/lib/liblapack.so /usr/lib/libcblas.so /usr/lib/libatlas.so)
--     Eigen:                       NO
--     Custom HAL:                  YES (carotene (ver 0.0.1))
--     Protobuf:                    build (3.5.1)
-- 
--   OpenCL:                        YES (no extra features)
--     Include path:                /home/big/Desktop/opencv-4.0.1/3rdparty/include/opencl/1.2
--     Link libraries:              Dynamic load
-- 
--   Python 2:
--     Interpreter:                 /home/big/Desktop/Syringenator/src/pi/pyVirtEnv/syringenator/bin/python (ver 2.7.12)
--     Libraries:                   /usr/lib/arm-linux-gnueabihf/libpython2.7.so (ver 2.7.12)
--     numpy:                       /home/big/Desktop/Syringenator/src/pi/pyVirtEnv/syringenator/lib/python2.7/site-packages/numpy/core/include (ver 1.16.1)
--     install path:                lib/python2.7/site-packages/cv2/python-2.7
-- 
--   Python (for build):            /home/big/Desktop/Syringenator/src/pi/pyVirtEnv/syringenator/bin/python
-- 
--   Java:                          
--     ant:                         NO
--     JNI:                         NO
--     Java wrappers:               NO
--     Java tests:                  NO
-- 
--   Install to:                    /usr/local
-- -----------------------------------------------------------------
-- 
-- Configuring done
-- Generating done
-- Build files have been written to: /home/big/Desktop/opencv-4.0.1/build
```

