//==============================================================
//
// SAMPLE SOURCE CODE - SUBJECT TO THE TERMS OF SAMPLE CODE LICENSE AGREEMENT,
// http://software.intel.com/en-us/articles/intel-sample-source-code-license-agreement/
//
// Copyright 2012 Intel Corporation
//
// THIS FILE IS PROVIDED "AS IS" WITH NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, NON-INFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS.
//
// ===============================================================


#include <stdio.h>
#include <stdlib.h>

const int N = 1024;
 
int a[N], b[N], c[N];

void init() {
  
  for (int i = 0; i < N; i++) a[i] = i;
}
 
__declspec(noinline)
__declspec(vector)
int vfun_add_one(int x)
{
  return x+1;
}
 

__declspec(noinline)
int	checksum()
{
  int i;
  int sum = 0;
  for (i = 0; i < N; i++) {
    sum += a[i];
  }
  return sum;
}

 
__declspec(noinline)
void d5() {
  int h,g = 0;
  init();
#pragma simd
  for (h = 0; h < N; h++) {
    a[h] = vfun_add_one(a[h]);
    b[h] = c[h] - a[h];
    c[h] = a[h] - b[h];
  }
  g = checksum();
  printf("SIMD for loop: passed %d\n",g);
}
 
int main(int argc, char *argv[])
{
  int g = 0, h = 0;

  d5();
  return 0;
}
