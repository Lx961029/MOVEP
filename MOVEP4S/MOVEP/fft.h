#ifndef FFT_H
#define FFT_H
                                                  //定义福利叶变换的点数
#define PI 3.1415926535897932384626433832795028841971               //定义圆周率值
#define NPT 1950
#define RNPT 4096
#define MNPT 2048
//#define fs 1000
struct compx { float real, imag; };                                    //定义一个复数结构

extern void create_sin_tab(float* sin_t, int PointNum);
extern void FFT(struct compx* xin);


#endif // FFT_H

