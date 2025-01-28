#include <iostream>
#include <fstream>
#include <string>
#include <memory>

// ����������� ������� ����� ��� ������
class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

// ����� ��� ������ ��������� � �������
class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << "Console Log: " << message << std::endl;
    }
};

// ����� ��� ������ ��������� � ����
class FileLogCommand : public LogCommand {
private:
    std::ofstream* filePath;
public:
    explicit FileLogCommand(std::ofstream* path) : filePath(path) {}

    void print(const std::string& message) override {
        if (filePath->is_open()) {
            *filePath << "File Log: " << message << std::endl;
            filePath->close();
        }
        else {
            std::cerr << "Failed to open file for writing!" << std::endl;
        }
    }
};

// �������, ������� ��������� �������
void print(LogCommand& command, const std::string& message) {
    command.print(message);
}

int main() {
    std::string logMessage = "This is a test log message.";

    // ������ ������������� ConsoleLogCommand
    ConsoleLogCommand consoleLog;
    print(consoleLog, logMessage);

    // ������ ������������� FileLogCommand
    std::ofstream outFile("log.txt");
    FileLogCommand fileLog(&outFile);
    print(fileLog, logMessage);

    return 0;
}