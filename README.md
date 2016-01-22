# Polo

First steps into OSDev. The project is based off of the excellent [OSDev](http://wiki.osdev.org)'s Meaty skeleton project.

**Features Implemented:**

 - Loads GDT
 - Serial Driver
 - Minimal libc (See the libc project)
 - Interrupt Handlers
 - Minmal Keyboard support
 - Timer

**Features planned:**

 - Expand keyboard support so it deals with modifer keys properly (Shifts, Ctrls etc.)
 - Memory manager
 - Minimal Unix like shell
 - Expand libc or integrate a 3rd party [libc](http://wiki.osdev.org/C_Library)
 - File system Driver

**Nice to haves:**

 - Networking
 - VGA support
 - Userspace programs

**Requirements**

 - [Cross compiler](http://wiki.osdev.org/GCC_Cross-Compiler)
 - qemu / bochs. I'd recommend bochs. If you're using Ubuntu, apt-get bochs sets bochs up without support for debugging. I'm not sure if it's possible to add debugging to this, so I compiled bochs from source with the appropriate configuration options. These are pretty complicated, so I'd recommend OSDev's bochs ./configure [guide](http://wiki.osdev.org/Bochs#Compiling_Bochs_from_Source).

**Running**

 - FIX THIS BEFORE PROCEEDING
 - Please navigate to ./bochs.sh and provide the right path to your bochs installation. This could've been done in so many better ways (environment variables, putting bochs in bin etc. but I was just excited to get going)
 - ./build_master.sh should clean, compile everything and run bochs. If you'd like to run qemu instead, change the last step in ./build_master.sh to run ./qemu.sh instead.
