### Initialize submodules (RENESAS internal only)
This project uses several submodules for external software components. You need to initialize the submodules first:
```
git submodule update --init --recursive
```

### Compiler
For the CR52 (arm-gnu) the following toolchain have been used: 
“arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz” downloaded from https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
 Extract tool chain to <your_tool_chain_path>.

### Build
 export CROSS_COMPILE=<your_tool_chain_path>/bin/arm-none-eabi-
 export FREERTOS_DIR=<your_freertos_source>
 cd FreeRTOS/Demo/R-Car_Gen5_CR52/
 make			# compile Blink Demo
 cd sample_apps
 cd hello_app
 make

### Output
 freertos.a, freertos.bin and freertos.elf are contained in FreeRTOS/Demo/R-Car_Gen5_CR52/bin.
 freertos.map, freertos.bin and freertos.elf are contained in FreeRTOS/Demo/R-Car_Gen5_CR52/sample_apps/hello_world/bin.
