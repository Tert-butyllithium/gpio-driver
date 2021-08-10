
CROSS_COMPILE := /sdk/lichee/brandy-2.0/tools/toolchain/riscv64-linux-x86_64-20200528/bin/riscv64-unknown-linux-gnu-
CC := $(CROSS_COMPILE)gcc

CFLAGS = -nostdlib -static -mcmodel=medany -g 

link_script = drv_gpio.lds

src := sunxi_gpio.c

objs := $(src:%.c=%.o)


all: drv_gpio

drv_gpio: drv_gpio.bin
	$(CROSS_COMPILE)objcopy -O binary --set-section-flags .bss=alloc,load,contents $< $@

drv_gpio.bin: $(objs)
	$(CROSS_COMPILE)gcc $(CFLAGS) $(objs) -T $(link_script) -o $@

clean:
	-@rm $(objs) drv_gpio drv_gpio.bin