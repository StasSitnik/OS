#include <iostream>
#include <fstream>
#include <windows.h>
#include "employee.h"

using namespace std;

void printBinaryFile(const char* filename) {
    ifstream fin(filename, ios::binary);
    employee e;
    while (fin.read((char*)&e, sizeof(e))) {
        cout << e.num << "\t" << e.name << "\t" << e.hours << endl;
    }
    fin.close();
}

void printTextFile(const char* filename) {
    ifstream fin(filename);
    char ch;
    while (fin.get(ch)) {
        cout << ch;
    }
    fin.close();
}

int main() {
    setlocale(LC_ALL, "rus");

    char binFile[100], reportFile[100];
    int count;
    double pay;

    cout << "¬ведите им€ файла ввода и кол-во работников:\n";
    cin >> binFile >> count;

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    char cmd[200];
    sprintf(cmd, "creator.exe %s %d", binFile, count);
    CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    cout << "\n‘айл содержит:\n";
    printBinaryFile(binFile);

    cout << "\n¬ведите им€ файла вывода и зарплату:\n";
    cin >> reportFile >> pay;

    sprintf(cmd, "reporter.exe %s %s %.2f", binFile, reportFile, pay);
    CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);


    cout << "\nќтчЄт:\n";
    printTextFile(reportFile);

    return 0;
}
