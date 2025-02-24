## RPMsg sample application

### How to build

Please refer to the project [Readme.md](../../Readme.md).

Make sure  `-DENABLE_OPENAMP=1` is in your CMake configuration command.

### Important Note

This application demonstrates how to use the OpenAMP library on FreeRTOS. However, the functions are not fully implemented, so the application will not run. Please refer to the flow for guidance only.

### Reference documents:

OpenAMP User Manual: https://openamp.readthedocs.io/en/latest/openamp/overview.html

Linux RPMsg and Remote Processor documentation:
- https://docs.kernel.org/staging/rpmsg.html
- https://docs.kernel.org/staging/remoteproc.html

Linux RPMsg sample application: https://github.com/renesas-rcar/linux-bsp/tree/v6.1.102/rcar-5.2.0.rc11_vpf.rc15/samples/rpmsg
