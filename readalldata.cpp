#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
using namespace std;
typedef std::map<int, std::vector<int> > component_map_t;
typedef std::map<int, std::vector<int> > way_nodes_t;
typedef std::map<int, std::vector<int> > nodes_ways_t;
typedef std::map<int, std::map<int,int> > way_component_t; // a way can be a member of many components, we need to figure out how and way

int read_way_nodes(way_nodes_t & waynodemap,nodes_ways_t & nodeways) {
  FILE * pFile;
  pFile = fopen ( "waynodes.bin" , "r" );
  if (!pFile)   {
    printf("file could not be opened\n");
    return 2;
  }
  int waycount;
  int br=fread ((void*)&waycount , sizeof(int), 1 , pFile ); // count of ways

  // loop over all the ways
  int i=0;
  for (i =0; i < waycount; i++)    {

    int i2;
    fread ((void*)&i2 , sizeof(int), 1 , pFile ); // index
    if (i2 != i)   {
        printf("Reading way index %d failed with a different %d\n",i,i2);
        return 1;
      }

    
    int j;
    fread ((void*)&j , sizeof(int), 1 , pFile ); // size

    int waynodes[j];
    fread ((void*)&waynodes, sizeof(int), j , pFile ); // data    

    // now we can process the edges...    
    int j2;// index
    for (j2 =0; j2< j; j2++)    {
      int tonode = (int)waynodes[j2];
      waynodemap[i].push_back(tonode); //the way contains this node
      nodeways[tonode].push_back(i); //the node is in this way
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
  printf("Reading bytes %d count %d",br,count);
  T data[count];
  br= fread ((void*)&data , sizeof(int), count , pFile ); // index
  fclose (pFile);
  for (i =0; i < count; i++)    {
    vdata.push_back((T)data[i]);
  }
  return 0;
}

template <class T> void emit(vector<T> & data) {
  vector<int>::const_iterator cii;
  cout << endl << "Iterator:" << endl;
  for(cii=data.begin(); cii!=data.end(); cii++)
    {
      cout << *cii << endl;
    }
}

void reverse_components(vector<int> & components , component_map_t & out)
{

  int index=0;
  vector<int>::const_iterator cii;
  for(cii=components.begin(); cii!=components.end(); cii++)
    {
      out[*cii].push_back(index);
      index++;
    }
}

void emit_ways_components(way_component_t & way_component)
{
  cout << "output the way components " << endl;
  way_component_t::iterator wcit;
  for (wcit=way_component.begin();wcit!=way_component.end();wcit++)    {
    std::map<int,int>::iterator wcit2;
    for (wcit2=wcit->second.begin();wcit2!=wcit->second.end();wcit2++)    {
      int comp = wcit2->first;
      int count = wcit2->second;
      cout << "way:" << wcit->first << "\tcomponent:" <<  comp << "\tweight:" << count <<  endl;
    }
  }
}

int main()
{
  vector<int> node_components;
  int ret=read_data<int> ("components.bin",node_components);

  vector<double> node_lat;
  ret=read_data<double> ("nodelat.bin",node_lat);

  vector<double> node_lon;
  ret=read_data<double> ("nodelon.bin",node_lon);

  vector<int> node_id;
  ret=read_data<int> ("nodeindex.bin",node_id);

  component_map_t component_map;
  reverse_components(node_components,component_map);

  way_nodes_t  way_nodes;
  nodes_ways_t node_ways;
  read_way_nodes(way_nodes,node_ways);

  way_component_t way_component;

  component_map_t::iterator iter;
  for (iter=component_map.begin();iter!=component_map.end();iter++)    {
    //      cout << "component " << iter->first << endl;
      vector<int>::const_iterator cii;
      vector<int> &data =iter->second;
      
      for(cii=data.begin(); cii!=data.end(); cii++)
        {
          int nodeidx=*cii;
          //          cout << "Looking at node " << nodeidx << endl;

          // now lets build up a list of ways for this node
          // does the way over the node only occur in one component?
          // way -> component 
          nodes_ways_t::iterator node_ways_it;
          node_ways_it=node_ways.find(nodeidx);

          if (node_ways_it!=node_ways.end()) {

            std::vector<int> & wayids=node_ways_it->second;

            std::vector<int>::iterator waysit;
            for (waysit=wayids.begin();waysit != wayids.end();waysit++) {
              int wayid=*waysit;

              //cout << "The node " << nodeidx << " is in way " << wayid << endl;

              int component=node_components[nodeidx];

              // for each way that this node occurs in
              way_component[wayid][component]++;
              //cout << "The node " << nodeidx << " is in way " << wayid << " and component " << component <<endl;
            }// for each way
          }// if there are any ways

      
          /*  
          cout << "<node ";
          cout << "id='"<< node_id[i] << "' ";          
          cout << "lat='";
          cout.precision(3);
          cout.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed      
          cout << node_lat[i];
          cout << "' " ;  
          cout << "lon='";
          cout.precision(3);
          cout.setf(ios::fixed,ios::floatfield);   // floatfield set to fixed      
          cout << node_lon[i] << "' ";  
          cout << ">";
          cout << "<tag k='component' v='" << component << "'/>" ;  
          cout << "</node> " << endl;  
          */
        }
    }

  // -------------


}
