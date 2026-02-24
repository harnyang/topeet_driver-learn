# export CROSS_COMPILE=/home/harn/rk3568_linux_sdk/prebuilts/gcc/linux-x86/aarch64/gcc-buildroot-9.3.0-2020.03-x86_64_aarch64-rockchip-linux-gnu/bin/aarch64-rockchip930-linux-gnu-
KERNELDIR := /home/harn/rk3568_linux_sdk/kernel
# KERNELDIR := /lib/modules/5.15.0-139-generic/kernel
CURRENT_PATH := $(shell pwd)
obj-m := 23_device_tree_and_platform_driver.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
# all:
# 	make -C $(KERNELDIR) M=$(CURRENT_PATH) modules
# clean:
# 	rm -f *.ko *.o  *.mode.o  *.mod.c *.symvers *.order