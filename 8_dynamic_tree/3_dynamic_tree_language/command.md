- 编译的路径和命令是
```
/home/harn/rk3568_linux_sdk/kernel/scripts/dtc/dtc -I dts -O dtb ./overlay.dts -o overlay.dtbo
```
- 反编译的路径和命令是
```
/home/harn/rk3568_linux_sdk/kernel/scripts/dtc/dtc -I dtb -O dts ./overlay.dtbo -o 1.dts
```