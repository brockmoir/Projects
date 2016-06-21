/***************************************************************************************************************************************************************
*       This is essentially the main component.  Here the dimensions of the    *
*               spot are determined, transformed and printed into an output    *
*               file to be plotted alongside the lat/long printout             *
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
*       This is the main function it evaluates the location of the spot, and   *
*               the size of the spot.                                          *
*               This is also where the flux is determined.                     *
*******************************************************************************/

double schw(char outname[], double t, double aalpha[], double apsi[]){
int i, j, k, a;			
double	cphi[ns], sphi[ns],	
	sthe[ns], cthe[ns],		
	rs, 			
	xc, yc, zc, 		
 	xyz[3], zy[2], xf, yf, zf, thphcs[4],
	ssthe[ns][ns], scthe[ns][ns],
	eta, vf, F, I, vx, v,
	zetalp[2], dth, dph;

FILE *out;

v=R*w/sqrt(1.0-2.0*M/R);
rs=R*sin(sdan/2.0);		

dph=2.0*((double) sdan/ns), dth=((double) sdan/(ns));
spec(t, cphi, sphi, cthe, sthe, scthe, ssthe, thphcs);	
xc=R*cphi[ns/2-1]*sthe[ns/2-1]; 
yc=R*sphi[ns/2-1]*sthe[ns/2-1]; 
zc=R*cthe[ns/2-1];

out=fopen(outname, "w");

/*******************************************************************************
*	This loop prints the coordinates of the spot, and determines the flux  *
*	of the spot at the given time				               *
*******************************************************************************/
for(j=0, F=0.0;j<ns;j++){
	for(i=0, k=1; i<ns; i++){
	  veltoobs(xyz, cphi[j], sphi[j], scthe[j][i], ssthe[j][i], thphcs);
	  vx=-(xyz[1]*zu-xyz[2]*yu)*w*R/sqrt(1.0-2.0*M/R);
	  map(xyz, zy, zetalp, aalpha, apsi);
	    if(rad(xc, yc, zc, R*cphi[j]*sthe[i], R*sphi[j]*sthe[i], R*cthe[i])
		<=rs){
		xf=R*xyz[0];
		if(zy[1]<9.0){ 
			yf=R*zy[1], zf=R*zy[0];
	                eta=sqrt(1.0-SQ(v/c))/(1.0-(vx/c));
	                vf=sqrt(1.0-2.0*M/R)*eta*v_em;
	                I=I_em(zetalp[1], eta);
			if(eta*cos(zetalp[1]>0))
				F+=cos(zetalp[1])*sthe[i]*I/SQ(eta);
			fprintf(out, "%f %f %f %f %f\n", xf, yf, zf, I, vf);
			k++;
		}

	  }	
	
	}

	if(k==ns || k==1) fprintf(out, "\n");
	else{

		for(;k<=ns;k++){
			fprintf(out, "%f %f %f %f %f\n", xf, yf, zf, I, vf);
		}

		fprintf(out, "\n");
		
	}

}

fclose(out);

/*******************************************************************************
*       The flux is now multiplied by constants that were previously ommitted  *
*               and the return value is the flux.                              *
*******************************************************************************/

F*=SQ(R/d)*dth*dph/(1.0-2.0*M/R);
return F;

}
