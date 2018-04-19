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
  Root macro Work in progress...
*/

using namespace std;

unsigned int filenum=14;

void raw2()
{
  
  TDirectory *where = gDirectory; //gDirectory = global directory         

  // Set canvasses

  //  TCanvas *c1= new TCanvas ("c1", "TEST", 1000, 1000);  
  //TCanvas *c2= new TCanvas ("c2", "TEST", 1000, 1000);
  //TCanvas *c3= new TCanvas ("c3", "TEST", 1000, 1000);

  //  TH1F* RoIEta = new TH1F("RoI eta","RoI eta", 51,-100,100);
  //TH1F* RoIPhi = new TH1F("RoI phi","RoI phi", 51,-100,100);

  gROOT->Reset();
  gStyle->SetPalette(kBird); // Set the "COLZ" palette to a nice one
  gStyle->SetOptStat(101111);  
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);


  // File 1
  TString filestring1; //TFile will only accept a TString as an input
  stringstream filestringstream1; //TStringstreamer class flakey but can use regular
  filestringstream1 << "expert-monitoring1.root";    
  filestring1 = filestringstream1.str(); //TString now contains variable filename
  TFile* fileparent1 = new TFile(filestring1); //opens file dependent on Momentum and Theta values
  TDirectoryFile* file1=(TDirectoryFile*)fileparent1->Get("TrigSteer_HLT");
  
  // File 2
  TString filestring2; //TFile will only accept a TString as an input
  stringstream filestringstream2; //TStringstreamer class flakey but can use regular
  filestringstream2 << "expert-monitoring2.root";    
  filestring2 = filestringstream2.str(); //TString now contains variable filename
  TFile* fileparent2 = new TFile(filestring2); //opens file dependent on Momentum and Theta values
  TDirectoryFile* file2=(TDirectoryFile*)fileparent2->Get("TrigSteer_HLT");
  

    //todays work************************************

	TObject *obj1;
	TObject *obj2;
	TKey *key1;
	//	TKey *key2;
	TIter next1( file1->GetListOfKeys());
	//	TIter next2( file2->GetListOfKeys());
	//c3->cd();
	int i=0;
	int j=0;
	int errorCtr=0;

	double ChiSqrd = 0;
	while ((key1 = (TKey *) next1())) { // while file 1 still has objects
	  //	  if ((key2 = (Tkey *) next2())){
	  i++;	  
	  j++;
	  char const k1 = static_cast<char>(i);
	  char const k2 = static_cast<char>(j);
	  //	  TCanvas *temp= new TCanvas (&k, "oi", 1000, 1000);


	  obj1 = file1->Get(key1->GetName()); // copy object to memory
	  obj2 = file2->Get(key1->GetName()); // copy object to memory
	  // do something with obj
	  //	  if(obj->InheritsFrom("TH1")) obj->Draw();
	  if(obj1->InheritsFrom("TH1")) {
	    TH1F * h1 = (TH1F*)file1->Get(key1->GetName());
	    TH1F * h2 = (TH1F*)file2->Get(key1->GetName());
	    //	    ChiSqrd = h1->Chi2Test(h2);	   
	    h1->Add(h2, -1);
	    if (h1->GetEntries() != 0.0){
	      errorCtr++;
	      printf("MISMATCH: object: %s has h1NumEntries + h2NumEntries =  %f\n", key1->GetName(), h1->GetEntries());
	    // printf(" ...object: %s has Chi2 result %f\n", key1->GetName(), h1->Chi2Test(h2));	   	    
	    }
//	    TCanvas *temp1= new TCanvas (&k1, "oi", 1000, 1000);
	    // temp1->cd();
	    // obj1->Draw();
	    // TCanvas *temp2= new TCanvas (&k2, "oi", 1000, 1000);
	    // temp2->cd();
	    // obj2->Draw();
	   
	   
	  }	 
	  printf(" found object:%s\n",key1->GetName());
	  }

	if (errorCtr){
	  printf(" ERROR: No. of Mismatches: %d\n", errorCtr);
	} else {
	  printf(" SUCCESS! No mismatches\n");
	}
	//	c3->Update();

    //todays work -END *****************************


    //    where->cd(); //cd() sets current pad
    
	/*    
    file1->ReadTObject(RoIEta, "RoIsL1Eta");
    file1->ReadTObject(RoIPhi, "RoIsL1Phi");
  
  c1->cd();
  RoIEta->GetXaxis()->SetTitle("Axis Title");
  RoIEta->GetYaxis()->SetTitle();
  RoIEta->SetTitle("Plot Title");
  RoIEta->Draw();

  c2->cd();
  RoIPhi->GetXaxis()->SetTitle("Axis Title");
  RoIPhi->GetYaxis()->SetTitle();
  RoIPhi->SetTitle("Plot Title");
  RoIPhi->Draw();
  */
}
