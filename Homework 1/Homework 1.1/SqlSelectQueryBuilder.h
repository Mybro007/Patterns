#pragma once
#include <vector>
#include <string>
#include <sstream>

class SqlSelectQueryBuilder {
private:
    std::vector<std::string> columns;    // ������ ��������
    std::string from_table;              // ������� ��� SELECT
    std::vector<std::pair<std::string, std::string>> where_conditions; // ������� WHERE

public:
    // ����� ��� ���������� �������
    SqlSelectQueryBuilder& AddColumn(const std::string& column);

    // ����� ��� �������� �������
    SqlSelectQueryBuilder& AddFrom(const std::string& table);

    // ����� ��� ���������� ������� WHERE
    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value);

    // ����� ��� ���������� ��������� �������
    std::string BuildQuery() const;
};

