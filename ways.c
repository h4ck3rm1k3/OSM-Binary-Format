#include <stdio.h>
extern int * waynodes[];
extern int  waycount;
int main ()
{
  int i=0;
  for (i =0; i < waycount; i++)    {
    printf("way %d :",i);
    int j;
    for (j =0; waynodes[i][j]; j++)    {
      printf("%d,",waynodes[i][j]);
    }
    printf("\n");
  }
}
