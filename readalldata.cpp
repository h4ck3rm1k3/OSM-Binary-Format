#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

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

typedef std::map<int, std::vector<int> > component_map_t;

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

  component_map_t::iterator iter;
  for (iter=component_map.begin();iter!=component_map.end();iter++)    {
      cout << "component " << iter->first << endl;
      vector<int>::const_iterator cii;
      vector<int> &data =iter->second;
      
      for(cii=data.begin(); cii!=data.end(); cii++)
        {
          int i=*cii;
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
          cout << "<tag k='component' v='" << node_components[i] << "'/>" ;  
          cout << "</node> " << endl;  
        }
    }
}
