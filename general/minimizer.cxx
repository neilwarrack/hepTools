#include "MendelMin_MY_clean.hh"
//#include "MendelMin_MY_wprintout.hh" // <- use this one instead for lots of printout

#include<iostream>
using namespace std;


double scale = 10.0;

double chibyhand(double p) {
  double p_scaled = p*scale;

  // Examp1e 1
  const double bin1 = (10.0 - p_scaled * 5.0);
  const double bin2 = (12.0 - p_scaled * 6.0);
  const double bin3 = (10.0 - p_scaled * 5.0);
  const double bin4 = (8.0  - p_scaled * 4.0);
  const double bin5 = (6.0  - p_scaled * 3.0);

  // Example 2
  // const double bin1 = (10.0 - p_scaled * 5.1);
  // const double bin2 = (12.0 - p_scaled * 5.9);
  // const double bin3 = (10.0 - p_scaled * 5.0);
  // const double bin4 = (8.0  - p_scaled * 4.0);
  // const double bin5 = (6.0  - p_scaled * 3.0);


  return bin1*bin1 + bin2*bin2 + bin3*bin3 + bin4*bin4 + bin5*bin5;
}


double f_chibyhand(const MendelMin::Params& p) {
  return chibyhand(p[0]);
}
  

int main()
{

  
  MendelMin mm(f_chibyhand, 1, 4); // (function, nParams, nGuessPerEvolution)

  // if you have a feel for the numbers, you can add a guess by hand:
  //const MendelMin::Params& testguess = {0.2};
  //mm.guess(testguess);

  const double best = mm.evolve(3);

  valarray<double> fittest = mm.fittest();
  cout << endl;
  cout << "MendleMin's FOUND results" << endl;
  cout << "FOUND minimum   (the chi2)       : " << f_chibyhand(fittest) << endl ;
  cout << "FOUND parameter (the scaled sf)  : " << fittest[0] << endl ;
  cout << "FOUND value     (the real sf)    : " << fittest[0]*scale << endl ;

  valarray<double> right{2.0};
  cout << endl;
  cout << "Correct results" << endl;
  cout << "CORRECT minimum   (the chi2)     : " << f_chibyhand(right/scale) << endl ;
  cout << "CORRECT parameter (the scaled sf): " << right[0]/scale << endl ;
  cout << "CORRECT value     (the real sf)  : " << right[0] << endl ;

  cout << endl;
  return 0;
}
