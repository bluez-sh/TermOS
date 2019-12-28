C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC = /usr/local/i386-elf-gcc/bin/i386-elf-gcc 
GDB = /usr/local/i386-elf-gcc/bin/i386-elf-gdb
CFLAGS = -g -m32 -ffreestanding -Wall -Wextra -fno-exceptions

os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

run: os-image.bin
	qemu-system-i386 -boot menu=on -fda $< -hda hdd.img

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -boot menu=on -fda $< -hda hdd.img &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o *.elf os-image.bin
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o
