#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include "employee.h"

using namespace std;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");
    if (argc != 4) {
        cerr << "Ошибка передачи аргументов." << endl;
        return 1;
    }

    const char* bin_file = argv[1];
    const char* report_file = argv[2];
    double pay = atof(argv[3]);

    ifstream fin(bin_file, ios::binary);
    ofstream fout(report_file);
    if (!fin || !fout) {
        cerr << "Ошибка файла!" << endl;
        return 1;
    }

    fout << "Отчет по файлу \"" << bin_file << "\"\n";
    fout << "Номер\tИмя\tЧасы\tЗарплата\n";

    employee e;
    while (fin.read((char*)&e, sizeof(e))) {
        fout << e.num << '\t' << e.name << '\t'
            << fixed << setprecision(2) << e.hours << '\t'
            << e.hours * pay << '\n';
    }

    fin.close();
    fout.close();
    return 0;
}
