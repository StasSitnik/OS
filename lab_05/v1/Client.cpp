#include <windows.h>
#include <iostream>
#include <string>
#include "employ.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Expected pipe number as argument\n";
        return 1;
    }

    string pipeName = "\\\\.\\pipe\\EmployeePipe" + string(argv[1]);

    HANDLE hPipe = CreateFileA(pipeName.c_str(),
        GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        cerr << "Could not connect to pipe.\n";
        return 1;
    }

    DWORD bytesWritten, bytesRead;

    while (true) {
        string command;
        cout << "Enter command (read/write/exit): ";
        getline(cin, command);

        if (command != "read" && command != "write" && command != "exit") {
            cout << "Invalid command. Please enter 'read', 'write', or 'exit'.\n";
            continue;
        }

        WriteFile(hPipe, command.c_str(), command.size() + 1, &bytesWritten, NULL);

        if (command == "exit") break;

        int id;
        while (true) {
            cout << "Enter ID (positive integer): ";
            cin >> id;

            if (cin.fail() || id <= 0) {
                cin.clear();
                cin.ignore(1000000000, '\n');
                cout << "Invalid input. Please enter a valid positive number.\n";
            }
            else {
                cin.ignore(1000000000, '\n');
                break;
            }
        }

        WriteFile(hPipe, &id, sizeof(id), &bytesWritten, NULL);

        if (command == "read") {
            employee e;
            ReadFile(hPipe, &e, sizeof(e), &bytesRead, NULL);

            if (e.num == -1) {
                cout << "Record not found or locked.\n";
                continue;
            }

            cout << "ID: " << e.num << ", Name: " << e.name << ", Hours: " << e.hours << "\n";
            cout << "Press Enter to finish reading...\n";
            cin.get();

            string endCmd = "end";
            WriteFile(hPipe, endCmd.c_str(), endCmd.size() + 1, &bytesWritten, NULL);
        }

        else if (command == "write") {
            employee e;
            ReadFile(hPipe, &e, sizeof(e), &bytesRead, NULL);

            if (e.num == -1) {
                cout << "Record not found or locked.\n";
                continue;
            }

            cout << "Current Name: " << e.name << ", Hours: " << e.hours << "\n";

            cout << "New Name (max 100 chars): ";
            while (true) {
                cin >> e.name;
                if (strlen(e.name) == 0 || strlen(e.name) >= 101) {
                    cout << "Invalid name. Try again: ";
                }
                else break;
            }

            while (true) {
                cout << "New Hours (>= 0): ";
                cin >> e.hours;

                if (cin.fail() || e.hours < 0) {
                    cin.clear();
                    cin.ignore(1000000000, '\n');
                    cout << "Invalid input. Please enter a non-negative number.\n";
                }
                else {
                    cin.ignore(1000000000, '\n');
                    break;
                }
            }

            WriteFile(hPipe, &e, sizeof(e), &bytesWritten, NULL);

            cout << "Press Enter to finish writing...\n";
            cin.get();

            string endCmd = "end";
            WriteFile(hPipe, endCmd.c_str(), endCmd.size() + 1, &bytesWritten, NULL);
        }
    }

    CloseHandle(hPipe);
    return 0;
}
