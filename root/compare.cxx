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
  Root macro to extract all objects from two root (identical in format, different in histo content) 
  files and compare the histograms.
  Designed to confirm that changes made to the ATLAS trigger code do not effect the code functionality
*/

using namespace std;

unsigned int filenum=14;

void compare()
{
  
  TDirectory *where = gDirectory; //gDirectory = global directory         

  gROOT->Reset();
  gStyle->SetPalette(kBird); // Set the "COLZ" palette to a nice one
  gStyle->SetOptStat(101111);  
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);

  TString filestring1;
  TString filestring2;
  stringstream filestringstream1;
  stringstream filestringstream2;
  filestringstream1 << "expert-monitoring1.root";    
  filestringstream2 << "expert-monitoring2.root";    
  filestring1 = filestringstream1.str();
  filestring2 = filestringstream2.str();
  TFile* fileparent1 = new TFile(filestring1);
  TFile* fileparent2 = new TFile(filestring2);
  TDirectoryFile* file1=(TDirectoryFile*)fileparent1->Get("TrigSteer_HLT");
  TDirectoryFile* file2=(TDirectoryFile*)fileparent2->Get("TrigSteer_HLT");  

  TObject *obj1;
  TObject *obj2;
  TKey *key1;
  TIter next1( file1->GetListOfKeys());

  int i=0;
  int j=0;
  int errorCtr=0;

  
  while ((key1 = (TKey *) next1())) { // while file 1 still has objects
    
    i++;	  
    j++;
    char const k1 = static_cast<char>(i);
    char const k2 = static_cast<char>(j);

    obj1 = file1->Get(key1->GetName()); // copy object to memory
    obj2 = file2->Get(key1->GetName());


    if(obj1->InheritsFrom("TH1")) {
      TH1F * h1 = (TH1F*)file1->Get(key1->GetName());
      TH1F * h2 = (TH1F*)file2->Get(key1->GetName());

      h1->Add(h2, -1);

      if (h1->GetEntries() != 0.0){
	errorCtr++;
	printf("MISMATCH: object: %s has h1NumEntries + h2NumEntries =  %f\n", key1->GetName(), h1->GetEntries());
      }
    }	 

    printf(" found object:%s\n",key1->GetName());
  }



  // final information output for user:
  if (errorCtr){
    printf(" ERROR: No. of Mismatches: %d\n", errorCtr);
  } else {
    printf(" SUCCESS! No mismatches\n");
  }

}
