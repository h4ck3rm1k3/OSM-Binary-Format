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

typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> Graph;
typedef typename Graph::vertex_descriptor vertex_descriptor;
typedef typename Graph::edge_property_type edge_property; 

int read_graph(Graph & g) {
  // now read the graph
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
      boost::add_edge(prev,to, g);
      prev= to;
    }

  }

  fclose (pFile);
  return 0;
}

int main(int, char*[])
{
  using namespace boost;
  using namespace std;
  const char* name = "kosovo osm ways";

  Graph G;
  boost::dynamic_properties dp; 
  typedef typename Graph::vertex_descriptor vertex_descriptor;
  //dp.property("node_id", boost::make_null_property<vertex_descriptor, string>); 

  int ok =read_graph(G);
  if(ok !=0)   {
    return ok;
  }

  std::cout << "A directed graph:" << std::endl;
  //  print_graph(G);
  std::cout << std::endl;
  std::cout << "Done:" << std::endl;

  typedef graph_traits<Graph>::vertex_descriptor Vertex;
    
  std::vector<int> component(num_vertices(G)), discover_time(num_vertices(G));
  std::vector<default_color_type> color(num_vertices(G));
  std::vector<Vertex> root(num_vertices(G));
  int num = connected_components(G, // graph
                              &component[0],  // component map
                              root_map(&root[0]). //
                              color_map(&color[0]).
                              discover_time_map(&discover_time[0])
                              );
    
  std::cout << "Total number of components: " << num << std::endl;
  std::vector<int>::size_type i;
  for (i = 0; i != component.size(); ++i) {
    std::cout <<  i <<"\t" << component[i] << std::endl;
  }
    
  return 0;
}
