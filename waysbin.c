#include <stdio.h>
extern int * waynodes[];
extern int  waycount;
int main ()
{
  FILE * pFile;
  pFile = fopen ( "myfile.bin" , "wb" );
  int i=0;
  fwrite ((void*)&waycount , sizeof(int), 1 , pFile ); // count of ways
  printf("waycount %d\n",waycount);
  
  for (i =0; i < waycount; i++)    {
    int j;
    for (j =0; waynodes[i][j]; j++)    {
      //      printf("%d,",waynodes[i][j]);
    }

    printf("way index %d\t",i);
    fwrite ((void*)&i , sizeof(int), 1 , pFile ); // index

    printf("size %d\n",j);
    fwrite ((void*)&j , sizeof(int), 1 , pFile ); // size
    fwrite ((void*)&waynodes[i][0] , sizeof(int), j , pFile ); // data
  }
  fclose (pFile);
}

