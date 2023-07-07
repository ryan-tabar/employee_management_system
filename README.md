# Employee Management System (C and SQLite3)

This is an Employee Management System implemented in C programming language using SQLite3 as the underlying database. The system allows you to store, retrieve, update, and delete employee information.

## Setup

1. Clone the repository or download the source code.

2. Use the provided Makefile to compile the code:

   ```bash
   make
   ```

3. Run the compiled executable:

   ```bash
   ./program
   ```

4. The program will create a SQLite database file named "employees.db" if it doesn't already exist. It will also create the necessary table schema in the database.

## Usage

The program provides a command-line interface (CLI) for interacting with the Employee Management System. Here are the available commands:

- **Add Employee**: Add a new employee to the system.
- **List Employees**: Retrieve and display a list of all employees.
- **Search Employee**: Search for an employee by ID or name.
- **Update Employee**: Update the details of an existing employee.
- **Delete Employee**: Remove an employee from the system.

Follow the on-screen prompts and provide the required information to perform the respective actions.

## Database Structure

The Employee Management System uses a single table named "Employees" to store employee information. The table schema includes the following columns:

- **EmployeeID**: Integer (Primary Key)
- **Name**: Text
- **Department**: Text
- **Position**: Text
- **Salary**: Real

Feel free to customize and modify the system according to your requirements. If you encounter any issues or have suggestions for improvement, please submit them in the issue tracker or contribute to the project by creating pull requests.

Enjoy managing your employees efficiently with this Employee Management System!

---
Feel free to further customize the README.md file according to your specific implementation details or any other relevant information for users.