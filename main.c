/*
Name:Pentakota Karthik Sai
Duration:August 3rd to 8th

Description of Address Book

The Address Book is a C programming project designed to efficiently store and manage contact information. 
It allows users to add, search, edit, delete, and display contacts such as names, phone numbers, and email addresses.
The project can also use file handling to permanently store contact details and retrieve them when needed.
This project provides practical experience with structures, strings, functions, arrays, file handling, and dynamic memory allocation in C.



*/


#include <stdio.h>
#include "contact.h"
#include "file.h"


int main()
{
    int choice;
    AddressBook addressBook;

    initialize(&addressBook); // Initialize the address book (loads saved contacts)

    do
    {
        printf("\n");
        printf("+-------------------------------------------------+\n");
        printf("|                 ADDRESS BOOK                    |\n");
        printf("+-------------------------------------------------+\n");
        printf("|  1. Create Contact                              |\n");
        printf("|  2. Search Contact                              |\n");
        printf("|  3. Edit Contact                                |\n");
        printf("|  4. Delete Contact                              |\n");
        printf("|  5. List All Contacts                           |\n");
        printf("|  6. Save and Exit                               |\n");
        printf("|  7. Exit Without Saving                         |\n");
        printf("+-------------------------------------------------+\n");

        printf("Enter your choice: ");
        choice = read_choice();

        switch (choice)
        {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:
                listContacts(&addressBook);
                break;
            case 6:
                printf("Saving and Exiting...\n");
                saveContactsToFile(&addressBook);
                break;
            case 7:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
}