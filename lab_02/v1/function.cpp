#include "function.h"
#include <iostream>
using namespace std;
DWORD WINAPI min_max(LPVOID param) {
    double* massiv = static_cast<double*>(param);
    for (int i = 0; i < n; i++) {
        if (massiv[i] > maxInMassiv) {
            maxInMassiv = massiv[i];
        }
        Sleep(7);
        if (massiv[i] < minInMassiv) {
            minInMassiv = massiv[i];
        }
        Sleep(7);
    }
    cout << "����������� ������� ������� " << minInMassiv << endl;
    cout << "������������ ������� ������� " << maxInMassiv << endl;
    return 0;
}

DWORD WINAPI average(LPVOID param) {
    double* massiv = static_cast<double*>(param);
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += massiv[i];
        Sleep(12);
    }
    averageInMassiv = sum / n;
    cout << "������� �������� ��������� ������� " << averageInMassiv << endl;
    return 0;
}