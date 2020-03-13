C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c fs/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h fs/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC = /usr/local/i386-elf-gcc/bin/i386-elf-gcc 
GDB = /usr/local/i386-elf-gcc/bin/i386-elf-gdb
CFLAGS = -g -m32 -ffreestanding -Wall -Wextra -fno-exceptions -fno-pic \
-mno-sse -mno-sse2 -mno-mmx -mno-80387 -mno-red-zone

os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x8000 $^ --oformat binary -Map mem.map

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x8000 $^

run: os-image.bin
	qemu-system-i386 -m 512M -boot menu=on -fda $< -hda hdd.img

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -no-shutdown -no-reboot -d int -boot menu=on -fda $< -hda hdd.img &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o *.elf os-image.bin
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o fs/*.o
