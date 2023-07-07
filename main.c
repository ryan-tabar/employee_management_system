#include <stdio.h>
#include <stdbool.h>
#include <sqlite3.h>

bool executeSQL_query(sqlite3* db, const char* sql) {
    char* err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return false;
    }
    return true;
}

bool createEmployeeSchema(sqlite3* db) {
    const char* sql = "CREATE TABLE IF NOT EXISTS Employees ("
        "EmployeeID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "Name TEXT,"
        "Department TEXT,"
        "Position TEXT,"
        "Salary REAL"
        ");";
    return executeSQL_query(db, sql);
}

bool insertDefaultData(sqlite3* db) {
    const char* sql = "INSERT INTO Employees (Name, Department, Position, Salary)"
        "VALUES ('John Doe', 'IT', 'Manager', 5000.00);";
    return executeSQL_query(db, sql);
}

int main() {
    sqlite3* db;
    
    int rc = sqlite3_open("database.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    printf("Database found/created.\n");

    if (!createEmployeeSchema(db)) {
        return 1;
    }

    if (!insertDefaultData(db)) {
        return 1;
    }
    
    sqlite3_close(db);
    printf("Database closed.\n");
    return 0;
}