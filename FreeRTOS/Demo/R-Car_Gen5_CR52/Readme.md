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

```
 export PATH=$PATH:<your_tool_chain_path>/bin/
 cd FreeRTOS/Demo/R-Car_Gen5_CR52/
 mkdir build && cd build
 cmake -G "Unix Makefiles" \
     -DCMAKE_TOOLCHAIN_FILE=../toolchain_arm_none_eabi.cmake \
	 -DCMAKE_INSTALL_PREFIX=<path/to/install/dir> \
	 -DENABLE_OPENAMP=1 \ 			# OPTIONAL: Enable OpenAMP and Libmetal build 
 ..
 make
```

To install output binaries and headers to `path/to/install/dir`:

```
make install
```

### Output

`<install_dir>/lib/libfreertos_bsp.a` The archieve file contains FreeRTOS Kernel and R-Car BSP, POSIX libraries

`<install_dir>/objects/dummy/common/dummy.c.o` Link your application with this object to suspend some warning: `warning: <symbol> is not implemented and will always fail`

`<install_dir>/include` The public include header directory

`build/<app_name>/<app_name>.elf` for sample apps

`build/<app_name>/<app_name>.map` for memory map


### Build and link out-of-tree application

Out-of-tree application are the applications use FreeRTOS output build to compile and link with. Some configurations need to be set in app's CMakeLists.txt:

```
include_directories(
	<INSTALL_DIR>/include
	<INSTALL_DIR>/include/ARM_CR52
	<INSTALL_DIR>/include/kernel/include
	<INSTALL_DIR>/include/POSIX/POSIX_portable
	<INSTALL_DIR>/include/POSIX/include
	<INSTALL_DIR>/include/POSIX/include/private
	<INSTALL_DIR>/include/POSIX/include/portable
)

link_directories(
	<INSTALL_DIR>/lib
)
link_libraries(
    freertos_bsp
	<INSTALL_DIR>/objects/dummy/common/dummy.c.o
)

# Add linker script and your custom link options
target_link_options(${app_name}
	PRIVATE
	-Wl,--no-warn-rwx-segments -lnosys --specs=nosys.specs
    -L<INSTALL_DIR>/linker -T lscript_sram.ld
)
```
