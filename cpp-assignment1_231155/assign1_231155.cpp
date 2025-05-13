#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <algorithm>

using namespace std;

class Book
{
    // title, author, publisher, year, and ISBN.
    // Status: Tracks whether the book is available, borrowed, or reserved.
public:
    string title;
    string author;
    string publisher;
    int year;
    string ISBN_code;
    string status;

    Book(string title, string author, string publisher, int year, string ISBN)
    {
        this->title = title;
        this->author = author;
        this->publisher = publisher;
        this->year = year;
        this->ISBN_code = ISBN;
        this->status = "Available";
    }

    string BookDataString()
    {
        return ISBN_code + "," + title + "," + author + "," + publisher + "," + (status);
    }

    void display() const
    {
        cout << "Title: " << title << ", Author: " << author
             << ", ISBN: " << ISBN_code << ", Status: "
             << (status)
             << endl;
    }

    string getTitle()
    {
        return title;
    }

    string getISBN()
    {
        return ISBN_code;
    }

    string getStatus()
    {
        return status;
    }
    void setStatus(string s)
    {
        status = s;
        ifstream bookread("books.txt");

        string line;
        vector<string> lines;
        while (getline(bookread, line))
        {
            stringstream ss(line);
            string t, a, p, y, i, st;
            getline(ss, t, ',');
            getline(ss, a, ',');
            getline(ss, p, ',');
            getline(ss, y, ',');
            getline(ss, i, ',');
            getline(ss, st, ',');
            if (i == this->getISBN())
            {
                st = s;
                lines.push_back(t + "," + a + "," + p + "," + y + "," + i + "," + s + ",");
            }
            else
            {
                lines.push_back(line);
            }
        }
        bookread.close();

        ofstream bookwrite("books.txt");
        for (const auto &line : lines)
        {
            bookwrite << line << endl;
        }
        bookwrite.close();
    }
    string serialize() const
    {
        return title + "," + author + "," + publisher + "," + to_string(year) + "," + ISBN_code + "," + (status) + ",";
    }

    static Book deserialize(string line)
    {
        stringstream ss(line);
        string t, a, p, y, i, s;
        getline(ss, t, ',');
        getline(ss, a, ',');
        getline(ss, p, ',');
        getline(ss, y, ',');
        getline(ss, i, ',');
        getline(ss, s, ',');
        Book b(t, a, p, stoi(y), i);
        b.setStatus(s);
        return b;
    }
    //  Constraints:
    // Start with at least 5 books in the system.
    // Books can only be borrowed if their status is ”Available.”
};

class Account
{
private:
    string userId;
    vector<pair<Book *, time_t>> borrowedBooks;
    vector<string> borrowHistory;
    vector<pair<Book *, time_t>> overdueBooks;
    int fine;

public:
    Account(string id)
    {
        fine = 0;
        userId = id;
    }
    string getUserId()
    {
        return userId;
    }
    void showBorrowHistory(vector<Book> libraryBooks)
    {
        if (borrowHistory.size() == 0)
        {
            cout << "No borrow history." << endl;
            return;
        }
        cout << "** Your Borrow History ** \n\n"; 
        for (const auto &line : borrowHistory)
        {
            for (auto &book : libraryBooks)
            {
                if (book.getISBN() == line)
                {
                    cout << book.getISBN() << " " << book.getTitle() << " " << book.author << endl;
                }
            }
        }
    }
    void showOverdueBooks(int maxDays)
    {
        int flag = 0;
        time_t now = time(0);
        for (const auto &book : borrowedBooks)
        {
            if ((difftime(now, book.second) / (24 * 3600)) > maxDays)
            {
                flag = 1;
                auto it = find_if(overdueBooks.begin(), overdueBooks.end(), [&](const pair<Book *, time_t> &p)
                                  { return p.first->getISBN() == book.first->getISBN(); });
                if (it != overdueBooks.end())
                {
                    // If the book is already in the overdueBooks vector, update the time
                    it->second = difftime(now, book.second) / (24 * 3600);
                }
                else
                {
                    // If the book is not in the overdueBooks vector, add it
                    overdueBooks.push_back(make_pair(book.first, difftime(now, book.second) / (24 * 3600)));
                }
            }
        }
        if (flag == 0)
        {
            cout << "No overdue books." << endl;
            return;
        }
        vector<string> overDueBooksISBN;
        vector<string> overDueBookTimes;

        ifstream accountread("accounts.txt");

        string line;
        vector<string> lines;
        while (getline(accountread, line))
        {
            stringstream ss(line);
            string id, f, size, isbn, timestamp, overDueBooksStr, overDueTimeStr;
            getline(ss, id, ',');
            getline(ss, f, ',');
            getline(ss, size, ',');
            getline(ss, isbn, ',');
            getline(ss, timestamp, ',');
            getline(ss, overDueBooksStr, ',');
            getline(ss, overDueTimeStr, ',');
            if (id == userId)
            {
                for (const auto &book : overdueBooks)
                {
                    size_t pos = overDueBooksStr.find(book.first->getISBN());
                    if (pos != string::npos)
                    {
                        // Find the end position of the ISBN
                        size_t endPos = overDueBooksStr.find('|', pos);
                        if (endPos != string::npos)
                        {
                            endPos++; // Include the '|' character
                        }
                        else
                        {
                            endPos = overDueBooksStr.length();
                        }
                        overDueBooksStr.erase(pos, endPos - pos);

                        // Find the corresponding period position
                        size_t periodPos = 0;
                        size_t currentPos = 0;
                        for (size_t i = 0; i < pos; i++)
                        {
                            if (overDueBooksStr[i] == '|')
                            {
                                currentPos++;
                            }
                        }
                        for (size_t i = 0; i < currentPos; i++)
                        {
                            periodPos = overDueTimeStr.find('|', periodPos) + 1;
                        }

                        // Find the end position of the period
                        size_t periodEndPos = overDueTimeStr.find('|', periodPos);
                        if (periodEndPos != string::npos)
                        {
                            periodEndPos++; // Include the '|' character
                        }
                        else
                        {
                            periodEndPos = overDueTimeStr.length();
                        }
                        overDueTimeStr.erase(periodPos, periodEndPos - periodPos);
                    }

                    overDueBooksStr += book.first->getISBN() + "|";
                    overDueTimeStr += to_string(book.second) + "|";
                }
                lines.push_back(id + "," + f + "," + size + "," + isbn + "," + timestamp + "," + overDueBooksStr + "," + overDueTimeStr + ",");
            }
            else
            {
                lines.push_back(line);
            }
        }
        accountread.close();

        ofstream accountwrite("accounts.txt");
        for (const auto &line : lines)
        {
            accountwrite << line << endl;
        }
        accountwrite.close();
        for (const auto &book : overdueBooks)
        {
            cout << book.first->getISBN() << " " << book.first->getTitle() << " " << book.second << " days" << endl;
        }
    }

    vector<pair<Book *, time_t>> getOverDueBooks(int maxDays)
    {
        time_t now = time(0);
        for (const auto &book : borrowedBooks)
        {
            if ((difftime(now, book.second) / (24 * 3600)) > maxDays)
            {
                overdueBooks.push_back(book);
            }
        }
        return overdueBooks;
    }

    void addAccountToFile()
    {
        ifstream accountread("accounts.txt");

        string line;
        vector<string> lines;
        while (getline(accountread, line))
        {
            lines.push_back(line);
        }
        // lines.push_back(serialize());
        lines.push_back(userId + "," + "0" + "," + "0" + "," + "" + "," + "0" + "," + "" + "," + "" + ",");
        accountread.close();
        ofstream accountwrite("accounts.txt");
        for (const auto &line : lines)
        {
            accountwrite << line << endl;
        }
    }
    void deleteAccountFromFile()
    {
        ifstream accountread("accounts.txt");
        string line;
        vector<string> lines;
        while (getline(accountread, line))
        {
            stringstream ss(line);
            string id, f, size, isbn, time;
            getline(ss, id, ',');
            getline(ss, f, ',');
            getline(ss, size, ',');
            getline(ss, isbn, ',');
            getline(ss, time, ',');
            if (id != userId)
            {
                lines.push_back(line);
            }
        }
        accountread.close();
        ofstream accountwrite("accounts.txt");
        for (const auto &line : lines)
        {
            accountwrite << line << endl;
        }
        accountwrite.close();
    }
    vector<pair<Book *, time_t>> getBorrowedBooks()
    {
        return borrowedBooks;
    }
    void displayBorrowedBooks()
    {
        if (borrowedBooks.size() == 0)
        {
            cout << "No borrowed books." << endl;
            return;
        }
        for (const auto &book : borrowedBooks)
        {
            cout << "Title: " << book.first->title << ", Author: " << book.first->author << endl;
        }
    }
    void borrowBook(Book *book, vector<Book> &libraryBooks)
    {
        borrowedBooks.push_back(make_pair(book, time(0)));
        book->setStatus("Borrowed");
        ifstream accountread("accounts.txt");

        string line;
        vector<string> lines;
        while (getline(accountread, line))
        {
            stringstream ss(line);
            string id, f, size, isbn, timestamp, overdueBooks, overTime, borrowHistoryISBNs;
            getline(ss, id, ',');
            getline(ss, f, ',');
            getline(ss, size, ',');
            getline(ss, isbn, ',');
            getline(ss, timestamp, ',');
            getline(ss, overdueBooks, ',');
            getline(ss, overTime, ',');
            getline(ss, borrowHistoryISBNs, ',');
            if (id == userId)
            {
                isbn = isbn + book->ISBN_code + '|';
                size = to_string(borrowedBooks.size());
                timestamp = timestamp + to_string(time(0)) + '|';
                lines.push_back(id + "," + f + "," + size + "," + isbn + "," + timestamp + "," + overdueBooks + "," + overTime + "," + borrowHistoryISBNs + ",");
            }
            else
            {
                lines.push_back(line);
            }
        }
        accountread.close();
        ofstream accountwrite("accounts.txt");
        for (const auto &line : lines)
        {
            accountwrite << line << endl;
        }
    }
    void returnBook(Book *book, vector<Book> &libraryBooks)
    {
        int flag = 0;
        for (auto it = borrowedBooks.begin(); it != borrowedBooks.end(); it++)
        {
            if (it->first->getISBN() == book->getISBN())
            {
                flag = 1;
                borrowHistory.push_back(book->getISBN());
                borrowedBooks.erase(it);
                // book.setStatus("Available");
                cout << "Book returned Successfully!!" << endl;
                break;
            }
        }
        if (flag == 0)
        {
            cout << "You have not borrowed this book!!" << endl;
            return;
        }
        for (auto &libBook : libraryBooks)
        {
            if (libBook.getISBN() == book->getISBN())
            {
                libBook.setStatus("Available");
                break;
            }
        }
        for (auto it = overdueBooks.begin(); it != overdueBooks.end(); it++)
        {
            if (it->first->getISBN() == book->getISBN())
            {
                overdueBooks.erase(it);
                break;
            }
        }
        ifstream accountread("accounts.txt");

        string line;
        vector<string> lines;
        while (getline(accountread, line))
        {
            stringstream ss(line);
            string id, f, size, isbn, timestamp, overdueBooksStr, overduePeriodsStr, borrowHistoryISBNs;
            getline(ss, id, ',');
            getline(ss, f, ',');
            getline(ss, size, ',');
            getline(ss, isbn, ',');
            getline(ss, timestamp, ',');
            getline(ss, overdueBooksStr, ',');
            getline(ss, overduePeriodsStr, ',');
            getline(ss, borrowHistoryISBNs, ',');
            if (id == userId)
            {
                size_t pos = isbn.find(book->getISBN()+"|");
                if (pos != string::npos)
                {
                    // Find the end position of the ISBN
                    size_t endPos = isbn.find('|', pos);
                    if (endPos != string::npos)
                    {
                        endPos++; // Include the '|' character
                    }
                    else
                    {
                        endPos = isbn.length();
                    }
                    isbn.erase(pos, endPos - pos);

                    // Find the corresponding timestamp position
                    size_t timestampPos = 0;
                    size_t currentPos = 0;
                    for (size_t i = 0; i < pos; i++)
                    {
                        if (isbn[i] == '|')
                        {
                            currentPos++;
                        }
                    }
                    for (size_t i = 0; i < currentPos; i++)
                    {
                        timestampPos = timestamp.find('|', timestampPos) + 1;
                    }

                    // Find the end position of the timestamp
                    size_t timestampEndPos = timestamp.find('|', timestampPos);
                    if (timestampEndPos != string::npos)
                    {
                        timestampEndPos++; // Include the '|' character
                    }
                    else
                    {
                        timestampEndPos = timestamp.length();
                    }
                    // Erase the timestamp
                    timestamp.erase(timestampPos, timestampEndPos - timestampPos);
                }
                // Erase the overdue book and period if present
                size_t overdueBookPos = overdueBooksStr.find(book->getISBN());
                if (overdueBookPos != string::npos)
                {
                    size_t overdueBookEndPos = overdueBooksStr.find('|', overdueBookPos);
                    if (overdueBookEndPos != string::npos)
                    {
                        overdueBookEndPos++; // Include the '|' character
                    }
                    else
                    {
                        overdueBookEndPos = overdueBooksStr.length();
                    }
                    overdueBooksStr.erase(overdueBookPos, overdueBookEndPos - overdueBookPos);

                    size_t overduePeriodPos = 0;
                    size_t currentOverduePos = 0;
                    for (size_t i = 0; i < overdueBookPos; i++)
                    {
                        if (overdueBooksStr[i] == '|')
                        {
                            currentOverduePos++;
                        }
                    }
                    for (size_t i = 0; i < currentOverduePos; i++)
                    {
                        overduePeriodPos = overduePeriodsStr.find('|', overduePeriodPos) + 1;
                    }

                    size_t overduePeriodEndPos = overduePeriodsStr.find('|', overduePeriodPos);
                    if (overduePeriodEndPos != string::npos)
                    {
                        overduePeriodEndPos++; // Include the '|' character
                    }
                    else
                    {
                        overduePeriodEndPos = overduePeriodsStr.length();
                    }
                    overduePeriodsStr.erase(overduePeriodPos, overduePeriodEndPos - overduePeriodPos);
                }
                size = to_string(borrowedBooks.size());
                f = to_string(getFine());
                borrowHistoryISBNs += book->getISBN() + "|";
                lines.push_back(id + "," + f + "," + size + "," + isbn + "," + timestamp + "," + overdueBooksStr + "," + overduePeriodsStr + "," + borrowHistoryISBNs + ",");
            }
            else
            {
                lines.push_back(line);
            }
        }
        accountread.close();

        ofstream accountwrite("accounts.txt");
        for (const auto &line : lines)
        {
            accountwrite << line << endl;
        }
        accountwrite.close();
    }
    int calculateFine(int maxDays)
    {
        fine = 0;
        time_t now = time(0);
        for (const auto &entry : borrowedBooks)
        {
            double days = difftime(now, entry.second) / (24 * 3600);
            if (days > maxDays)
            {
                fine += (days - maxDays) * 10; // 10 rupees per day
            }
        }
        return fine;
    }
    int getBorrowedCount()
    {
        return borrowedBooks.size();
    }
    int getFine()
    {
        return fine;
    }
    void clearFine()
    {
        if (fine > 0)
        {
            cout << "You fine amount is: " << fine << endl;
            cout << "Confirm to clear fine ?\n1. Yes\n2. No" << endl;
            int choice;
            cout << "Choice: ";
            cin >> choice;
            if (cin.fail())
            {
                cin.clear(); // Clear the error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the input
                cout << "Invalid choice. Please enter a valid number.\n";
                return;
            }
    
            cin.ignore();
            if (choice == 2)
            {
                return;
            }
            fine = 0;
            ifstream accountread("accounts.txt");

            string line;
            vector<string> lines;
            while (getline(accountread, line))
            {
                stringstream ss(line);
                string id, f, size, isbn, timestamp, overdueBooksStr, overduePeriodsStr, borrowHistoryISBNs;
                getline(ss, id, ',');
                getline(ss, f, ',');
                getline(ss, size, ',');
                getline(ss, isbn, ',');
                getline(ss, timestamp, ',');
                getline(ss, overdueBooksStr, ',');
                getline(ss, overduePeriodsStr, ',');
                getline(ss, borrowHistoryISBNs, ',');
                if (id == userId)
                {
                    lines.push_back(id + "," + "0" + "," + size + "," + isbn + "," + timestamp + "," + overdueBooksStr + "," + overduePeriodsStr + "," + borrowHistoryISBNs + ",");
                }
                else
                {
                    lines.push_back(line);
                }
            }
            accountread.close();

            ofstream accountwrite("accounts.txt");
            for (const auto &line : lines)
            {
                accountwrite << line << endl;
            }
            accountwrite.close();
        }
        else
        {
            cout << "No fine to clear" << endl;
        }
    }
    void displayHistory()
    {
        if (borrowHistory.size() == 0)
        {
            cout << "No borrowing history" << endl;
            return;
        }
        for (const auto &book : borrowHistory)
        {
            cout << book << endl;
        }
    }
    string serialize() const
    {
        stringstream ss;
        ss << userId << "," << fine << "," << borrowedBooks.size() << ",";
        for (const auto &b : borrowedBooks)
        {
            ss << b.first->getISBN() << "|";
        }
        ss << ",";
        for (const auto &b : borrowedBooks)
        {
            ss << b.second << "|";
        }
        ss << ",";
        for (const auto &b : overdueBooks)
        {
            ss << b.first->ISBN_code << "|";
        }
        ss << ",";
        for (const auto &b : borrowedBooks)
        {
            ss << b.second << "|";
        }
        ss << ",";
        for (const auto &b : borrowHistory)
        {
            ss << b << "|";
        }
        ss << ",";
        return ss.str();
    }

    static Account deserialize(string line, vector<Book> &libraryBooks)
    {
        stringstream ss(line);
        string id, f, size, isbnStr, timestampStr, overDueBooksStr, overDueTimeStr, borrowHistoryISBNs;
        getline(ss, id, ',');              // id
        getline(ss, f, ',');               // fine
        getline(ss, size, ',');            // borrowed books number
        getline(ss, isbnStr, ',');         // ISBNs of borrowed books stored as string in format bookISBN1|bookISBN2|... etc.
        getline(ss, timestampStr, ',');    // corresponding borrowed timestamps as timestamp1|timestamp2|... etc.
        getline(ss, overDueBooksStr, ','); // overDuebooks stored as ISBN1|ISBN2|...
        getline(ss, overDueTimeStr, ',');  // corresponding due days as num_days1|num_days2|...
        getline(ss, borrowHistoryISBNs, ',');

        Account acc(id);
        acc.fine = stoi(f);
        int n = stoi(size);

        vector<string> isbns;
        vector<string> timestamps;
        vector<string> overDueBooksISBN;
        vector<string> overDueBookTimes;

        // Split the isbnStr and timestampStr by '|'
        stringstream isbnStream(isbnStr);
        string isbn;
        while (getline(isbnStream, isbn, '|'))
        {
            if (!isbn.empty())
            {
                isbns.push_back(isbn);
            }
        }

        stringstream timestampStream(timestampStr);
        string timestamp;
        while (getline(timestampStream, timestamp, '|'))
        {
            if (!timestamp.empty())
            {
                timestamps.push_back(timestamp);
            }
        }

        stringstream overdueBooksStream(overDueBooksStr);
        string overdueISBN;
        while (getline(overdueBooksStream, overdueISBN, '|'))
        {
            if (!overdueISBN.empty())
            {
                overDueBooksISBN.push_back(overdueISBN);
            }
        }

        stringstream overduePeriodsStream(overDueTimeStr);
        string overduePeriod;
        while (getline(overduePeriodsStream, overduePeriod, '|'))
        {
            if (!overduePeriod.empty())
            {
                overDueBookTimes.push_back(overduePeriod);
            }
        }
        for (int i = 0; i < n; ++i)
        {

            for (auto &book : libraryBooks)
            {
                if (book.getISBN() == isbns[i])
                {
                    acc.borrowedBooks.push_back({&book, stol(timestamps[i])});
                    book.setStatus("Borrowed");
                    break;
                }
            }
        }
        for (int i = 0; i < overDueBooksISBN.size(); i++)
        {
            for (auto &book : libraryBooks)
            {
                if (book.getISBN() == overDueBooksISBN[i])
                {
                    acc.overdueBooks.push_back({&book, stol(overDueBookTimes[i])});
                    break;
                }
            }
        }
        stringstream borrowHistoryStream(borrowHistoryISBNs);
        string historyISBN;
        while (getline(borrowHistoryStream, historyISBN, '|'))
        {
            if (!historyISBN.empty())
            {
                acc.borrowHistory.push_back(historyISBN);
            }
        }
        return acc;
    }
};

class User
{
protected:
    string userId, name;
    Account account;

public:
    User(string id, string n) : account(id)
    {
        userId = id;
        name = n;
    }

    // getters
    string getUserId()
    {
        return userId;
    }
    string getUserName()
    {
        return name;
    }
    Account &getUserAccount()
    {
        return account;
    }
    void setUserAccount(Account &acc)
    {
        account = acc;
    }

    // virtual fns
    virtual void displayInfo() = 0;
    virtual void borrowBook(Book *book, vector<Book> &libraryBooks) = 0;
    virtual void returnBook(Book *book, vector<Book> &libraryBooks) = 0;
    virtual bool canBorrow() = 0;
    virtual string getRole() = 0;
};

class Student : public User
{
private:
    static const int MAX_BOOKS = 3;
    static const int MAX_DAYS = 15;

public:
    Student(string id, string n) : User(id, n) {}
    bool canBorrow()
    {
        return (account.getBorrowedCount() < 3 && account.calculateFine(MAX_DAYS) == 0);
    }
    void borrowBook(Book *book, vector<Book> &libraryBooks)
    {
        if (canBorrow() && book->getStatus() == "Available")
        {
            account.borrowBook(book, libraryBooks);
            cout << "Book borrowed successfully!!" << endl;
        }
        else
        {
            if (book->getStatus() != "Available")
            {
                cout << "Book is already " << book->getStatus() << "!!" << endl;
            }
            else
            {
                if (account.getBorrowedCount() >= 3)
                {
                    cout << "You have borrowed maximum number of books!!" << endl;
                }
                else
                {
                    cout << "You have due fine to be paid!!" << endl;
                }
            }
        }
    }
    void returnBook(Book *book, vector<Book> &libraryBooks)
    {
        account.returnBook(book, libraryBooks);
        int fineAmt = account.calculateFine(MAX_DAYS);
        if (fineAmt > 0)
        {
            cout << "Fine of " << fineAmt << "Rs. is due!!" << endl;
        }
    }
    string getRole()
    {
        return "Student";
    }
    void displayInfo()
    {
        cout << "Student - ID: " << userId << ", Name: " << name
             << ", Borrowed: " << account.getBorrowedCount()
             << ", Fines: " << account.getFine() << endl;
    }
};

class Faculty : public User
{
private:
    static const int MAX_BOOKS = 5;
    static const int MAX_DAYS = 30;

public:
    Faculty(string i, string n) : User(i, n) {}

    bool canBorrow()
    {
        return (account.getBorrowedCount() < MAX_BOOKS) && !(account.getOverDueBooks(60).size());
    }

    void borrowBook(Book *book, vector<Book> &libraryBooks) override
    {
        if (canBorrow() && book->getStatus() == "Available")
        {
            account.borrowBook(book, libraryBooks);
            cout << "Book borrowed successfully\n";
        }
        else
        {
            if (!canBorrow())
            {
                if (account.getBorrowedCount() >= MAX_BOOKS)
                {
                    cout << "You cannot borrow more than 5 books!!" << endl;
                }
                else
                {
                    cout << "You have overdue books!!" << endl;
                }
            }
            else
            {
                cout << "Cannot borrow book\n";
            }
        }
    }

    void returnBook(Book *book, vector<Book> &libraryBooks) override
    {
        int tmp = account.getBorrowedCount();
        account.returnBook(book, libraryBooks);
        if (account.getBorrowedCount() < tmp)
        {
            cout << "Book returned successfully\n";
        }
    }
    string getRole()
    {
        return "Faculty";
    }
    void displayInfo()
    {
        cout << "Faculty - ID: " << userId << ", Name: " << name
             << ", Borrowed: " << account.getBorrowedCount() << endl;
    }
};

class Librarian : public User
{
public:
    Librarian(string id, string n) : User(id, n) {}

    void borrowBook(Book *book, vector<Book> &libraryBooks)
    {
        cout << "You donot have access to borrow book\n";
    }
    void returnBook(Book *book, vector<Book> &libraryBooks) override
    {
        cout << "You do not have access to return book\n";
    }
    bool canBorrow()
    {
        return false;
    }
    void displayInfo()
    {
        cout << "Librarian - ID: " << userId << ", Name: " << name << endl;
    }
    string getRole()
    {
        return "Librarian";
    }

    void addBook(vector<Book> &books, Book book)
    {
        books.push_back(book);
        cout << "Book added successfully!\n";
        ifstream bookread("books.txt");

        string line;
        vector<string> lines;
        while (getline(bookread, line))
        {
            lines.push_back(line);
        }
        lines.push_back(book.serialize());
        bookread.close();
        ofstream bookwrite("books.txt");
        for (const auto &line : lines)
        {
            bookwrite << line << endl;
        }
        bookwrite.close();
    }

    void removeBook(vector<Book> &books, string isbn) // check
    {
        for (auto it = books.begin(); it != books.end(); ++it)
        {
            if (it->getISBN() == isbn)
            {
                books.erase(it);
                cout << "Book removed successfully!\n";
                ifstream bookread("books.txt");

                string line;
                vector<string> lines;
                while (getline(bookread, line))
                {
                    stringstream ss(line);
                    string t, a, p, y, i, s;
                    getline(ss, t, ',');
                    getline(ss, a, ',');
                    getline(ss, p, ',');
                    getline(ss, y, ',');
                    getline(ss, i, ',');
                    getline(ss, s, ',');
                    if (i == isbn)
                    {
                        continue;
                    }
                    else
                    {
                        lines.push_back(line);
                    }
                }
                bookread.close();
                ofstream bookwrite("books.txt");
                for (const auto &line : lines)
                {
                    bookwrite << line << endl;
                }
                return;
            }
        }
        cout << "Book not found.\n";
    }

    void addUser(vector<User *> &users, User *user)
    {
        users.push_back(user);
        cout << "User added successfully!\n";
        ifstream userread("users.txt");

        string line;
        vector<string> lines;
        while (getline(userread, line))
        {
            lines.push_back(line);
        }
        lines.push_back(user->getUserId() + "," + user->getUserName() + "," + user->getRole() + ",");
        userread.close();
        ofstream userwrite("users.txt");
        for (const auto &line : lines)
        {
            userwrite << line << endl;
        }
        userwrite.close();
        (user->getUserAccount()).addAccountToFile();
    }

    void removeUser(vector<User *> &users, string id)
    {
        for (auto it = users.begin(); it != users.end(); ++it)
        {
            if ((*it)->getUserId() == id)
            {
                ((*it)->getUserAccount()).deleteAccountFromFile();
                delete *it;
                users.erase(it);
                cout << "User removed successfully!\n";

                
                ifstream userread("users.txt");

                string line;
                vector<string> lines;
                while (getline(userread, line))
                {
                    stringstream ss(line);
                    string i, n, r;
                    getline(ss, i, ',');
                    getline(ss, n, ',');
                    getline(ss, r, ',');
                    if (id == i)
                    {
                        continue;
                    }
                    else
                    {
                        lines.push_back(line);
                    }
                }
                userread.close();
                ofstream userwrite("users.txt");
                for (const auto &line : lines)
                {
                    userwrite << line << endl;
                }

                return;
            }
        }
        cout << "User not found.\n";
    }
};

class Library
{
private:
    vector<Book> books;
    vector<User *> users;
    vector<Account> accounts;

public:
    Book findBook(string isbn)
    {
        for (auto book : books)
        {
            if (book.getISBN() == isbn)
            {
                return book;
            }
        }
        cout << "Book not found!!" << endl;
    }
    User *findUser(string id)
    {
        for (auto &user : users)
        {
            if (user->getUserId() == id)
            {
                return user;
            }
        }
        cout << "User Not Found!!" << endl;
        return nullptr;
    }
    void displayBooks()
    {
        cout << "\n** All Books **\n\n";
        if(books.size() == 0){
            cout << "No books in the library!!\n";
            return;
        }
        for (auto book : books)
            book.display();
    }
    vector<Book> &getBooks() { return books; }
    vector<User *> &getUsers() { return users; }

    Library()
    {
        loadBooks();
        loadUsers();
        loadAccounts();
    }

    void loadBooks()
    {
        ifstream file("books.txt");
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                books.push_back(Book::deserialize(line));
            }
            file.close();
        }
    }

    void saveBooks()
    {
        ofstream file("books.txt");
        for (const auto &book : books)
        {
            file << book.serialize() << endl;
        }
        file.close();
    }
    void loadUsers()
    {
        ifstream file("users.txt");
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string id, name, role;
                getline(ss, id, ',');
                getline(ss, name, ',');
                getline(ss, role, ',');
                if (role == "Student")
                    users.push_back(new Student(id, name));
                else if (role == "Faculty")
                    users.push_back(new Faculty(id, name));
                else if (role == "Librarian")
                    users.push_back(new Librarian(id, name));
            }
            file.close();
        }
    }

    void saveUsers()
    {
        ofstream file("users.txt");
        for (const auto &user : users)
        {
            file << user->getUserId() << "," << user->getUserName() << "," << user->getRole() << endl;
        }
        file.close();
    }

    void loadAccounts()
    {
        ifstream file("accounts.txt");
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                Account acc = Account::deserialize(line, books);
                for (auto &user : users)
                {
                    if (user->getUserId() == acc.getUserId())
                    {
                        user->setUserAccount(acc);
                        break;
                    }
                }
            }
            file.close();
        }
    }

    void saveAccounts()
    {
        ofstream file("accounts.txt");
        for (const auto &acc : accounts)
        {
            file << acc.serialize() << endl;
        }
        file.close();
    }
};
void userMenu(Library &lib, User *user)
{
    while (true)
    {
        cout << "\nWelcome, " << user->getUserName() << "(" << user->getRole() << ")" << "\n";
        cout << "1. View Books\n2. Borrow Book\n3. Return Book\n";
        if (user->getRole() == "Student" || user->getRole() == "Faculty")
        {
            cout << "4. View Borrowed Books\n";
            cout << "5. View and Pay Fine\n";
            cout << "6. View Borrow History\n";
            cout << "7. View Overdues\n";
        }
        if (user->getRole() == "Librarian")
        {
            cout << "4. Update BookStatus\n5. Add Book\n6. Remove Book\n7. Add User\n8. Remove User\n";
        }
        cout << "0. Logout\nChoice: ";
        int choice;
        cin >> choice;
        if (cin.fail())
        {
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the input
            cout << "Invalid choice. Please enter a valid number.\n";
            continue;
        }
        cin.ignore();

        if (choice == 0)
        {
            break;
        }
        else if (choice == 1)
        {
            lib.displayBooks();
        }
        else if (choice == 2)
        {
            if (user->getRole() == "Librarian")
            {
                cout << "You are not allowed to borrow books." << endl;
            }
            else
            {
                cout << "Enter ISBN: ";
                string isbn;
                getline(cin, isbn);
                cout << isbn << "\n\n\n";
                Book *book = nullptr;
                for (auto &b : lib.getBooks())
                {
                    if (b.getISBN() == isbn)
                    {
                        book = &b;
                        break;
                    }
                }
                if (book)
                {
                    user->borrowBook(book, lib.getBooks());
                }
                else
                {
                    cout << "Book not found!!" << endl;
                }
            }
        }
        else if (choice == 3)
        {
            if (user->getRole() == "Librarian")
            {
                cout << "You are not allowed to return books." << endl;
            }
            else
            {

                cout << "Enter ISBN: ";
                string isbn;
                getline(cin, isbn);
                Book *book = nullptr;
                for (auto &b : lib.getBooks())
                {
                    if (b.getISBN() == isbn)
                    {
                        book = &b;
                        break;
                    }
                }
                if (book)
                {
                    user->returnBook(book, lib.getBooks());
                }
                else
                {
                    cout << "Book not found!!" << endl;
                }
            }
        }
        else if (user->getRole() != "Librarian" && choice == 4)
        {
            user->getUserAccount().displayBorrowedBooks();
        }
        else if (user->getRole() == "Librarian" && choice == 4)
        {
            cout << "Enter ISBN: ";
            string isbn;
            getline(cin, isbn);
            Book *book = nullptr;
            for (auto &b : lib.getBooks())
            {
                if (b.getISBN() == isbn)
                {
                    book = &b;
                    break;
                }
            }
            if (book)
            {
                string status;
                cout << "Enter Status (Available/Borrowed/Reserved): ";
                getline(cin, status);
                if ((status == "Available") || (status == "Borrowed") || (status == "Reserved"))
                {
                    book->setStatus(status);
                }
                else
                {
                    cout << "Incorrect status\n";
                }
            }
        }
        else if ((user->getRole() == "Student" || user->getRole() == "Faculty") && choice == 5)
        {
            user->getUserAccount().clearFine();
        }
        else if ((user->getRole() == "Student" || user->getRole() == "Faculty") && choice == 6)
        {
            // TODO: view borrow history logic
            user->getUserAccount().showBorrowHistory(lib.getBooks());
        }
        else if ((user->getRole() == "Student" || user->getRole() == "Faculty") && choice == 7)
        {
            int maxDays = user->getRole() == "Student" ? 15 : 30;
            user->getUserAccount().showOverdueBooks(maxDays);
        }
        else if (user->getRole() == "Librarian")
        {
            auto *libUser = dynamic_cast<Librarian *>(user);
            if (choice == 5)
            {
                string t, a, p, i;
                int y;
                cout << "Enter Title: ";
                getline(cin, t);
                cout << "Enter Author: ";
                getline(cin, a);
                cout << "Enter Publisher: ";
                getline(cin, p);
                cout << "Enter Year: ";
                cin >> y;
                cin.ignore();
                cout << "Enter ISBN: ";
                getline(cin, i);
                libUser->addBook(lib.getBooks(), Book(t, a, p, y, i));
            }
            else if (choice == 6)
            {
                cout << "Enter ISBN: ";
                string isbn;
                getline(cin, isbn);
                libUser->removeBook(lib.getBooks(), isbn);
            }
            else if (choice == 7)
            {
                string id, name, role;
                cout << "Enter ID: ";
                getline(cin, id);
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Role (Student/Faculty/Librarian): ";
                getline(cin, role);
                if (role == "Student")
                    libUser->addUser(lib.getUsers(), new Student(id, name));
                else if (role == "Faculty")
                    libUser->addUser(lib.getUsers(), new Faculty(id, name));
                else if (role == "Librarian")
                    libUser->addUser(lib.getUsers(), new Librarian(id, name));
            }
            else if (choice == 8)
            {
                cout << "Enter User ID: ";
                string id;
                getline(cin, id);
                libUser->removeUser(lib.getUsers(), id);
            }
        }
    }
}
int main()
{
    Library lib;
    while (true)
    {
        cout << "\nLibrary Management System\n1. Login\n0. Exit\nChoice: ";
        int choice;
        cin >> choice;
        if (cin.fail())
        {
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the input
            cout << "Invalid choice. Please enter a valid number.\n";
            continue;
        }
        cin.ignore();

        if (choice == 0)
        {
            break;
        }
        else if (choice == 1)
        {
            cout << "Enter User ID: ";
            string id;
            getline(cin, id);
            User *user = lib.findUser(id);
            if (user)
            {
                userMenu(lib, user);
            }
            else
            {
                cout << "User not found.\n";
            }
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}