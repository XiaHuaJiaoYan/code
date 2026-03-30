@goto Begin 
This is an example dos batch file of how to set environment variable SDK_INSTALL_DIR and MinGW_DIR
to build applications with Makefile
This batch file alters the PATH variable, restores it when done.
:Begin



@rem set SDK installed directory 
@set SDK_INSTALL_DIR=D:\kiosoft\iPaylabSDK

@set SDK_API_DIR=D:\kiosoft\iPaylabSDK\api\1.0.0\inc

@set SDK_LIB_DIR=D:\kiosoft\iPaylabSDK\api\arm\sigmastar\2.0.0\LIB

@set SDK_SYS_LIB_DIR=D:\kiosoft\iPaylabSDK\api\arm\sigmastar\2.0.0\syslib

@rem Set Gmake
@set MAKE_DIR=%SDK_INSTALL_DIR%\tools\gmake

@rem Set MinGW_DIR
@set MinGW_DIR=%SDK_INSTALL_DIR%\third-party\mingw

@rem Set CROSS_COMPILER_DIR
@set CROSS_COMPILER_DIR=%SDK_INSTALL_DIR%\tools\cross-compiler\arm\sigmastar\gcc-10.2.1

@rem Set DEBUGGER_DIR
@set DEBUGGER_DIR=%SDK_INSTALL_DIR%\tools\debugger

@set LD_LIBRARY_PATH=D:\kiosoft\iPaylabSDK\tools\cross-compiler\arm\sigmastar\gcc-10.2.1\arm-none-linux-gnueabihf\lib

@rem Set Complite Path
@set Path=%MinGW_DIR%\bin;%MinGW_DIR%\msys\1.0\bin;%MAKE_DIR%;%CROSS_COMPILER_DIR%\bin;%PACKAGE_GENERATOR_DIR%\pkgNLD;%PACKAGE_GENERATOR_DIR%\bin;%DEBUGGER_DIR%\bin;%LD_LIBRARY_PATH%;

@rem use Makefileto buid application
gmake -f Makefile-tpk cpu_ssd222d:=yes

pause



pause

