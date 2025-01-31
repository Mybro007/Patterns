#include <fstream>
#include <string>

// ��������� ��� ������ � HTML
class IHTMLPrintable
{
public:
    virtual ~IHTMLPrintable() = default;
    virtual std::string printAsHTML() const = 0;
};

// ��������� ��� ������ � Text
class ITextPrintable
{
public:
    virtual ~ITextPrintable() = default;
    virtual std::string printAsText() const = 0;
};

// ��������� ��� ������ � JSON
class IJSONPrintable
{
public:
    virtual ~IJSONPrintable() = default;
    virtual std::string printAsJSON() const = 0;
};

// ����� ��� ������ � ������� ������
class TextData : public ITextPrintable
{
public:
    explicit TextData(std::string data) : data_(std::move(data)) {}

    std::string printAsText() const override
    {
        return data_;
    }

private:
    std::string data_;
};

// ����� ��� ������ � ������� HTML
class HTMLData : public IHTMLPrintable
{
public:
    explicit HTMLData(std::string data) : data_(std::move(data)) {}

    std::string printAsHTML() const override
    {
        return "<html>" + data_ + "</html>";
    }

private:
    std::string data_;
};

// ����� ��� ������ � ������� JSON
class JSONData : public IJSONPrintable
{
public:
    explicit JSONData(std::string data) : data_(std::move(data)) {}

    std::string printAsJSON() const override
    {
        return "{ \"data\": \"" + data_ + "\" }";
    }

private:
    std::string data_;
};

// ����� ��� ���������� ������ � ����
class DataSaver
{
public:
    static void saveTo(std::ofstream& file, const ITextPrintable& printable)
    {
        file << printable.printAsText();
    }

    static void saveTo(std::ofstream& file, const IHTMLPrintable& printable)
    {
        file << printable.printAsHTML();
    }

    static void saveTo(std::ofstream& file, const IJSONPrintable& printable)
    {
        file << printable.printAsJSON();
    }
};

// ��������������� ������� ��� �������� ������
void saveToAsHTML(std::ofstream& file, const IHTMLPrintable& printable)
{
    DataSaver::saveTo(file, printable);
}

void saveToAsText(std::ofstream& file, const ITextPrintable& printable)
{
    DataSaver::saveTo(file, printable);
}

void saveToAsJSON(std::ofstream& file, const IJSONPrintable& printable)
{
    DataSaver::saveTo(file, printable);
}


int main()
{
    TextData t("Smth data");
    std::ofstream fin("data.txt");
    saveToAsText(fin, t);
    return 0;
}