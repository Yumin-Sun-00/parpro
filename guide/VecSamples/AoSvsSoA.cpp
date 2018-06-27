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


#ifdef AOS

typedef struct {
  float red;
  float green;
  float blue;
  float alpha;
} CData32;

void SepiaFilterAoS(float *pImage, float *pResult, unsigned int imageSize)
{
  CData32 *pSrc = (CData32 *)pImage;
  CData32 *pDst = (CData32 *)pResult;
#ifdef __INTEL_COMPILER
__assume_aligned(pSrc, 16);
__assume_aligned(pDst, 16);
#endif
  for(int i=0;i<imageSize;i++)
  {
    pDst[i].red = 0.393f*pSrc[i].red + 0.769f*pSrc[i].green + 0.189f*pSrc[i].blue;
    pDst[i].green = 0.349f*pSrc[i].red + 0.686f*pSrc[i].green + 0.168f*pSrc[i].blue;
    pDst[i].blue = 0.272f*pSrc[i].red + 0.534f*pSrc[i].green + 0.131f*pSrc[i].blue;
    pDst[i].alpha = pSrc[i].alpha;
    if(pDst[i].red > 255) pDst[i].red = 255;
    if(pDst[i].green > 255) pDst[i].green = 255;
    if(pDst[i].blue > 255) pDst[i].blue = 255;
  }
}
#endif // AOS




#ifdef SOA

typedef struct {
  float red[4];
  float green[4];
  float blue[4];
  float alpha[4];
} CData32T4;

const float fIntensity = 0.0234;
extern CData32T4 * GetCurrentSrcPointer32T4(void);


void CrossfadeTransposedFloatKernel()
{
  CData32T4 *pSrc1 = GetCurrentSrcPointer32T4();
  CData32T4 *pSrc2 = GetCurrentSrcPointer32T4();
  CData32T4 *pDst  = GetCurrentSrcPointer32T4();
#ifdef __INTEL_COMPILER
__assume_aligned(pSrc1, 16);
__assume_aligned(pSrc2, 16);
__assume_aligned(pDst, 16);
#endif
#pragma ivdep
  for(int j=0;j<4;j++)
  {
    pDst->red[j] = (1.0f-fIntensity)*pSrc1->red[j] + fIntensity*pSrc2->red[j];
    pDst->green[j] = (1.0f-fIntensity)*pSrc1->green[j] + fIntensity*pSrc2->green[j];
    pDst->blue[j] = (1.0f-fIntensity)*pSrc1->blue[j] + fIntensity*pSrc2->blue[j];
    pDst->alpha[j] = pSrc1->alpha[j];
  }
}
#endif // SOA


