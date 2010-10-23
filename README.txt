
INTRODUCTION:

 sSen is a horizon broadening scientfic research feature & future providing
 state of the art Operating System.
 In other words it's a hobby OS which primary goal is to examine and
 understand the innards of a "modern" computer.
 but as one have to learn breathing before one could take-off flying around
 sSen (at this point) lacks nearly everything and hardly provides anything.
 thus  ... let's go exploring!

 sSen is free software, released under the GNU GPL;
 see the file "LICENSE.txt" for details.

 ------------------------------------------------------------------------------ 

SETUP:

 If you have a sSen-package you can copy the image "build/ssen.img"
 to your floppy and youll be all set. (all data on the floppy will be lost)
 Note that you will need a special utility like "dd" to do that.

 Alternativeley follow the instructions under COMPILATION.

 ------------------------------------------------------------------------------ 

COMPILATION:

 To assemble the sources you only need "nasm".
 "make" and "dd" are recommended but not neccesary.

 simply type "make" in the "source" directory and look for what you want
 if you haven't installed make (anyone out there?) or are on a M$ machine you
 can inspect the "Makefile" sections "boot:" and "kernel:". 

 After compilation the file "build/boot.bin" needs to be copyed to sector 1 of
 a floppy. Again a program like "dd" is needed
 (If it is a standard M$ formated floppy your data theoretically (so take kare)
 stays intact, but it is recommended to format a floppy first.
 then the file "build/KERNEL.BIN" can easyly be copyed to the floppy
 as you would do with every other file.

 ------------------------------------------------------------------------------ 

RUNNING:

 If you have setup a sSen-floppy simply restart your PC and leave the floppy
 attached to the drive.
 In case sSen don't starts go into your BIOS and make the floppy-drive bootable.

 On a second option you may want to start sSen in a x86-emulator/virtualizer
 in that case yust boot the floppy-image or preperated floppy through your
 favorite. (personally i use "Bochs")

 ------------------------------------------------------------------------------ 

                    sSen is (c) Kaworu [ssen.dev@gmail.com]

