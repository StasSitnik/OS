#ifndef FUNCTION_H
#define FUNCTION_H
#include<Windows.h>

extern volatile double minInMassiv;
extern volatile double maxInMassiv;
extern volatile double averageInMassiv;
extern volatile int n;

DWORD WINAPI min_max(LPVOID param);
DWORD WINAPI average(LPVOID param);

#endif 