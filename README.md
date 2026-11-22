# Address Book (C)

A simple command-line Address Book written in C. It stores contacts (name, phone number, email) and saves them to a file so they are still there the next time you run the program.

## Features

- Create a new contact
- Search a contact by name, phone number, or email
- Edit a contact's name, phone number, or email
- Delete a contact
- List all contacts, sorted by name
- Save contacts to a file and load them automatically when the program starts

## Input rules

- **Name:** letters, spaces, and dots only
- **Phone:** exactly 10 digits, and it must be unique
- **Email:** lowercase letters, digits, `@` and `.` only, must end with `.com`, and must be unique
- **Limit:** up to 100 contacts

## Project structure

| File | Purpose |
|---|---|
| `main.c` | Main menu and program loop |
| `contact.c` / `contact.h` | Contact operations (create, search, edit, delete, list) and input validation |
| `file.c` / `file.h` | Saving contacts to a file and loading them back |

## How to run

You need a C compiler such as `gcc`.

Compile:

```
gcc main.c contact.c file.c
```

Run:

```
./a.out
```

On Windows, run `a.exe` instead of `./a.out`.

## Menu

```
1. Create Contact
2. Search Contact
3. Edit Contact
4. Delete Contact
5. List All Contacts
6. Save and Exit
7. Exit Without Saving
```

Choose **6** to save your contacts to `AddressBook.csv`. Choose **7** to quit without saving your changes.

## Data format

Contacts are saved in `AddressBook.csv`. The first line holds the number of contacts, and each next line is one contact:

```
#2
Alice Smith,9876543210,alice@example.com
Bob Jones,9123456780,bob@example.com
```

## Author

Pentakota Karthik Sai