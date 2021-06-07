# TermOS
An x86 based hobby OS made from scratch

## Features
- Custom bootloader written in x86 asm
- Kernel's own libc (library used by the kernel as well as the user programs)
- Linear memory manager (physical memory only)
- SimpleFS File System support (inode based)
- Necessary drivers (Keyboard, Screen, ATA)
- Program interface to register and handle user programs (though they still run in kernel space)
- A simple system shell to execute commands (and run user programs) - No GUI
- FCFS program scheduler (can also interrupt the queue and enqueue new programs with <code>LCtrl+C</code>)
- Multiple commands can also be added to the queue from a file and executed using command "fexec \<file descrpitor\>"

## Possible Features
- File system abstraction in libc
- A decent text editor
- Task Management using Task State Segment (TSS) feature of x86
- Round Robin scheduling of tasks
- A better memory manager

## Build Instructions

### Requirements
- i386-elf-gcc (crosscompiler toolchain)
- i386-elf-gdb (for debugging)
- nasm (Netwide assembler)
- qemu-system-i386 (included in qemu-arch-extra package in Arch Linux)
- make

### To Build
- Change the paths for <code>CC</code> and <code>GDB</code> in the Makefile, if required
- Run the command: <code>qemu-img create hdd.img 512M</code> to create a virtual image for hard disk
- Then run:<br>
<code>make</code> - to build <br>
<code>make run</code> - to build and run the OS on qemu <br>
<code>make debug</code> - to build and start gdb server (change the debug rule in Makefile, if required)
- Note: After running the OS for first time, run the OS command <code>formatfs</code> to format 
<code>hdd.img</code> with SimpleFS
<br><br>

> Find the full project report here: https://docs.google.com/document/d/e/2PACX-1vQ-U2EtcHYvwEpf5zWgf_OnuEpHnjlT0J8zZ0_djFvCirTCkCu-3jNLo5R0jY5SCs4F849UCYV8o1Dq/pub


