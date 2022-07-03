# os

Minimal operating system for E93. (WORK IN PROGRESS!!!)

## Building and Running

In [Pardus 21](https://www.pardus.org.tr/), clone this repository. After that, install GCC x86 cross compiler, NASM and QEMU:

`sudo apt install nasm qemu-system-x86 gcc-i686-linux-gnu`

After that, move to the src directory:

`cd src`

After that, build the OS:

`make`

After building, you can run:

`make run`

## TODO

- Finish reading [os-tutorial](https://github.com/cfenollosa/os-tutorial).
- Read Tanenbaum's Modern Operating Systems (Perason International 3rd edition). (eersoy93)

## References

- [https://github.com/cfenollosa/os-tutorial](https://github.com/cfenollosa/os-tutorial)
- [https://en.wikipedia.org/](https://en.wikipedia.org/)
- [https://www.felixcloutier.com/x86/index.html](https://www.felixcloutier.com/x86/index.html)
- [https://www.fountainware.com/EXPL/vga_color_palettes.htm](https://www.fountainware.com/EXPL/vga_color_palettes.htm)
- [https://en.wikipedia.org/wiki/VGA_text_mode](https://en.wikipedia.org/wiki/VGA_text_mode)
- [https://www.rapidtables.com/code/text/ascii-table.html](https://www.rapidtables.com/code/text/ascii-table.html)
- [Pardus 21's Calculator :)](https://apps.pardus.org.tr/app/gnome-calculator)

## Copyright and License

Copyright (c) 2022 Erdem Ersoy (eersoy93).

Licensed with MIT license. See LICENSE file for deatils.
