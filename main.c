#include <stdio.h>
#include <stdbool.h>
#include <sqlite3.h>

// Callback function to handle each row of the result set
int callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

bool executeSQL_query(sqlite3* db, const char* sql) {
    char* err_msg = 0;
    int rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    
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
        "VALUES" 
        "('John Doe', 'IT', 'Software Engineer', 30000.00),"
        "('Alice Brown', 'HR', 'Sales Executive', 20000.00),"
        "('Ryan Tabar', 'Customer Service', 'Manager', 35000.00),"
        "('Ryan Drown', 'IT', 'Jr Developer', 30000.00),"
        "('Edward Bell', 'Buisness Process', 'Business Analyst', 30000.00);";
    return executeSQL_query(db, sql);
}

bool searchEmployee(sqlite3* db, const char* name) {
    char sql[100];
    sprintf(sql, "SELECT Name FROM Employees "
                 "WHERE UPPER(Name) LIKE UPPER('%%%s%%');", name);
    return executeSQL_query(db, sql);
}

enum MenuState {
    Main,
    SearchEmployee,
    AddEmployee,
    ModifyEmployee,
    Exit
};

void handleMenuState(enum MenuState* menuState) {
    printf("\n----------Main Menu-----------\n");
    printf("1. Search Employee.\n");
    printf("2. Add Employee.\n");
    printf("\nEnter your choice: ");
    
    int rc = scanf("%d", (int*)menuState);
    if (rc != 1 || *menuState < 0 || *menuState > 5) {
        printf("Invalid input!\n");
        *menuState = Main;
    }
}

void handleSearchEmployeeState(sqlite3* db, enum MenuState* menuState) {
    printf("\n-------Search Employee--------\n");
    // This will print a list of peoples names that match the name inputed to the console
    char name[40];
    printf("\nType a name: ");

    // Clear the input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (scanf("%39[^\n]", name) != 1) {
        printf("Invalid input!\n");
        *menuState = SearchEmployee;
    }
    printf("Searching for %s...\n", name);
    searchEmployee(db, name);
}

int main() {
    sqlite3* db;
    
    int rc = sqlite3_open("database.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Create Employee Schema if it doesn't exist
    if (!createEmployeeSchema(db)) {
        return 1;
    }

    // Flood table with arbitrary data
    if (!insertDefaultData(db)) {
        return 1;
    }

    printf("\nWelcome to the Employee Management System!\n");
    enum MenuState menuState = Main;
    while (menuState != Exit) {
        switch (menuState) {
            case Main:
                handleMenuState(&menuState);
                break;
            case SearchEmployee:
                handleSearchEmployeeState(db, &menuState);
                break;
            case AddEmployee:
                break;
            default:
                menuState = Exit;
        }
    }
    
    sqlite3_close(db);
    return 0;
}