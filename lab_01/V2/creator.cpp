#include <iostream>
#include <fstream>
#include <set>
#include "employee.h"

using namespace std;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");
    if (argc != 3) {
        cerr << "������ �������� ����������." << endl;
        return 1;
    }

    const char* filename = argv[1];
    int count = atoi(argv[2]);

    ofstream fout(filename, ios::binary);
    if (!fout) {
        cerr << "�� ������� ������� ����." << endl;
        return 1;
    }

    set<int> existingNums;  
    for (int i = 0; i < count; ++i) {
        employee e;

        while (true) {
            cout << "������� �����. �����, ���, ���-�� �����:\n";
            cin >> e.num >> e.name >> e.hours;

            if (existingNums.find(e.num) != existingNums.end()) {
                cout << "������!!! ����� " << e.num << " ����������. ������� �����." << endl;
            }
            else {
                existingNums.insert(e.num);  
                break; 
            }
        }

        fout.write((char*)&e, sizeof(e));
    }

    fout.close();
    return 0;
}