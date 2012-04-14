#include <stdio.h>
int main ()
{
  FILE * pFile;
  pFile = fopen ( "components.bin" , "r" );
  if (!pFile)   {
    printf("file could not be opened\n");
    return 2;
  }
  int nodecount;
  int i=0;
  int br=fread ((void*)&nodecount , sizeof(int), 1 , pFile ); // count of ways
  printf("Reading bytes %d nodes %d",br,nodecount);
  int nodecomponents[nodecount];
  br= fread ((void*)&nodecomponents , sizeof(int), nodecount , pFile ); // index
  fclose (pFile);


  if (br != nodecount)
    {
      printf("Reading way wanted %d got %d\n",nodecount,br);
      return 1;
    }

  for (i =0; i < nodecount; i++)    {
    printf("%d -> %d\n",i, (int)nodecomponents[i]);
  }


}

