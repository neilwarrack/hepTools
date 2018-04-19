#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLatex.h"
#include "TObject.h"
#include "TKey.h"
#include <iostream>
#include "TPad.h"
#include "TF1.h"

/*
  Root macro to extract and nice-ify all raw histograms produced from Tag and Probe Trigger Monitoring
*/

using namespace std;

unsigned int filenum=14;

void raw_hist_plotter_multi()
{
  
  TDirectory *where = gDirectory; //gDirectory = global directory         

  // Set canvasses

  TCanvas *c1= new TCanvas ("c1", "TEST", 1000, 1000);  
  
  TH1F* tester = new TH1F("Pt","Pt", 51,-100,100);

  gROOT->Reset();
  gStyle->SetPalette(kBird); // Set the "COLZ" palette to a nice one
  gStyle->SetOptStat(101111);  
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);



    TString filestring; //TFile will only accept a TString as an input
    stringstream filestringstream; //TStringstreamer class flakey but can use regular
    filestringstream << "expert-monitoring.root";    
    filestring = filestringstream.str(); //TString now contains variable filename
    TFile* fileparent = new TFile(filestring); //opens file dependent on Momentum and Theta values
    TDirectoryFile* file=(TDirectoryFile*)fileparent->Get("TrigMuonEFCombinerHypo_Muon_26GeV_v15a");
    
    //    where->cd(); //cd() sets current pad
    
    
    file->ReadTObject(tester,"Pt");

  
  c1->cd();
  tester->GetXaxis()->SetTitle("Dimuon Invariant Mass [GeV]");
  tester->GetYaxis()->SetTitle();
  tester->SetTitle("Event Filter Invariant Mass [GeV]");
  tester->Draw();
}
