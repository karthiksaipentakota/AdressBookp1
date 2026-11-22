#include <stdio.h>
#include <string.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *ptr = fopen("AddressBook.csv", "w");
    if (ptr == NULL)
    {
        printf("Error: could not open AddressBook.csv for saving.\n");
        return;
    }

    fprintf(ptr, "#%d\n", addressBook->Contactcount);
    for (int i = 0; i < addressBook->Contactcount; i++)
    {
        fprintf(ptr, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(ptr);
}

void loadContactsFromFile(AddressBook *addressBook)
{
    int count = 0;
    addressBook->Contactcount = 0;

    FILE *fptr = fopen("AddressBook.csv", "r");
    if (fptr == NULL)
    {
        printf("No saved contacts found. Starting with an empty address book.\n");
        return;
    }

    if (fscanf(fptr, "#%d", &count) != 1 || count < 0)
    {
        printf("Saved file is not valid. Starting with an empty address book.\n");
        fclose(fptr);
        return;
    }

    if (count > MAX_CONTACTS)
    {
        printf("File has more than %d contacts. Only the first %d will be loaded.\n",
               MAX_CONTACTS, MAX_CONTACTS);
        count = MAX_CONTACTS;
    }

    for (int i = 0; i < count; i++)
    {
        Contact *c = &addressBook->contacts[i];

        if (fscanf(fptr, " %49[^,],%19[^,],%49[^\n]", c->name, c->phone, c->email) != 3)
            break;

        /* Remove a stray '\r' if the file was edited on Windows */
        char *cr = strchr(c->email, '\r');
        if (cr != NULL)
            *cr = '\0';

        addressBook->Contactcount++;
    }

    fclose(fptr);
}