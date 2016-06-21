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


double special(char outname[], double t){
int i, j, k, a;			
double	cphi[ns], sphi[ns],	
	sthe[ns], cthe[ns],		
	rs, xc, yc, zc, 	
 	f, xyz[3], xf, yf, zf, thphcs[4],
	ssthe[ns][ns], scthe[ns][ns],
	eta, v, vx, vf, F, alpha, dth, dph;


FILE *out;		

v=R*w;

dph=2.0*((double) sdan/ns), dth=((double) sdan/(ns));
rs=sin(sdan/2.0);		 
spec(t, cphi, sphi, cthe, sthe, scthe, ssthe, thphcs);	
xc=cphi[ns/2-1]*sthe[ns/2-1]; 
yc=sphi[ns/2-1]*sthe[ns/2-1]; 
zc=cthe[ns/2-1];

out=fopen(outname, "w");

/*******************************************************************************
*	This loop prints the coordinates of the spot, and determines the flux  *
*		of the spot at the given time.			               *
*******************************************************************************/
for(j=0, F=0; j<ns; j++){
	for(i=0, k=1; i<ns; i++){
	  veltoobs(xyz, cphi[j], sphi[j], scthe[j][i], ssthe[j][i], thphcs);
	    if(rad(xc, yc, zc, cphi[j]*sthe[i], sphi[j]*sthe[i], cthe[i])
		<=rs){
		xf=xyz[0], yf=xyz[1], zf=xyz[2];
		alpha=acos(xf);
	    	vx= -(xyz[1]*zu-xyz[2]*yu)*v;
		eta=sqrt(1.0-SQ(v/c))/(1.0-(vx/c));
		if(eta*xf>0) F+=cos(alpha)*sthe[i]*I_em(alpha, eta)/SQ(eta);
		f=I_em(acos(xf), eta);
		vf=v_em*eta;
		fprintf(out, "%f %f %f %f %f\n", R*xf, R*yf, R*zf, f, vf);
		k++, a++;
	  }	
	
	}

	if(k==ns || k==1){fprintf(out, "\n");}
	else{

		for(;k<=ns;k++){
			fprintf(out, "%f %f %f %f %f\n", 
				R*xf, R*yf, R*zf, f, vf);
		}

		fprintf(out, "\n");
	}

}

fclose(out);

/*******************************************************************************
*       The flux is now multiplied by constants that were previously ommitted  *
*               and the return value is the flux.                              *
*******************************************************************************/

F*=SQ(R/d)*dth*dph;
return F;

}
