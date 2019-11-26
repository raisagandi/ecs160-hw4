// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
void merge(int arr[], char* name_list[], int l, int m, int r)
{
int i, j, k;
int n1 = m - l + 1;
int n2 = r - m;
int L[n1], R[n2];
char* L1[n1], R1[n2];
for (i = 0; i < n1; i++){
    L[i] = arr[l + i];
    L1[i] = name_list[l + i];   
}

for (j = 0; j < n2; j++){
    R[j] = arr[m + 1+ j];
    R1[j] = name_list[m + 1+ j];
}

i = 0;
j = 0;
k = l;
while (i < n1 && j < n2)
{
if (L[i] <= R[j])
{
arr[k] = L[i];
name_list[k] = L1[i];
i++;
}
else
{
arr[k] = R[j];
name_list[k] = R1[j];
j++;
}
k++;
}
while (i < n1)
{
arr[k] = L[i];
name_list[k] = L1[i];
i++;
k++;
}
while (j < n2)
{
arr[k] = R[j];
name_list[k] = R1[j];
j++;
k++;
}
}

void mergeSort(int arr[],char* list_name, int l, int r)
{
if (l < r)
{
int m = l+(r-l)/2;
mergeSort(arr, list_name, l, m);
mergeSort(arr,list_name, m+1, r);
merge(arr, list_name, l, m, r);
}
}
