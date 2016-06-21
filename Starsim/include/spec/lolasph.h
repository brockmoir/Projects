/***************************************************************************************************************************************************************
**	This module produces the lines of latitude and longitude for the      **
**		sphere.                 				      **
***************************************************************************************************************************************************************/

/*******************************************************************************
*     x_y() generates the 2-D polar coordinates of for x and y	               *
*	for every value of phi needed.                                         *
*		=>phi_o is the begining coordinate,                            *
*       	=>dph is the incrementation in phi,                            *
*		=>n is the size of the arrays   			       ********************************************************************************/

void x_y(double cphi[], double sphi[], double phi_o, double dph){
double phi;
int i;

	phi = phi_o;

	for(i=0;i<np+1;i++){
		cphi[i]=cos(phi);
		sphi[i]=sin(phi);
		phi+=dph;
	}
}

/*******************************************************************************
*	x_y_z() generates the azimuthal components for x, y, and z             *
*	   for every value of theta needed		                       *
*	   	 =>the_o is the initial theta value,			       *
*		 =>dth is the theta incrementation,                            *
*	   	 =>and n is the size of each array			       ********************************************************************************/

void x_y_z(double sthet[], double cthet[], double the_o, double dth){
double theta;
int i;

	theta=the_o;
	for(i=0;i<np+1;i++){
		sthet[i]=sin(theta);
		cthet[i]=cos(theta);
		theta+=dth;
	}
}

/*******************************************************************************
*	lola() prints to file the lat/lon coordinates for a sphere	       *
*        required input is the filename, time, and angular arrays              *
*******************************************************************************/

void lola(char outname[], double t, 
	  double cphi[], double sphi[], double cthet[], double sthet[]){
int i, j, k, dilo, dila;
double	xf, yf, zf, xyz[3],
	zeta;
FILE *out;	

if(nlon==0) dilo=np;		/* If we want no longitude       	     */
else if(np<nlon) dilo=np;
else dilo=np/nlon;

if(nlat==0) dila=np;		/* If we want no latitude 		     */
else if(np<nlat) dila=np;
else dila=np/nlat;

out=fopen(outname, "w");

/*******************************************************************************
*	This loop prints the coordinates of the lines of longitude             *
*	 allign() is in the file rotate.h, it alligns each set of coordinates  *
*	 to rotate about the defined axis.				       *
*	 The output format required by gnuplot is that the data is gridded     *
*	 this is why each block of data must have the same number of rows      *
*******************************************************************************/

if(dilo<np){
 for(i=0;i<np;i+=dilo){
	for(j=0, k=0;j<np+1;j++){
	  allign(t, xyz, cphi[i], sphi[i], sthet[j], cthet[j]);
       		xf=R*xyz[0],  yf=R*xyz[1], zf=R*xyz[2];
		fprintf(out, "%f %f %f\n", xf, yf, zf);
		k++;
	}

	if(k==np) fprintf(out, "\n");
	else {  
		for(;k<=np;k++){
			fprintf(out, "%f %f %f\n", xf, yf, zf);
		}
		fprintf(out, "\n");
	 }

 }
}

/*******************************************************************************
*	This loop prints the coordinates of the lines of latitude	       *
*******************************************************************************/
fprintf(out, "\n");
if(dila<np){
 for(i=0;i<np;i+=dila){
	for(j=0, k=0;j<np+1;j++){
	  allign(t, xyz, cphi[j], sphi[j], cthet[i], sthet[i]);
		xf=xyz[0], yf=xyz[1], zf=xyz[2];
		fprintf(out, "%f %f %f\n", xf, yf, zf);
		k++;
	}

	if(k==np) fprintf(out, "\n");
	else{ 
		for(;k<=np;k++){
			fprintf(out, "%f %f %f\n", xf, yf, zf);
		}
		fprintf(out, "\n");
	}
 }

}

/*******************************************************************************
*	I have included a circle around the star so as to ensure smoothness    *
*		around the edges in the animation.                             *
*******************************************************************************/

for(zeta=0.0; zeta<=2.0*pi; zeta+=(pi/90.0))
	fprintf(out, "0.0 %f %f\n", sin(zeta), cos(zeta));

fclose(out);

}
