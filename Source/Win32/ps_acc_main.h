#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//   fxc /T ps_3_0 /O3 /E acc_main /Fhps_acc_main.h crt.ps
//
//
// Parameters:
//
//   sampler2D tex0;
//   sampler2D tex1;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   tex0         s0       1
//   tex1         s1       1
//

    ps_3_0
    def c0, 0.959999979, 0, 0, 0
    dcl_texcoord v0.xy
    dcl_color v1
    dcl_2d s0
    dcl_2d s1
    texld r0, v0, s0
    mul r0, r0, v1
    texld r1, v0, s1
    mul r1, r1, c0.x
    max oC0, r0, r1

// approximately 5 instruction slots used (2 texture, 3 arithmetic)
#endif

const BYTE g_ps30_acc_main[] =
{
      0,   3, 255, 255, 254, 255, 
     44,   0,  67,  84,  65,  66, 
     28,   0,   0,   0, 123,   0, 
      0,   0,   0,   3, 255, 255, 
      2,   0,   0,   0,  28,   0, 
      0,   0,   0, 129,   0,   0, 
    116,   0,   0,   0,  68,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   2,   0,  76,   0, 
      0,   0,   0,   0,   0,   0, 
     92,   0,   0,   0,   3,   0, 
      1,   0,   1,   0,   6,   0, 
    100,   0,   0,   0,   0,   0, 
      0,   0, 116, 101, 120,  48, 
      0, 171, 171, 171,   4,   0, 
     12,   0,   1,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 116, 101, 120,  49, 
      0, 171, 171, 171,   4,   0, 
     12,   0,   1,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 112, 115,  95,  51, 
     95,  48,   0,  77, 105,  99, 
    114, 111, 115, 111, 102, 116, 
     32,  40,  82,  41,  32,  72, 
     76,  83,  76,  32,  83, 104, 
     97, 100, 101, 114,  32,  67, 
    111, 109, 112, 105, 108, 101, 
    114,  32,  57,  46,  50,  57, 
     46,  57,  53,  50,  46,  51, 
     49,  49,  49,   0,  81,   0, 
      0,   5,   0,   0,  15, 160, 
    143, 194, 117,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  31,   0, 
      0,   2,   5,   0,   0, 128, 
      0,   0,   3, 144,  31,   0, 
      0,   2,  10,   0,   0, 128, 
      1,   0,  15, 144,  31,   0, 
      0,   2,   0,   0,   0, 144, 
      0,   8,  15, 160,  31,   0, 
      0,   2,   0,   0,   0, 144, 
      1,   8,  15, 160,  66,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0, 228, 144,   0,   8, 
    228, 160,   5,   0,   0,   3, 
      0,   0,  15, 128,   0,   0, 
    228, 128,   1,   0, 228, 144, 
     66,   0,   0,   3,   1,   0, 
     15, 128,   0,   0, 228, 144, 
      1,   8, 228, 160,   5,   0, 
      0,   3,   1,   0,  15, 128, 
      1,   0, 228, 128,   0,   0, 
      0, 160,  11,   0,   0,   3, 
      0,   8,  15, 128,   0,   0, 
    228, 128,   1,   0, 228, 128, 
    255, 255,   0,   0
};
