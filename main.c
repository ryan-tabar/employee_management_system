#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sqlite3.h>

// Callback function to handle each row of the result set
int callback(void* data, int argc, char** argv, char** azColName) {
    (void)data; // Cast the unused parameter to void to avoid the warning
    printf("\n");
    for (int i = 0; i < argc; i++) {
        printf("%s: %s, ", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}

bool executeSQL_query(sqlite3* db, int (*callback)(void *, int, char **, char **), const char* sql) {
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
    return executeSQL_query(db, 0, sql);
}

bool insertDefaultData(sqlite3* db) {
    const char* sql = "INSERT INTO Employees (Name, Department, Position, Salary)"
        "VALUES" 
        "('John Doe', 'IT', 'Software Engineer', 30000.00),"
        "('Alice Brown', 'HR', 'Sales Executive', 20000.00),"
        "('Ryan Tabar', 'Customer Service', 'Manager', 35000.00),"
        "('Ryan Drown', 'IT', 'Jr Developer', 30000.00),"
        "('Edward Bell', 'Buisness Process', 'Business Analyst', 30000.00);";
    return executeSQL_query(db, 0, sql);
}

bool searchEmployee(sqlite3* db, const char* name, const char* attributes) {
    char sql[100];
    sprintf(sql, "SELECT %s FROM Employees "
                 "WHERE UPPER(Name) LIKE UPPER('%%%s%%');", attributes, name);
    return executeSQL_query(db, callback, sql);
}

bool searchEmployeeById(sqlite3* db, int id, const char* attributes) {
    char sql[100];
    sprintf(sql, "SELECT %s FROM Employees "
                 "WHERE EmployeeId = %d;", attributes, id);
    return executeSQL_query(db, callback, sql);
}

enum MenuState {
    Main,
    SearchEmployee,
    AddEmployee,
    ModifyEmployee,
    Exit
};

void handleMenuState(enum MenuState* menuState) {
    printf("\n\n----------Main Menu-----------");
    printf("\n\n1. Search Employee.");
    printf("\n2. Add Employee.");

    printf("\n\nEnter your choice: ");
    int rc = scanf("%d", (int*)menuState);
    if (rc != 1 || *menuState < 0 || *menuState > 5) {
        printf("Invalid input!");
        *menuState = Main;
    }
}

void handleSearchEmployeeState(sqlite3* db, enum MenuState* menuState) {
    printf("\n\n-------Search Employee--------");
    // This will print a list of peoples names that match the name inputed to the console
    char name[40];
    printf("\n\nType a name: ");

    if (scanf("%39[^\n]", name) != 1) {
        printf("Invalid input!");
        *menuState = SearchEmployee;
    }
    if (strcmp(name, "main") == 0 || strcmp(name, "Main") == 0) {
        *menuState = Main;
        return;
    }

    printf("\nResults:");
    searchEmployee(db, name, "EmployeeID, Name");

    int id;
    printf("\n\nChoose an employee to view using their id: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input!");
        *menuState = SearchEmployee;
        return;
    }
    searchEmployeeById(db, id, "*");

}

int main() {
    sqlite3* db;
    
    int rc = sqlite3_open("employees.db", &db);
    
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

    printf("\n\nWelcome to the Employee Management System!");
    printf("\nAt any point: type 'main' or 'Main' to return the main menu.");
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

        // Clear the input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    sqlite3_close(db);
    return 0;
}