#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include "employee.h"

void readBinaryFile(const std::string& filename, std::vector<employee>& employees) {
    std::ifstream fin(filename, std::ios::binary);
    employee e;
    while (fin.read(reinterpret_cast<char*>(&e), sizeof(e))) {
        employees.push_back(e);
    }
    fin.close();
}

std::vector<std::string> readTextFile(const std::string& filename) {
    std::ifstream fin(filename);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    fin.close();
    return lines;
}

TEST_CASE("GeneratesBinaryFile") {
    const char* command = "creator.exe test.bin 2";
    int result = std::system(command);
    REQUIRE(result == 0); 

    std::ifstream file("test.bin");
    REQUIRE(file.good());
    file.close();
}

TEST_CASE("ValidatesEmployeeData") {
    const char* command = "creator.exe test.bin 2";
    std::system(command);

    std::vector<employee> employees;
    readBinaryFile("test.bin", employees);

    REQUIRE(employees.size() == 2); 

    std::set<int> ids;
    for (const auto& e : employees) {
        REQUIRE(ids.insert(e.num).second); 
    }
}

TEST_CASE("GeneratesReportFile") {
    const char* command = "reporter.exe test.bin report.txt 100.0"; 
    int result = std::system(command);
    REQUIRE(result == 0); 

    std::ifstream file("report.txt");
    REQUIRE(file.good());
    file.close();
}

TEST_CASE("ValidatesReportContents") {
    const char* command = "reporter.exe test.bin report.txt 100.0";
    std::system(command);

    std::vector<std::string> reportLines = readTextFile("report.txt");

    REQUIRE(reportLines.size() >= 2); 
    REQUIRE(reportLines[0] == "Отчет по файлу \"test.bin\"");
    REQUIRE(reportLines[1] == "Номер\tИмя\tЧасы\tЗарплата");

    for (size_t i = 2; i < reportLines.size(); ++i) {
        REQUIRE(!reportLines[i].empty()); 
    }
}