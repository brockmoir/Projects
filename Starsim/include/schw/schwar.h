/***************************************************************************************************************************************************************
*	Here we deal with all of the general relativistic aspects of the spot  *
***************************************************************************************************************************************************************/

/*******************************************************************************
*       I_em determines the observed intensity of a point on the spot in the   *
*               general relativistic sense.                                    *
*               ==> alphao is the observed angle between the x-axis and the    *
*                       point                                                  *
*               ==> eta is the lorentz boost factor for the point in question  *
*******************************************************************************/

double I_em(double alphao, double eta){
 double I, calphae;
 calphae=eta*cos(alphao);
 if(calphae>1.0 || calphae<0.0) I=0;
 else I=I_o*(1.0-calphae)*CU(eta*sqrt(1.0-2.0*M/R));
 return I;
}

/*******************************************************************************
*	y() is the value of the integrand for a given point this is the        *
*	this is the function that gives the alpha psi dependance               *
*******************************************************************************/

double y(double u, double b){
 double f;
 f=b/sqrt(1.0-SQ(u*b)*(1.0-2.0*M*u/R));
 return f;
}

/*******************************************************************************
*	intpsi() determines the value of psi for a given alpha                 *
*******************************************************************************/

double intpsi(double alpha){
 int n, i;
 double b, h, x, psi=0; 

 b=sin(alpha)/sqrt(1.0-2.0*M/R);
 if(b==0.0){ psi=0;
 }else{
	n=(int) sqrt(1.0/(24*acc*(1.0-2.0*M/R)));
	h=1.0/((double) n); 
	for(i=1; i<=n; i++){
		x=(((double) i)-0.5)*h;
		psi+=y(x, b);
	}
	psi*=h;
 }
 return psi;
}

/*******************************************************************************
*	psialp() records coresponding values of psi and alpha into the aalpha  *
*	and apsi arrays                                                        *
*******************************************************************************/

void psialp(double aalpha[], double apsi[]){
 double psi, alpha, 
	dalp;
 int i;

 dalp=(pi/2.0)/((double) m);
 
 for(alpha=0, i=0; i<m; alpha+=dalp, i++){
	psi=intpsi(alpha);
	apsi[i]=psi, aalpha[i]=alpha;
 }
}

/*******************************************************************************
*	alphaf() uses the previously recorded values of alpha and psi to       *
*	find a value of alpha for a given psi by interpolation.                *
*	The dependance of psi on alpha is fairly linear in small regions, which*
*	is why a linear interpolation method is used.                          *
*******************************************************************************/

double alphaf(double psi, double aalpha[], double apsi[]){

 int j, i, deg=2;
 double alp[deg], ps[deg],
	weight, alpha; 

 if(psi>apsi[m-1]) alpha=4.0;
 else{
	for(i=0; i<m; i++){
		if(psi==apsi[i]){
			return aalpha[i];
		}
	
		for(j=0;j<deg-1;j++){
			ps[j]=ps[j+1];
			alp[j]=alp[j+1];
		}
	
		ps[deg-1]=apsi[i]; alp[deg-1]=aalpha[i];

		if(psi>ps[((int) deg/2)-1] && 
		   psi<ps[((int) deg/2)]) break;		
	}	


alpha=alp[0]+((alp[1]-alp[0])/(ps[1]-ps[0]))*(psi-ps[0]);
 }

return alpha;
}

/*******************************************************************************
*	map() determines, for a given coordinate, the alpha and zeta values    *
*	needed to place the spot according to the schwarzchild metric          *
*******************************************************************************/

void map(double xyz[], double zy[], double zetalp[], 
	  double aalpha[], double apsi[]){

double alpha, psi, zeta, b;

psi=acos(xyz[0]);
if(sqrt(SQ(xyz[1])+SQ(xyz[2]))==0.0) zeta=0.0;
else zeta=acos(xyz[2]/sqrt(SQ(xyz[2])+SQ(xyz[1])));
if(xyz[1]<0) zeta=(-zeta);
alpha=alphaf(psi, aalpha, apsi);
if(alpha<3.9){
 b=sin(alpha)/sqrt(1.0-2.0*M/R);
 zy[0]=b*cos(zeta), zy[1]=b*sin(zeta);
}else{
 zy[0]=10, zy[1]=10;
}
zetalp[0]=zeta, zetalp[1]=alpha;
}
