#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
//#include <dir.h>
using namespace std;

class Geography
{
public:
  typedef std::map<int, std::vector<int> > component_map_t;
  typedef std::map<int, std::vector<int> > way_nodes_t;
  typedef std::map<int, std::vector<int> > nodes_ways_t;
  typedef std::map<int, int > way_component_t; // a way can be a member of many components, we need to figure out how and way

  vector<int> node_components;
  vector<double> node_lat;
  vector<double> node_lon;
  vector<int> node_id;
  vector<int> way_id;
  component_map_t component_map;
  way_nodes_t  way_nodes;
  nodes_ways_t node_ways;
  way_component_t way_components;
  int debug;
public:
  Geography()
  {
    debug=1;
  }

  int read_way_nodes() {

    FILE * pFile;
    pFile = fopen ( "waynodes.bin" , "r" );
    if (!pFile)   {
      printf("file could not be opened\n");
      return 2;
    }
    int waycount;
    int br=fread ((void*)&waycount , sizeof(int), 1 , pFile ); // count of ways

    // loop over all the ways
    int wayindex=0;
    for (wayindex =0; wayindex < waycount; wayindex++)    {
      int i2;
      fread ((void*)&i2 , sizeof(int), 1 , pFile ); // index
      if (i2 != wayindex)   {
        printf("Reading way index %d failed with a different %d\n",wayindex,i2);
        return 1;
      }
  
      int node_count;
      const int max_node_count=1024;// FIXME!
      //fread ((void*)&node_count , sizeof(int), 1 , pFile ); // size
      // if it is zero we can read until we find a null

      int waynodes[max_node_count];
      //fread ((void*)&waynodes, sizeof(int), node_count , pFile ); // dont read it, we look for 0 to end the list of data    

      // now we can process the edges...    
      int j2;// index
      do {
        fread ((void*)&waynodes[j2], sizeof(long int), 1 , pFile ); // we look for 0 to end the list of data            
        if (waynodes[j2]!=0)    j2++;
      } until (waynodes[j2]==0) 

      //      for (j2 =0; j2< node_count; j2++)    {
      for (j2 =0; j2< node_count; j2++)    {
        int tonode = (int)waynodes[j2];

        //
        if (debug > 10)
          {
            cerr << "reading from node : [" << j2 << "]" << tonode <<"(" << node_id[tonode] << ")" 
                 << " emit way: "  << wayindex   <<"(" << way_id[wayindex]    << ")"         
                 << endl;
          }

        way_nodes[wayindex].push_back(tonode); //the way contains this node
        node_ways[tonode].push_back(wayindex); //the node is in this way
      }
    }
    fclose (pFile);
    return 0;
  }


  template <class T> int read_data (const char * filename,vector<T> & vdata)
  {
    FILE * pFile;
    pFile = fopen ( filename , "r" );
    if (!pFile)   {
      printf("file could not be opened\n");
      return 2;
    }
    int count;
    int i=0;
    int br=fread ((void*)&count , sizeof(int), 1 , pFile ); // count of ways
    //    printf("Reading bytes %d count %d\n",br,count);
    T data[count];
    br= fread ((void*)&data , sizeof(T), count , pFile ); // index
    fclose (pFile);
    for (i =0; i < count; i++)    {
      vdata.push_back((T)data[i]);
    }
    return 0;
  }

  template <class T> void emit(vector<T> & data) {
    vector<int>::const_iterator cii;
    cerr << endl << "Iterator:" << endl;
    for(cii=data.begin(); cii!=data.end(); cii++)
      {
        cerr << *cii << endl;
      }
  }

  void reverse_components () 
  {

    int index=0;
    vector<int>::const_iterator cii;
    for(cii=node_components.begin(); cii!=node_components.end(); cii++)
      {
        component_map[*cii].push_back(index);
        if (debug > 8)
          {
            cout << "Component " << *cii << "contains node "<< index << endl;
          }
        index++;
      }
  }

  void emit_ways_components()
  {
    cerr << "output the way components " << endl;
    way_component_t::iterator wcit;
    for (wcit=way_components.begin();wcit!=way_components.end();wcit++)    {
      std::map<int,int>::iterator wcit2;
      int comp = wcit->second;
      cerr << "way:" << wcit->first << "\tcomponent:" <<  comp  <<  endl;
    }
  }

  void read_data()
  {
    read_node_lat();
    read_node_lon();
    read_node_id();
    read_way_id();
    read_way_nodes();
  }

  void read_components() {
        read_data<int> ("components.bin",node_components);    
        reverse_components();
  }

  void read_node_lat(){ read_data<double> ("nodelat.bin"   , node_lat); }
  void read_node_lon(){ read_data<double> ("nodelon.bin"   , node_lon); }
  void read_node_id() { read_data<int>    ("nodeindex.bin" , node_id);  }
  void read_way_id()  { read_data<int>    ("wayindex.bin"  , way_id);   }


};

