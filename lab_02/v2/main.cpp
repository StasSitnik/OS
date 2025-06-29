#include "function.h"
#include <iostream>


using namespace std;

volatile double minInMassiv = 1.7976931348623158e+308;
volatile double maxInMassiv = -1.7976931348623158e+308;
volatile double averageInMassiv = 0;
volatile int n = 0;


int main() {
    setlocale(LC_ALL, "rus");
    HANDLE MMThread;
    HANDLE AThread;

    DWORD IDThreadMM;
    DWORD IDThreadA;

    int term;

    cout << "¬ведите кол-во элементов в массиве" << endl;
    cin >> term;
    n = term;
    if (n <= 0) {
        cout << "ќшибка ввода: n <= 0" << endl;
        return 1;
    }
    cout << "¬ведите элементы массива" << endl;

    double* mas = new double[n];

    for (int i = 0; i < n; i++) {
        cin >> mas[i];
    }

    MMThread = CreateThread(NULL, 0, min_max, mas, 0, &IDThreadMM);
    if (MMThread == NULL)
        return GetLastError();
    WaitForSingleObject(MMThread, INFINITE);
    CloseHandle(MMThread);

    AThread = CreateThread(NULL, 0, average, mas, 0, &IDThreadA);
    if (AThread == NULL)
        return GetLastError();
    WaitForSingleObject(AThread, INFINITE);
    CloseHandle(AThread);

    cout << "»сходный массив: ";
    for (int i = 0; i < n; i++) {
        cout << mas[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < n; i++) {
        if (mas[i] == minInMassiv || mas[i] == maxInMassiv) {
            mas[i] = static_cast<double>(averageInMassiv);
        }
    }

    cout << "»тоговый массив: ";
    for (int i = 0; i < n; i++) {
        cout << mas[i] << " ";
    }
    cout << endl;

    delete[] mas;
    return 0;
}