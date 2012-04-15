#include "FOSMBin.hpp"
int main()
{
  Geography geo;
  geo.read_data();

  int nodecount=(int) geo.node_id.size();
  int waycount =(int) geo.way_id.size();

  // header
  cout << nodecount << " " << waycount << endl;
  
  int way_index=0;
  for (ways_nodes_it  =  geo.way_nodes.begin();
       ways_nodes_it !=   geo.way_nodes.end();
       ways_nodes_it++)
    {
      vector<int>::const_iterator cii;
      vector<int> &node_list =ways_nodes_it->second;
      int nodecount=0;
      for(cii=node_list.begin(); cii!=node_list.end(); cii++) {
        int nodeidx=*cii;
        cout << nodeidx << "\t";
      }
      cout << endl;
    }
}
