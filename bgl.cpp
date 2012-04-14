#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <fstream>
#include <string>
#include <boost/graph/graphviz.hpp>
using namespace boost;
using namespace std;

#include <stdio.h>

// see also : http://www.informit.com/articles/article.aspx?p=673259&seqNum=7

int main (int argc, char* argv[])
{
  //  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS> Graph;
  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::directedS> Graph;
  
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
    vertex_descriptor prev = (int)waynodes[0];
    for (j2 =1; j2< j; j2++)    {
      vertex_descriptor to = (int)waynodes[j2];
      //      edges are from the nodes to each other, not from the way to the edge
      boost::add_edge(prev,to, g);
      prev= to;
    }

  }

  boost::write_graphviz(std::cout, g);

  fclose (pFile);
}
