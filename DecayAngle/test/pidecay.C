#include "Riostream.h"
#include <cmath>
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include "TLorentzVector.h"
#include "Math/Vector4D.h"

//   edited version of the macro to produce points on a sphere
//   this macro's purpose is to generate decay angles from pi to gamma
//   the angles are first calculated in the CMS frame, then boosted to 
//   the lab frame
//   All units of energy are in GeV

void pidecay()
{
   gROOT->Reset();

Double_t E, p, Px, Py, Pz;
Double_t phi,cos_theta;
Double_t Elab [] = {1, 30, 50, 70, 100};
const float PI=3.14159;
const float mpi=0.13957018;

  // Define a root file.
TFile *f = new TFile("pidecay.root","RECREATE");
  // create trees
TTree picms("picms","Pi to photons in CMS frame");
TTree pibo1("piboost1","Pi to photons in lab frame");
TTree pibo2("piboost2","Pi to photons in lab frame");
TTree pibo3("piboost3","Pi to photons in lab frame");
TTree pibo4("piboost4","Pi to photons in lab frame");
TTree pibo5("piboost5","Pi to photons in lab frame");
TNtuple *boost=new TNtuple("Elab", "boost energy", "E");

for(int i=0; i<5; i++) boost->Fill(Elab[i]);

  // need some lorentz vectors
TLorentzVector *P1=new TLorentzVector();
TLorentzVector *P2=new TLorentzVector();
TLorentzVector *bP1=new TLorentzVector();
TLorentzVector *bP2=new TLorentzVector();
  
  // create branches
picms.Branch("P1.","TLorentzVector",&P1);
picms.Branch("P2.","TLorentzVector",&P2);
  
pibo1.Branch("b1P1.","TLorentzVector",&bP1);
pibo1.Branch("b1P2.","TLorentzVector",&bP2);
  
pibo2.Branch("b2P1.","TLorentzVector",&bP1);
pibo2.Branch("b2P2.","TLorentzVector",&bP2);

pibo3.Branch("b3P1.","TLorentzVector",&bP1);
pibo3.Branch("b3P2.","TLorentzVector",&bP2);

pibo4.Branch("b4P1.","TLorentzVector",&bP1);
pibo4.Branch("b4P2.","TLorentzVector",&bP2);

pibo5.Branch("b5P1.","TLorentzVector",&bP1);
pibo5.Branch("b5P2.","TLorentzVector",&bP2);

for (int i=0; i<100000; i++) {
    // generate phi and cos(theta)
  phi = 2.0 * PI *(gRandom->Rndm());
  cos_theta = 1.0 - 2.0 * (gRandom->Rndm());

    // fill the cms lorentz vectors
  E=mpi/2.0;
  p=E;
  Px = p * cos(phi) * sqrt( 1.0 - pow(cos_theta,2));
  Py = p * sin(phi) * sqrt( 1.0 - pow(cos_theta,2));
  Pz = p * cos_theta;
  P1->SetPxPyPzE(Px, Py, Pz, E);
  P2->SetPxPyPzE(-Px, -Py, -Pz, E);
  picms.Fill();

    // do some boosts and fill the branches with lorentz vectors
  bP1=P1;
  bP2=P2;  
  bP1->Boost(0,0,sqrt(Elab[0]*Elab[0]-mpi*mpi)/Elab[0]);
  bP2->Boost(0,0,sqrt(Elab[0]*Elab[0]-mpi*mpi)/Elab[0]);
  pibo1.Fill();

  bP1=P1;
  bP2=P2;  
  bP1->Boost(0,0,sqrt(Elab[1]*Elab[1]-mpi*mpi)/Elab[1]);
  bP2->Boost(0,0,sqrt(Elab[1]*Elab[1]-mpi*mpi)/Elab[1]);
  pibo2.Fill();

  bP1=P1;
  bP2=P2;  
  bP1->Boost(0,0,sqrt(Elab[2]*Elab[2]-mpi*mpi)/Elab[2]);
  bP2->Boost(0,0,sqrt(Elab[2]*Elab[2]-mpi*mpi)/Elab[2]);
  pibo3.Fill();

  bP1=P1;
  bP2=P2;  
  bP1->Boost(0,0,sqrt(Elab[3]*Elab[3]-mpi*mpi)/Elab[3]);
  bP2->Boost(0,0,sqrt(Elab[3]*Elab[3]-mpi*mpi)/Elab[3]);
  pibo4.Fill();

  bP1=P1;
  bP2=P2;  
  bP1->Boost(0,0,sqrt(Elab[4]*Elab[4]-mpi*mpi)/Elab[4]);
  bP2->Boost(0,0,sqrt(Elab[4]*Elab[4]-mpi*mpi)/Elab[4]);
  pibo5.Fill();
  }

f->Write();
}
