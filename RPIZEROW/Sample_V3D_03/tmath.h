#pragma once
#define SIN_TABLE_MAX  4096
#define SIN_MN_DIV     651.8986469f //((SIN_TABLE_MAX / 2) / PI)
float tsin(float a);
float tcos(float a);

