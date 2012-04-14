#include <stdio.h>
int main ()
{
  FILE * pFile;
  pFile = fopen ( "waynodes.bin" , "r" );
  if (!pFile)   {
    printf("file could not be opened\n");
    return 2;
  }
  int  waycount;
  int i=0;
  int br=fread ((void*)&waycount , sizeof(int), 1 , pFile ); // count of ways
  printf("Reading bytes %d ways %d",br,waycount);
  for (i =0; i < waycount; i++)    {
    printf("Reading way index %d\n",i);
    int j;
    int i2;
    fread ((void*)&i2 , sizeof(int), 1 , pFile ); // index
    if (i2 != i)
      {
        printf("Reading way index %d failed with a different %d\n",i,i2);
        return 1;
      }

    fread ((void*)&j , sizeof(int), 1 , pFile ); // size
    printf("Reading count %d\n",j);
    int waynodes[j];
    fread ((void*)&waynodes, sizeof(int), j , pFile ); // data

    printf("way %d :",i);
    int j2=0;
    /*    for (j2 =0; j2< j; j2++)    {
      printf("%d,",(int)waynodes[j2]);
      }*/
    printf("\n");
  }
  fclose (pFile);
}

