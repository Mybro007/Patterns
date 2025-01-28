#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

// Интерфейс Наблюдателя
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) = 0;
    virtual void onError(const std::string& message) = 0;
    virtual void onFatalError(const std::string& message) = 0;
};

// Наблюдаемый класс (Logger)
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

// Наблюдатель для предупреждений (выводит в консоль)
class ConsoleWarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }

    void onError(const std::string& message) override {}
    void onFatalError(const std::string& message) override {}
};

// Наблюдатель для ошибок (записывает в файл)
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

// Наблюдатель для фатальных ошибок (выводит в консоль и записывает в файл)
class ConsoleAndFileFatalErrorObserver : public Observer {
private:
    std::ofstream* filePath;

public:
    explicit ConsoleAndFileFatalErrorObserver(std::ofstream* path) : filePath(path) {}

    void onWarning(const std::string& message) override {}

    void onError(const std::string& message) override {}

    void onFatalError(const std::string& message) override {
        // Печать в консоль
        std::cerr << "Fatal Error: " << message << std::endl;

        // Запись в файл
        if (filePath->is_open()) {
            *filePath << "Fatal Error: " << message << std::endl;
            filePath->close();
        }
        else {
            std::cerr << "Failed to open file for writing!" << std::endl;
        }
    }
};

// Пример использования
int main() {
    Logger logger;

    // Создаем наблюдателей
    ConsoleWarningObserver consoleWarningObserver;
    std::ofstream errorsout("errors.txt");
    FileErrorObserver fileErrorObserver(&errorsout);
    std::ofstream fatalerrorsout("fatalerrors.txt");
    ConsoleAndFileFatalErrorObserver fatalErrorObserver(&fatalerrorsout);

    // Добавляем наблюдателей в логгер
    logger.addObserver(&consoleWarningObserver);
    logger.addObserver(&fileErrorObserver);
    logger.addObserver(&fatalErrorObserver);

    // Логирование различных событий
    logger.warning("This is a warning message.");
    logger.error("This is an error message.");
    logger.fatalError("This is a fatal error message.");

    // Убираем наблюдателя, если нужно
    logger.removeObserver(&fileErrorObserver);
    logger.error("This is another error message.");

    return 0;
}
