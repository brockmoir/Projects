This is the first significant body of C code I have created.  It was created in 2008, and it appears some of the files are now corrupted.

/***************************************************************************************************************************************************************
			Neutron Star program
				v1.0
***************************************************************************************************************************************************************/

This program is designed to generate an animation of a spot on a neutron star in
 newtonian mechanics (newt.c), special relativistic mechanics (spec.c), and 
 general relativity using the Schwarzchild metric (schw.c).

There are particular parameters of the star and spot that can be user defined 
 before compilation.  The include file include/const.h contains all of these 
 variables including: the spin axis and rotational frequency of the star; the 
 mass and the radius of the star; the distance between the star and the
 observer; the size and location of the spot on the star; and the emitted 
 intensity and frequency of photons from the spot.

There are a number of additional .h include files associated with each of the 
 options determining the star's mechanics.  These files are located in the 
 /include directory under the file containing the respective name of the
 particular physics involved.

Compilation:
	The Makefile is a template for compilation, depending on your system
	 you may need to edit it somewhat.  The format to generate each exe and
	 execute it is simply make [newt, spec, schw, or all], where all will 
	 generate all the output.

Output:
	Once the make command has been executed, the program will compile and 
	 run.  After the program is finished the useable output can be found in 	 the folder /out.  It will contain two outputs for each physical system:
	 the file ending in Fvt.out can be plotted in gnuplot to create a flux
	 vs time graph as the spot completes one full revolution; while the
	 file ending in out.plt must be loaded into gnuplot to generate an
	 animation of the spot revolving around the star.  For example, if I 
	 were to create a spot under special relativistic mechanics, after
	 compilation I would go to the /out folder and begin gnuplot.  At the 
	 gnuplot command line prompt typing plot "specFvt.plt" creates the flux 
	 vs time graph, and typing load "specout.plt" creates the animation.
