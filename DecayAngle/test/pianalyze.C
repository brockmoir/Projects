#include "Riostream.h"
#include <cmath>
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include "TLorentzVector.h"
#include "Math/Vector4D.h"

void pianalyze(){
Double_t ang;
Double_t p1mag, p2mag;
double PI=3.14159;

TFile *f= new TFile("pidecay.root", "Update");
TTree *pibo1 = (TTree*)f->Get("piboost1");
TTree *pibo2 = (TTree*)f->Get("piboost2");
TTree *pibo3 = (TTree*)f->Get("piboost3");
TTree *pibo4 = (TTree*)f->Get("piboost4");
TTree *pibo5 = (TTree*)f->Get("piboost5");
Long64_t nentries = pibo1->GetEntries();
TH1D *h1 = new TH1D("ang1", "Decay Angles", nentries/100, 0, PI);
TH1D *h2 = new TH1D("ang2", "Decay Angles", nentries/100, 0, PI);
TH1D *h3 = new TH1D("ang3", "Decay Angles", nentries/100, 0, PI);
TH1D *h4 = new TH1D("ang4", "Decay Angles", nentries/100, 0, PI);
TH1D *h5 = new TH1D("ang5", "Decay Angles", nentries/100, 0, PI);


TLorentzVector *P1;
TLorentzVector *P2;


for (Long64_t i=0; i<nentries; i++){

pibo1->SetBranchAddress("b1P1.", &P1);
pibo1->SetBranchAddress("b1P2.", &P2);
pibo1->GetEntry(i);
ang = (P1->Vect()).Angle(P2->Vect()); 
//p1mag = P1->Beta() * P1->E();
//p2mag = P2->Beta() * P2->E();
h1->Fill(ang);
//if(ang>1.5)cout<<ang<<"\n";  
P1=0;
P2=0;
ang=0;

pibo2->SetBranchAddress("b2P1.", &P1);
pibo2->SetBranchAddress("b2P2.", &P2);
pibo2->GetEntry(i);
ang = (P1->Vect()).Angle(P2->Vect());
h2->Fill(ang);
if(ang>1.5)cout<<ang<<"\n"; 

pibo3->SetBranchAddress("b3P1.", &P1);
pibo3->SetBranchAddress("b3P2.", &P2);
pibo3->GetEntry(i);
ang = (P1->Vect()).Angle(P2->Vect()); 
h3->Fill(ang);

pibo4->SetBranchAddress("b4P1.", &P1);
pibo4->SetBranchAddress("b4P2.", &P2);
pibo4->GetEntry(i);
ang = (P1->Vect()).Angle(P2->Vect()); 
h4->Fill(ang);

pibo5->SetBranchAddress("b5P1.", &P1);
pibo5->SetBranchAddress("b5P2.", &P2);
pibo5->GetEntry(i);
ang = (P1->Vect()).Angle(P2->Vect()); 
h5->Fill(ang);

}
f->Write();
}
