#include "SqlSelectQueryBuilder.h"

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddColumn(const std::string& column) {
    columns.push_back(column);
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddFrom(const std::string& table) {
    from_table = table;
    return *this;
}

SqlSelectQueryBuilder& SqlSelectQueryBuilder::AddWhere(const std::string& column, const std::string& value) {
    where_conditions.push_back({ column, value });
    return *this;
}

std::string SqlSelectQueryBuilder::BuildQuery() const {
    std::ostringstream query;

    // SELECT часть
    query << "SELECT ";
    if (columns.empty()) {
        query << "*";
    }
    else {
        for (size_t i = 0; i < columns.size(); ++i) {
            query << columns[i];
            if (i < columns.size() - 1) {
                query << ", ";
            }
        }
    }

    // FROM часть
    query << " FROM " << from_table;

    // WHERE часть
    if (!where_conditions.empty()) {
        query << " WHERE";
        for (size_t i = 0; i < where_conditions.size(); ++i) {
            query << " " << where_conditions[i].first << "=" << where_conditions[i].second;
            if (i < where_conditions.size() - 1) {
                query << " AND";
            }
        }
    }

    // «авершающа€ точка с зап€той
    query << ";";
    return query.str();
}