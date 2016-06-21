#include "Riostream.h"
#include "TLorentzVector.h"
#include <cmath>
//   edited version of the macro to produce points on a sphere
//   this macro's purpose is to generate decay angles from pi to gamma, and 
//   rho to pi decay
//   the angles are first calculated in the CMS frame, then boosted to 
//   the lab frame
//   All units of energy are in GeV

void decayang1()
{
   gROOT->Reset();

  float E, p, px, py, pz;
  float phi,cos_theta;

  const float PI=3.14159;
  const float mpi=0.13957018;
  const float mrho=0.77549; 
  TLorentzVector P1, P2;

  TFile *f = new TFile("decayang.root","RECREATE");

  TNtuple *cmspi = new TNtuple("cmspi","Pi decay to two photons in CMS","px1:py1:pz1:E1:px2:py2:pz2:E2");
  TNtuple *cmsrho = new TNtuple("cmsrho","Rho decay to two pions in CMS","px1:py1:pz1:E1:px2:py2:pz2:E2");
  TNtuple *boostpi = new TNtuple("boostpi","Pi decay to two photons in lab frames","b1px1:b1py1:b1pz1:b1e1:b1px2:b1py2:b1pz2:b1e2");


  for (int i=0; i<10000; i++) {
    phi = 2.0 * PI *(gRandom->Rndm());                // generate phi
    cos_theta = 2.0 * (gRandom->Rndm())-1;            //generate cos(theta)

     //Start with the pi to photon decays
    E=mpi/2.0;
    p=E;
    px=p*cos(phi)*sqrt(1.0-pow(cos_theta,2));
    py=p*sin(phi)*sqrt(1.0-pow(cos_theta,2));
    pz=p*cos_theta;
    cmspi->Fill(px, py, pz, E, -px, -py, -pz, E);
    
    

     //Now do the rho to pi decays
    E=mrho/2.0;
    p=sqrt(E*E-mpi*mpi);
    px = p * cos(phi) * sqrt( 1.0 - pow(cos_theta,2));
    py = p * sin(phi) * sqrt( 1.0 - pow(cos_theta,2));
    pz = p * cos_theta;
    cmsrho->Fill(px, py, pz, E, -px, -py, -pz, E);
  }

  f->Write(); 
}


