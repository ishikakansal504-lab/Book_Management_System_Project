#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PRICE_PER_MONTH 50
#define DAILY_LATE_FEE 2

typedef struct {
    char title[100];
    char author[100];
    int pages;
} Book;

typedef struct {
    char customerName[100];
    char email[100];
    char phone[15];
    char bookTitle[100];
    char bookAuthor[100];
    char issueDate[15];
    char returnDate[15];
    int isIssued;
} Customer;

Book *library = NULL;
Customer *customers = NULL;
int bookCount = 0, bookCapacity = 0;
int customerCount = 0, customerCapacity = 0;

int isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

int daysInMonth(int year, int month) {
    int daysInMonthArray[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && isLeapYear(year)) return 29;
    return daysInMonthArray[month - 1];
}

int totalDaysFromStart(int day, int month, int year) {
    int totalDays = 0;
    for (int i = 1; i < year; i++)
        totalDays += isLeapYear(i) ? 366 : 365;
    for (int i = 1; i < month; i++)
        totalDays += daysInMonth(year, i);
    totalDays += day;
    return totalDays;
}

void trimWhitespace(char *str) {
    int start = 0, end = strlen(str) - 1;
    while (isspace((unsigned char)str[start])) start++;
    while (end >= start && isspace((unsigned char)str[end])) end--;
    for (int i = start; i <= end; i++)
        str[i - start] = str[i];
    str[end - start + 1] = '\0';
}

void addBook() {
    char yn;
    while (1) {
        printf("\nDo you want to add a book? (Y/N): ");
        scanf(" %c", &yn);
        if (yn == 'Y' || yn == 'y') {
            if (bookCount == bookCapacity) {
                bookCapacity += 10;
                Book *temp = realloc(library, bookCapacity * sizeof(Book));
                if (!temp) {
                    printf("Memory allocation failed!\n");
                    exit(1);
                }
                library = temp;
            }

            printf("\n####################################\n");
            printf("Enter book title: ");
            scanf(" %[\n]", library[bookCount].title);
            trimWhitespace(library[bookCount].title);
            printf("Enter author: ");
            scanf(" %[\n]", library[bookCount].author);
            trimWhitespace(library[bookCount].author);
            do {
                printf("Enter number of pages: ");
                scanf("%d", &library[bookCount].pages);
                if (library[bookCount].pages <= 0)
                    printf("Number of pages must be positive!\n");
            } while (library[bookCount].pages <= 0);
            bookCount++;
            printf("Book added successfully!\n");
        } else break;
    }
}

void displayBooks() {
    if (bookCount == 0) {
        printf("No books in the library.\n");
        return;
    }
    for (int i = 0; i < bookCount; i++) {
        printf("\nBook %d\nTitle: %s\nAuthor: %s\nPages: %d\n", i + 1, library[i].title, library[i].author, library[i].pages);
    }
}

void searchByAuthor() {
    char author[100];
    printf("\nEnter author name to search: ");
    scanf(" %[\n]", author);
    trimWhitespace(author);
    int found = 0;
    for (int i = 0; i < bookCount; i++) {
        char bookAuthor[100];
        strcpy(bookAuthor, library[i].author);
        trimWhitespace(bookAuthor);
        if (strcasecmp(bookAuthor, author) == 0) {
            printf("\nTitle: %s\nAuthor: %s\nPages: %d\n", library[i].title, library[i].author, library[i].pages);
            found = 1;
        }
    }
    if (!found)
        printf("No books found by this author.\n");
}

void issueBook() {
    if (customerCount >= customerCapacity) {
        customerCapacity += 10;
        Customer *temp = realloc(customers, customerCapacity * sizeof(Customer));
        if (!temp) {
            printf("Memory allocation failed!\n");
            exit(1);
        }
        customers = temp;
    }

    printf("\nEnter customer name: ");
    scanf(" %[\n]", customers[customerCount].customerName);
    trimWhitespace(customers[customerCount].customerName);
    printf("Enter email: ");
    scanf(" %[\n]", customers[customerCount].email);
    trimWhitespace(customers[customerCount].email);
    printf("Enter phone number: ");
    scanf(" %[\n]", customers[customerCount].phone);
    trimWhitespace(customers[customerCount].phone);
    printf("Enter book title: ");
    scanf(" %[\n]", customers[customerCount].bookTitle);
    trimWhitespace(customers[customerCount].bookTitle);
    printf("Enter book author: ");
    scanf(" %[\n]", customers[customerCount].bookAuthor);
    trimWhitespace(customers[customerCount].bookAuthor);
    printf("Enter issue date (dd/mm/yyyy): ");
    scanf(" %[\n]", customers[customerCount].issueDate);
    trimWhitespace(customers[customerCount].issueDate);

    customers[customerCount].isIssued = 1;
    strcpy(customers[customerCount].returnDate, "Not Returned");
    customerCount++;
    printf("Book issued successfully!\n");
}

void returnBook() {
    char customerName[100], phone[15], bookTitle[100], bookAuthor[100], returnDate[15];
    printf("\nEnter customer name: ");
    scanf(" %[\n]", customerName);
    trimWhitespace(customerName);
    printf("Enter phone number: ");
    scanf(" %[\n]", phone);
    trimWhitespace(phone);
    printf("Enter book title: ");
    scanf(" %[\n]", bookTitle);
    trimWhitespace(bookTitle);
    printf("Enter book author: ");
    scanf(" %[\n]", bookAuthor);
    trimWhitespace(bookAuthor);
    printf("Enter return date (dd/mm/yyyy): ");
    scanf(" %[\n]", returnDate);
    trimWhitespace(returnDate);

    int found = 0;
    for (int i = 0; i < customerCount; i++) {
        if (customers[i].isIssued &&
            strcasecmp(customers[i].customerName, customerName) == 0 &&
            strcmp(customers[i].phone, phone) == 0 &&
            strcasecmp(customers[i].bookTitle, bookTitle) == 0 &&
            strcasecmp(customers[i].bookAuthor, bookAuthor) == 0) {

            int day1, month1, year1, day2, month2, year2;
            if (sscanf(customers[i].issueDate, "%d/%d/%d", &day1, &month1, &year1) != 3 ||
                sscanf(returnDate, "%d/%d/%d", &day2, &month2, &year2) != 3) {
                printf("Invalid date format. Please use dd/mm/yyyy.\n");
                return;
            }

            if (totalDaysFromStart(day2, month2, year2) < totalDaysFromStart(day1, month1, year1)) {
                printf("Return date cannot be earlier than issue date.\n");
                return;
            }

            customers[i].isIssued = 0;
            strcpy(customers[i].returnDate, returnDate);

            int diff = totalDaysFromStart(day2, month2, year2) - totalDaysFromStart(day1, month1, year1);
            int lateDays = diff - 30;
            if (lateDays > 0) {
                int lateFee = lateDays * DAILY_LATE_FEE;
                printf("Late fee: Rs. %d\n", lateFee);
                printf("Total Amount to Pay: Rs. %d\n", lateFee + PRICE_PER_MONTH);
            } else {
                printf("No late fee.\n");
                printf("Total Amount to Pay: Rs. %d\n", PRICE_PER_MONTH);
            }
            printf("Book returned successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("No such issued book found for the given customer details.\n");
}

void searchCustomerDetails() {
    char customerName[100];
    printf("\nEnter customer name to search: ");
    scanf(" %[\n]", customerName);
    trimWhitespace(customerName);

    int found = 0;
    for (int i = 0; i < customerCount; i++) {
        if (strcasecmp(customers[i].customerName, customerName) == 0) {
            printf("\nCustomer Name: %s\nEmail: %s\nPhone: %s\nBook Title: %s\nBook Author: %s\nIssue Date: %s\nReturn Date: %s\nIssued: %s\n",
                   customers[i].customerName, customers[i].email, customers[i].phone,
                   customers[i].bookTitle, customers[i].bookAuthor, customers[i].issueDate,
                   customers[i].returnDate, customers[i].isIssued ? "Yes" : "No");
            found = 1;
        }
    }
    if (!found) printf("No customer found with the given name.\n");
}

void menu() {
    int choice;
    do {
        printf("\n========= Book Management System =========\n");
        printf("1. Add Book\n2. Display Books\n3. Search by Author\n4. Issue Book\n5. Return Book\n6. Search Customer\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchByAuthor(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: searchCustomerDetails(); break;
            case 7: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 7);
}

int main() {
    menu();
    free(library);
    free(customers);
    return 0;
}
