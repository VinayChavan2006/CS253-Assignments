# Library Management System
This assignment has a C++ implementation of a Library Management System, using the Object Oriented Programming Concepts in C++.

### Features
The system has three types of Users : **Students, Faculty, Librarian.**
-   **User Management**: 
    - The system allows Students/Faculty to Login, View books in Library, Borrow Book, Return Book, see their borrow history as well as their overdue books and clear their Fines(if any).
    - It also allows Librarian to add, remove users and also allows to add, remove the books from library and can also update the status of book.

### Assumptions
-   The system assumes that the Students cannot borrow more than 3 books at a time with a fine of 10Rs per day of overdues.
-   The system assumes that the Faculty cannot borrow more than 5 books at a time. No fine for Faculty.
-   The system assumes that each book has a unique ISBN code.
-   The quantity of each book in library is 1 i.e. This system doesnot address for multiple copies of same book.

### Implementation Strategies
-   The system uses File System in C++ and stores the
    -   User Information in ```users.txt```.
    -   Book Information in ```books.txt```.
    -   Account Information in ```accounts.txt```.

-   **Structure of txt files used**
    - ```users.txt```
        - id, name, Role, :in each line
    - ```books.txt```
        - title , author , publisher , to_string(year) , ISBN_code , status , :in each line
    - ```accounts.txt```
        - userId , Fine Amt , Number of Borrowed Books , borrowedBooksISBNs as string joined by '|' , corressponding time stamps of borrowed books joined by '|' , overDue Book ISBNs as string joined by '|' , Number of days of overdue joined by '|' , BorrowHistoryISBNs as string joined by '|' : in each line
        - In summary:
            userId | Fine | No.of borrowed books | BorrowedBookISBNs | Corresponding Timestamps | Overdue bookISBNs | Corresponding Overdue days | BorrowhistoryBooksISBNs 
            --- | --- | --- |--- |--- |--- |--- |---
            S1 | 20 | 3 | ISBN1\|ISBN2\|ISBN3\| | 17623546\|1762354\|17623553\| | ISBN2\|ISBN1\| | 1\|1\| | ISBN6\|ISBN7\|ISBN1\|

**Caution** : Do not add empty lines at the bottom of txt files by simply pressing enter as it reads every line while loading data. 

### Instructions
1.  Ensure that the txt files are in same directory in which the C++ file is present.
2.  Compile and Run the C++ file.
3.  The program loads the data from txt files and initializes with 10 books,5 students,3 faculty and 1 Librarian.
4.  Follow the menu driven system to perform operations.
5.  Enter the choice of operation(choice integer) and follow the prompts.
