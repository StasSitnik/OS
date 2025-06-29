#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

constexpr int MSG_SIZE = 20;

int main() {
    std::string fileName;
    std::cout << "Enter binary file name: ";
    std::cin >> fileName;

    {
        std::ofstream out(fileName, std::ios::binary | std::ios::trunc);
        char empty[MSG_SIZE] = { 0 };
        out.write(empty, MSG_SIZE);
    }

    HANDLE hMutex = CreateMutex(NULL, FALSE, "FileMutex");
    HANDLE hMsgWritten = CreateEvent(NULL, TRUE, FALSE, "MessageWritten");
    HANDLE hMsgRead = CreateEvent(NULL, TRUE, TRUE, "MessageRead");
    HANDLE hSenderReady = CreateEvent(NULL, TRUE, FALSE, "SenderReady");

    STARTUPINFO si = { sizeof(si) };
    ZeroMemory(&si, sizeof(STARTUPINFO));
    PROCESS_INFORMATION pi;
    std::string cmdLine = "sender.exe " + fileName;
    if (!CreateProcess(NULL, &cmdLine[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cerr << "Failed to launch sender.\n";
        return 1;
    }

    std::cout << "Waiting for sender to be ready...\n";
    WaitForSingleObject(hSenderReady, INFINITE);
    std::cout << "Sender is ready.\n";

    std::string cmd;
    while (true) {
        std::cout << "Enter command (read / exit): ";
        std::cin >> cmd;
        if (cmd == "exit") break;

        if (cmd == "read") {
            WaitForSingleObject(hMsgWritten, INFINITE);
            WaitForSingleObject(hMutex, INFINITE);

            std::ifstream in(fileName, std::ios::binary);
            char buffer[MSG_SIZE + 1] = { 0 };
            in.read(buffer, MSG_SIZE);
            in.close();

            std::cout << "Received message: " << buffer << "\n";

            std::ofstream out(fileName, std::ios::binary | std::ios::trunc);
            char empty[MSG_SIZE] = { 0 };
            out.write(empty, MSG_SIZE);
            out.close();

            ReleaseMutex(hMutex); 
            ResetEvent(hMsgWritten); 
            SetEvent(hMsgRead);      
        }
    }

    TerminateProcess(pi.hProcess, 0);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    CloseHandle(hMutex);
    CloseHandle(hMsgWritten);
    CloseHandle(hMsgRead);
    CloseHandle(hSenderReady);

    return 0;
}