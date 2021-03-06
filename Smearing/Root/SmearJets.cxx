/****************************************************
*****************************************************
SmearJets
Author: Brock Moir

This class corrects the MC using the smearing function that 
has previously been generated using anothe tool.  

*****************************************************
*****************************************************/

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <EventLoop/OutputStream.h>

#include <Smearing/SmearJets.h>
#include <Smearing/BinningTool.h>

#include <TFile.h>

#include <iostream>
#include <sstream>
#include <string>

// this is needed to distribute the algorithm to the workers
ClassImp(SmearJets);


// constructor
SmearJets :: SmearJets ()
{
  m_niter = 1000;
  m_smearingfile = 0;
  
  m_analysistree=0;
  m_outputStreamName="output";
  m_bintool = 0;
};


// things to do before the job builds
EL::StatusCode SmearJets :: setupJob (EL::Job& job)
{
  EL::OutputStream stream (m_outputStreamName.Data());
  job.outputAdd	(stream);
  job.useD3PDReader ();

  return EL::StatusCode::SUCCESS;
};


// things to do when changing files
EL::StatusCode SmearJets :: changeInput (bool firstFile)
{
  m_analysistree= new AnalysisTree(wk()->tree());

  return EL::StatusCode::SUCCESS;
};


// things to do before the job runs
EL::StatusCode SmearJets :: initialize ()
{
  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();

  outputFile= wk()->getOutputFile (m_outputStreamName.Data());
  m_smearingfile = new TFile(m_smearpath.c_str());

  m_bintool->Initialize();

  this->BookHistos();

  return EL::StatusCode::SUCCESS;
};


// this produces the result of my analysis
// execute automatically loops over each entry in my data set
// each entry has been cleaned to only contain a back to back jet and photon
EL::StatusCode SmearJets :: execute ()
{
  m_analysistree->GetEntry(wk()->treeEntry());

  // start by finding the appropriate bin for this jet
  std::string binname = m_bintool->GetBinName((*m_analysistree->jet_eta)[0], (*m_analysistree->jet_phi)[0], (*m_analysistree->jet_pt)[0]);
  std::vector <int> binindex = m_bintool->GetBinIndices((*m_analysistree->jet_eta)[0], (*m_analysistree->jet_phi)[0], (*m_analysistree->jet_pt)[0]);

  // each event has a unique event weight
  float weight =  m_analysistree->weight;

  // the smearing histogram was produced earlier in the analysis
  // by using a unique unfolding method based on Bayes' theorem
  hsmear = (TH1F*) m_smearingfile->Get(binname.c_str());
  if(hsmear->Integral() == 0) return EL::StatusCode::SUCCESS;

  // to eliminate the effect of noise caused by stochastically smearing each event
  // this analysis smears a large number of times and averages the results
  for(int i=0; i<m_niter; i++){
    float jetpt = (*m_analysistree->jet_pt)[0] * (1 + hsmear->GetRandom());
    float phpt = (*m_analysistree->ph_pt)[0];
    float res = jetpt/phpt - 1.0;

    histos[i][binindex[0]]->Fill(res, weight);
    h_etabins[i][binindex[1]]->Fill(res, weight);
    h_phibins[i][binindex[2]]->Fill(res, weight);
    h_ptbins[i][binindex[3]]->Fill(res, weight);

    allresbin[i]->Fill(res, weight);
    htjetpt[i]->Fill(jetpt/1000, weight);
    htdiffpt[i]->Fill((jetpt-phpt)/1000, weight);
  }

  return EL::StatusCode::SUCCESS;
};


// things to do once all jobs have run
// this includes filling and comparing histograms for each bin
EL::StatusCode SmearJets :: finalize ()
{
  float x=0, y=0;
  TH1F  *hdata;

  // fill and compare the histograms with full binning
  for(int j=0; j<m_bintool->nbins; j++){
    std::string histname = m_bintool->binnames[j];
    hdata = (TH1F*)data->Get(histname.c_str());
    hdata->Scale(1.0/hdata->Integral("width"));
    for(int i=0; i<m_niter; i++){
      histos[i][j]->Scale(1.0/histos[i][j]->Integral("width"));
      float chi2ndf = hdata->Chi2Test(histos[i][j], "WW CHI2/NDF");
      float chi2    = hdata->Chi2Test(histos[i][j], "WW CHI2");
      hchi2NDF[j]->Fill(chi2ndf);
      hchi2[j]   ->Fill(chi2);
      for(int k=0; k<histos[i][j]->GetNbinsX(); k++){
        y = histos[i][j]->GetBinContent(k+1);
        x = histos[i][j]->GetBinCenter(k+1);
        histos2d[j]->Fill(x,y);
      }
    }
  }
 
  // fill and compare histograms with just eta binning
  for(unsigned int j=0; j<m_bintool->etanames.size(); j++){
    std::string histname = m_bintool->etanames[j];
    hdata = (TH1F*)data->Get(histname.c_str());
    hdata->Scale(1.0/hdata->Integral("width"));
    for(int i=0; i<m_niter; i++){
      h_etabins[i][j]->Scale(1.0/h_etabins[i][j]->Integral("width"));
      float chi2ndf = hdata->Chi2Test(h_etabins[i][j], "WW CHI2/NDF");
      float chi2    = hdata->Chi2Test(h_etabins[i][j], "WW CHI2");
      hchi2NDF_eta[j]->Fill(chi2ndf);
      hchi2_eta[j]   ->Fill(chi2);
      for(int k=0; k<h_etabins[i][j]->GetNbinsX(); k++){
        y = h_etabins[i][j]->GetBinContent(k+1);
        x = h_etabins[i][j]->GetBinCenter(k+1);
        hist_etabins[j]->Fill(x,y);
      }
    }
  }
 
  // fill and compare histograms with just phi binning
  for(unsigned int j=0; j<m_bintool->phinames.size(); j++){
    std::string histname = m_bintool->phinames[j];
    hdata = (TH1F*)data->Get(histname.c_str());
    hdata->Scale(1.0/hdata->Integral("width"));
    for(int i=0; i<m_niter; i++){
      h_phibins[i][j]->Scale(1.0/h_phibins[i][j]->Integral("width"));
      float chi2ndf = hdata->Chi2Test(h_phibins[i][j], "WW CHI2/NDF");
      float chi2    = hdata->Chi2Test(h_phibins[i][j], "WW CHI2");
      hchi2NDF_phi[j]->Fill(chi2ndf);
      hchi2_phi[j]   ->Fill(chi2);
      for(int k=0; k<h_phibins[i][j]->GetNbinsX(); k++){
        y = h_phibins[i][j]->GetBinContent(k+1);
        x = h_phibins[i][j]->GetBinCenter(k+1);
        hist_phibins[j]->Fill(x,y);
      }
    }
  }

  // fill and compare histograms with just pt binning
  for(unsigned int j=0; j<m_bintool->ptnames.size(); j++){
    std::string histname = m_bintool->ptnames[j];
    hdata = (TH1F*)data->Get(histname.c_str());
    hdata->Scale(1.0/hdata->Integral("width"));
    for(int i=0; i<m_niter; i++){
      h_ptbins[i][j]->Scale(1.0/h_ptbins[i][j]->Integral("width"));
      float chi2ndf = hdata->Chi2Test(h_ptbins[i][j], "WW CHI2/NDF");
      float chi2    = hdata->Chi2Test(h_ptbins[i][j], "WW CHI2");
      hchi2NDF_pt[j]->Fill(chi2ndf);
      hchi2_pt[j]   ->Fill(chi2);
      for(int k=0; k<h_ptbins[i][j]->GetNbinsX(); k++){
        y = h_ptbins[i][j]->GetBinContent(k+1);
        x = h_ptbins[i][j]->GetBinCenter(k+1);
        hist_ptbins[j]->Fill(x,y);
      }
    }
  }

  // fill histogram with just jet pt
  for(int i=0; i<m_niter; i++){
    for(int k=0; k<htjetpt[i]->GetNbinsX(); k++){
      y = htjetpt[i]->GetBinContent(k+1);
      x = htjetpt[i]->GetBinCenter(k+1);
      hjetpt->Fill(x,y);
    }
  }

  // fill histogram with jet pt minus photon pt
  for(int i=0; i<m_niter; i++){
    for(int k=0; k<htdiffpt[i]->GetNbinsX(); k++){
      y = htdiffpt[i]->GetBinContent(k+1);
      x = htdiffpt[i]->GetBinCenter(k+1);
      hdiffpt->Fill(x,y);
    }
  }

  return EL::StatusCode::SUCCESS;
};


// here is where all of the histograms are declared and initialized
void SmearJets :: BookHistos()
{

  int nbins=100;
  double xmin=-2, xmax=2;
  
  for(unsigned int i=0; i < m_bintool->binnames.size(); i++){
    TH2F *htemp2d = new TH2F((m_bintool->binnames[i]).c_str(), (m_bintool->binnames[i]).c_str(), nbins, xmin, xmax, nbins, 0.0, 2.0);
    histos2d.push_back(htemp2d);
    wk()->addOutput(htemp2d);

    TH1F *htchi2 = new TH1F(("hchi2" + m_bintool->binnames[i]).c_str(), ("hchi2" + m_bintool->binnames[i]).c_str(), 100, 0, 500);
    hchi2.push_back(htchi2);
    wk()->addOutput(htchi2);
 
    TH1F *htchi2NDF = new TH1F(("hchi2NDF" + m_bintool->binnames[i]).c_str(), ("hchi2NDF" + m_bintool->binnames[i]).c_str(), 100, 0, 10);
    hchi2NDF.push_back(htchi2NDF);
    wk()->addOutput(htchi2NDF);

    TH1F *thistav = new TH1F(("av" + m_bintool->binnames[i]).c_str(), ("av" + m_bintool->binnames[i]).c_str(), 100, -2, 2);
    histav.push_back(thistav);
    wk()->addOutput(thistav);
  }

  for(unsigned int i=0; i < m_bintool->etanames.size(); i++){
    TH2F *htemp = new TH2F((m_bintool->etanames[i]).c_str(), (m_bintool->etanames[i]).c_str(), nbins, xmin, xmax, nbins, 0, 2.0);
    hist_etabins.push_back(htemp);
    wk()->addOutput(htemp);

    TH1F *htchi2 = new TH1F(("hchi2" + m_bintool->etanames[i]).c_str(), ("hchi2" + m_bintool->etanames[i]).c_str(), 100, 0, 700);
    hchi2_eta.push_back(htchi2);
    wk()->addOutput(htchi2);

    TH1F *htchi2NDF = new TH1F(("hchi2NDF" + m_bintool->etanames[i]).c_str(), ("hchi2NDF" + m_bintool->etanames[i]).c_str(), 100, 0, 10);
    hchi2NDF_eta.push_back(htchi2NDF);
    wk()->addOutput(htchi2NDF);
  }

  for(unsigned int i=0; i < m_bintool->phinames.size(); i++){
    TH2F *htemp = new TH2F((m_bintool->phinames[i]).c_str(), (m_bintool->phinames[i]).c_str(), nbins, xmin, xmax, nbins, 0, 2.0);
    hist_phibins.push_back(htemp);
    wk()->addOutput(htemp);

    TH1F *htchi2 = new TH1F(("hchi2" + m_bintool->phinames[i]).c_str(), ("hchi2" + m_bintool->phinames[i]).c_str(), 100, 350, 650);
    hchi2_phi.push_back(htchi2);
    wk()->addOutput(htchi2);

    TH1F *htchi2NDF = new TH1F(("hchi2NDF" + m_bintool->phinames[i]).c_str(), ("hchi2NDF" + m_bintool->phinames[i]).c_str(), 100, 0, 10);
    hchi2NDF_phi.push_back(htchi2NDF);
    wk()->addOutput(htchi2NDF);
  }

  for(unsigned int i=0; i < m_bintool->ptnames.size(); i++){
    TH2F *htemp = new TH2F((m_bintool->ptnames[i]).c_str(), (m_bintool->ptnames[i]).c_str(), nbins, xmin, xmax,nbins, 0, 2.0);
    hist_ptbins.push_back(htemp);
    wk()->addOutput(htemp);

    if (i==0){
      TH1F *htchi2 = new TH1F(("hchi2" + m_bintool->ptnames[i]).c_str(), ("hchi2" + m_bintool->ptnames[i]).c_str(), 100, 2000, 2500);
      hchi2_pt.push_back(htchi2);
      wk()->addOutput(htchi2);
    }else if(i==1){
      TH1F *htchi2 = new TH1F(("hchi2" + m_bintool->ptnames[i]).c_str(), ("hchi2" + m_bintool->ptnames[i]).c_str(), 100, 1000, 1500);
      hchi2_pt.push_back(htchi2);
      wk()->addOutput(htchi2);
    }else{
      TH1F *htchi2 = new TH1F(("hchi2" + m_bintool->ptnames[i]).c_str(), ("hchi2" + m_bintool->ptnames[i]).c_str(), 100, 0, 250);
      hchi2_pt.push_back(htchi2);
      wk()->addOutput(htchi2);
    }

    float max;
    if(i==0) max = 60;
    if(i==1) max = 25;
    if(i==2) max = 10;   

    TH1F *htchi2NDF = new TH1F(("hchi2NDF" + m_bintool->ptnames[i]).c_str(), ("hchi2NDF" + m_bintool->ptnames[i]).c_str(), 100, 0, max);
    hchi2NDF_pt.push_back(htchi2NDF);
    wk()->addOutput(htchi2NDF);
  }

  for(int j=0; j<m_niter; j++){
    std::vector <TH1F*> htemprow; 
    std::vector <TH1F*> htempptrow; 
    std::vector <TH1F*> htempphirow; 
    std::vector <TH1F*> htempetarow; 
    std::ostringstream it;
    it<<j;
    std::string iter = it.str();

    htjetpt.push_back( new TH1F((iter + "tjetpt").c_str(), (iter + "tjetpt").c_str(), nbins, 30, 150));
    htdiffpt.push_back( new TH1F((iter + "tdiffpt").c_str(), (iter + "tdiffpt").c_str(), nbins, -150, 150));

    TH1F *temp1 = new TH1F((iter + "temp1").c_str(), (iter + "temp1").c_str(), nbins, xmin, xmax);
    TH1F *temp2 = new TH1F((iter + "temp2").c_str(), (iter + "temp2").c_str(), nbins, xmin, xmax);
    allresbin.push_back(temp1);
    allres.push_back(temp2);
    for(unsigned int i=0; i < m_bintool->binnames.size(); i++){
      std::string histname;
      histname = iter;
      histname += m_bintool->binnames[i];
      TH1F *htemp = new TH1F((iter + histname).c_str(), 
			     (iter + histname).c_str(), nbins, xmin, xmax);
      htemprow.push_back(htemp);
    }
    for(unsigned int i=0; i < m_bintool->ptnames.size(); i++){
      std::string histname;
      histname = iter;
      histname += m_bintool->ptnames[i];
      TH1F *htemp = new TH1F((iter + histname).c_str(), 
			     (iter + histname).c_str(), nbins, xmin, xmax);
      htempptrow.push_back(htemp);
    }
    for(unsigned int i=0; i < m_bintool->phinames.size(); i++){
      std::string histname;
      histname = iter;
      histname += m_bintool->phinames[i];
      TH1F *htemp = new TH1F((iter + histname).c_str(), 
			     (iter + histname).c_str(), nbins, xmin, xmax);
      htempphirow.push_back(htemp);
    }
    for(unsigned int i=0; i < m_bintool->etanames.size(); i++){
      std::string histname;
      histname = iter;
      histname += m_bintool->etanames[i];
      TH1F *htemp = new TH1F((iter + histname).c_str(), 
			     (iter + histname).c_str(), nbins, xmin, xmax);
      htempetarow.push_back(htemp);
    }
    h_ptbins.push_back(htempptrow);
    h_phibins.push_back(htempphirow);
    h_etabins.push_back(htempetarow);
    histos.push_back(htemprow);
  }

  hallres = new TH2F("allres","allres",100, -2, 2, 100, 0, 2);
  wk()->addOutput(hallres); 

  hallresbin = new TH2F("allresbin","allresbin",100, -2, 2, 100, 0, 2);
  wk()->addOutput(hallresbin);

  hallchi2 = new TH1F("hchi2allres","hchi2allres",100, 1500, 2000);
  wk()->addOutput(hallchi2);

  hallchi2NDF = new TH1F("hchi2NDFallres","hchi2NDFallres",50, 20, 30);
  wk()->addOutput(hallchi2NDF);

  hallNDF = new TH1F("hallNDF","hallNDF",100, 0, 100);
  wk()->addOutput(hallNDF);

  hallchi2bin = new TH1F("hchi2allresbin","hchi2allresbin",100, 1500, 2000);
  wk()->addOutput(hallchi2bin);

  hallchi2NDFbin = new TH1F("hchi2NDFallresbin","hchi2NDFallresbin",50, 20, 30);
  wk()->addOutput(hallchi2NDFbin);

  hallNDFbin = new TH1F("hallNDFbin","hallNDFbin",100, 0, 100);
  wk()->addOutput(hallNDFbin);

  hjetpt = new TH2F("jetpt", "jetpt", 100, 30, 150, 500, 0, 450);
  wk()->addOutput(hjetpt);

  hdiffpt = new TH2F("diffpt", "diffpt", 100, -150, 150, 500, 0, 850);
  wk()->addOutput(hdiffpt);
}

