# GetBounds
nuke realtime auto-crop node


tested on Windows 10, Nuke 12.1v2


linux compilation requirements (based on RHEL):

mesa-libGL-devel
mesa-libGLU-devel
libXi-devel
libXmu-devel
freeglut-devel.x86_64

linux build instructions:

mkdir build && cd build
cmake -DNUKE_INSTALL_PATH=/path/to/Nuke12.1v2/ ..
make
