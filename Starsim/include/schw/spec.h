/***************************************************************************************************************************************************************
*	This component deals with the special relativistic aspects of the spot *
***************************************************************************************************************************************************************/

/*******************************************************************************
*	spec deals with creating the spot as it would appear to be shifted due *
*		to special relativity.					       *
*******************************************************************************/

void spec(double t, double cphi[], double sphi[], double cthe[], double sthe[], 
	  double scthe[ns][ns], double ssthe[ns][ns], double thphc[]){
double xyz[3],
	dth, dph, dzo,
	v, vx, vy, vz, h, eta; 
int i, j, k;

v=R*w;

allign(t, xyz, cos(phi_c), sin(phi_c), cos(theta_c), sin(theta_c));
obstovel(xyz, xyz[0], xyz[1], xyz[2], thphc);


k=ns/2-1;
cthe[k]=xyz[2]; sthe[k]=sqrt(1-SQ(xyz[2]));
if(xyz[2]==1.0 || xyz[2]==-1.0){ cphi[k]=0, sphi[k]=0;}
else {cphi[k]=xyz[0]/sthe[k], sphi[k]=xyz[1]/sthe[k];}

dph=2.0*((double) sdan/ns), dth=((double) sdan/(ns));

for(i=k+1, h=1.0;i<ns;i++, h+=1.0){
	cphi[i]=cphi[k]*cos(h*dph)-sphi[k]*sin(h*dph);
	sphi[i]=sphi[k]*cos(h*dph)+cphi[k]*sin(h*dph);
	cthe[i]=cthe[k]*cos(h*dth)-sthe[k]*sin(h*dth);
	sthe[i]=sthe[k]*cos(h*dth)+cthe[k]*sin(h*dth);
}

for(i=k-1, h=1.0;i>=0;i--, h+=1.0){
	cphi[i]=cphi[k]*cos(h*dph)+sphi[k]*sin(h*dph);
	sphi[i]=sphi[k]*cos(h*dph)-cphi[k]*sin(h*dph);
	cthe[i]=cthe[k]*cos(h*dth)+sthe[k]*sin(h*dth);
	sthe[i]=sthe[k]*cos(h*dth)-cthe[k]*sin(h*dth);
}

for(i=0;i<ns;i++){
scthe[i][k]=cthe[k]; ssthe[i][k]=sthe[k];
}

for(i=0;i<ns;i++){
	for(j=k;j>=0;j--){
		veltoobs(xyz, cphi[i], sphi[i], cthe[j], sthe[j], thphc);
		vx	  =-(xyz[1]*zu-xyz[2]*yu)*v;
		eta       =sqrt(1.0-SQ(v/c))/(1.0-(vx/c));
		if(j-1>=0){
		dzo=sthe[j]*dth/(SQ(eta)*ssthe[i][j]);
		scthe[i][j-1]= scthe[i][j]*cos(dzo)+ssthe[i][j]*sin(dzo);
		ssthe[i][j-1]= ssthe[i][j]*cos(dzo)-scthe[i][j]*sin(dzo);
		}
	}

	for(j=k;j<ns;j++){
		veltoobs(xyz, cphi[i], sphi[i], cthe[j], sthe[j], thphc);
		vx	  =-(xyz[1]*zu-xyz[2]*yu)*v;
		eta	  =sqrt(1.0-SQ(v/c))/(1.0-(vx/c));
		if(j+1<ns){
		dzo=sthe[j]*dth/(SQ(eta)*ssthe[i][j]);
		scthe[i][j+1]= scthe[i][j]*cos(dzo)-ssthe[i][j]*sin(dzo);
		ssthe[i][j+1]= ssthe[i][j]*cos(dzo)+scthe[i][j]*sin(dzo);
		}
	}	
}

}

