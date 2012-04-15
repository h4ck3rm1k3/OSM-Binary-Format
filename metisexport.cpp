//=======================================================================
// Copyright 1997-2001 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <iostream>
#include <vector>
#include <math.h>
#include "FOSMBin.hpp"

typedef std::map<int, int >  node_edge_t; // a way can be a member of many components, we need to figure out how and way


typedef std::map<int, node_edge_t > node_node_t; // a way can be a member of many components, we need to figure out how and way

//int edgecount;
int read_graph(node_node_t & g,int &edgecount) {
  edgecount =0;
  FILE * pFile;
  pFile = fopen ( "waynodes.bin" , "r" );
  if (!pFile)   {
    printf("file could not be opened\n");
    return 2;
  }
  int  waycount;
  int i=0;
  int br=fread ((void*)&waycount , sizeof(int), 1 , pFile ); // count of ways

  for (i =0; i < waycount; i++)    {
    int j;
    int i2;
    fread ((void*)&i2 , sizeof(int), 1 , pFile ); // index
    if (i2 != i)
      {
        printf("Reading way index %d failed with a different %d\n",i,i2);
        return 1;
      }

    fread ((void*)&j , sizeof(int), 1 , pFile ); // size
    int waynodes[j];
    fread ((void*)&waynodes, sizeof(int), j , pFile ); // data
    
    // now we can process the edges...
    int j2=0;
    int prev = (int)waynodes[0];

    for (j2 =1; j2< j; j2++)    {
      int to = (int)waynodes[j2];
      int count=0;
      if (prev != to)        {


        count = g[prev][to]++;
        if (count == 0)
          {
            edgecount++;
          }

        count = g[to][prev]++; // and the reverse
        if (count == 0)
          {
            edgecount++;
          }

      }
      prev= to;
    }

  }



  fclose (pFile);
  return 0;
}


int main(int, char*[])
{

  using namespace std;
  const char* name = "kosovo osm ways";

  node_node_t G;
  Geography geo;
  int   edgecount ;
  int ok =read_graph(G,edgecount);
  
  if(ok !=0)   {
    return ok;
  }


  geo.read_data<int>    ("nodeindex.bin" , geo.node_id); 
  int nodecount= geo.node_id.size();
  edgecount = ceil(edgecount/2); // should round up
  cout << nodecount << " "<< edgecount << endl;

  vector<int>::const_iterator nid;
  int index=1; // metis uses index 1 not 0
  for(nid=geo.node_id.begin(); nid!=geo.node_id.end(); nid++) {
    node_node_t::const_iterator nit;
    nit=G.find(index);
    if (nit != G.end()) {
      const node_edge_t & e=nit->second;
      node_edge_t::const_iterator eit;
      for(eit=e.begin(); eit!=e.end(); eit++) {
        if (eit->first != index) { //no self loops
          
          cout << eit->first << " ";
        }
      }
    }
    cout << endl;
    index++;
  }
  cout << "%eof" << endl; // need this because of empty lines
  return 0;
}
