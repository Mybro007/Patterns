#include <iostream>
#include <fstream>
#include <string>
#include <memory>

// Абстрактный базовый класс для команд
class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

// Класс для вывода сообщения в консоль
class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << "Console Log: " << message << std::endl;
    }
};

// Класс для записи сообщения в файл
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

// Функция, которая выполняет команду
void print(LogCommand& command, const std::string& message) {
    command.print(message);
}

int main() {
    std::string logMessage = "This is a test log message.";

    // Пример использования ConsoleLogCommand
    ConsoleLogCommand consoleLog;
    print(consoleLog, logMessage);

    // Пример использования FileLogCommand
    std::ofstream outFile("log.txt");
    FileLogCommand fileLog(&outFile);
    print(fileLog, logMessage);

    return 0;
}