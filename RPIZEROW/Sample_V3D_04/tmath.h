#ifndef _TMATH_H_
#define _TMATH_H_

#define SIN_TABLE_MAX       4096
#define SIN_TABLE_MAX_MASK  (SIN_TABLE_MAX - 1)
#define SIN_MN_DIV     651.8986469f //((SIN_TABLE_MAX / 2) / PI)
float tsin(float a);
float tcos(float a);
float ttan(float a);

#endif //_TMATH_H_
