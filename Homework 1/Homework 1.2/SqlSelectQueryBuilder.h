#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <map>

class SqlSelectQueryBuilder {
private:
    std::vector<std::string> columns;    // Список столбцов
    std::string from_table;              // Таблица для SELECT
    std::vector<std::pair<std::string, std::string>> where_conditions; // Условия WHERE

public:
    // Метод для добавления столбца
    SqlSelectQueryBuilder& AddColumn(const std::string& column);

    // Метод для добавления нескольких столбцов
    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& cols) noexcept;

    // Метод для указания таблицы
    SqlSelectQueryBuilder& AddFrom(const std::string& table);

    // Метод для добавления условия WHERE
    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value);

    // Метод для добавления нескольких условий WHERE
    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept;

    // Метод для построения итогового запроса
    std::string BuildQuery() const;
};

