#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contact.h"
#include "file.h"

#define NAME_SIZE   50
#define PHONE_SIZE  32   /* bigger than 10 so that too-long numbers are rejected, not cut */
#define EMAIL_SIZE  50

// HELPER FUNCTIONS

static void print_error(const char *message)
{
    printf("\n-------------------------------------------------\n");
    printf("  %s\n", message);
    printf("-------------------------------------------------\n");
}

/* Reads one line safely.
   Returns 1 if the line fit in the buffer, 0 if it was too long (extra input is discarded).
   Exits the program if the input has ended (Ctrl+D / Ctrl+Z). */
static int read_line(char *buf, int size)
{
    if (fgets(buf, size, stdin) == NULL)
    {
        printf("\nInput closed. Exiting...\n");
        exit(0);
    }

    size_t len = strlen(buf);

    if (len > 0 && buf[len - 1] == '\n')
    {
        buf[len - 1] = '\0';
        return 1;
    }

    if (feof(stdin))        /* last line without a newline */
        return 1;

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    return 0;
}

int read_choice(void)
{
    char line[32];
    char *end;

    if (!read_line(line, sizeof(line)))
        return -1;

    long value = strtol(line, &end, 10);

    if (end == line)                       /* no digits at all */
        return -1;
    while (*end == ' ' || *end == '\t')    /* allow trailing spaces */
        end++;
    if (*end != '\0')                      /* something like "3abc" */
        return -1;

    return (int)value;
}

static int phone_exists(AddressBook *addressBook, char *phone, int skip_index)
{
    for (int i = 0; i < addressBook->Contactcount; i++)
    {
        if (i != skip_index &&
            strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            return 1;
        }
    }
    return 0;
}

static int email_exists(AddressBook *addressBook, char *email, int skip_index)
{
    for (int i = 0; i < addressBook->Contactcount; i++)
    {
        if (i != skip_index &&
            strcmp(addressBook->contacts[i].email, email) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/* Keeps asking until a valid name is typed */
static void input_name(char *name)
{
    int res;
    do
    {
        printf("Enter the name : ");
        if (!read_line(name, NAME_SIZE))
        {
            print_error("Name is too long! Maximum 49 characters.");
            res = 0;
            continue;
        }
        res = validate_name(name);
    } while (res == 0);
}

/* Keeps asking until a valid, unique phone number is typed.
   skip_index = the contact being edited (so it does not clash with itself), or -1 */
static void input_phone(char *phone, AddressBook *addressBook, int skip_index)
{
    int res;
    do
    {
        printf("Enter the phone : ");
        if (!read_line(phone, PHONE_SIZE))
        {
            print_error("Invalid phone number! Please enter a valid number.");
            res = 0;
            continue;
        }

        res = validate_phone(phone);

        if (res == 1 && phone_exists(addressBook, phone, skip_index))
        {
            print_error("Phone number already exists in the Address Book!");
            printf("  Please enter a different phone number.\n");
            res = 0;
        }
    } while (res == 0);
}

/* Keeps asking until a valid, unique email is typed */
static void input_email(char *email, AddressBook *addressBook, int skip_index)
{
    int res;
    do
    {
        printf("Enter the email : ");
        if (!read_line(email, EMAIL_SIZE))
        {
            print_error("Email is too long! Maximum 49 characters.");
            res = 0;
            continue;
        }

        res = validate_email(email);

        if (res == 1 && email_exists(addressBook, email, skip_index))
        {
            print_error("Email address already exists in the Address Book!");
            printf("  Please enter a different email address.\n");
            res = 0;
        }
    } while (res == 0);
}

static int search_menu(void)
{
    printf("\n");
    printf("=================================================\n");
    printf("                 SEARCH CONTACT                  \n");
    printf("=================================================\n");
    printf("|  1.  Search by Name                           |\n");
    printf("|  2.  Search by Phone Number                   |\n");
    printf("|  3.  Search by Email                          |\n");
    printf("|  4.  Exit                                     |\n");
    printf("=================================================\n");
    printf("Enter your choice: ");
    return read_choice();
}

/* Shows the search menu until a contact is picked.
   Returns the contact index, -1 if not found, or -2 if the user chose Exit. */
static int pick_contact(AddressBook *addressBook)
{
    int choice;

    while (1)
    {
        choice = search_menu();

        if (choice == 4)
        {
            printf("Exit from this menu\n");
            return -2;
        }
        if (choice == 1) return search_by_name(addressBook);
        if (choice == 2) return search_by_phone(addressBook);
        if (choice == 3) return search_by_email(addressBook);

        print_error("Invalid choice! Please enter a valid option.");
    }
}

// LIST CONTACTS

void listContacts(AddressBook *addressBook)
{
    Contact temp;

    // Sort contacts by name using Bubble Sort
    for (int i = 0; i < addressBook->Contactcount - 1; i++)
    {
        for (int j = 0; j < addressBook->Contactcount - 1 - i; j++)
        {
            if (strcmp(addressBook->contacts[j].name,
                       addressBook->contacts[j + 1].name) > 0)
            {
                temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }

    printf("\n");
    printf("+------+---------------------------+-----------------+---------------------------+\n");
    printf("| %-4s | %-25s | %-15s | %-25s |\n", "S.No", "Name", "Phone", "Email");
    printf("+------+---------------------------+-----------------+---------------------------+\n");

    for (int i = 0; i < addressBook->Contactcount; i++)
    {
        printf("| %-4d | %-25s | %-15s | %-25s |\n",
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
    printf("+------+---------------------------+-----------------+---------------------------+\n");
}

// INITIALIZE

void initialize(AddressBook *addressBook)
{
    addressBook->Contactcount = 0;
    loadContactsFromFile(addressBook);
}

// CREATE CONTACT

void createContact(AddressBook *addressBook)
{
    char name[NAME_SIZE];
    char phone[PHONE_SIZE];
    char email[EMAIL_SIZE];

    if (addressBook->Contactcount >= MAX_CONTACTS)
    {
        print_error("Address book is full! Delete a contact first.");
        return;
    }

    input_name(name);
    input_phone(phone, addressBook, -1);
    input_email(email, addressBook, -1);

    // STORE CONTACT
    Contact *c = &addressBook->contacts[addressBook->Contactcount];
    strcpy(c->name, name);
    strcpy(c->phone, phone);
    strcpy(c->email, email);
    addressBook->Contactcount++;

    print_error("Contact created successfully!");
}

// SEARCH CONTACT MENU

void searchContact(AddressBook *addressBook)
{
    int index;

    do
    {
        index = pick_contact(addressBook);

        if (index == -1)
            print_error("Contact not found! Please try again.");

    } while (index == -1);
}

// EDIT CONTACT

void editContact(AddressBook *addressBook)
{
    int index = pick_contact(addressBook);
    int field;

    if (index == -2)
        return;

    if (index == -1)
    {
        print_error("Contact not found! Please try again.");
        return;
    }

    // EDIT FIELD
    do
    {
        printf("\n");
        printf("=================================================\n");
        printf("                  EDIT FIELD                     \n");
        printf("=================================================\n");
        printf("|  1.  Edit Name                                |\n");
        printf("|  2.  Edit Phone Number                        |\n");
        printf("|  3.  Edit Email                               |\n");
        printf("|  4.  Exit                                     |\n");
        printf("=================================================\n");
        printf("Enter your choice: ");
        field = read_choice();

        switch (field)
        {
            case 1:
                edit_name(addressBook, index);
                break;

            case 2:
                edit_phone(addressBook, index);
                break;

            case 3:
                edit_email(addressBook, index);
                break;

            case 4:
                printf("Exit from edit menu\n");
                return;

            default:
                print_error("Invalid choice! Please enter a valid option.");
        }
    } while (field != 4);
}

// EDIT NAME

void edit_name(AddressBook *addressBook, int index)
{
    char name[NAME_SIZE];

    input_name(name);
    strcpy(addressBook->contacts[index].name, name);

    print_error("Contact name updated successfully!");
}

// EDIT PHONE

void edit_phone(AddressBook *addressBook, int index)
{
    char phone[PHONE_SIZE];

    input_phone(phone, addressBook, index);
    strcpy(addressBook->contacts[index].phone, phone);

    print_error("Phone number updated successfully!");
}

// EDIT EMAIL

void edit_email(AddressBook *addressBook, int index)
{
    char email[EMAIL_SIZE];

    input_email(email, addressBook, index);
    strcpy(addressBook->contacts[index].email, email);

    print_error("Email address updated successfully!");
}

// DELETE CONTACT

void deleteContact(AddressBook *addressBook)
{
    int index = pick_contact(addressBook);

    if (index == -2)
        return;

    if (index == -1)
    {
        print_error("Contact not found! Please try again.");
        return;
    }

    // SHIFT CONTACTS
    for (int i = index; i < addressBook->Contactcount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }

    addressBook->Contactcount--;

    print_error("Contact deleted successfully!");
}

// VALIDATE NAME

int validate_name(char *str)
{
    if (str[0] == '\0')
    {
        print_error("Invalid name! Please enter a valid name.");
        return 0;
    }
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!((str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= 'a' && str[i] <= 'z') ||
              str[i] == ' ' ||
              str[i] == '.'))
        {
            print_error("Invalid name! Please enter a valid name.");
            return 0;
        }
    }
    return 1;
}

// VALIDATE PHONE

int validate_phone(char *str)
{
    int len = strlen(str);

    if (len != 10)
    {
        print_error("Invalid phone number! Please enter a valid number.");
        return 0;
    }
    for (int i = 0; i < len; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            print_error("Invalid phone number! Please enter a valid number.");
            return 0;
        }
    }
    return 1;
}

// VALIDATE EMAIL

int validate_email(char *str)
{
    int len = strlen(str);
    int at_count = 0;

    // Minimum length
    if (len <= 5)
    {
        print_error("Invalid email address! Please enter a valid email.");
        return 0;
    }
    // Must start with lowercase letter
    if (str[0] < 'a' || str[0] > 'z')
    {
        print_error("Invalid email address! Please enter a valid email.");
        return 0;
    }
    for (int i = 0; str[i] != '\0'; i++)
    {
        // Allowed characters
        if (!((str[i] >= 'a' && str[i] <= 'z') ||
              (str[i] >= '0' && str[i] <= '9') ||
              str[i] == '@' ||
              str[i] == '.'))
        {
            print_error("Invalid email address! Please enter a valid email.");
            return 0;
        }
        // Count @
        if (str[i] == '@')
        {
            at_count++;

            // Character after @ must be lowercase letter
            if (str[i + 1] < 'a' || str[i + 1] > 'z')
            {
                print_error("Invalid email address! Please enter a valid email.");
                return 0;
            }
        }
        // Don't allow ..
        if (str[i] == '.' && str[i + 1] == '.')
        {
            print_error("Invalid email address! Please enter a valid email.");
            return 0;
        }
    }
    // Exactly one @
    if (at_count != 1)
    {
        print_error("Invalid email address! Please enter a valid email.");
        return 0;
    }
    // Must end with .com
    if (len < 4 ||
        str[len - 4] != '.' ||
        str[len - 3] != 'c' ||
        str[len - 2] != 'o' ||
        str[len - 1] != 'm')
    {
        print_error("Invalid email address! Please enter a valid email.");
        return 0;
    }
    return 1;
}

// SEARCH BY NAME

int search_by_name(AddressBook *addressBook)
{
    char name[NAME_SIZE];
    int count = 0;
    int last_index = -1;

    printf("\nEnter the name to search: ");
    if (!read_line(name, NAME_SIZE))
        return -1;      /* too long to match any saved name */

    for (int i = 0; i < addressBook->Contactcount; i++)
    {
        if (strcmp(addressBook->contacts[i].name, name) == 0)
        {
            if (count == 0)
            {
                printf("\n%-5s %-25s %-15s %-30s\n", "S.No", "Name", "Phone", "Email");
                printf("--------------------------------------------------------------------------\n");
            }
            printf("%-5d %-25s %-15s %-30s\n",
                   count + 1,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);

            count++;
            last_index = i;
        }
    }

    // No contact found
    if (count == 0)
        return -1;

    // Multiple contacts with same name
    if (count > 1)
    {
        char phone[PHONE_SIZE];

        printf("\n-------------------------------------------------\n");
        printf("  Multiple contacts found with this name.\n");
        printf("  Please enter the phone number to select one.\n");
        printf("-------------------------------------------------\n");
        printf("Enter phone number: ");

        if (!read_line(phone, PHONE_SIZE))
            return -1;

        for (int i = 0; i < addressBook->Contactcount; i++)
        {
            if (strcmp(addressBook->contacts[i].name, name) == 0 &&
                strcmp(addressBook->contacts[i].phone, phone) == 0)
            {
                return i;
            }
        }
        return -1;
    }

    return last_index;
}

// SEARCH BY PHONE

int search_by_phone(AddressBook *addressBook)
{
    char phone[PHONE_SIZE];

    printf("\nEnter the phone number to search: ");
    if (!read_line(phone, PHONE_SIZE))
        return -1;

    for (int i = 0; i < addressBook->Contactcount; i++)
    {
        if (strcmp(addressBook->contacts[i].phone, phone) == 0)
        {
            printf("\n");
            printf("S.No\tName\t\tPhone\t\tEmail\n");
            printf("------------------------------------------------------------\n");

            printf("%d\t%s\t\t%s\t\t%s\n",
                   1,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);

            return i;
        }
    }
    return -1;
}

// SEARCH BY EMAIL

int search_by_email(AddressBook *addressBook)
{
    char email[EMAIL_SIZE];

    printf("\nEnter the email to search: ");
    if (!read_line(email, EMAIL_SIZE))
        return -1;

    for (int i = 0; i < addressBook->Contactcount; i++)
    {
        if (strcmp(addressBook->contacts[i].email, email) == 0)
        {
            printf("\n");
            printf("S.No\tName\t\tPhone\t\tEmail\n");
            printf("------------------------------------------------------------\n");

            printf("%d\t%s\t\t%s\t\t%s\n",
                   1,
                   addressBook->contacts[i].name,
                   addressBook->contacts[i].phone,
                   addressBook->contacts[i].email);

            return i;
        }
    }
    return -1;
}