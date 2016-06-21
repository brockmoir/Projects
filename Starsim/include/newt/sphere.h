/***************************************************************************************************************************************************************
*	This is essentially the main component.  Here the dimensions of the    *
*		spot are determined, transformed and printed into an output    *
*		file to be plotted alongside the lat/long printout             *
***************************************************************************************************************************************************************/



/*******************************************************************************
*	rad() determines the distance of each point on the spot                *
*	  from the center of the spot.                                         *
*	  	=>xc, yc, zc are the cartesian coordinates of the spot,        *
*	  	=>x, y, z are the cartesian coordinates of the point to be     *
*		  evaluated					               *
*       	=>return value is the distance to the center                   *
*******************************************************************************/

float rad(float xc, float yc, float zc, double x, double y, double z){
	float ra;
	ra=sqrt(SQ(x-xc)+SQ(y-yc)+SQ(z-zc));
return ra; 
}


/*******************************************************************************
*	This is the main function it evaluates the location of the spot, and   *
*		the size of the spot.                                          *
*		This is also where the flux is determined.	               *
*******************************************************************************/

double newton(char outname[], double t){
int i, j, k, a;			
double	theo, phio,
	cphi[ns], sphi[ns],	
	sthet[ns], cthet[ns],		
	dth, dph,		
	rs, 			
	xc, yc, zc,
	f, F, xyz[3], xf, yf, zf,
	alpha;
FILE *out;

theo=theta_c-(sdan/2.0);	/* initial theta and phi values */
phio=phi_c-(sdan/2.0);

rs=sin(sdan/2);		  /* spot's radius and central x, y, and z values */
xc=sin(theta_c)*cos(phi_c); 
yc=sin(theta_c)*sin(phi_c);
zc=cos(theta_c);

dph=2.0*sdan/((double) ns);	dth=sdan/((double) ns);

x_y(cphi, sphi, phio, dph, ns);
x_y_z(sthet, cthet, theo, dth, ns);

rs=sin(sdan/2.0);           /* spot's radius and central x, y, and z values */
xc=cphi[ns/2-1]*sthet[ns/2-1];
yc=sphi[ns/2-1]*sthet[ns/2-1];
zc=cthet[ns/2-1];


out=fopen(outname, "w");

/*******************************************************************************
*	This loop prints the coordinates of the spot, and evaluates the flux   *
*		of the spot for the given time			               *
*******************************************************************************/
for(i=0, F=0.0; i<ns; i++){
	for(j=0, k=1; j<ns; j++){
	  allign(t, xyz, cphi[j], sphi[j], cthet[i], sthet[i]);
	  if(rad(xc, yc, zc, cphi[j]*sthet[i], sphi[j]*sthet[i], cthet[i])
		<=rs){
	    xf=xyz[0], yf=xyz[1], zf=xyz[2]; f=I_em(acos(xf));
	
	    if(xf>=0.0) F+=xf*sthet[i]*I_em(acos(xf));

            fprintf(out, "%f %f %f %f\n", xf*R, yf*R, zf*R, f);
	    k++;
	  }	
	
	}

	if(k==ns || k==1){fprintf(out, "\n");}
	else{

		for(;k<=ns;k++){
			fprintf(out, "%f %f %f %f\n", xf, yf, zf, f);
		}

		fprintf(out, "\n");
	}

}


fclose(out);

/*******************************************************************************
*	The flux is now multiplied by constants that were previously ommitted  *
*		and the return value is the flux.                              *
*******************************************************************************/

F*=dth*dph*SQ(R/d);
return F;

}
