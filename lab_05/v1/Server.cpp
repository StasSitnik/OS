#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include "employ.h"

using namespace std;

struct LockInfo {
    int readers = 0;
    bool writer = false;
};

map<int, LockInfo> locks;
mutex lockMutex;
vector<employee> records;
string filename;

bool loadEmployee(int id, employee& e) {
    for (const auto& emp : records) {
        if (emp.num == id) {
            e = emp;
            return true;
        }
    }
    return false;
}

bool updateEmployee(const employee& updated) {
    for (auto& emp : records) {
        if (emp.num == updated.num) {
            emp = updated;
            return true;
        }
    }
    return false;
}

void saveFile() {
    ofstream out(filename, ios::binary | ios::trunc);
    for (const auto& emp : records) {
        out.write((char*)&emp, sizeof(emp));
    }
    out.close();
}

void printFile() {
    cout << "\n--- FILE CONTENT ---\n";
    for (const auto& emp : records) {
        cout << "ID: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << '\n';
    }
    cout << "--------------------\n";
}

void handleClient(int index) {
    string pipeName = "\\\\.\\pipe\\EmployeePipe" + to_string(index);
    HANDLE hPipe = CreateNamedPipeA(
        pipeName.c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1, 0, 0, 0, NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        cerr << "Error creating pipe: " << pipeName << "\n";
        return;
    }

    ConnectNamedPipe(hPipe, NULL);
    DWORD bytesRead, bytesWritten;

    while (true) {
        char command[10] = {};
        if (!ReadFile(hPipe, command, sizeof(command), &bytesRead, NULL)) break;

        string cmd(command);
        if (cmd == "exit") break;

        int id;
        if (!ReadFile(hPipe, &id, sizeof(id), &bytesRead, NULL)) break;

        lockMutex.lock();
        auto& lock = locks[id];

        if (cmd == "read") {
            if (lock.writer) {
                lockMutex.unlock();
                employee e = { -1, "", 0 };
                WriteFile(hPipe, &e, sizeof(e), &bytesWritten, NULL);
                continue;
            }

            lock.readers++;
            lockMutex.unlock();

            employee e;
            if (!loadEmployee(id, e)) e.num = -1;

            WriteFile(hPipe, &e, sizeof(e), &bytesWritten, NULL);

            char tmp[10];
            ReadFile(hPipe, tmp, sizeof(tmp), &bytesRead, NULL);

            lockMutex.lock();
            lock.readers--;
            lockMutex.unlock();
        }

        else if (cmd == "write") {
            if (lock.writer || lock.readers > 0) {
                lockMutex.unlock();
                employee e = { -1, "", 0 };
                WriteFile(hPipe, &e, sizeof(e), &bytesWritten, NULL);
                continue;
            }

            lock.writer = true;
            lockMutex.unlock();

            employee e;
            if (!loadEmployee(id, e)) e.num = -1;

            WriteFile(hPipe, &e, sizeof(e), &bytesWritten, NULL);

            ReadFile(hPipe, &e, sizeof(e), &bytesRead, NULL);
            updateEmployee(e);

            char tmp[10];
            ReadFile(hPipe, tmp, sizeof(tmp), &bytesRead, NULL);

            lockMutex.lock();
            lock.writer = false;
            lockMutex.unlock();
        }
    }

    CloseHandle(hPipe);
}

int main() {
    cout << "Enter binary filename: ";
    cin >> filename;

    int n;
    cout << "Enter number of records: ";
    cin >> n;

    set<int> existingIDs;
    for (int i = 0; i < n; ++i) {
        employee e;
        while (true) {
            cout << "ID (positive integer): ";
            cin >> e.num;
            if (cin.fail() || e.num <= 0 || !isUniqueID(existingIDs, e.num)) {
                cin.clear(); cin.ignore(100000000, '\n');
                cout << "Invalid or duplicate ID. Try again.\n";
                continue;
            }
            existingIDs.insert(e.num);
            break;
        }

        while (true) {
            cout << "Name (max 100 chars): ";
            cin >> e.name;
            if (strlen(e.name) == 0 || strlen(e.name) >= 101) {
                cout << "Invalid name. Try again.\n";
            }
            else break;
        }

        while (true) {
            cout << "Hours (>= 0): ";
            cin >> e.hours;
            if (cin.fail() || e.hours < 0) {
                cin.clear(); cin.ignore(100000000, '\n');
                cout << "Invalid hours. Try again.\n";
            }
            else break;
        }

        records.push_back(e);
    }

    saveFile();
    printFile();

    int clientCount;
    cout << "Enter number of clients: ";
    cin >> clientCount;

    vector<thread> threads;

    for (int i = 0; i < clientCount; ++i) {
        threads.emplace_back(handleClient, i);

        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        string cmd = "Client.exe " + to_string(i);
        CreateProcessA(NULL, cmd.data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    for (auto& t : threads) t.join();

    saveFile();
    printFile();

    string exitCommand;
    cout << "Enter 'exit' to quit: ";
    while (cin >> exitCommand) {
        if (exitCommand == "exit") break;
    }

    return 0;
}
