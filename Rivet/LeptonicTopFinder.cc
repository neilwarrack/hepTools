#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/IdentifiedFinalState.hh"
#include "Rivet/Projections/PartonicTops.hh"
#include "HepMC/IO_GenEvent.h"

// ============= Neil's Debug Toolkit ============== //
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
#define RET     do { std::cout << endl; } while(0)
// =================================================== //

namespace Rivet {


  /// Parton-level top-pair normalized differential distributions at 7 TeV. 
  class LeptonicTopFinder : public Analysis {
  public:


    /// Constructor
    DEFAULT_RIVET_ANALYSIS_CTOR(LeptonicTopFinder) ;

    
    void init() {

      Cut eta_full = Cuts::abseta < 5.0 && Cuts::pT >= 1.0*MeV;
     
      // Project out useful final state particles
      FinalState fs(eta_full);
      declareProjection(fs, "AllFsParticles");

      IdentifiedFinalState electronidfs(fs);
      electronidfs.acceptIdPair(PID::ELECTRON);
      declareProjection(electronidfs, "ELECTRON_idfs");

      IdentifiedFinalState muonidfs(fs);
      muonidfs.acceptIdPair(PID::MUON);
      declareProjection(muonidfs, "MUON_idfs");

      IdentifiedFinalState tauidfs(fs);
      tauidfs.acceptIdPair(PID::TAU);
      declareProjection(tauidfs, "TAU_idfs");	

      // Project out tops with leptonic and hadronic decay mode, includes by default decays via taus. 
      //      declare(PartonicTops(PartonicTops::E_MU), "LeptonicPartonTops") ; // Done! looks good.
      declareProjection(PartonicTops(PartonicTops::ALL),      "AllPartonTops") ;     
      declareProjection(PartonicTops(PartonicTops::E_MU_TAU), "LeptonicPartonTops") ; 
      declareProjection(PartonicTops(PartonicTops::HADRONIC), "HadronicPartonTops") ;
      declareProjection(PartonicTops(PartonicTops::ELECTRON), "ElectronPartonTops") ;
      declareProjection(PartonicTops(PartonicTops::MUON),     "MuonPartonTops") ;
      declareProjection(PartonicTops(PartonicTops::TAU),      "TauPartonTops") ;
      declareProjection(PartonicTops(PartonicTops::E_MU),     "E_MuPartonTops") ;

      declareProjection(PartonicTops(PartonicTops::E_MU_TAU, true, true), "LeptonicPartonTopsWithHadTaus") ; 

      _hepmcout.reset(new HepMC::IO_GenEvent("badtops.hepmc"));
      // Book histos
      //      _hSL_leptonicTopPt   = bookHisto1D(1,1,1) ;
    }


    void analyze(const Event& event) {
      //BEGIN;


      //____________________________________________________________________________________________________
      // Find partonic tops in event
      const Particles allPartonTops      = apply<ParticleFinder>(event, "AllPartonTops"     ).particlesByPt();
      const Particles leptonicPartonTops = apply<ParticleFinder>(event, "LeptonicPartonTops").particlesByPt();
      const Particles hadronicPartonTops = apply<ParticleFinder>(event, "HadronicPartonTops").particlesByPt();
      const Particles electronPartonTops = apply<ParticleFinder>(event, "ElectronPartonTops").particlesByPt();
      const Particles muonPartonTops     = apply<ParticleFinder>(event, "MuonPartonTops"    ).particlesByPt();
      const Particles tauPartonTops      = apply<ParticleFinder>(event, "TauPartonTops"     ).particlesByPt();
      const Particles e_muPartonTops     = apply<ParticleFinder>(event, "E_MuPartonTops"    ).particlesByPt();

      const Particles leptonicPartonTopsWithHadTaus = apply<ParticleFinder>(event, "LeptonicPartonTopsWithHadTaus").particlesByPt();  

      // Find other particles of interest
      const Particles allFsParticles = apply<ParticleFinder>(event, "AllFsParticles").particlesByPt();
      const Particles e_idfs         = apply<ParticleFinder>(event, "ELECTRON_idfs" ).particlesByPt();
      const Particles mu_idfs        = apply<ParticleFinder>(event, "MUON_idfs"     ).particlesByPt();
      const Particles tau_idfs       = apply<ParticleFinder>(event, "TAU_idfs"      ).particlesByPt();


      // Define simple PartonicTops bools
      const bool allTopsPresent_pt       = ( allPartonTops.size()      != 0 );
      const bool leptonicTopsPresent_pt  = ( leptonicPartonTops.size() != 0 );
      const bool hadronicTopsPresent_pt  = ( hadronicPartonTops.size() != 0 );
      const bool electronTopsPresent_pt  = ( electronPartonTops.size() != 0 );
      const bool muonTopsPresent_pt      = ( muonPartonTops.size()     != 0 );
      const bool tauTopsPresent_pt       = ( tauPartonTops.size()      != 0 );
      const bool e_muTopsPresent_pt      = ( e_muPartonTops.size()     != 0 );
      const bool noTops_pt = (!allTopsPresent_pt && !leptonicTopsPresent_pt && !hadronicTopsPresent_pt && !electronTopsPresent_pt 
			      && !muonTopsPresent_pt && !tauTopsPresent_pt && !e_muTopsPresent_pt);
     
      // Define final state bools
      const bool fs_e_identified   = ( e_idfs.size()   != 0 );
      const bool fs_mu_identified  = ( mu_idfs.size()  != 0 );
      const bool fs_tau_identified = ( tau_idfs.size() != 0 );

      // full process PartonicTops bools
      const bool semileptonic_pt = ( leptonicPartonTops.size() == 1 && hadronicPartonTops.size() == 1 );
      const bool dileptonic      = ( leptonicPartonTops.size() == 2 && hadronicPartonTops.size() == 0 );     
      const bool dihadronic_pt   = ( hadronicPartonTops.size() == 2 );
     
      const bool singleLeptonic_pt = ( leptonicPartonTops.size() == 1 && hadronicPartonTops.size() == 0 );
      const bool singleHadronic_pt = ( leptonicPartonTops.size() == 0 && hadronicPartonTops.size() == 1 );

      // tools                 
      int x = 0, i = 0 ;
      bool flag = false, taufsFound = false, elfsFound = false, mufsFound = false;      

      //____________________________________________________________________________________________________



      // 7)
      // use t->E_MU userhook
      // Similar to test 2, 3, 4 and 5.
      // In 5000 events the following test produced no errors (using t->E_MU userhook and Top:all=on (8TeV))
      // In 5000 events the following test produced no errors (using t->E_MU userhook and Top:qqbar2ttbar=on and Top:gg2ttbar=on (8TeV))
      //START OF TEST ================================================================================================
      
      if (noTops_pt) {N1; RET;} // this would be strange behaviour given the userhook
      if (!leptonicTopsPresent_pt) {// ERROR!
        N2; RET;

        for ( const Particle& p : allFsParticles ) {
          i++;

            if ( p.abspid() == 11){ //electron
            cout << "All particles by pT: No. " << i << " (e " << p.pid()/p.abspid() << ")" << endl ;
            elfsFound= true;
            }


            if ( p.abspid() == 13){ //muon
            cout << "All particles by pT: No. " << i << " (mu " << p.pid()/p.abspid() << ")" << endl ;
            mufsFound= true;
            }

        }

        if (mufsFound || elfsFound){ 
	  N3; RET;
	  /*	cout << "Lepton info: fs_e="   << e_idfs.size() << "; fs_mu="  << mu_idfs.size() << "; fs_tau="
	<< tau_idfs.size() << "." << endl;

          // print event record to file
          const HepMC::GenEvent* ge = event.genEvent();
          if (ge == nullptr) {cout << "nullpointer found: GenEvent*"<<endl;
          } else { //if (ge != nullptr) {
	    ge->print(); // sends (formatted) HepMC event to screen
            _hepmcout->write_event(ge); // writes to file
          }
        
	  */
	}
      } else {N9; RET;}

      //END OF TEST 7================================================================================================


      /// Previous tests and notes      



      // 1)
      // In 5000 events the following test produced no output for a simple Top:all=on run of pythia 
      //START OF TEST 1================================================================================================
      /*
      if (noTops_pt) {N1; RET;} else N9;
      */
      //END OF TEST 1================================================================================================
     
      // 2)
      // In 5000 events the following test produced no errors (using t->TAU userhook and Top:all=on (8TeV))
      // In 5000 events the following test produced no errors (using t->TAU userhook and Top:qqbar2ttbar=on and Top:gg2ttbar=on (8TeV))
      //START OF TEST 2================================================================================================
      /*   
      if (noTops_pt) {N1; RET;} // this would be strange behaviour given the userhook
      if (!leptonicTopsPresent_pt) {
	N2; RET;

	for ( const Particle& p : allFsParticles ) {
	  i++;
	    
	    if ( p.abspid() == 15){ // tau
	    cout << "All particles by pT: No. " << i << " (tau " << p.pid()/p.abspid() << ")" << endl ;
	    taufsFound= true;	    
	    }
	}
		
	if (taufsFound){ //ERROR!
	  
	  cout << "Lepton info: fs_e="   << e_idfs.size() << "; fs_mu="  << mu_idfs.size() << "; fs_tau=" 
	       << tau_idfs.size() << "." << endl;
	  
	  // print event record to file
	  const HepMC::GenEvent* ge = event.genEvent();
	  if (ge == nullptr) {cout << "nullpointer found: GenEvent*"<<endl;	   
	  } else { //if (ge != nullptr) {
	    ge->print(); // sends (formatted) HepMC event to screen
	    _hepmcout->write_event(ge); // writes to file
	  }
	}
      } else {N9; RET;}
      */
      //END OF TEST 2================================================================================================


      // 3)
      // similar to test 2, but with hadronic TAUs included in the 'if' statement. An error would therefor show up earlier
      // (see 'ERROR!'). 
      // In 5000 events the following test produced no errors (using t->TAU userhook and Top:all=on (8TeV))
      // In 5000 events the following test produced no errors (using t->TAU userhook and Top:qqbar2ttbar=on and Top:gg2ttbar=on(8TeV))
      //START OF TEST 3================================================================================================
      /*
      if (noTops_pt) {N1; RET;} // this would be strange behaviour given the userhook
      if (leptonicPartonTopsWithHadTaus.size()==0) { // ERROR!
        N2; RET;

        for ( const Particle& p : allFsParticles ) {
          i++;

            if ( p.abspid() == 15){ // tau
            cout << "All particles by pT: No. " << i << " (tau " << p.pid()/p.abspid() << ")" << endl ;
            taufsFound= true;
            }
        }

        if (taufsFound){

	cout << "Lepton info: fs_e="   << e_idfs.size() << "; fs_mu="  << mu_idfs.size() << "; fs_tau="
	<< tau_idfs.size() << "." << endl;

          // print event record to file
          const HepMC::GenEvent* ge = event.genEvent();
          if (ge == nullptr) {cout << "nullpointer found: GenEvent*"<<endl;
          } else { //if (ge != nullptr) {
            ge->print(); // sends (formatted) HepMC event to screen
            _hepmcout->write_event(ge); // writes to file
          }
        }
      } else {N9; RET;}
      */
      //END OF TEST 3================================================================================================


      // 4)
      // use t->E userhook
      // Similar to test 2 and 3.
      // In 5000 events the following test produced no errors (using t->E userhook and Top:all=on (8TeV))
      // In 5000 events the following test produced no errors (using t->E userhook and Top:qqbar2ttbar=on and Top:gg2ttbar=on (8TeV))
      //START OF TEST 4================================================================================================
      /*
      if (noTops_pt) {N1; RET;} // this would be strange behaviour given the userhook
      if (!electronTopsPresent_pt) {
        N2; RET;

        for ( const Particle& p : allFsParticles ) {
          i++;

            if ( p.abspid() == 11){ //electron
            cout << "All particles by pT: No. " << i << " (e " << p.pid()/p.abspid() << ")" << endl ;
            elfsFound= true;
            }
        }

        if (elfsFound){ //ERROR!

	cout << "Lepton info: fs_e="   << e_idfs.size() << "; fs_mu="  << mu_idfs.size() << "; fs_tau="
	<< tau_idfs.size() << "." << endl;

          // print event record to file
          const HepMC::GenEvent* ge = event.genEvent();
          if (ge == nullptr) {cout << "nullpointer found: GenEvent*"<<endl;
          } else { //if (ge != nullptr) {
	    ge->print(); // sends (formatted) HepMC event to screen
            _hepmcout->write_event(ge); // writes to file
          }
        }
      } else {N9; RET;}
      */
      //END OF TEST 4================================================================================================
    

      // 5)
      // use t->E_MU userhook
      // Similar to test 2, 3 and 4.
      // In 5000 events the following test produced no errors (using t->MU userhook and Top:all=on (8TeV))
      // In 5000 events the following test produced no errors (using t->MU userhook and Top:qqbar2ttbar=on and Top:gg2ttbar=on (8TeV))
      //START OF TEST 5================================================================================================
      /*
      if (noTops_pt) {N1; RET;} // this would be strange behaviour given the userhook
      if (!muonTopsPresent_pt) {
        N2; RET;

        for ( const Particle& p : allFsParticles ) {
          i++;

            if ( p.abspid() == 13){ //muon
            cout << "All particles by pT: No. " << i << " (mu " << p.pid()/p.abspid() << ")" << endl ;
            mufsFound= true;
            }

        }

        if (mufsFound){ //ERROR!

	cout << "Lepton info: fs_e="   << e_idfs.size() << "; fs_mu="  << mu_idfs.size() << "; fs_tau="
	<< tau_idfs.size() << "." << endl;

          // print event record to file
          const HepMC::GenEvent* ge = event.genEvent();
          if (ge == nullptr) {cout << "nullpointer found: GenEvent*"<<endl;
          } else { //if (ge != nullptr) {
	    ge->print(); // sends (formatted) HepMC event to screen
            _hepmcout->write_event(ge); // writes to file
          }
        }
      } else {N9; RET;}
      */
      //END OF TEST 5================================================================================================



      // 6)
      // use t->E_MU userhook
      // Similar to test 2, 3, 4 and 5.
      // In 5000 events the following test produced no errors (using t->E_MU userhook and Top:all=on (8TeV))
      // In 5000 events the following test produced no errors (using t->E_MU userhook and Top:qqbar2ttbar=on and Top:gg2ttbar=on (8TeV))
      //START OF TEST 6================================================================================================
      /*    
      if (noTops_pt) {N1; RET;} // this would be strange behaviour given the userhook
      if (!e_muTopsPresent_pt) {
        N2; RET;

        for ( const Particle& p : allFsParticles ) {
          i++;

            if ( p.abspid() == 11){ //electron
            cout << "All particles by pT: No. " << i << " (e " << p.pid()/p.abspid() << ")" << endl ;
            elfsFound= true;
            }


            if ( p.abspid() == 13){ //muon
            cout << "All particles by pT: No. " << i << " (mu " << p.pid()/p.abspid() << ")" << endl ;
            mufsFound= true;
            }

        }

        if (mufsFound || elfsFound){ //ERROR!

	cout << "Lepton info: fs_e="   << e_idfs.size() << "; fs_mu="  << mu_idfs.size() << "; fs_tau="
	<< tau_idfs.size() << "." << endl;

          // print event record to file
          const HepMC::GenEvent* ge = event.genEvent();
          if (ge == nullptr) {cout << "nullpointer found: GenEvent*"<<endl;
          } else { //if (ge != nullptr) {
	    ge->print(); // sends (formatted) HepMC event to screen
            _hepmcout->write_event(ge); // writes to file
          }
        }
      } else {N9; RET;}
*/
      //END OF TEST 6================================================================================================


  //_______________ONLY MESS BELOW HERE!__________________________________
  /*
      if (noTops_pt) {N1; RET;} // this would be strange behaviour given the userhook
      if (!leptonicTopsPresent_pt) {
	N2; RET;

	for ( const Particle& p : allFsParticles ) {
	  i++;
	    
	  //leptonic flag
	    
	  if ( p.abspid() == 11) { // electron
	    cout << "All particles by pT: No. " << i << " (el " << p.pid()/p.abspid() << ")" << endl ;
	    elfsFound = true;
	  }
	  
	    if ( p.abspid() == 13) { // muon
	    cout << "All particles by pT: No. " << i << " (mu " << p.pid()/p.abspid() << ")" << endl ;
	    mufsFound = true;
	    }
	    
	    if ( p.abspid() == 15){ // tau
	    cout << "All particles by pT: No. " << i << " (tau " << p.pid()/p.abspid() << ")" << endl ;
	    taufsFound= true;	    
	    }
	 
	}	 
	
	//      RET;
	
	
	if (elfsFound){ //ERROR!
	//if (mufsFound){ //ERROR!
	//if (taufsFound){ //ERROR!
	  
	  cout << "Lepton info: fs_e="   << e_idfs.size() << "; fs_mu="  << mu_idfs.size() << "; fs_tau=" 
	       << tau_idfs.size() << "." << endl;
	  
	  // print event record to file
	  const HepMC::GenEvent* ge = event.genEvent();
	  if (ge == nullptr) {cout << "nullpointer found: GenEvent*"<<endl;	   
	  } else { //if (ge != nullptr) {
	    ge->print(); // sends (formatted) HepMC event to screen
	    _hepmcout->write_event(ge); // writes to file
	  }
	  //	cout << "++++++++++++++++++++++++++++++++++++++++++ " << endl ; 	  
	  
	  //	if ( flag ) { cout << "fs leptons found" << endl; 
	}} else {N9; RET;}
     */


      // the WILDCARD bool!!!
      //      const bool SomethingElse = ( !semileptonic_pt && !dileptonic && !Dihadronic_Pt && !noPartonicTops && 
      //				   !singleLeptonicTopEvent && !singleHadronicTopEvent );

      // DEBUG CODES
      //      cout << "Rivet debug codes:" << endl;
    /*
      if ( leptonicPartonTops.size() + hadronicPartonTops.size() == 1 ) N1;
      if ( leptonicPartonTops.size() + hadronicPartonTops.size() == 2 ) N2;
      if ( noPartonicTops )  N3;      
      if ( Dihadronic_Pt )   N4;
      if ( dileptonic )      N5;
      if ( noAllPartonTops ) N7;      
      if ( SomethingElse )   N8;
      if ( semileptonic_pt)     N9;
      cout << endl ;
    */
      /*
      // DEBUG CODES
      if ( leptonicPartonTops.size() == 1 ) { std::cout << "1 " ; }
      if ( leptonicPartonTops.size() == 2 ) { std::cout << "2 " ; } 
      if ( hadronicPartonTops.size() == 1 ) { std::cout << "3 " ; } 
      if ( hadronicPartonTops.size() == 2 ) { std::cout << "4 " ; }
      if ( !leptonicTopsPresent ) { std::cout << "5 " ; }
      if ( !hadronicTopsPresent ) { std::cout << "6 " ; }
      if ( leptonicPartonTops.size() + hadronicPartonTops.size() == 0) cout << "7";      
      cout << endl;
      */
      
      //DEBUG INFO
      /*
      if ( leptonicPartonTops.size() == 1 ) { std::cout << "PartonicTops::E_MU_TAU.size() = 1" << endl; }
      if ( leptonicPartonTops.size() == 2 ) { std::cout << "PartonicTops::E_MU_TAU.size() = 2" << endl; } 
      if ( hadronicPartonTops.size() == 1 ) { std::cout << "PartonicTops::HADRONIC.size() = 1" << endl; } 
      if ( hadronicPartonTops.size() == 2 ) { std::cout << "PartonicTops::HADRONIC.size() = 2" << endl; }     
      if ( SomethingElse ) cout << "Bloodyhell!!!! " << endl;
      //      if ( !leptonicTopsPresent ) { std::cout << "no PartonicTops::E_MU found!" << endl ; }
      //      if ( !hadronicTopsPresent ) { std::cout << "no PartonicTops::HADRONIC found!" << endl ; }
      */

      /*
      if (   )
	if (fs_e_identified){
	cout <<	e_idfs.size() << " " ;
	}
      */

      /*    
      int x = 0, i = 0 ;
      bool flag = false;      
      */
      if ( leptonicPartonTops.size() + hadronicPartonTops.size() == 1 ) {
      //      if ( noPartonicTops ) { 
	//cout << "+++++++++NO_LEPTONIC_TOPS_PRESENT_++++++++++ " << endl ; 
	//if ( leptonicPartonTops.size() + hadronicPartonTops.size() == 0 ) {
	//cout << "+++++++++NO_PARTONIC_TOPS_PRESENT_++++++++++ " << endl ; 
	/*	  
	  for ( const Particle& p : allFsParticles ) {
	
	  i++;

	  //leptonic flag
	  if ( p.abspid() == 11) {
	    cout << "pT_n=" << i << ", e:" << p.pid() << endl ;
	    flag = true;
	  }
	  if ( p.abspid() == 13) {
	    cout << "pT_n=" << i << ", mu:" << p.pid() << endl ;
	    flag = true;
	  }
	  if ( p.abspid() == 15){
	    cout << "pT_n=" << i << ", tau:" << p.pid() << endl ;
	    flag = true;	    
	  }
	 
	  }
	*/
	  //cout << "fs_e="   << e_idfs.size() << " ";
	  //cout << "fs_mu="  << mu_idfs.size() << " ";
	  //cout << "fs_tau=" << tau_idfs.size() << " ";
	

	  //	  cout << "FLAGset!" <<endl;	    
	/*  
	const HepMC::GenEvent* ge = event.genEvent();
	    if (ge != nullptr) {
	      ge->print(); // sends (formatted) HepMC event to screen
	      _hepmcout->write_event(ge); // writes to file
	    }
	    if (ge == nullptr) cout << "nullpointer found: GenEvent*"<<endl;
	    //	cout << "++++++++++++++++++++++++++++++++++++++++++ " << endl ; 	  
	    if ( flag ) { cout << "fs leptons found" << endl; 
	    } else { cout << "no fs leptons found in event record." << endl; }
	*/  
      }
      
      //     const FourMomentum t1P4 = leptonicpartontops[0] ;
      //     const double weight = event.weight() ;
      //     cout << "END_ANALYZE()" << endl << endl ;
      //      if ( leptonicPartonTops.size() == 0 ) vetoEvent ;

      //      cout << endl << "-END   ANALYZE()" << endl ;

      //      _hSL_leptonicTopPt->fill(t1P4.pT(), weight) ;
      //      cout << endl ;
    
    }

    
    /// Normalise histograms
    void finalize() {

      _hepmcout->clear() ;
      //  delete _hepmcout;
      _hepmcout.reset() ;
      // Unit-normalized
      //      normalize(_hSL_leptonicTopPt) ;

    }
    
    /// @name Histograms
    //    Histo1DPtr _hSL_leptonicTopPt ;
    
    std::unique_ptr<HepMC::IO_GenEvent> _hepmcout;

    } ;
  
  
  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(LeptonicTopFinder) ;
  
  
  }
