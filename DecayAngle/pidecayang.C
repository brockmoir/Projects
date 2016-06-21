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

void pidecayang()
{
   gROOT->Reset();

float E, p, Px, Py, Pz;
float phi,cos_theta;
const float PI=3.14159;
const float mpi=0.13957018;
Double_t ang [5];
Double_t Elab [] = {1, 50, 100, 500, 1000};
Double_t min [] = {PI, PI, PI, PI, PI};
Double_t max [] = {0, 0, 0, 0, 0};


  // Define a root file.
  TFile *f = new TFile("pidecayang.root","RECREATE");
  // create tree
  TTree picms("picms","Pi to photons in CMS frame");
  TTree pibo1("piboost1","Pi to photons in lab frame 1GeV");
  TTree pibo50("piboost50","Pi to photons in lab frame 50GeV");
  TTree pibo100("piboost100","Pi to photons in lab frame 100GeV");
  TTree pibo500("piboost500","Pi to photons in lab frame 500GeV");
  TTree pibo1000("piboost1000","Pi to photons in lab frame 1000GeV");
  TNtuple *angE=new TNtuple("Eang", "Energy and Angles", "min:max:energy");
  TNtuple *angle=new TNtuple("Angle", "Angles", "ang1:ang50:ang100:ang500:ang1000");

  TH1D *h1 = new TH1D("ang1", "Decay Angles 1GeV lab frame", 100, 0, PI);
  TH1D *h2 = new TH1D("ang50", "Decay Angles 50GeV lab frame", 100, 0., 0.2);
  TH1D *h3 = new TH1D("ang100", "Decay Angles 100GeV lab frame", 100, 0., 0.00004);
  TH1D *h4 = new TH1D("ang500", "Decay Angles 500GeV lab frame", 100, 0., 0.00000004);
  TH1D *h5 = new TH1D("ang1000", "Decay Angles 1000GeV lab frame", 100, 0., 0.00000004);


  TLorentzVector *P1=new TLorentzVector();
  TLorentzVector *P2=new TLorentzVector();
  TLorentzVector *bP1=new TLorentzVector();
  TLorentzVector *bP2=new TLorentzVector();

  picms.Branch("P1","TLorentzVector",&P1);
  picms.Branch("P2","TLorentzVector",&P2);
  
  pibo1.Branch("b1P1","TLorentzVector",&bP1);
  pibo1.Branch("b1P2","TLorentzVector",&bP2);
  
  pibo50.Branch("b50P1","TLorentzVector",&bP1);
  pibo50.Branch("b50P2","TLorentzVector",&bP2);

  pibo100.Branch("b100P1","TLorentzVector",&bP1);
  pibo100.Branch("b100P2","TLorentzVector",&bP2);

  pibo500.Branch("b500P1","TLorentzVector",&bP1);
  pibo500.Branch("b500P2","TLorentzVector",&bP2);

  pibo1000.Branch("b1000P1","TLorentzVector",&bP1);
  pibo1000.Branch("b1000P2","TLorentzVector",&bP2);

for (int i=0; i<1000; i++) {
  phi = 2.0 * PI *(gRandom->Rndm());                // generate phi
  cos_theta = 2.0 * (gRandom->Rndm())-1;            //generate cos(theta)

  E=mpi/2.0;
  p=E;
  if(i=0){
    Px = 0;
    Py = 0;
    Pz = p;
  }else{
    Px = p * cos(phi) * sqrt( 1.0 - pow(cos_theta,2));
    Py = p * sin(phi) * sqrt( 1.0 - pow(cos_theta,2));
    Pz = p * cos_theta;
  }
  P1->SetPxPyPzE(Px, Py, Pz, E);
  P2->SetPxPyPzE(-Px, -Py, -Pz, E);
  picms.Fill();

  bP1=P1;
  bP2=P2;  
  bP1->Boost(0,0,sqrt(Elab[0]*Elab[0]-mpi*mpi)/Elab[0]);
  bP2->Boost(0,0,sqrt(Elab[0]*Elab[0]-mpi*mpi)/Elab[0]);
  pibo1.Fill();
  ang[0]=  bP1->Angle(bP2->Vect());
  if(ang[0]<min[0]){ min[0]=ang[0];}
  if(ang[0]>max[0]){ max[0]=ang[0];}

  bP1=P1;
  bP2=P2;  
  bP1->Boost(0,0,sqrt(Elab[1]*Elab[1]-mpi*mpi)/Elab[1]);
  bP2->Boost(0,0,sqrt(Elab[1]*Elab[1]-mpi*mpi)/Elab[1]);
  pibo50.Fill();
  ang[1]=  bP1->Angle(bP2->Vect());
  if(ang[1]<min[1]){ min[1]=ang[1];}
  if(ang[1]>max[1]){ max[1]=ang[1];}

  bP1=P1;
  bP2=P2;  
  bP1->Boost(0,0,sqrt(Elab[2]*Elab[2]-mpi*mpi)/Elab[2]);
  bP2->Boost(0,0,sqrt(Elab[2]*Elab[2]-mpi*mpi)/Elab[2]);
  pibo100.Fill();
  ang[2]=  bP1->Angle(bP2->Vect());
  if(ang[2]<min[2]){ min[2]=ang[2];}
  if(ang[2]>max[2]){ max[2]=ang[2];}


  bP1=P1;
  bP2=P2;  
  bP1->Boost(0,0,sqrt(Elab[3]*Elab[3]-mpi*mpi)/Elab[3]);
  bP2->Boost(0,0,sqrt(Elab[3]*Elab[3]-mpi*mpi)/Elab[3]);
  pibo500.Fill();
  ang[3]=  bP1->Angle(bP2->Vect());
  if(ang[3]<min[3]){ min[3]=ang[3];}
  if(ang[3]>max[3]){ max[3]=ang[3];}

  bP1=P1;
  bP2=P2;  
  bP1->Boost(0,0,sqrt(Elab[4]*Elab[4]-mpi*mpi)/Elab[4]);
  bP2->Boost(0,0,sqrt(Elab[4]*Elab[4]-mpi*mpi)/Elab[4]);
  pibo1000.Fill();
  ang[4]=  bP1->Angle(bP2->Vect());
  if(ang[4]<min[4]){ min[4]=ang[4];}
  if(ang[4]>max[4]){ max[4]=ang[4];}
  
  angle->Fill(ang[0], ang[1], ang[2], ang[3], ang[4]);
  h1->Fill(ang[0]);
  h2->Fill(ang[1]);
  h3->Fill(ang[2]);
  h4->Fill(ang[3]);
  h5->Fill(ang[4]);
  }

for(int i =0; i<5; i++){
  angE->Fill(min[i], max[i], Elab[i]);
}   
cout<<min[0]<< " "<<max[0]<<endl;
cout<<min[1]<< " "<<max[1]<<endl;
cout<<min[2]<< " "<<max[2]<<endl;
cout<<min[3]<< " "<<max[3]<<endl;
cout<<min[4]<< " "<<max[4]<<endl;
 f->Write();
}
