#include <iostream>
#include <string>
#include <vector>
#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"


int main(int argc, char *argv[]) {

  std::string current_exec_name = argv[0]; // Name of the current exec program
  std::string first_arg;
  std::vector<std::string> all_args;

  if (argc > 1) {
    first_arg = argv[1];
    //    all_args.assign(argv + 1, argv + argc);
    if (argc > 2) {std::cout << "too many arguments..."<< std::endl; return 1;}  
  } else return 1;


  { // begin scope of ascii_in and ascii_out

    // specify an input file
    HepMC::IO_GenEvent ascii_in(first_arg,std::ios::in);

    //........................................EVENT LOOP
    int eventNum=0;
    // get first event
    HepMC::GenEvent* evt = ascii_in.read_next_event();
    
    // loop over events until they finish
    while ( evt ) {
      eventNum = evt->event_number();
      // delete the created event from memory
      delete evt;
      // read the next event
      ascii_in >> evt;
    }

    //  std::cout << "Number of events1 = " << icount << std::endl;  
  std::cout << "Number of events: " << eventNum + 1 << std::endl;  

  } // end scope of ascii_in and ascii_out

return 0;
}
