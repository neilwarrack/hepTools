/**************************************************************************************************
* Debug your loopy code with codecounter.cc!                                                      *
*                                                                                                 *
* Program to analyse output from any repetative code that spits out integers in the range 1-9     *
* (aka "debug codes")                                                                             *
* Example usage in c++ (take note of spaces after numbers!):                                      *
*                                                                                                 *
*in your code:                                                                                    *
* ****************************************************************************                    *
* *                                                                          *                    *
* * cout << "possible other output stuff" << endl;                           *                    *
* * cout << "my code will now execute a loop and << endl;                    *                    *
* * cout << "spit out space-separated numbers 1-9" << endl;                  *                    *
* * for (int i=0; i<1500; i++){                                              *                    *
* *   if (conditionA){cout << "1 ";}                                         *                    *
* *   if (conditionB){cout << "2 ";}                                         *                    *
* *   if (conditionC){cout << "3 ";}                                         *                    *
* *   if (!conditionA && !conditionB && !conditionC) {                       *                    *
* *     cout << "9 ";                                                        *                    *
* *   } // NB: '9' usually stands for a 'pass' (but it doesnt have to!)      *                    *
* *                                                                          *                    *
* * cout << endl; // NEW LINE FOR EVERY LOOP!                                *                    *
* *                                                                          *                    *
* *} // end of loop                                                          *                    *
* *                                                                          *                    *
* *                                                                          *                    *
* *                                                                          *                    *
* ****************************************************************************                    *
*                                                                                                 *
*                                                                                                 *
* On command line:                                                                                *
* 1) compile this code                                                                            *
* 2) run your code and save the output to file                                                    *
* 3) run codecounter (choosing your output file and number of lines to skip at start)             *
*                                                                                                 *
* For example:                                                                                    *
*                                                                                                 *
*  ba$h: g++ -std=c++11 codecounter.cc -o codecounter                                             *
*  ba$h: ./yourCode > yourcode.log                                                                *
*  ba$h: ./codecounter yourcode.log 3  #< this would skip the first 3 lines of yourcode.log       *
*                                                                                                 *
*                                                                                                 *
*OUTPUT (something like...):                                                                      *
*                                                                                                 *
*  ba$h:     code 1: 2                                                                            *
*  ba$h:     code 2: 34                                                                           *
*  ba$h:     code 3: 30                                                                           *
*  ba$h:     code 4: 561                                                                          *
*  ba$h:     code 5: 450                                                                          *
*  ba$h:     code 6: 2                                                                            *
*  ba$h:     code 7: 100                                                                          *
*  ba$h:     code 8: 81                                                                           *
*  ba$h: PASSED (9): 240                                                                          *
*  ba$h:            ______                                                                        *
*  ba$h:      Total: 1500                                                                         * 
*                                                                                                 * 
**************************************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>

using namespace std;


int lineCounter(ifstream& dat) {
  int ctr = 0 ;
  string line ;
  while (getline(dat, line)) ctr++ ;
  // clear tags and return ifstream to beginning of input file
  dat.clear();
  dat.seekg(0, ios::beg);
  return ctr ;
}


template<typename T>
void printout(T const &rhs) {
  cout << rhs << endl ;
}



int main ( int argc, char *argv[] ){

  // check for appropriate number of command line arguments  
  if (argc != 3) {
    cout << "codecounter-Error: requires an input (.log) file and an integer (number of lines to skip) as arguments... " << endl ;
    cout << "codecounter-Error: (see input file - count number of lines to skip by eye)" << endl ; 
    cout << "codecounter-Error: ...aborting."<< endl;
    return 1;
  }


  // feedback user info:
  cout << "codecounter-INFO: input file: " ; 
  printout(argv[1]) ;
  cout << "codecounter-INFO: integer number of lines to skip: " ; 
  printout(argv[2]) ;


 // Open an existing file given as argument 
  string filename = argv[1] ; 
  ifstream mydata ;
  mydata.open(filename) ;
  if ( mydata.fail() ) {
    cout << "codecounter-Error: couldn’t open file: " << filename << endl ;
    return 1; 
  }

  
  // Count lines in input file
  int const static lines = lineCounter(mydata) ;
  int codeCounter[] = {0,0,0,0,0,0,0,0,0} ;
  const static int m = atoi(argv[2]) ;
  int x = 0 ;
  int line = 0 ;
  string linestring ;
  

  // abort of lines to skip > lines in file
  if (m > lines) {
   cout << "codecounter-Error: line to skip is larger than lines in file " << endl ; 
   return 1;
  }


  // Begin file analysis
  while ( mydata.good()){
    
    if ( line < m ) {
      if( mydata.eof() ) break ;             
      getline(mydata, linestring);
      // cout << "WARNING: ignoring first " << m << " lines..." << line + 1 << " -> " << infoline << endl; // uncomment this line to print skipped lines to screen
      line++ ;
      
    } else { // if ( line >= m )
      
      // output info to user
      cout << "codecounter-INFO: Ignored first " << m << " lines..." << endl ;
      cout << "codecounter-INFO: reading remaining " << lines - m << " lines from file" << endl ;
      
      while(getline(mydata, linestring))
	{
	  istringstream iss(linestring) ;
	  while(iss >> x){
	    //___________________________________________________________________________
	    if ( x == 1 ) {codeCounter[0] =(codeCounter[0]+1);}else{
	      if ( x == 2 ) {codeCounter[1] =(codeCounter[1]+1);}else{
		if ( x == 3 ) {codeCounter[2] =(codeCounter[2]+1);}else{
		  if ( x == 4 ) {codeCounter[3] =(codeCounter[3]+1);}else{
		    if ( x == 5 ) {codeCounter[4] =(codeCounter[4]+1);}else{
		      if ( x == 6 ) {codeCounter[5] =(codeCounter[5]+1);}else{
			if ( x == 7 ) {codeCounter[6] =(codeCounter[6]+1);}else{
			  if ( x == 8 ) {codeCounter[7] =(codeCounter[7]+1);}else{
			    if ( x == 9 ) {codeCounter[8] =(codeCounter[8]+1);}
			   }
			 }
		       }
		     }
		   }
		 }
	       }
	     }
	     //___________________________________________________________________________
	   } // end of while statement
	   
	   line++;
	 }
     }
 }
 
 mydata.close(); // close when finished   

 int total=0;
 for (int i=0; i<9; i++){
   //cout << "adding code" << i+1 << "=" << codeCounter[i] << "to total..." << endl;
   total += codeCounter[i];
 }
 
 cout << "    code 1: " << codeCounter[0] <<endl ;
 cout << "    code 2: " << codeCounter[1] <<endl ;
 cout << "    code 3: " << codeCounter[2] <<endl ;
 cout << "    code 4: " << codeCounter[3] <<endl ;
 cout << "    code 5: " << codeCounter[4] <<endl ;
 cout << "    code 6: " << codeCounter[5] <<endl ;
 cout << "    code 7: " << codeCounter[6] <<endl ;
 cout << "    code 8: " << codeCounter[7] <<endl ;
 cout << "PASSED (9): " << codeCounter[8] <<endl ;
 cout << "            ________" << endl;
 cout << "     Total: " << total << endl << endl;    
 return 0;
 
}
