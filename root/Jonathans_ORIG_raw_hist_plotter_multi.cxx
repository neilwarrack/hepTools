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

  //-------------------------------------------------Set up the canvasses---------------------------------------------------

  TCanvas *c1= new TCanvas ("c1", "Dimuon Invariant Mass", 1000, 1000);  
  TCanvas *c2= new TCanvas ("c2", "Delta_Z", 1000, 1000);  
  TCanvas *c3= new TCanvas ("c3", "Delta_R_TAG", 1000, 1000);
  TCanvas *c4= new TCanvas ("c4", "Delta_R_PROBE", 1000, 1000);
  TCanvas *c5= new TCanvas ("c5", "Delta_R_By_Threshold", 1000, 1000);


  //---------------------------------------------Setup the final histogram objects to be written to and summed up------------------
  
  TH1F* invmass = new TH1F("EF_FS_InvMass","EF_FS_InvMass", 51,0,140);
  TH1F* deltaZ = new TH1F("EF_FS_DeltaZ","EF_FS_#DeltaZ", 51,0,0.5);

//For some reason the axis limits don't match up on these histograms so as a crude fix set lower limit>upper limit to force histogram constructor to use variable bin size which then allows it to be forceably matched with root file histogram dimensions
  TH1F* deltaRT = new TH1F("LVL1_EF_DeltaR_TAG","LVL1_EF_#DeltaR", 51,0.2,0.1); 
  TH1F* deltaRP = new TH1F("LVL1_EF_DeltaR_PROBE","LVL1_EF_#DeltaR", 51,0.2,0.1);
  TH1F* deltaR1 = new TH1F("LVL1_EF_DeltaR_thresh1","LVL1_EF_#DeltaR", 51,0.2,0.1);
  TH1F* deltaR2 = new TH1F("LVL1_EF_DeltaR_thresh2","LVL1_EF_#DeltaR", 51,0.2,0.1);
  TH1F* deltaR3 = new TH1F("LVL1_EF_DeltaR_thresh3","LVL1_EF_#DeltaR", 51,0.2,0.1);
  TH1F* deltaR4 = new TH1F("LVL1_EF_DeltaR_thresh4","LVL1_EF_#DeltaR", 51,0.2,0.1);
  TH1F* deltaR5 = new TH1F("LVL1_EF_DeltaR_thresh5","LVL1_EF_#DeltaR", 51,0.2,0.1);
  TH1F* deltaR6 = new TH1F("LVL1_EF_DeltaR_thresh6","LVL1_EF_#DeltaR", 51,0.2,0.1);



  //---------------------------------------------Setup the holder histogram objects to be written to intermediately------------------

  TH1F* invmassh = new TH1F("EF_FS_InvMassh","EF_FS_InvMassh", 51,0,140);
  TH1F* deltaZh = new TH1F("EF_FS_DeltaZh","EF_FS_#DeltaZh", 51,0,0.5);
  TH1F* deltaRTh = new TH1F("LVL1_EF_DeltaR_TAGh","LVL1_EF_#DeltaRh", 51,0,0.1);
  TH1F* deltaRPh = new TH1F("LVL1_EF_DeltaR_PROBEh","LVL1_EF_#DeltaRh", 51,0,0.1);
  TH1F* deltaR1h = new TH1F("LVL1_EF_DeltaR_thresh1h","LVL1_EF_#DeltaRh", 51,0,0.1);
  TH1F* deltaR2h = new TH1F("LVL1_EF_DeltaR_thresh2h","LVL1_EF_#DeltaRh", 51,0,0.1);
  TH1F* deltaR3h = new TH1F("LVL1_EF_DeltaR_thresh3h","LVL1_EF_#DeltaRh", 51,0,0.1);
  TH1F* deltaR4h = new TH1F("LVL1_EF_DeltaR_thresh4h","LVL1_EF_#DeltaRh", 51,0,0.1);
  TH1F* deltaR5h = new TH1F("LVL1_EF_DeltaR_thresh5h","LVL1_EF_#DeltaRh", 51,0,0.1);
  TH1F* deltaR6h = new TH1F("LVL1_EF_DeltaR_thresh6h","LVL1_EF_#DeltaRh", 51,0,0.1);




  gROOT->Reset();
  gStyle->SetPalette(kBird); // Set the "COLZ" palette to a nice one
  gStyle->SetOptStat(101111);  
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);

  for (unsigned int i=1;i<filenum+1;i++){ //loop over Momentum values
    TString filestring; //TFile will only accept a TString as an input
    stringstream filestringstream; //TStringstreamer class flakey but can use regular streamer class to TString
    if(i<10){
      //      filestringstream << "/home/ppe/j/jjamieson/muon_trigger_development/run/user.jojamies.00304008.physics_Main.recon.DRAW_ZMUMU.r9264.021217v1_EXPERT.166434589/user.jojamies.12734690.EXPERT._0000"<<0<<i<<".root";
      //}
      //else{
      //filestringstream << "/home/ppe/j/jjamieson/muon_trigger_development/run/user.jojamies.00304008.physics_Main.recon.DRAW_ZMUMU.r9264.021217v1_EXPERT.166434589/user.jojamies.12734690.EXPERT._0000"<<i<<".root";

      filestringstream << "/home/ppe/j/jjamieson/muon_trigger_development/legacy_output/GRID_job_outputs/user.jojamies.00304008.physics_Main.recon.DRAW_ZMUMU.r9264.031217v1_EXPERT_total/user.jojamies.12739616.EXPERT._0000"<<0<<i<<".root";
    }
    else{
      filestringstream << "/home/ppe/j/jjamieson/muon_trigger_development/legacy_output/GRID_job_outputs/user.jojamies.00304008.physics_Main.recon.DRAW_ZMUMU.r9264.031217v1_EXPERT_total/user.jojamies.12739616.EXPERT._0000"<<i<<".root";
    }
    filestring = filestringstream.str(); //TString now contains variable filename
    TFile* fileparent = new TFile(filestring); //opens file dependent on Momentum and Theta values
    TDirectoryFile* file=(TDirectoryFile*)fileparent->Get("TrigMuonEFTagandProbe");
    
    where->cd(); //cd() sets current pad
    
    
    file->ReadTObject(invmassh,"EF_FS_InvMass");
    file->ReadTObject(deltaZh,"EF_FS_DeltaZ");
    file->ReadTObject(deltaRTh,"LVL1_EF_DeltaR_TAG");
    file->ReadTObject(deltaRPh,"LVL1_EF_DeltaR_PROBE");
    file->ReadTObject(deltaR1h,"LVL1_EF_DeltaR_Thresh1");
    file->ReadTObject(deltaR2h,"LVL1_EF_DeltaR_Thresh2");
    file->ReadTObject(deltaR3h,"LVL1_EF_DeltaR_Thresh3");
    file->ReadTObject(deltaR4h,"LVL1_EF_DeltaR_Thresh4");
    file->ReadTObject(deltaR5h,"LVL1_EF_DeltaR_Thresh5");
    file->ReadTObject(deltaR6h,"LVL1_EF_DeltaR_Thresh6");
    
    
    invmass->Add(invmassh);
    deltaZ->Add(deltaZh);
    deltaRT->Add(deltaRTh);
    deltaRP->Add(deltaRPh);
    deltaR1->Add(deltaR1h);
    deltaR2->Add(deltaR2h);
    deltaR3->Add(deltaR3h);
    deltaR4->Add(deltaR4h);
    deltaR5->Add(deltaR5h);
    deltaR6->Add(deltaR6h);
    
    
  }
  
  c1->cd();
  invmass->GetXaxis()->SetTitle("Dimuon Invariant Mass [GeV]");
  invmass->GetYaxis()->SetTitle();
  //  invmass->GetXaxis()->SetRangeUser(0, 140);
  invmass->SetTitle("Event Filter Invariant Mass [GeV]");
  invmass->Draw();

  c2->cd();
  deltaZ->GetXaxis()->SetTitle("#DeltaZ [mm]");
  deltaZ->GetYaxis()->SetTitle("Muon/Muon pairs");
  deltaZ->SetTitle("Tag_and_Probe_#DeltaZ");
  deltaZ->Draw();

  c3->cd();
  deltaRT->GetXaxis()->SetTitle("#DeltaR [Rad]");
  deltaRT->GetYaxis()->SetTitle("Tag Muon/RoI pairs");
  deltaRT->SetTitle("Lvl1_RoI_vs_EF_reconstructed_tag_muon_#DeltaR");
  deltaRT->Draw();
  
  c4->cd();
  deltaRP->GetXaxis()->SetTitle("#DeltaR [Rad]");
  deltaRP->GetYaxis()->SetTitle("Probe Muon/RoI pairs");
  deltaRP->SetTitle("Lvl1_RoI_vs_EF_reconstructed_probe_muon_#DeltaR");
  deltaRP->Draw();
  

  c5->Divide(3,2);
  c5->cd(1);
  deltaR1->GetXaxis()->SetTitle("#DeltaR [Rad]");
  deltaR1->GetYaxis()->SetTitle("Muon/RoI pairs");//"Probe Muon Candidates");
  deltaR1->SetTitle("Threshold 1 #DeltaR");
  deltaR1->Draw();

  c5->cd(2);
  deltaR2->GetXaxis()->SetTitle("#DeltaR [Rad]");
  deltaR2->GetYaxis()->SetTitle("Muon/RoI pairs");//"Probe Muon Candidates");
  deltaR2->SetTitle("Threshold 2 #DeltaR");
  deltaR2->Draw();

  c5->cd(3);
  deltaR3->GetXaxis()->SetTitle("#DeltaR [Rad]");
  deltaR3->GetYaxis()->SetTitle("Muon/RoI pairs");//"Probe Muon Candidates");
  deltaR3->SetTitle("Threshold 3 #DeltaR");
  deltaR3->Draw();

  c5->cd(4);
  deltaR4->GetXaxis()->SetTitle("#DeltaR [Rad]");
  deltaR4->GetYaxis()->SetTitle("Muon/RoI pairs");//"Probe Muon Candidates");
  deltaR4->SetTitle("Threshold 4 #DeltaR");
  deltaR4->Draw();

  c5->cd(5);
  deltaR5->GetXaxis()->SetTitle("#DeltaR [Rad]");
  deltaR5->GetYaxis()->SetTitle("Muon/RoI pairs");//"Probe Muon Candidates");
  deltaR5->SetTitle("Threshold 5 #DeltaR");
  deltaR5->Draw();

  c5->cd(6);
  deltaR6->GetXaxis()->SetTitle("#DeltaR [Rad]");
  deltaR6->GetYaxis()->SetTitle("Muon/RoI pairs");//"Probe Muon Candidates");
  deltaR6->SetTitle("Threshold 6 #DeltaR");
  deltaR6->Draw();
   
}
