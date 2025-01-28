#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

// Классы для типов сообщений
enum class Type {
    Warning,
    Error,
    FatalError,
    Unknown
};

// Класс для сообщения
class LogMessage {
private:
    Type messageType;
    std::string messageContent;

public:
    LogMessage(Type type, const std::string& message)
        : messageType(type), messageContent(message) {}

    Type type() const {
        return messageType;
    }

    const std::string& message() const {
        return messageContent;
    }
};

// Абстрактный базовый класс обработчика
class Handler {
protected:
    Handler* nextHandler;

public:
    Handler() : nextHandler(nullptr) {}

    virtual ~Handler() = default;

    void setNext(Handler* handler) {
        nextHandler = handler;
    }

    virtual void handle(const LogMessage& message) = 0;
};

// Конкретный обработчик для фатальных ошибок
class FatalErrorHandler : public Handler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == Type::FatalError) {
            throw std::runtime_error("Fatal Error: " + message.message());
        }
        else if (nextHandler) {
            nextHandler->handle(message);
        }
    }
};

// Конкретный обработчик для ошибок
class ErrorHandler : public Handler {
private:
    std::ofstream* filePath;

public:
    explicit ErrorHandler(std::ofstream* path) : filePath(path) {}

    void handle(const LogMessage& message) override {
        if (message.type() == Type::Error) {
            if (filePath->is_open()) {
                *filePath << "Error: " << message.message() << std::endl;
                filePath->close();
            }
            else {
                std::cerr << "Failed to open file for writing!" << std::endl;
            }
        }
        else if (nextHandler) {
            nextHandler->handle(message);
        }
    }
};

// Конкретный обработчик для предупреждений
class WarningHandler : public Handler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == Type::Warning) {
            std::cout << "Warning: " << message.message() << std::endl;
        }
        else if (nextHandler) {
            nextHandler->handle(message);
        }
    }
};

// Конкретный обработчик для неизвестных сообщений
class UnknownMessageHandler : public Handler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == Type::Unknown) {
            throw std::invalid_argument("Unknown message type: " + message.message());
        }
        else if (nextHandler) {
            nextHandler->handle(message);
        }
    }
};

// Пример использования
int main() {
    // Создаем обработчиков
    FatalErrorHandler fatalErrorHandler;
    std::ofstream outErr("errors.txt");
    ErrorHandler errorHandler(&outErr);
    WarningHandler warningHandler;
    UnknownMessageHandler unknownMessageHandler;

    // Настроим цепочку обработчиков
    warningHandler.setNext(&errorHandler);
    errorHandler.setNext(&fatalErrorHandler);
    fatalErrorHandler.setNext(&unknownMessageHandler);

    // Пример сообщений
    LogMessage warningMessage(Type::Warning, "This is a warning!");
    LogMessage errorMessage(Type::Error, "This is an error!");
    LogMessage fatalMessage(Type::FatalError, "This is a fatal error!");
    LogMessage unknownMessage(Type::Unknown, "This is an unknown message!");

    try {
        // Пример обработки сообщений
        warningHandler.handle(warningMessage);
        warningHandler.handle(errorMessage);
        warningHandler.handle(fatalMessage);
        warningHandler.handle(unknownMessage);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

    return 0;
}
