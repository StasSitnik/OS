#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

constexpr int MSG_SIZE = 20;

bool isValidMessageLength(const std::string& msg) {
    return msg.length() <= MSG_SIZE;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Missing file name.\n";
        return 1;
    }

    std::string fileName = argv[1];

    HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, "FileMutex");
    HANDLE hMsgWritten = OpenEvent(EVENT_MODIFY_STATE, FALSE, "MessageWritten");
    HANDLE hMsgRead = OpenEvent(SYNCHRONIZE, FALSE, "MessageRead");
    HANDLE hSenderReady = OpenEvent(EVENT_MODIFY_STATE, FALSE, "SenderReady");

    SetEvent(hSenderReady);

    std::string cmd;
    while (true) {
        std::cout << "Enter command (send / exit): ";
        std::cin >> cmd;
        if (cmd == "exit") break;

        if (cmd == "send") {
            std::cin.ignore();
            std::string msg;
            std::cout << "Enter message (<20 chars): ";
            std::getline(std::cin, msg);

            if (!isValidMessageLength(msg)) {
                std::cerr << "Message too long! Max length is " << MSG_SIZE << " characters.\n";
                continue;
            }

            WaitForSingleObject(hMsgRead, INFINITE);
            WaitForSingleObject(hMutex, INFINITE);

            std::ofstream out(fileName, std::ios::binary | std::ios::trunc);
            char buffer[MSG_SIZE] = { 0 };
            memcpy(buffer, msg.c_str(), msg.size());
            out.write(buffer, MSG_SIZE);
            out.close();

            ReleaseMutex(hMutex);
            ResetEvent(hMsgRead);
            SetEvent(hMsgWritten);
        }
    }

    return 0;
}