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


void foo(int *A, int *B, int *restrict C, int n){
  int i;
  int t = 0;

#ifdef PRIVATE
#pragma simd private(t)
#endif
  for (i=0; i<n; i++){
    if (A[i] > 0) {
      t = A[i];
    }
    if (B[i] < 0) {
      t = B[i];
    }
    C[i] = t;
  }
}
