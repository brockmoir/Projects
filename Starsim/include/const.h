#define pi 3.1415972  /* value of pi */
#define c 3e8	      /* speed of light */
#define acc 0.000001  /* accepted accuracy for certain numerical quantities */

/*******************************************************************************
*	General properties of the star and info about the spot                 *
*		==> R is the star's radius  				       *
*		==> I_o is the maximum emitted intensity                       *
*		==> v_em is the frequency in the emitter's frame               *
*		==> d is the observer's distance to the star                   *
*		==> M is the mass of the star                                  *
*		==> set l=0 to show intensity, or l=1 to show doppler shift    *
*******************************************************************************/

#define R 1.0	   
#define I_o 1.0
#define v_em 1.0
#define d 3.0
#define M 1.0/3.5
#define l 0

/*******************************************************************************
*         these quantities define the star's rotation, keeping in mind         *
*	         we are to be observing the star from the x-axis	       *
*		==> w gives the angular velocity about the rotation axis       *
*		==> xu, yu, zu are the coordinates defining the rotation axis  *
*			note that xu*xu+yu*yu+zu*zu must = 1                   *
*******************************************************************************/

#define w (1.0e8) 
#define xu 0.0
#define yu 1.0/sqrt(2.0)
#define zu 1.0/sqrt(2.0)

/*******************************************************************************
*        General properties of the spot to be taken in the emitter's           *
*	 frame of reference						       *
*			==> sdan is angular size of the spot (radians)         *
*			==> theta_c is initial zenith location of the spot     *
*			==> phi_c is initial circumfrential location of spot   *
*******************************************************************************/

#define sdan  0.5 
#define theta_c pi/4.0
#define phi_c -pi 

/*******************************************************************************
*	These constants are used to define the points in the                   *
*        latitude, longitude and spot's location                               *
*		==> nlat is the number of latitudinal lines                    *
*		==> nlon is the number of longitudinal lines                   *
*		==> np is the number of points in each line of lat/lon         *
*		==> ns is the number of points in the spot                     *
*		==> m is the number of points used to store a number of alpha  *
*			and psi values used to find alpha from psi             *
*******************************************************************************/

#define nlat 30     
#define	nlon 30	
#define np 60   
#define ns 200  
#define m 2000

/* some quick macros */
#define SQ(x) ((x)*(x))
#define CU(x) ((x)*(x)*(x))
