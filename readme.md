## Computer Systems: A Programmer's Perspective 3rd edition

Here you can find all of the end of chapter problems in the csapp book, as well  
as some practice problems and random experiments. This is mainly for me to  
keep everything in one place as I am using multiple machines, but I made it  
public so potentially someone else might get something out of it.    


#### Building the Y86-64 simulators for Chapter 4
The simulator tools and documentation can be downloaded from [this page](http://csapp.cs.cmu.edu/3e/students.html)   
under Chapter 4: Processor Architecture  
  
I had to fix a few things in order to get the simulator to build. I was  
running this on Raspberry Pi OS, should be similar for other linux distros:    
  
1. Install flex  
    `sudo apt install flex`
2. Download and build Tcl  
3. Download and build Tk  
    Both can be downloaded from [here](https://www.tcl.tk/software/tcltk/download.html)
4. Update the outermost Makefile in the sim directory
    - enable GUI mode by uncommenting the line:  
    `GUIMODE=-DHAS_GUI`   
    - Point the linker to the tcl and tk libraries, for me this was:   
    `TKLIBS=-L/usr/local/lib -ltcl8.6 -ltk8.6`
    - Point the compiler to the tcl and tk header files (tcl.h and tk.h).  
      Also since tcl apparently deprecated some things in 8.5, in order to    
      build projects using some deprecated code, you have to    
      `#define USE_INTERP_RESULT` before including tcl.h. This can be done  
      via a compiler flag.   
    `TKINC=-D USE_INTERP_RESULT -isystem /usr/include/tcl8.6`    

