//=======================================================================
// Copyright 1997-2001 University of Notre Dame.
// Authors: Andrew Lumsdaine, Lie-Quan Lee, Jeremy G. Siek
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================

#include <boost/config.hpp>
#include <iostream>
#include <vector>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include "FOSMBin.hpp"

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> Graph;
typedef typename Graph::vertex_descriptor vertex_descriptor;
typedef typename Graph::edge_property_type edge_property; 


int edgecount;
int read_graph(Graph & g) {
  // now read the graph
  edgecount=0;

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
    vertex_descriptor prev = (int)waynodes[0];

    for (j2 =1; j2< j; j2++)    {
      vertex_descriptor to = (int)waynodes[j2];
      //      edges are from the nodes to each other, not from the way to the edge
      boost::add_edge(prev +1,to+1, g);
      boost::add_edge(to+1,prev +1, g);
      edgecount++;
      prev= to;
    }

  }

  fclose (pFile);
  return 0;
}

template <class Name>
void print_graph_dispatch(const Graph& G, Name name)
  {
    boost::graph_traits<Graph>::vertex_iterator ui,ui_end;
    for (boost::tie(ui,ui_end) = vertices(G); ui != ui_end; ++ui) {
      std::cout << get(name,*ui)+1 << " "; // self edge
      boost::graph_traits<Graph>::out_edge_iterator ei, ei_end;
      for(boost::tie(ei,ei_end) = out_edges(*ui,G); ei != ei_end; ++ei)
        std::cout << get(name,target(*ei,G)) << " ";
      std::cout << std::endl;
    }
  }

int main(int, char*[])
{
  using namespace boost;
  using namespace std;
  const char* name = "kosovo osm ways";
  Graph G;
  Geography geo;
  int ok =read_graph(G);

  geo.read_data<int>    ("nodeindex.bin" , geo.node_id); 
  int nodecount=(int) geo.node_id.size()+1;
  
  if(ok !=0)   {
    return ok;
  }

 
  cout << nodecount << " "<< edgecount << endl;
  print_graph_dispatch(G, get(vertex_index, G));

  return 0;
}
