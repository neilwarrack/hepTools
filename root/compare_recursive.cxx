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



// ============= Neil's Debug Toolkit ============== //                                                                         
unsigned int olYeller = 1; 
#define N1 do { std::cout << "1 "; } while(0)
#define N2 do { std::cout << "2 "; } while(0)
#define N3 do { std::cout << "3 "; } while(0)
#define N4 do { std::cout << "4 "; } while(0)
#define N5 do { std::cout << "5 "; } while(0)
#define N6 do { std::cout << "6 "; } while(0)
#define N7 do { std::cout << "7 "; } while(0)
#define N8 do { std::cout << "8 "; } while(0)
#define N9 do { std::cout << "9 "; } while(0)
#define WORRY   do { std::cout << "WORRY!"   << endl; } while(0)
#define SUCCESS do { std::cout << "SUCCESS!" << endl; } while(0)
#define BEGIN   do { std::cout << "Begin..." << endl; } while(0)
#define END     do { std::cout << "End..."   << endl; } while(0)
#define YELLN   do { std::cout << " -> Error-" << olYeller << endl; olYeller++; } while(0)
#define YELL    do { std::cout << " -> Error" << endl; } while(0)
#define RET     do { std::cout << endl; } while(0)
// =================================================== //        

using namespace std;

int funcshun(string a);



int compTH1F(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key);
int compTH1I(TDirectoryFile*, TDirectoryFile*, TKey*);
int compTH2F(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key);
int compTH2I(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key);
int compTProfile(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key);





unsigned int filenum=14;

void compare()
{
  YELLN;  
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
  TFile* file1 = new TFile(fileStr1);
  TFile* file2 = new TFile(fileStr2);
  
  // define variables
  int i=0, j=0, errorCtr=0;
  TKey *key1, *key2, *subKey1, *subKey2;
  TObject *obj1, *obj2;
  string dirStr1, dirStr2, subDirStr1, subDirStr2, subSubDirStr1, subSubDirStr2, classname ;

  
  //search first file
  TIter dirItr1( file1->GetListOfKeys());
  TIter dirItr2( file2->GetListOfKeys());

  while ((key1 = (TKey *) dirItr1())) { // while directory 1 still has sub directories 
    key2 = (TKey *) dirItr2(); // iterate directory 2
    //    YELLN; 
    // get and compare directory names
    dirStr1 = key1->GetName();
    dirStr2 = key2->GetName();
    if (dirStr1 != dirStr2) {
      cout << "ABORT: Mismatch of top-level directory names! " << endl;
      cout << "Root file must have same directory layout." << endl;
      cout << "NB: "<< dirStr1 << " != " << dirStr2 << endl; 
    } else {
      
      // Get directory
      TDirectoryFile* dir1=(TDirectoryFile*)file1->Get(key1->GetName());  
      TDirectoryFile* dir2=(TDirectoryFile*)file2->Get(key2->GetName());  
      cout << dir1->ClassName() << endl;
      cout << key2->GetName() << endl;
      
      string dir1ClassName = dir1->ClassName();
      classname = dir1ClassName;
      if (classname == "TDirectoryFile"){
	
	///////////////////////////////////////////////////////////////////////////
	// create sub-directory iterators
	TIter subDirItr1( dir1->GetListOfKeys());
	TIter subDirItr2( dir2->GetListOfKeys());
	
	while ((subKey1 = (TKey *) subDirItr1())) { // while file1 still has objects in sub-directory
	  subKey2 = (TKey *) subDirItr2(); // iterate file2
	  
	  // get and compare sub-directory names
	  subDirStr1 = subKey1->GetName();
	  subDirStr2 = subKey2->GetName();
	  if (subDirStr1 != subDirStr2) {
	    cout << "ABORT: Mismatch of sub-directory names! " << endl;
	    cout << "Root file must have same directory format." << endl;
	    cout << "NB: "<< subDirStr1 << " != " << subDirStr2 << endl; 
	  } else {
	    
	    
	    // get sub-directories
	    TDirectoryFile* subDir1=(TDirectoryFile*)dir1->Get(subKey1->GetName());
	    TDirectoryFile* subDir2=(TDirectoryFile*)dir2->Get(subKey2->GetName());
	    //cout << subDir1->ClassName() << endl;
	    
	    
	    string subDir1ClassName = subDir1->ClassName();
	    classname = subDir1ClassName;  
	    if (classname == "TDirectoryFile"){
	      
	      ///////////////////////////////////////////////////////////////////////////
	      // create sub-directory iterators
	      TIter subSubDirItr1( subDir1->GetListOfKeys());
	      TIter subSubDirItr2( subDir2->GetListOfKeys());
	      TKey *subSubKey1, *subSubKey2;
	      while ((subSubKey1 = (TKey *) subSubDirItr1())) {
		subSubKey2 = (TKey *) subSubDirItr2();
		
		// get and compare sub-directory names
		subSubDirStr1 = subKey1->GetName();
		subSubDirStr2 = subKey2->GetName();
		if (subSubDirStr1 != subSubDirStr2) {
		  cout << "ERROR: Mismatch of sub-sub-directory names! " << endl;
		  cout << "Root file must have same directory format." << endl;
		  cout << "NB: "<< subSubDirStr1 << " != " << subSubDirStr2 << endl; 
		} else {
		  
		  
		  // get sub-sub-directories
		  TDirectoryFile* subSubDir1=(TDirectoryFile*)subDir1->Get(subSubKey1->GetName());
		  TDirectoryFile* subSubDir2=(TDirectoryFile*)subDir2->Get(subSubKey2->GetName());
		  
		  string subSubDir1ClassName = subSubDir1->ClassName();
		  classname = subSubDir1ClassName;
		  if (classname == "TDirectoryFile"){
		    cout << "Code doesn't go deep enough!" << endl;
		    cout << "There is a directory with unexplored content! " <<endl;
		    cout << "Unexplored directory Name: " << subSubKey1->GetName() << endl;
		    //continue;
		  } else {
		    //		    cout << "classname = " << classname ;
		    if (classname == "TH1F") errorCtr += compTH1F(subSubDir1, subSubDir2, subSubKey1);
		    if (classname == "TH1I") errorCtr += compTH1I(subSubDir1, subSubDir2, subSubKey1);
		    if (classname == "TH2F") errorCtr += compTH2F(subSubDir1, subSubDir2, subSubKey1);
		    if (classname == "TH2I") errorCtr += compTH2I(subSubDir1, subSubDir2, subSubKey1); 
		    if (classname == "TProfile") errorCtr += compTProfile(subSubDir1, subSubDir2, subSubKey1); 
		  } // end of comparison of non-directory phase
		} // end of matching names phase
	      } // end of while loop
	    } // end of directory case
	    ///////////////////////////////////////////////////////////////////////////
	    
	    
	    else { // look at subDir which isn't a directory!
	      //cout << "HEEEREEE!!" << endl;
	      //N1; RET;
	      if (classname == "TH1F") errorCtr += compTH1F(subDir1, subDir2, subKey1);
	      //N2; RET;
	      if (classname == "TH1I") errorCtr += compTH1I(subDir1, subDir2, subKey1);
	      //N3; RET;
	      if (classname == "TH2F") errorCtr += compTH2F(subDir1, subDir2, subKey1);
	      //N4; RET;
	      if (classname == "TH2I") errorCtr += compTH2I(subDir1, subDir2, subKey1);  
	      //N5; RET;
	      if (classname == "TProfile") errorCtr += compTProfile(subDir1, subDir2, subKey1);  
	    } // end of compasison of non-directories
	  } // end of matching names case
	} // end of while loop
      } // end of directory case
      else { // look at Dirs which aren't directories
	if (classname == "TH1F") errorCtr += compTH1F(dir1, dir2, key1);
	if (classname == "TH1I") errorCtr += compTH1I(dir1, dir2, key1);
	if (classname == "TH2F") errorCtr += compTH2F(dir1, dir2, key1); 
	if (classname == "TH2I") errorCtr += compTH2I(dir1, dir2, key1);	  
	if (classname == "TProfile") errorCtr += compTProfile(dir1, dir2, key1);	  
      }  // end of compasison of non-directories
    } // end of matching names case
  } // end of while loop
  
    // copy objects from sub-directories to memory
    //   obj1 = file1->Get(key1->GetName()); // copy object to memory 
    //    if (!(subDir1->Get(subKey1->GetName()))) continue; // TODO: this needs to be extended to deal with sub-sub-directories
    
    //obj2 = subDir2->Get(subKey2->GetName());
    //    cout << "NW1"<< subKey1->GetName() << endl;   
    // if (!obj1) continue; // TODO: this needs to be extended to deal with sub-sub-directories
    //cout << "NW2"<< subKey1->GetName() << endl;   
    //    cout << subKey1->GetName() << endl;
    // make histograms 
    // N1; RET;
    /*
    i++;	  
    j++;
    char const k1 = static_cast<char>(i);
    char const k2 = static_cast<char>(j);
    */
    //    cout << obj1->ClassName() << endl;
    //  N2; RET;
    //    if(obj1->InheritsFrom("TH1")){// && obj2->InheritsFrom("TH1") ) {
    /*
    if(*(obj1->ClassName()) == "TH1"){// && obj2->InheritsFrom("TH1") ) {
      N3; RET;
      TH1F * h1 = (TH1F*)subDir1->Get(subKey1->GetName());
      TH1F * h2 = (TH1F*)subDir2->Get(subKey1->GetName());

      h1->Add(h2, -1);

      if (h1->GetEntries() != 0.0){
	errorCtr++;
	printf("MISMATCH: object: %s has h1NumEntries + h2NumEntries =  %f\n", subKey1->GetName(), h1->GetEntries());
      }
    } else N3;  
    */
    //    printf(" found object:%s\n",subKey1->GetName());




  
  // final information output for user:
  if (errorCtr){
    printf(" ERROR: No. of Mismatches: %d\n", errorCtr);
  } else {
    printf(" SUCCESS! No mismatches\n");
  }

}



int compTH1F(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key){
  TH1F * h1 = (TH1F*)f1;//->Get(key->GetName());
  TH1F * h2 = (TH1F*)f2;//->Get(key->GetName());
  h1->Add(h2, -1);
  if (h1->GetEntries() != 0.0){
    printf("MISMATCH: object: %s has h1NumEntries + h2NumEntries =  %f\n", key->GetName(), h1->GetEntries());
    printf("          check: >%s< should be the same as >%s<\n", f1->GetName(), f2->GetName());
    return 1;  
  }
  //SUCCESS;
  return 0;
}


int compTH1I(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key){
  //BEGIN;
  //cout << "FUNCTION-GetName: " << key->GetName() << endl; //works
  //BEGIN; 
  TH1I * h1 = (TH1I*)f1;
  //BEGIN;
  TH1I * h2 = (TH1I*)f2;
  //BEGIN;
  h1->Add(h2, -1);
  if (h1->GetEntries() != 0.0){
    printf("MISMATCH (TH1I): object: %s has h1NumEntries + h2NumEntries =  %f\n", key->GetName(), h1->GetEntries());
    printf("          check: >%s< should be the same as >%s<\n", f1->GetName(), f2->GetName());
    return 1;  
  }
  //SUCCESS;
  return 0;
}


int compTH2F(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key){
  TH2F * h1 = (TH2F*)f1;//->Get(key->GetName());
  TH2F * h2 = (TH2F*)f2;//->Get(key->GetName());
  h1->Add(h2, -1);
  if (h1->GetEntries() != 0.0){
    printf("MISMATCH (TH2F): object: %s has h1NumEntries + h2NumEntries =  %f\n", key->GetName(), h1->GetEntries());
    printf("          check: >%s< should be the same as >%s<\n", f1->GetName(), f2->GetName());
    return 1;  
  }
  //SUCCESS;
  return 0;
}


      
int compTH2I(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key){
  TH2I * h1 = (TH2I*)f1;
  TH2I * h2 = (TH2I*)f2;
  h1->Add(h2, -1);
  if (h1->GetEntries() != 0.0){
    cout << "made it4!" << endl;
    printf("MISMATCH (TH2I): object: %s has h1NumEntries + h2NumEntries =  %f\n", key->GetName(), h1->GetEntries());
    printf("          check: >%s< should be the same as >%s<\n", f1->GetName(), f2->GetName());
    return 1;  
  }
  //SUCCESS;
  return 0;
}



int compTProfile(TDirectoryFile* f1, TDirectoryFile* f2, TKey* key){
  TProfile * p1 = (TProfile*)f1;
  TProfile * p2 = (TProfile*)f2;

TH1D * temp1 = p1->ProjectionX("temp1");
TH1D * temp2 = p2->ProjectionX("temp2");
TH1D * h3 = new TH1D(*temp1);


h3->Add(temp1,temp2,-1,1); 
/*
  //TProfile * P1 = (TProfile*)f1;
  //TProfile * P2 = (TProfile*)f2;

  TH1D * temp1 = (TH1D*)f1;
  TH1D * temp2 = (TH1D*)f2;
  temp1->Add(temp2, -1);
*/  
// TODO: This produces Warning ("possible memory leak")
  if (h3->GetEntries() != 0.0){
    printf("MISMATCH (Tprf): object: %s has h1NumEntries + h2NumEntries =  %f\n", key->GetName(), temp1->GetEntries());
    printf("          check: >%s< should be the same as >%s<\n", f1->GetName(), f2->GetName());
    return 1;  
  }
  //SUCCESS;
  return 0;
}


