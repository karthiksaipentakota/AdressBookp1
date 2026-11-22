#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[MAX_CONTACTS];
    int Contactcount;
} AddressBook;

void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *addressBook);

int validate_name(char *str);
int validate_phone(char *str);
int validate_email(char *str);

int search_by_name(AddressBook *addressBook);
int search_by_phone(AddressBook *addressBook);
int search_by_email(AddressBook *addressBook);

void edit_name(AddressBook *addressBook, int index);
void edit_phone(AddressBook *addressBook, int index);
void edit_email(AddressBook *addressBook, int index);

/* Reads a number safely. Returns -1 if the user typed something that is not a number. */
int read_choice(void);

#endif