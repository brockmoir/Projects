/***************************************************************************************************************************************************************
	This is the main function to create a newtonian star with a spot 
	of particular properties (as can be defined in const.h.
	There are a number of .h include files that are essential to this 
	program that can be found in the /include file.
	This program itself will generate a number of different outputs.
		in /out
		==> newtout.plt is a file to be loaded into gnuplot to create 
			a .gif animation of the star
		==> newtFvt.plt is a file to be plotted by gnuplot that gives
			the flux vs time dependance of the spot for one
			complete revolution.
		in /out/newt all the files are labelled by their time 
			dependance, the ones begining with an s give the spot's
			coordinates, while the others give the lines of 
			latitude and longitude
***************************************************************************************************************************************************************/


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "include/const.h"
#include "include/newt/rotate.h"
#include "include/newt/lolasph.h"
#include "include/newt/newt.h"
#include "include/newt/sphere.h"

int main(){
double t, dt, tmax, tdelay, b;
double F, cphi[np+1], sphi[np+1], cthet[np+1], sthet[np+1], dphi, dtheta;
FILE *out1, *out2;
char outname[30];

dphi=2.0*pi/((double) np);  
dtheta=2.0*pi/((double) np);

x_y(cphi, sphi, 0, dphi, np+1);
x_y_z(sthet, cthet, 0, dtheta, np+1);

b=1.0/sqrt(1.0-2.0*M/R);
tmax=1.0;
dt=0.005;
tdelay=dt*200.0;
out1=fopen("out/newtout.plt", "w");
out2=fopen("out/newtFvt.plt", "w");

fprintf(out1, "set term gif animate crop optimize delay %f\n", tdelay);
fprintf(out1, "set output \'newt.gif\'\n");
fprintf(out1, "set pm3d scansforward\n");
fprintf(out1, "set xrange [0:%f]\n", 2*R);
fprintf(out1, "set yrange [-%f:%f]\n", R*b, R*b);
fprintf(out1, "set zrange [-%f:%f]\n", R*b, R*b);
fprintf(out1, "set cbrange[0:%f]\n", I_o);
fprintf(out1, "set size square\n");
fprintf(out1, "set view 90,90\n");
fprintf(out1, "unset xtics\n");
fprintf(out1, "unset ytics\n");
fprintf(out1, "unset ztics\n");
fprintf(out1, "unset title\n");
fprintf(out1, "unset border\n");
fprintf(out1, "unset key\n");


for(t=0.0; t<tmax; t+=dt){
	fprintf(out1, "splot \"newt/%f.out\" w lines,", t);
	fprintf(out1, " \"newt/s%f.out\" w pm3d\n", t);
	sprintf(outname, "out/newt/s%f.out", t);
	F=newton(outname, (t*2.0*pi/w));
	sprintf(outname, "out/newt/%f.out", t);
	lola(outname, (t*2.0*pi/w), cphi, sphi, cthet, sthet);
	fprintf(out2, "%f %f\n", t, F);
}
fprintf(out1, "unset out\n");

fclose(out1);
fclose(out2);

return 0;
}
