TH2D* MatBuild(TH1D* res){
  Int_t n = res->GetNbinsX();
  Double_t xmin = res->GetXaxis()->GetXmin();
  Double_t xmax = res->GetXaxis()->GetXmax();
  std::ostringstream t_name;
  t_name << res->GetName() << "mat";
  TString name = t_name.str();
  TH2D *mat = new TH2D(name, name, n, xmin, xmax, n, xmin, xmax);
  int nzero=res->FindBin(0);
  for(int i=0; i<n; i++){
    for(int j=0; j<n; j++){
      if(i-j+nzero<n){
        mat->SetBinContent(i+1, j+1, res->GetBinContent(i-j+nzero));
        mat->SetBinError(i+1, j+1, res->GetBinError(i-j+nzero));
      }
    }
  }
  return mat;
}

TH1D* Unfold (TH1D* data, TH1D* res)
{
  TH2D *mat = MatBuild(res);
  RooUnfoldResponse R(0, 0, mat);
  RooUnfoldBayes unfold(&R, data);
  TH1D* un = (TH1D*) unfold.Hreco();

  return un;
}

void gauss()
{
  gStyle->SetOptStat(111111111);
  //  gSystem->Load("/home1x/hep/atlas/RooUnfold-1.1.1/libRooUnfold");

  TFile *fout = new TFile("gauss.root", "RECREATE");

  Double_t x1, x2, x3;
  Int_t nbins=100, npoints=10000;
  Double_t xlow=-1, xhigh=1;
  Double_t mu1=-0.1, mu2=0.3, mu3=0.2;
  Double_t sigma1=0.04, sigma2=0.03, sigma3=0.05;
  Double_t norm = 1./sqrt(2*3.14159);

  TH1D *G1 = new TH1D("G1","G1", nbins,xlow, xhigh);
  TH1D *G2 = new TH1D("G2","G2", nbins,xlow, xhigh);
  TH1D *G3 = new TH1D("G3","G3", nbins,xlow, xhigh);
  TH1D *G3con = new TH1D("G3con","G3con", nbins,xlow, xhigh);

  TH2D matG1;
  TH2D matG2;

  TH1D *unG1;
  TH1D *unG2;

  for(int i=0; i<npoints; i++){
     x1 = gRandom->Gaus(mu1, sigma1); 
     x2 = gRandom->Gaus(mu2, sigma2);
     x3 = gRandom->Gaus(mu3, sigma3);
     G1->Fill(x1);
     G2->Fill(x2);
     G3->Fill(x3);
     G3con->Fill(x1+x2);
  }
   
  G1->Scale(norm/(sigma1*G1->Integral()));
  G2->Scale(norm/(sigma2*G2->Integral()));
  G3->Scale(norm/(sigma3*G3->Integral()));
  G3con->Scale(norm/(sigma3*G3con->Integral()));

  matG1 = MatBuild(G1);
  matG2 = MatBuild(G2);

  fout->Write();
}

