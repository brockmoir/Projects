/*******************************************************************************
********************************************************************************
*	This module includes any transformations needed to produce the star    *
*		and the spot.						       *
***************************************************************************************************************************************************************/

/*******************************************************************************
*	allign transforms a given set of coordinates from the star's frame,    *
*		with a rotation axis about the z axis, to the observer's frame,*
*		with a rotation axis user specified in const.h.                *
*******************************************************************************/

void allign(double t, double xyz[], double cphi, double sphi, 
	    double cthet, double sthet){
	double x=sthet*(cphi*cos(w*t)
		-sphi*sin(w*t)), 
	       y=sthet*(sphi*cos(w*t)
		+cphi*sin(w*t)), 
	       z=cthet;
	double sthu, cphu, sphu;
	
	if(zu == 1.0){
		xyz[0]=x, xyz[1]=y, xyz[2]=z;	

	}else if(zu == -1.0){
		xyz[0]=x, xyz[1]=-y, xyz[2]=-z;
	}else{
		sthu=sqrt(1.0-SQ(zu)); 
		cphu=xu/sthu, sphu=yu/sthu;
	xyz[0] = x*zu*cphu-y*sphu+z*sthu*cphu;
	xyz[1] = x*zu*sphu+y*cphu+z*sthu*sphu;
	xyz[2] = -x*sthu+z*zu;

	}
}

/*******************************************************************************
*	obstovel transforms a point from the observer's frame into the frame   *
*		in which the velocity lies along the z axis.  This makes       *
*		creating the relativistic spot easier.                         *
*******************************************************************************/

void obstovel(double xyz[], double xc, double yc, double zc, double thphc[]){
double A;

A = sqrt(SQ(zc*yu-yc*zu)+SQ(xc*zu-zc*xu)+SQ(yc*xu-xc*yu));

thphc[0]=(yc*xu-xc*yu)/A; thphc[1]=sqrt(1.0-SQ(thphc[0]));

if(thphc[0]==1.0 || thphc[0]==-1.0){ 
	thphc[2]=0, thphc[3]=0;
}else{
	thphc[2]=(zc*yu-yc*zu)/(thphc[1]*A), 
	thphc[3]=(xc*zu-zc*xu)/(thphc[1]*A);
}

xyz[0] = xc*thphc[0]*thphc[2]+yc*thphc[0]*thphc[3]-zc*thphc[1];
xyz[1] = -xc*thphc[3]+yc*thphc[2];
xyz[2] = xc*thphc[1]*thphc[2]+yc*thphc[1]*thphc[3]+zc*thphc[0];

}

/*******************************************************************************
*	veltoobs transforms the spot from the velocity frame back into the     *
*		observer's frame.                                              *
*******************************************************************************/

void veltoobs(double xyz[], double cph, double sph, double cth, double sth,
	      double thphc[]){
	double x, y, z;
	x=sth*cph, y=sth*sph, z=cth;
xyz[0] = x*thphc[0]*thphc[2]-y*thphc[3]+z*thphc[1]*thphc[2];
xyz[1] = x*thphc[0]*thphc[3]+y*thphc[2]+z*thphc[1]*thphc[3];
xyz[2] = -x*thphc[1]+z*thphc[0];

}

