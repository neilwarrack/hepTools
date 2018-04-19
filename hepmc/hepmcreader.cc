#include <iostream>
#include <string>
#include <vector>
#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"


int main(int argc, char *argv[]) {

  // return info for argc != 2
  if (argc == 1 || argc > 2) {
    std::cout << "supply one argument; a HepMC file to turn it into a more "
	      << "collimated, 'readable' output" << std::endl ;
    return 1;
  }

  std::string current_exec_name = argv[0]; // Name of the current exec program
  std::string first_arg;
  first_arg = argv[1];
  
  std::cout<<"running " << current_exec_name << " for " << first_arg << "."
	   << std::endl ;


  { // begin scope of ascii_in and ascii_out

    // specify an input file
    HepMC::IO_GenEvent ascii_in(first_arg,std::ios::in);

    //........................................EVENT LOOP
    int eventNum=0;
    // get first event
    HepMC::GenEvent* evt = ascii_in.read_next_event();
   
    // loop over events until they finish
    while ( eventNum < 20 ) {
      eventNum = evt->event_number();
      evt->print();    

      // delete the created event from memory
      delete evt;
      // read the next event
      ascii_in >> evt;
    }


  } // end scope of ascii_in and ascii_out

return 0;
}
