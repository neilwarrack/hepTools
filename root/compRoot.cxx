#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TH2F.h"
#include "TH2I.h"
#include "TLatex.h"
#include "TObject.h"
#include "TKey.h"
#include <iostream>
#include "TPad.h"
#include "TF1.h"
#include "TF2.h"
#include "TProfile.h"
/*
  Root macro to extract all objects from two root (identical in format, different in histo content) 
  files and compare the histograms.
  Designed to confirm that changes made to the ATLAS trigger code do not effect the code functionality
*/


using namespace std;

int compareContents(TDirectoryFile* directory1, TDirectoryFile* directory2, int* mCtr);
int compTH1F(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key);
int compTH1I(TDirectoryFile*, TDirectoryFile*, TKey*);
int compTH2F(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key);
int compTH2I(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key);
int compTProfile(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key);




unsigned int filenum=14;

void compRoot()
{

  TDirectory *where = gDirectory; //gDirectory = global directory         

  gROOT->Reset();
  gStyle->SetPalette(kBird); // Set the "COLZ" palette to a nice one
  gStyle->SetOptStat(101111);  
  gStyle->SetStatY(0.9);
  gStyle->SetStatX(0.9);

  // open root files to compare
  TString fileStr1;
  TString fileStr2;
  stringstream strStream1;
  stringstream strStream2;
  strStream1 << "expert-monitoring1.root";    
  strStream2 << "expert-monitoring2.root";    
  fileStr1 = strStream1.str();
  fileStr2 = strStream2.str();
  TFile *file1 = new TFile(fileStr1);
  TFile *file2 = new TFile(fileStr2);

  int *errorCtr = new int(0);

  *errorCtr = compareContents(file1, file2, errorCtr);      

  cout << " - - - - - - FINISHED - - - - - - - " << endl << endl;
 
  if (*errorCtr){
    printf(" ERROR: No. of Mismatches: %d\n", *errorCtr);
  } else {
    cout << "SUCCESS! No mismatches in any files" << endl;
    
  }
  cout << "NB: Only TH1I, TH1F, TH2I, TH2F and TProfile files were checked." << endl;
}



int compareContents(TDirectoryFile* directory1, TDirectoryFile* directory2, int* mCtr){

  TKey *fileKey1, *fileKey2;
  string name1, name2, subDirectory1ClassName;

  //search first file
  TIter iterator1( directory1->GetListOfKeys());
  TIter iterator2( directory2->GetListOfKeys());

  while ((fileKey1 = (TKey *) iterator1())) { // while directory 1 still has sub directories 
    fileKey2 = (TKey *) iterator2(); // iterate directory 2
   
    // get and compare directory names
    name1 = fileKey1->GetName();
    name2 = fileKey2->GetName();
    if (name1 != name2) {
      cout << "ERROR: Mismatch of file names! " << endl;
      cout << "ERROR: "<< name1 << " != " << name2 << endl; 
      cout << "ERROR: Solution: Root files must have identicle directory layouts." << endl;
      cout << "ERROR: no further equiry will be made in to this file...";
    } else {
      
      // Get directory
      TDirectoryFile* subDirectory1=(TDirectoryFile*)directory1->Get(fileKey1->GetName());  
      TDirectoryFile* subDirectory2=(TDirectoryFile*)directory2->Get(fileKey2->GetName());  
   
      
      subDirectory1ClassName = subDirectory1->ClassName();
      if (subDirectory1ClassName == "TDirectoryFile"){
	cout << " In TDirectoryFile: " << fileKey1->GetName() << ":" << endl;      
	*mCtr = compareContents( subDirectory1, subDirectory2, mCtr);
      } else {
	cout << "comparing " << subDirectory1ClassName << ": " << fileKey1->GetName() << " ...";      

	// compare histos
	if (subDirectory1ClassName == "TH1F")     *mCtr += compTH1F(subDirectory1, subDirectory2, fileKey1);
	if (subDirectory1ClassName == "TH1I")     *mCtr += compTH1I(subDirectory1, subDirectory2, fileKey1);
	if (subDirectory1ClassName == "TH2F")     *mCtr += compTH2F(subDirectory1, subDirectory2, fileKey1);
	if (subDirectory1ClassName == "TH2I")     *mCtr += compTH2I(subDirectory1, subDirectory2, fileKey1);
	if (subDirectory1ClassName == "TProfile") *mCtr += compTProfile(subDirectory1, subDirectory2, fileKey1);
	
      }
    }
  }
  return *mCtr;
}


int compTH1F(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key){
  TH1F * h1 = (TH1F*)f1;
  TH1F * h2 = (TH1F*)f2;
  double bin1=0, bin2=0, binDif=0, difTot=0;
  int difCtr=0;
  int totBins = h1->GetSize(); 

  for (int i=0; i<totBins; i++ ){
    bin1 = h1->GetBinContent(i);
    bin2 = h1->GetBinContent(i);
    binDif = bin1 - bin2;
    if (binDif){
      difTot += abs(binDif);     	
      difCtr++;
    }
  }
  
   // user output 
  if (difTot){
    cout << "FAIL! (difference in " << difCtr << "/" << totBins << " bins)" << endl;
    return 1;
  } 
  else {
    cout << "PASS! (no difference in " << totBins << " bins)" << endl;
    return 0;
  }
}


int compTH1I(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key){
  TH1I * h1 = (TH1I*)f1;
  TH1I * h2 = (TH1I*)f2;
  int bin1=0, bin2=0, binDif=0, difCtr=0, difTot=0;
  int totBins = h1->GetSize();  

  for (int i=0; i<totBins; i++ ){
    bin1 = h1->GetBinContent(i);
    bin2 = h1->GetBinContent(i);
    binDif = bin1 - bin2;
    if (binDif){
      difTot += abs(binDif);     	
      difCtr++;
    }
  }
  
  // user output 
  if (difTot){
    cout << "FAIL! (difference in " << difCtr << "/" << totBins << " bins)" << endl;
    return 1;
  } 
  else {
    cout << "PASS! (no difference in " << totBins << " bins)" << endl;
    return 0;
  }
}
  

int compTH2F(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key){
  TH2F * h1 = (TH2F*)f1;
  TH2F * h2 = (TH2F*)f2;
  double bin1=0, bin2=0, binDif=0, difTot=0;
  unsigned int difCtr=0;
  int xTotBins = h1->GetNbinsX();
  int yTotBins = h1->GetNbinsY();

  for (int i=0; i<xTotBins; i++ ){
    for (int j=0; j<yTotBins; j++ ){
        bin1 = h1->GetBinContent(i,j);
      bin2 = h1->GetBinContent(i,j);
      binDif = bin1 - bin2;
      if (binDif){
	difTot += abs(binDif);     	
	difCtr++;
      }
    }
  }
  
  // user output 
  if (difTot){
    cout << "FAIL! (difference in " << difCtr << "/" << xTotBins*yTotBins << " bins)" << endl;
    return 1;
  } 
  else {
    cout << "PASS! (no difference in " << xTotBins*yTotBins << " bins)" << endl;
    return 0;
  }
}


int compTH2I(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key){
  TH2I * h1 = (TH2I*)f1;
  TH2I * h2 = (TH2I*)f2;
  int bin1=0, bin2=0, binDif=0, difCtr=0, difTot=0;
  int xTotBins = h1->GetNbinsX();
  int yTotBins = h1->GetNbinsY();
  
  for (int i=0; i<xTotBins; i++ ){
    for (int j=0; j<yTotBins; j++ ){
      bin1 = h1->GetBinContent(i,j);
      bin2 = h1->GetBinContent(i,j);
      binDif = bin1 - bin2;
      if (binDif){
	difTot += abs(binDif);     	
	difCtr++;
      }
    }
  }
 
  // user output 
  if (difTot){
    cout << "FAIL! (difference in " << difCtr << "/" << xTotBins*yTotBins << " bins)" << endl;
    return 1;
  } 
  else {
    cout << "PASS! (no difference in " << xTotBins*yTotBins << " bins)" << endl;
    return 0;
  }
}


int compTProfile(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key){
  TProfile * p1 = (TProfile*)f1;
  TProfile * p2 = (TProfile*)f2;

  TH1D * temp1 = p1->ProjectionX("temp1");
  TH1D * temp2 = p2->ProjectionX("temp2");
  TH1D * h3 = new TH1D(*temp1);

  h3->Add(temp1,temp2,-1,1); 

  delete temp1;
  delete temp2;
  

  if (h3->GetEntries() != 0.0){
    
    delete h3;
    cout << "FAIL!" << endl;
    return 1;  
  }
 
  delete h3;
  cout << "PASS!" << endl;
   return 0;
}


