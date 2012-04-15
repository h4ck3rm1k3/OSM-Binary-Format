#include "FOSMBin.hpp"

class EmitOSM : public Geography
{
public:
  EmitOSM ()
    : Geography()
  {
    debug=0;
  }
  
  void ProcessNodes() {
    
    component_map_t::iterator iter;
    for (iter=component_map.begin();iter!=component_map.end();iter++)    {
      cout << "component " << iter->first << endl;
      vector<int>::const_iterator cii;
      vector<int> &data =iter->second;
      std::map<int, int> component_ways; //each way from this component, with a count   

      char buffer [255];
      int component=iter->first;
      sprintf(buffer,"split/%d.osm",component);
      
      ofstream of(buffer);
      of << "<?xml version='1.0' encoding='UTF-8'?>" << endl;
      of << "<osm version=\"0.6\" generator=\"fosmbin2osm\">" << endl;

      for(cii=data.begin(); cii!=data.end(); cii++)
        {
          int nodeidx=*cii;// 
          nodes_ways_t::iterator node_ways_it;
          node_ways_it=node_ways.find(nodeidx); // get all the ways from that node

          if (node_ways_it!=node_ways.end()) {
            std::vector<int> & wayids=node_ways_it->second;
            std::vector<int>::iterator waysit;

            for (waysit=wayids.begin();waysit != wayids.end();waysit++) {
              int wayid=*waysit;

              // for each way that this node occurs in
              way_components[wayid]=component;

              if (debug > 20)
                {
                  cerr << "from node : " << nodeidx <<"(" << node_id[nodeidx] << ")" 
                       << " emit way: "  << wayid   <<"(" << way_id[wayid]    << ")"         
                       << " in component :" << component  << endl;
                }
              component_ways[wayid]++;// we will output these later
            }// for each way
          }// if there are any ways                  

          emitnode(nodeidx,of); // emit the nodes
        }// noide

      // now emit all ways
      {
        std::map<int, int>::iterator it;
        for ( it = component_ways.begin(); it != component_ways.end();   it ++      )  {
          int wayid = it->first;
          if (debug)  {
            cerr << " emit way: "  << wayid   <<"(" << way_id[wayid]    << ")"         
                 << " in component :" << component  << endl;
          }
          emitway(wayid,of);
        }
      }
      of << "</osm>" << endl;

    }// component
  }

  void emitnode(int i, ostream & os) {
  
    int component=node_components[i];
    os << "<node ";
    os << "id='"<< node_id[i] << "' ";          
    os << " version='1' ";
    os << " lat='";
    os.precision(6);
    os.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed      
    os << node_lat[i];
    os << "' " ;  
    os << " lon='";
    os.precision(6);
    os.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed      
    os << node_lon[i] << "' ";  
    os << ">";
    os << "<tag k='component' v='" << component << "'/>" ;  
    os << "</node> " << endl;  
  }

  void emitway(int i, ostream & os) {
 
    int component=way_components[i];

    // way_component
    os << "<way ";
    os << " version='1' ";
    os << "id='"<< way_id[i] << "' ";          
    os << ">";
    os << "<tag k='component' v='" << component << "'/>" ;  
    //way_nodes

    way_nodes_t::iterator ways_nodes_it;
    ways_nodes_it=way_nodes.find(i);
    if (ways_nodes_it ==way_nodes.end()) { /* empty */ }
    else {
      vector<int>::const_iterator cii;
      vector<int> &data =ways_nodes_it->second;
    
      os << endl;  
      for(cii=data.begin(); cii!=data.end(); cii++)       {
        int nodeidx=*cii;
        int nodeid = node_id[nodeidx];
        os << "<nd ref='" << nodeid << "' />" << endl;  
      }
    os << "</way> " << endl;  
    }
  }

  
};

int main()
{
  EmitOSM geo;
  geo.read_data();
  geo.read_components(); // this optional data is read in
  geo.ProcessNodes();  
}
