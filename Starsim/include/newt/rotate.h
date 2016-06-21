/*******************************************************************************
*	allign() is used to orient each set of coordinates to rotate along     *
*	 the stars spin axis.  The transformation is from a coordinate system  *
*	 with the z-axis as a spin axis to a coordinate system where the       *
*	 vector u (defined in const.h) is the spin axis.                       **		==> t is the time (determines how much rotation to give        *
*		==> xyz[] is a 3D array which will contain the final x,y,z     *
*		==> cphi and sphi are the cosine and sine of phi in the initial*
*			frame 						       *
*		==> cthet and sthet are the cosine and sine of theta in the    *
*			initial frame                                          *
*		==> w is the angular frequency of the star (deined in const.h  *
*******************************************************************************/
 
void allign(double t, double xyz[], double cphi, double sphi, 
	    double cthet, double sthet){
	double x=sthet*(cphi*cos(w*t)-sphi*sin(w*t)), 
	       y=sthet*(sphi*cos(w*t)+cphi*sin(w*t)), 
	       z=cthet;
	double sthu, cphu, sphu,
		sx, sy, sz;
	
	if(zu == 1){
		xyz[0]=x, xyz[1]=y, xyz[2]=z;	

	}else if(zu == -1){
		xyz[0]=x, xyz[1]=-y, xyz[2]=-z;
	}else{
		sthu=sqrt(1.0-SQ(zu)); 
		cphu=xu/sthu; 
		sphu=yu/sthu;
	xyz[0] = x*zu*cphu-y*sphu+z*sthu*cphu;
	xyz[1] = x*zu*sphu+y*cphu+z*sthu*sphu;
	xyz[2] = -x*sthu+z*zu;

	}
}

