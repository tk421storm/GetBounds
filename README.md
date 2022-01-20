# GetBounds
nuke realtime auto-crop node. Determines correct bbox of input nodes in real-time!

NOTE: this node hijacks the validation stage of a node tree and forces it to draw the entire frame above it. As such, it is generally designed to be used directly after a read. If it is placed further down in the tree, the UI may lock up while this node forces the nodes above it to render the entire frame.

tested on Windows 10, Nuke 12.1v2

# Build on Windows

build on windows:
(built using VisualStudio 2019)

CL:
Environment Variables passed to tool:
      VS_UNICODE_OUTPUT=2016
C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.16.27023\bin\HostX86\x64\CL.exe /c /I"C:\Program Files\Nuke12.1v2\include" /Zi /nologo /W3 /WX- /diagnostics:classic /O2 /Oi /GL /D WIN32 /D NDEBUG /D _WINDOWS /D _USRDLL /D FN_OS_WINDOWS /D _WINDLL /D _MBCS /Gm- /EHsc /MD /GS /Gy /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /Fo"x64\Release\\" /Fd"x64\Release\vc141.pdb" /Gd /TP /FC /errorReport:prompt GetBounds.cpp

Link:
Environment Variables passed to tool:
     VS_UNICODE_OUTPUT=2028
C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.16.27023\bin\HostX86\x64\link.exe /ERRORREPORT:PROMPT /OUT:"[OUTPUT_Folder]\GetBounds.dll" /INCREMENTAL:NO /NOLOGO /LIBPATH:"C:\Program Files\Nuke12.1v2" DDImage.lib glew32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /MANIFEST /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /manifest:embed /DEBUG:FULL /PDB:"[OUTPUT_FOLDER]\GetBounds.pdb" /OPT:REF /OPT:ICF /LTCG:incremental /TLBID:1 /DYNAMICBASE /NXCOMPAT /IMPLIB:"[OUTPUT_FOLDER]\GetBounds.lib" /MACHINE:X64 /DLL x64\Release\ShiftRGB.obj

# Build on Linux
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

#Install
Simply copy the .dll/.so file somewhere into your NUKE_PLUGIN_PATH. To see the node, right-click in the node graph and do "Other -> Update" then type in "GetBounds" in the tab menu.


