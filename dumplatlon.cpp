#include "FOSMBin.hpp"

int main()
{
  Geography geo;
  geo.read_data<double> ("nodelat.bin",geo.node_lat);
  geo.read_data<double> ("nodelon.bin",geo.node_lon);
  geo.read_data<int> ("nodeindex.bin",geo.node_id);    

  vector<int>::const_iterator cii;
  int index=0;
  for(cii=geo.node_id.begin(); cii!=geo.node_id.end(); cii++)  {

    cout << index ;
    cout << "\t";

    cout << *cii ;
    cout << "\t";

    cout.precision(6);
    cout.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed      
    cout << geo.node_lat[index];
    cout << "\t";

    cout.precision(6);
    cout.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed      
    cout << geo.node_lon[index];
    cout << "\n";

    index++;
  }
}
