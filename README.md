# Polo

First steps into OSDev. The project is based off of the excellent [OSDev](http://wiki.osdev.org)'s Meaty skeleton project.

**Features Implemented:**

 - Loads GDT
 - Serial Driver
 - Minimal libc (See the libc project)

**Features planned:**

 - Interrupt Handlers
 - Proper exception handlers so we don't silently tripe-fault
 - Expand libc or integrate a 3rd party [libc](http://wiki.osdev.org/C_Library)
 - Memory manager
 - File system Driver
 - Minimal Unix like shell

**Nice to haves:**

 - Networking
 - VGA support
 - Userspace programs

**Requirements**

 - Cross compile [GCC](http://wiki.osdev.org/GCC_Cross-Compiler)
 - qemu / bochs. I'd recommend bochs. If you're using Ubuntu, apt-get bochs sets bochs up without support for debugging. I'm not sure if it's possible to add debugging to this, so I compiled bochs from source with the appropriate configuration options. These are pretty complicated, so I'd recommend OSDev's bochs ./configure [guide](http://wiki.osdev.org/Bochs#Compiling_Bochs_from_Source).
