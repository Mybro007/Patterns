#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

// ��������� �����������
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) = 0;
    virtual void onError(const std::string& message) = 0;
    virtual void onFatalError(const std::string& message) = 0;
};

// ����������� ����� (Logger)
class Logger {
private:
    std::vector<Observer*> observers;

public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void warning(const std::string& message) const {
        for (auto observer : observers) {
            observer->onWarning(message);
        }
    }

    void error(const std::string& message) const {
        for (auto observer : observers) {
            observer->onError(message);
        }
    }

    void fatalError(const std::string& message) const {
        for (auto observer : observers) {
            observer->onFatalError(message);
        }
    }
};

// ����������� ��� �������������� (������� � �������)
class ConsoleWarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }

    void onError(const std::string& message) override {}
    void onFatalError(const std::string& message) override {}
};

// ����������� ��� ������ (���������� � ����)
class FileErrorObserver : public Observer {
private:
    std::ofstream* filePath;

public:
    explicit FileErrorObserver(std::ofstream* path) : filePath(path) {}

    void onWarning(const std::string& message) override {}

    void onError(const std::string& message) override {
        if (filePath->is_open()) {
            *filePath << "Error: " << message << std::endl;
            filePath->close();
        }
        else {
            std::cerr << "Failed to open file for writing!" << std::endl;
        }
    }

    void onFatalError(const std::string& message) override {}
};

// ����������� ��� ��������� ������ (������� � ������� � ���������� � ����)
class ConsoleAndFileFatalErrorObserver : public Observer {
private:
    std::ofstream* filePath;

public:
    explicit ConsoleAndFileFatalErrorObserver(std::ofstream* path) : filePath(path) {}

    void onWarning(const std::string& message) override {}

    void onError(const std::string& message) override {}

    void onFatalError(const std::string& message) override {
        // ������ � �������
        std::cerr << "Fatal Error: " << message << std::endl;

        // ������ � ����
        if (filePath->is_open()) {
            *filePath << "Fatal Error: " << message << std::endl;
            filePath->close();
        }
        else {
            std::cerr << "Failed to open file for writing!" << std::endl;
        }
    }
};

// ������ �������������
int main() {
    Logger logger;

    // ������� ������������
    ConsoleWarningObserver consoleWarningObserver;
    std::ofstream errorsout("errors.txt");
    FileErrorObserver fileErrorObserver(&errorsout);
    std::ofstream fatalerrorsout("fatalerrors.txt");
    ConsoleAndFileFatalErrorObserver fatalErrorObserver(&fatalerrorsout);

    // ��������� ������������ � ������
    logger.addObserver(&consoleWarningObserver);
    logger.addObserver(&fileErrorObserver);
    logger.addObserver(&fatalErrorObserver);

    // ����������� ��������� �������
    logger.warning("This is a warning message.");
    logger.error("This is an error message.");
    logger.fatalError("This is a fatal error message.");

    // ������� �����������, ���� �����
    logger.removeObserver(&fileErrorObserver);
    logger.error("This is another error message.");

    return 0;
}
