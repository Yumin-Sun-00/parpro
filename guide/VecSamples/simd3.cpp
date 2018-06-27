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


char foo(char *A, int n){

  int i;
  char x = 0;
  
#ifdef SIMD
#pragma simd   // Generates incorrect code
#endif
#ifdef REDUCTION
#pragma simd reduction(+:x)  // Generates correct code 
#endif
#ifdef IVDEP
#pragma ivdep
#endif
  for (i=0; i<n; i++){
    x = x + A[i];
  }
  return x;
}
