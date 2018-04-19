#!/bin/bash
export RIVET_ANALYSIS_PATH=$PWD
#rivet -a LeptonicTopFinder fifo.hepmc 
#rivet -a LeptonicTopFinder fifo.hepmc > ana.log
rivet -a LeptonicTopFinder fifo.hepmc 2>&1 | tee ana.log
#rivet -a LeptonicTopFinder hi.hepmc &> ana.log
#rivet -a ATLAS_2014_I1301856 fifo.hepmc > ana.log
#rivet -a LeptonicTopFinder ../ATLAS_2014_I1301856/pwgevents_pythia-ttbar-8000.hepmc > ana.log
#rivet -a ATLAS_2014_I1301856 fifo.hepmc 2>&1 | tee ana.log
./codecounter.sh ana.log 1
echo "============================================"
echo "--------------tail -20 ana.log--------------"
echo "============================================"
tail -20 ana.log
echo "============================================"
echo "----------------tail - DONE!----------------"
echo "============================================"