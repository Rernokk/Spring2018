#include <stdio.h>

void main ()
{
  FILE *fp, *ofp;
  char mask[1024];
  fp = fopen("DemonMaskASCII.txt", "w+");
  ofp = fopen("out.txt", "w");
  while (fscanf(fp, "%s", mask) != 1) {
    fprintf(ofp, "%s\n", mask);
  }
  fprintf(fp, "Tada");
  fclose(fp);
  getch();
  return;
}