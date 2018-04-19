#!/bin/bash
#export RIVET_ANALYSIS_PATH=$PWD
echo "Beam energy (TeV):"
read beamEnergy
echo "number of events:"
read numEvents
#run-pythia -n $numEvents -c Top:all=on -L P8Hook_W_to_E_MU.so -u W_to_E_MU -o fifo.hepmc &
#run-pythia -n $numEvents -c Top:all=on -L P8Hook_W_to_E_MU_TAU.so -u W_to_E_MU_TAU -o fifo.hepmc &
#run-pythia -s -n $numEvents -c Top:all=on -L P8Hook_W_to_E_MU_TAU.so -u W_to_E_MU_TAU -o fifo.hepmc &
#run-pythia -n $numEvents -c Top:all=on -L P8Hook_TTbarSemiLep.so -u TTbarSemiLep -o fifo.hepmc &
#run-pythia -s -n $numEvents -c "Top:qq2tq(t:W)=on" -L P8Hook_W_to_E_MU_TAU.so -u W_to_E_MU_TAU -o fifo.hepmc &
#run-pythia -n $numEvents -c "Top:ffbar2tqbar(s:W)=on" -L P8Hook_W_to_E_MU_TAU.so -u W_to_E_MU_TAU -o fifo.hepmc &
#run-pythia -s -n $numEvents -c " Top:ffbar2tqbar(s:W)=on" -L P8Hook_W_to_E_MU_TAU.so -u W_to_E_MU_TAU -o fifo.hepmc &
#run-pythia -n $numEvents -c "Top:qqbar2ttbar=on" -L P8Hook_W_to_E_MU_TAU.so -u W_to_E_MU_TAU -o fifo.hepmc &
#run-pythia -n $numEvents -c Top:all=on -L P8Hook_TTbarDiLep.so -u TTbarDiLep -o fifo.hepmc &
#run-pythia -n $numEvents -e $(($beamEnergy*1000)) -c Top:all=on -o fifo.hepmc &
#run-pythia -n $numEvents -e $(($beamEnergy*1000)) -c Top:all=on -L P8Hook_W_to_E_MU_TAU.so -u W_to_E_MU_TAU -o fifo.hepmc &
run-pythia -n $numEvents -e $(($beamEnergy*1000)) -c Top:all=on -L P8Hook_W_to_E_MU.so -u W_to_E_MU -o fifo.hepmc &
#run-pythia -n $numEvents -e $(($beamEnergy*1000)) -c "Top:qqbar2ttbar=on" -c "Top:gg2ttbar=on" -L P8Hook_W_to_E.so -u W_to_E -o fifo.hepmc &
#run-pythia -n $numEvents -e $(($beamEnergy*1000)) -c "Top:ffbar2tqbar(s:W)=on" -c "Top:qq2tq(t:W)=on" -L P8Hook_W_to_TAU.so -o fifo.hepmc &
#run-pythia -n $numEvents -e $(($beamEnergy*1000)) -c Top:all=on -L P8Hook_W_to_E.so -u W_to_E -o fifo.hepmc &
#run-pythia -n $numEvents -c "Top:qqbar2ttbar=on" -c "Top:gg2ttbar=on" -L P8Hook_TTbarDiLep.so -u TTbarDiLep -o fifo.hepmc &
#run-pythia -n $numEvents -e $(($beamEnergy*1000)) -c "Top:qqbar2ttbar=on" -c "Top:gg2ttbar=on" -o fifo.hepmc &
#run-pythia -n $numEvents -e $(($beamEnergy*1000)) -c "Top:all=on" -c "Print:quiet=on" -o fifo.hepmc &
#run-pythia -n $numEvents -e $(($beamEnergy*1000)) -c "Top:qqbar2ttbar=on" -c "Top:gg2ttbar=on" -c "Print:quiet=on" -o fifo.hepmc &