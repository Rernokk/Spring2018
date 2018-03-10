#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void printArr(int arr[], int n);
int compare(const void *p, const void *q);

int main() {
  int nums[] = { 1,2,3,4,5,6,7,8,9 };
  int size = sizeof(nums) / sizeof(nums[0]);
  qsort((void*)nums, size, sizeof(nums[0]), compare);
  printf("Sorted array is\n");
  printArr(nums, size);
  
  getch();
  return 0;
}

void printArr(int arr[], int n) {
  int i;
  for (int i = 0; i < n; ++i) {
    printf("%d ", arr[i]);
  }
}

int compare(const void *p, const void *q) {
  if (*((int*)p) % 2 != *((int*)q) % 2) {
    return -1;
  }
  if (*((int*)p) % 2 == 0 && *((int*)q) % 2 == 0) {
    return *((int*)q) - *((int*)p);
  }
  if (*((int*)p) % 2 != 0 && *((int*)q) % 2 != 0) {
    return *((int*)p) - *((int*)q);
  }

  return 0;
}