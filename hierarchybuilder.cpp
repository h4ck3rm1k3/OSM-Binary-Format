#include "FOSMBin.hpp"
#include <set>
#include <math.h>
class Distance
{
public:
  int index;
  double distance;

  Distance(int index, double distance)
    :index(index),
     distance(distance)
  {
    
  }

  bool operator < (const Distance & other) const
  {
    if(distance < other.distance )
      return true;
    else
      return false;
  }
};


class Box
{
public:
  //double coord[4];
  double minlat, maxlat, minlon, maxlon;
  Box():
    minlat(0),
    maxlat(0),
    minlon(0),
    maxlon(0)
  {    
  }

  Box(double minlat, double maxlat, double minlon, double maxlon)
    : minlat(minlat),
      maxlat(maxlat),
      minlon(minlon),
      maxlon(maxlon)
  {    
  }

  bool contains(const Box & b) const 
  {
    if (
        (b.minlat > minlat)
        && 
        (b.maxlat < maxlat)
        &&
        (b.minlon < minlon)
        &&
        (b.maxlon < maxlon)
        )
      {
        return 1;
      }    

    return 0;
  }

  void report()
  {
    cout << " lat(" <<  minlat       << " , "  << maxlat 
         << " lon(" <<  minlon       << " , "  << maxlon
         << ")" << endl;

    vector<Box>::iterator bi;
    cout << "Children" << endl;
    for(bi=children.begin(); bi!=children.end(); bi++) {
      cout << "Child : " << endl;
      bi->report();
    }
    cout << "end of Children" << endl;
          
  }

  void add(Box & b)
  {
    // now we add this box to the children, or put it in the top level.
    if (contains(b))
      {
        vector<Box>::iterator bi;
        for(bi=children.begin(); bi!=children.end(); bi++) {
          if (bi->contains(b))
            {
              bi->add(b); // add this
              cout << "adding to child" << endl;
              report();
              return;
            }
          else if (b.contains(*bi)) {
              children.erase(bi);
              b.add(*bi);
              children.push_back(b);              
              cout << "Replacing child" << endl;
              report();
              return;
            }
          
        } 
        cout << "adding to child of main" << endl;
        children.push_back(b); // otherwise just insert it at the end
      }
    else
      {
        cout << "expanding main" << endl;
        add(b.minlat, b.minlon);
        add(b.maxlat, b.maxlon);
      }
    report();
  }
  
  void add(double lat, double lon)
  {
    if (minlat ==0 )
      {
        minlat=lat;
      }
    if (minlon ==0 )
      {
        minlon=lon;
      }
    if (lat < minlat)
      {
        minlat=lat;
      }
    if (lat > maxlat)
      {
        maxlat=lat;
      }
    if (lon < minlon)
      {
        minlon=lon;
      }
    if (lon > maxlon)
      {
        maxlon=lon;
      }
    report();
  }
  vector<Box> children; // all the children boxes that are contained in this box  
};


int main()
{
  Geography geo;
  geo.read_data();

  Geography::way_nodes_t::iterator ways_nodes_it;
  set<Distance> LongestWay;
  Box topBoxWay; // this is the biggest box found of ways
  Box topBoxPoint; // this is the biggest box found of points

  int way_index=0;
  for (ways_nodes_it  =  geo.way_nodes.begin();
       ways_nodes_it !=   geo.way_nodes.end();
       ways_nodes_it++)
    {
      vector<int>::const_iterator cii;
      vector<int> &data =ways_nodes_it->second;
      double distance=0;
      int nodecount=0;
      Box bbox; // this is the bbox of this way
      for(cii=data.begin(); cii!=data.end(); cii++) {
        int nodeidx=*cii;
        int nodeid = geo.node_id[nodeidx];
        double lat = geo.node_lat[nodeidx];
        double lon = geo.node_lon[nodeidx];
        
        bbox.add(lat,lon); // expand the bbox
        topBoxPoint.add(lat,lon); //expand the main bbox
        topBoxPoint.report();
        nodecount++;

        if (cii!=data.begin())
          {
            double lat2 = geo.node_lat[nodeidx];
            double lon2 = geo.node_lon[nodeidx];
            // distance 
            double dlat = lat2 - lat;
            double dlon = lon2 - lon;
            double pyth = sqrt((dlat * dlat) + (dlon + dlon));
            distance += pyth;
          }
      }

      cout << "way" << way_index << " nodecount " << nodecount  << "distance" <<distance<< endl;
      LongestWay.insert(Distance(way_index,distance));
      way_index++;

      topBoxWay.add(bbox);
      topBoxWay.report();
    }
  
  // find the longest ways in general, based on the length of the node.
     // compare each way with each other.
  
  ///    use those as central splitting things.
  // left or right of that way, starting with the longest. for each segment. 

  // find circles that contain all the nodes, outer circle
  // find the most extreme nodes, max x, max y, min x, min y.
  //   look in the binary middle, then try and find a splitting way.

  // sort the
  // for each extreme point, look for the next point that increases the area the most, find a connection between them.
  // for points on top of each other, withing a certain threshhold, merge them together.
  // for nodes that are only alone a road, merge into one.
  // for ways that forme a circle, create a single object from that.
  
  // look for circles inside, ones that touch or are near the outer,
  // that will fit into the major outer one.
  // look for ways that cut across the entire area, splitting it. 
  // split up the data based on what is already there, left of road, right of road, use the biggest objects to start with. 
  // create a hierarchy based on that. 
}
