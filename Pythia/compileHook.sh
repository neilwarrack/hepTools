#!/bin/bash
#g++ W_to_E_MU.cc -I$HOME/heplocal/include -shared -fPIC -o P8Hook_W_to_E_MU.so
#g++ W_to_E_MU_TAU.cc -I$HOME/heplocal/include -shared -fPIC -o P8Hook_W_to_E_MU_TAU.so
#g++ W_to_E_MU_TAU.cc -I$HOME/include -shared -fPIC -o P8Hook_W_to_E_MU_TAU.so
#g++ W_to_TAU.cc -I$HOME/include -shared -fPIC -o P8Hook_W_to_TAU.so
#g++ W_to_E.cc -I$HOME/include -shared -fPIC -o P8Hook_W_to_E.so
#g++ W_to_MU.cc -I$HOME/include -shared -fPIC -o P8Hook_W_to_MU.so
g++ W_to_E_MU.cc -I$HOME/include -shared -fPIC -o P8Hook_W_to_E_MU.so
#g++ TTbarSemiLep.cc -I$HOME/heplocal/include -shared -fPIC -o P8Hook_TTbarSemiLep.so