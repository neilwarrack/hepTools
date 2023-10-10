// -*- C++ -*-
#include "Rivet/Analysis.hh"
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/FastJets.hh"
#include "Rivet/Projections/DressedLeptons.hh"
#include "Rivet/Projections/MissingMomentum.hh"
#include "Rivet/Projections/PromptFinalState.hh"

namespace Rivet {


  /// @brief ttbar spin correlations; full density matrix
  class ttbar_dil_spin : public Analysis {
  public:

    /// Constructor
    DEFAULT_RIVET_ANALYSIS_CTOR(ttbar_dil_spin);


    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // Initialise and register projections
      const FinalState fs(Cuts::abseta < 4.9);

      FastJets jetfs(fs, FastJets::ANTIKT, 0.4, JetAlg::Muons::NONE, JetAlg::Invisibles::NONE);
      declare(jetfs, "jets");

      PromptFinalState bare_leps(Cuts::abspid == PID::MUON || Cuts::abspid == PID::ELECTRON);

      Cut lepton_cuts = Cuts::abseta < 2.5 && Cuts::pT > 20*GeV;

      IdentifiedFinalState photons(fs);
      photons.acceptIdPair(PID::PHOTON);
      DressedLeptons dressed_leps(photons, bare_leps, 0.1, lepton_cuts);
      declare(dressed_leps, "leptons");

      // missing momentum
      declare(MissingMomentum(fs), "MET");

      // Parton-level top quarks
      declare(PartonicTops(PartonicTops::DecayMode::E_MU, false), "LeptonicPartonTops");

      // Book histograms
      book(_h["b_kplus"], "bkp", {-1.0, -0.5, 0.0, 0.5, 1.0});

    }


    /// Perform the per-event analysis
    void analyze(const Event& event) {

      // retrieve dressed leptons (pT ordered)
      vector<DressedLepton> leptons = apply<DressedLeptons>(event, "leptons").dressedLeptons();

      // retrieve clustered jets with a cut
      Jets jets = apply<FastJets>(event, "jets").jetsByPt(Cuts::pT > 30*GeV);

      // remove all jets within dR < 0.2 of a dressed lepton
      idiscardIfAnyDeltaRLess(jets, leptons, 0.2);

      // select jets ghost-associated to B-hadrons with a certain fiducial selection
      Jets bjets = filter_select(jets, [](const Jet& jet) {
        return  jet.bTagged(Cuts::pT > 5*GeV && Cuts::abseta < 2.5);
      });

      // veto event if there is not 2 b-jets
      if (bjets.size() != 2)  vetoEvent;

      // apply a missing-momentum cut
      if (apply<MissingMomentum>(event, "MET").missingPt() < 30*GeV)  vetoEvent;

      // Neil: A lot of the following is taken from the CMS spin cors rivet routine (CMS_2016_I1413748.cc), then edited.

      // Get the leptonically decaying tops
      const Particles& leptonicpartontops = apply<ParticleFinder>(event, "LeptonicPartonTops").particlesByPt();
      Particles chargedleptons;
      unsigned int ntrueleptonictops = 0;
      bool oppositesign = false;

      if ( leptonicpartontops.size() == 2 ) {
        for (size_t k = 0; k < leptonicpartontops.size(); ++k) {

          // Get the lepton
          const Particle lepTop = leptonicpartontops[k];
          const auto isPromptChargedLepton = [](const Particle& p){return (isChargedLepton(p) && isPrompt(p, false, false));};
          Particles lepton_candidates = lepTop.allDescendants(firstParticleWith(isPromptChargedLepton), false);
          if ( lepton_candidates.size() < 1 ) MSG_WARNING("error, PartonicTops::DecayMode::E_MU top quark had no daughter lepton candidate, skipping event.");

          // In some cases there is no lepton from the W decay but only leptons from the decay of a radiated gamma.
          // These hadronic PartonicTops are currently being mistakenly selected by PartonicTops::DecayMode::E_MU (as of April 2017), and need to be rejected.
          // PartonicTops::DecayMode::E_MU is being fixed in Rivet, and when it is the veto below should do nothing.
          /// @todo Should no longer be necessary -- remove
          bool istrueleptonictop = false;
          for (size_t i = 0; i < lepton_candidates.size(); ++i) {
            const Particle& lepton_candidate = lepton_candidates[i];
            if ( lepton_candidate.hasParent(PID::PHOTON) ) {
              MSG_DEBUG("Found gamma parent, top: " << k+1 << " of " << leptonicpartontops.size() << " , lepton: " << i+1 << " of " << lepton_candidates.size());
              continue;
            }
            if ( !istrueleptonictop && sameSign(lepTop,lepton_candidate) ) {
              chargedleptons.push_back(lepton_candidate);
              istrueleptonictop = true;
            }
            else MSG_WARNING("Found extra prompt charged lepton from top decay (and without gamma parent), ignoring it.");
          }
          if ( istrueleptonictop ) ++ntrueleptonictops;
        }
      }

      if ( ntrueleptonictops == 2 ) {
        oppositesign = !( sameSign(chargedleptons[0],chargedleptons[1]) );
        if ( !oppositesign ) MSG_WARNING("error, same charge tops, skipping event.");
      }

      if ( ntrueleptonictops == 2 && oppositesign ) {

        // Get the four-momenta of the positively- and negatively-charged leptons
        FourMomentum lepPlus = chargedleptons[0].charge() > 0 ? chargedleptons[0] : chargedleptons[1];
        FourMomentum lepMinus = chargedleptons[0].charge() > 0 ? chargedleptons[1] : chargedleptons[0];

        const double dphi_temp = deltaPhi(lepPlus,lepMinus);

        // Get the four-momenta of the positively- and negatively-charged tops
        FourMomentum topPlus_p4 = leptonicpartontops[0].pid() > 0 ? leptonicpartontops[0] : leptonicpartontops[1];
        FourMomentum topMinus_p4 = leptonicpartontops[0].pid() > 0 ? leptonicpartontops[1] : leptonicpartontops[0];
        const FourMomentum ttbar_p4 = topPlus_p4 + topMinus_p4;

        const double tt_mass_temp = ttbar_p4.mass();
        const double tt_absrapidity_temp = ttbar_p4.absrapidity();
        const double tt_pT_temp = ttbar_p4.pT();

        // Lorentz transformations to calculate the spin observables in the helicity basis

        // Transform everything to the ttbar CM frame
        LorentzTransform ttCM;
        ttCM.setBetaVec(-ttbar_p4.betaVec());

        topPlus_p4 = ttCM.transform(topPlus_p4);
        topMinus_p4 = ttCM.transform(topMinus_p4);

        lepPlus = ttCM.transform(lepPlus);
        lepMinus = ttCM.transform(lepMinus);

        // Now boost the leptons to their parent top CM frames
        LorentzTransform topPlus, topMinus;
        topPlus.setBetaVec(-topPlus_p4.betaVec());
        topMinus.setBetaVec(-topMinus_p4.betaVec());

        lepPlus = topPlus.transform(lepPlus);
        lepMinus = topMinus.transform(lepMinus);

        const double lepPlus_costheta_temp = lepPlus.vector3().dot(topPlus_p4.vector3()) / (lepPlus.vector3().mod() * topPlus_p4.vector3().mod());
        const double lepMinus_costheta_temp = lepMinus.vector3().dot(topMinus_p4.vector3()) / (lepMinus.vector3().mod() * topMinus_p4.vector3().mod());
        const double c1c2_temp = lepPlus_costheta_temp * lepMinus_costheta_temp;
        const double cos_opening_angle_temp = lepPlus.vector3().dot(lepMinus.vector3()) / (lepPlus.vector3().mod() * lepMinus.vector3().mod());

      }

      double pol = 0.1;
      
      // fill histograms
      _h["b_kplus"]->fill(pol);

    }


    /// Normalise histograms etc., after the run
    void finalize() {

      //normalize(_h["b_kplus"]); // normalize to unity
      scale(_h["b_kplus"], crossSection()/picobarn/sumOfWeights()); // norm to cross section

    }

    //@}


    /// @name Histograms
    //@{
    map<string, Histo1DPtr> _h;
    map<string, Profile1DPtr> _p;
    map<string, CounterPtr> _c;
    //@}


  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(ttbar_dil_spin);


}
