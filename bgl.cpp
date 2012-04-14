
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

/*
#include <boost/graph/use_mpi.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/parallel/distributed_property_map.hpp>
#include <boost/property_map/parallel/caching_property_map.hpp>
#include <boost/graph/parallel/algorithm.hpp>
#include <boost/graph/parallel/process_group.hpp>
#include <boost/graph/distributed/queue.hpp>
#include <boost/pending/indirect_cmp.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include <boost/graph/overloading.hpp>
#include <boost/graph/distributed/concepts.hpp>
#include <boost/graph/distributed/local_subgraph.hpp>
#include <boost/graph/parallel/properties.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/distributed/reverse_graph.hpp>
#include <boost/optional.hpp>
#include <boost/graph/distributed/detail/filtered_queue.hpp>
#include <boost/graph/distributed/adjacency_list.hpp>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <sys/time.h>
#include <boost/graph/distributed/graphviz.hpp> // for ostringstream
#include <vector>
#include <map>
#include <boost/graph/parallel/container_traits.hpp>


// Enable PBGL interfaces to BGL algorithms
#include <boost/graph/use_mpi.hpp>

// Communication via MPI
#include <boost/graph/distributed/mpi_process_group.hpp>

// Dijkstra's single-source shortest paths algorithm
#include <boost/graph/dijkstra_shortest_paths.hpp>

// Distributed adjacency list
#include <boost/graph/distributed/adjacency_list.hpp>

// METIS Input
#include <boost/graph/metis.hpp>
*/
//#include <boost/fusion/include/pair.hpp>

// Standard Library includes
#include <fstream>
#include <string>
#include <boost/graph/graphviz.hpp>
using namespace boost;
using namespace std;
//using boost::graph::distributed::mpi_process_group;

#include <stdio.h>

int main (int argc, char* argv[])
{
  //  boost::mpi::environment env(argc,argv);
  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> Graph;
  
  //  typedef graph_traits<Graph> Traits;
  typedef typename Graph::vertex_descriptor vertex_descriptor;
  typedef typename Graph::edge_property_type edge_property; 
  //const typename Config::edge_property_type& p,

//  typedef adjacency_list< vecS, vecS, directedS> Graph;
  Graph g;
  
  // now read the graph
  FILE * pFile;
  pFile = fopen ( "myfile.bin" , "r" );
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
    for (j2 =0; j2< j; j2++)    {
      //      printf("adding edge %d -> %d\n",i2, (int)waynodes[j2]);
      vertex_descriptor from=i;
      vertex_descriptor to = (int)waynodes[j2];
      //      edge_property e;
      boost::add_edge(from,to, g);
    }

  }

  boost::write_graphviz(std::cout, g);

  fclose (pFile);
}
