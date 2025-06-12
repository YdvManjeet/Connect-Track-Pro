#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structures for contact and diary entries
typedef struct
{
    char name[50];
    char phone[15];
    char email[50];
    char address[100];
} Contact;

typedef struct
{
    char date[15];
    char entry[200];
} DiaryEntry;

// Global variables
Contact contacts[100];
DiaryEntry diaryEntries[100];
int contactCount = 0;
int diaryCount = 0;

// Function declarations
void loadContactsFromFile();
void saveContactsToFile();
void loadDiaryEntriesFromFile();
void saveDiaryEntriesToFile();
void addContact();
void viewContacts();
void deleteContact();
void updateContact();
void addDiaryEntry();
void viewDiaryEntries();
void updateDiaryEntry();
void searchContacts();
void searchDiaryEntries();
void displayMenu();
void clearScreen();
int confirmAction(const char *action);
int isValidName(char *name);
int isValidEmail(char *email);
int isValidDate(char *date);
void printBox(const char *text);

// Main function
int main()
{
    loadContactsFromFile();
    loadDiaryEntriesFromFile();

    while (1)
    {
        displayMenu();
        int choice;
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear the input buffer

        switch (choice)
        {
        case 1:
            addContact();
            break;
        case 2:
            viewContacts();
            break;
        case 3:
            deleteContact();
            break;
        case 4:
            updateContact();
            break;
        case 5:
            addDiaryEntry();
            break;
        case 6:
            viewDiaryEntries();
            break;
        case 7:
            updateDiaryEntry();
            break;
        case 8:
            searchContacts();
            break;
        case 9:
            searchDiaryEntries();
            break;
        case 10:
            printf("\nSaving data and exiting... Goodbye!\n");
            saveContactsToFile();
            saveDiaryEntriesToFile();
            exit(0);
        default:
            printf("\nInvalid choice! Please try again.\n");
        }

        printf("\nPress Enter to return to the menu...");
        getchar();
        clearScreen();
    }

    return 0;
}

// Function to print boxes around text
void printBox(const char *text)
{
    int length = strlen(text);
    printf("\n-");
    for (int i = 0; i < length + 2; i++)
    {
        printf("-");
    }
    printf("-\n");
    printf("| %s |\n", text);
    printf("-");
    for (int i = 0; i < length + 2; i++)
    {
        printf("-");
    }
    printf("-\n");
}

// Display the main menu
void displayMenu()
{
    clearScreen();
    printBox("Connect Track Pro");
    printf("\n");
    printf("1. Add Contact\n");
    printf("2. View Contacts\n");
    printf("3. Delete Contact\n");
    printf("4. Update Contact\n");
    printf("5. Add Diary Entry\n");
    printf("6. View Diary Entries\n");
    printf("7. Update Diary Entry\n");
    printf("8. Search Contacts\n");
    printf("9. Search Diary Entries\n");
    printf("10. Exit\n");
}

// Clear the terminal screen
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Load contacts from the file
void loadContactsFromFile()
{
    FILE *file = fopen("contacts.txt", "r");
    if (file == NULL)
    {
        printf("No existing contacts file found. Starting fresh.\n");
        return;
    }

    while (fscanf(file, " %[^\n] %[^\n] %[^\n] %[^\n]",
                  contacts[contactCount].name,
                  contacts[contactCount].phone,
                  contacts[contactCount].email,
                  contacts[contactCount].address) != EOF)
    {
        contactCount++;
    }
    fclose(file);
}

// Save contacts to the file
void saveContactsToFile()
{
    FILE *file = fopen("contacts.txt", "w");
    if (file == NULL)
    {
        printf("Error saving contacts to file!\n");
        return;
    }

    for (int i = 0; i < contactCount; i++)
    {
        fprintf(file, "%s\n%s\n%s\n%s\n",
                contacts[i].name,
                contacts[i].phone,
                contacts[i].email,
                contacts[i].address);
    }
    fclose(file);
}

// Add a contact
void addContact()
{
    if (contactCount < 100)
    {
        char name[50];
        do
        {
            printf("\nEnter Name (cannot be empty): ");
            fgets(name, 50, stdin);
            name[strcspn(name, "\n")] = 0;
        } while (!isValidName(name));
        strcpy(contacts[contactCount].name, name);

        printf("Enter Phone: ");
        fgets(contacts[contactCount].phone, 15, stdin);
        contacts[contactCount].phone[strcspn(contacts[contactCount].phone, "\n")] = 0;

        char email[50];
        do
        {
            printf("Enter Email: ");
            fgets(email, 50, stdin);
            email[strcspn(email, "\n")] = 0;
        } while (!isValidEmail(email));
        strcpy(contacts[contactCount].email, email);

        printf("Enter Address: ");
        fgets(contacts[contactCount].address, 100, stdin);
        contacts[contactCount].address[strcspn(contacts[contactCount].address, "\n")] = 0;

        contactCount++;
        printf("\nContact added successfully!\n");
        saveContactsToFile(); // Save immediately
    }
    else
    {
        printf("\nContact list is full!\n");
    }
}

// View all contacts
void viewContacts()
{
    if (contactCount == 0)
    {
        printf("\nNo contacts to display.\n");
    }
    else
    {
        printf("\nContacts:\n");
        for (int i = 0; i < contactCount; i++)
        {
            printf("\nContact %d:\n", i + 1);
            printf("Name: %s\n", contacts[i].name);
            printf("Phone: %s\n", contacts[i].phone);
            printf("Email: %s\n", contacts[i].email);
            printf("Address: %s\n", contacts[i].address);
            printf("------------------------------\n");
        }
    }
}

// Delete a contact   
void deleteContact()
{
    int index;
    viewContacts();
    if (contactCount == 0)
        return;

    printf("\nEnter the contact number to delete: ");
    scanf("%d", &index);
    getchar(); // Clear the input buffer

    if (index < 1 || index > contactCount)
    {
        printf("\nInvalid contact number!\n");
    }
    else
    {
        if (confirmAction("delete this contact"))
        {
            for (int i = index - 1; i < contactCount - 1; i++)
            {
                contacts[i] = contacts[i + 1];
            }
            contactCount--;
            printf("\nContact deleted successfully!\n");
            saveContactsToFile(); // Save immediately
        }
        else
        {
            printf("\nAction cancelled.\n");
        }
    }
}

// Update a contact
void updateContact()
{
    int index;
    viewContacts();
    if (contactCount == 0)
        return;

    printf("\nEnter the contact number to update: ");
    scanf("%d", &index);
    getchar(); // Clear the input buffer

    if (index < 1 || index > contactCount)
    {
        printf("\nInvalid contact number!\n");
    }
    else
    {
        if (confirmAction("update this contact"))
        {
            index--; // Adjust for array indexing
            printf("Enter New Name (leave empty to keep current): ");
            char newName[50];
            fgets(newName, 50, stdin);
            if (strlen(newName) > 1)
            {
                newName[strcspn(newName, "\n")] = 0;
                strcpy(contacts[index].name, newName);
            }

            printf("Enter New Phone (leave empty to keep current): ");
            fgets(contacts[index].phone, 15, stdin);
            contacts[index].phone[strcspn(contacts[index].phone, "\n")] = 0;

            printf("Enter New Email (leave empty to keep current): ");
            char newEmail[50];
            fgets(newEmail, 50, stdin);
            if (strlen(newEmail) > 1)
            {
                newEmail[strcspn(newEmail, "\n")] = 0;
                strcpy(contacts[index].email, newEmail);
            }

            printf("Enter New Address (leave empty to keep current): ");
            char newAddress[100];
            fgets(newAddress, 100, stdin);
            if (strlen(newAddress) > 1)
            {
                newAddress[strcspn(newAddress, "\n")] = 0;
                strcpy(contacts[index].address, newAddress);
            }

            printf("\nContact updated successfully!\n");
            saveContactsToFile(); // Save immediately
        }
        else
        {
            printf("\nAction cancelled.\n");
        }
    }
}

// Search for contacts by name
void searchContacts()
{
    char searchTerm[50];
    printf("\nEnter the name to search: ");
    fgets(searchTerm, 50, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < contactCount; i++)
    {
        if (strstr(contacts[i].name, searchTerm) != NULL)
        {
            printf("\nFound Contact %d:\n", i + 1);
            printf("Name: %s\n", contacts[i].name);
            printf("Phone: %s\n", contacts[i].phone);
            printf("Email: %s\n", contacts[i].email);
            printf("Address: %s\n", contacts[i].address);
            printf("------------------------------\n");
            found = 1;
        }
    }

    if (!found)
    {
        printf("\nNo contacts found matching '%s'.\n", searchTerm);
    }
}

// Add a diary entry
void addDiaryEntry()
{
    if (diaryCount < 100)
    {
        char date[15];
        do
        {
            printf("Enter Date (DD-MM-YYYY): ");
            fgets(date, 15, stdin);
            date[strcspn(date, "\n")] = 0;
        } while (!isValidDate(date));
        strcpy(diaryEntries[diaryCount].date, date);

        printf("Enter Diary Entry: ");
        fgets(diaryEntries[diaryCount].entry, 200, stdin);
        diaryEntries[diaryCount].entry[strcspn(diaryEntries[diaryCount].entry, "\n")] = 0;

        diaryCount++;
        printf("\nDiary entry added successfully!\n");
        saveDiaryEntriesToFile(); // Save immediately
    }
    else
    {
        printf("\nDiary is full!\n");
    }
}

// View all diary entries
void viewDiaryEntries()
{
    if (diaryCount == 0)
    {
        printf("\nNo diary entries to display.\n");
    }
    else
    {
        printf("\nDiary Entries:\n");
        for (int i = 0; i < diaryCount; i++)
        {
            printf("\nEntry %d:\n", i + 1);
            printf("Date: %s\n", diaryEntries[i].date);
            printf("Entry: %s\n", diaryEntries[i].entry);
            printf("------------------------------\n");
        }
    }
}

// Search for diary entries by date
void searchDiaryEntries()
{
    char searchTerm[15];
    printf("\nEnter the date to search (DD-MM-YYYY): ");
    fgets(searchTerm, 15, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < diaryCount; i++)
    {
        if (strstr(diaryEntries[i].date, searchTerm) != NULL)
        {
            printf("\nFound Diary Entry %d:\n", i + 1);
            printf("Date: %s\n", diaryEntries[i].date);
            printf("Entry: %s\n", diaryEntries[i].entry);
            printf("------------------------------\n");
            found = 1;
        }
    }

    if (!found)
    {
        printf("\nNo diary entries found matching '%s'.\n", searchTerm);
    }
}

// Update a diary entry
void updateDiaryEntry()
{
    int index;
    viewDiaryEntries();
    if (diaryCount == 0)
        return;

    printf("\nEnter the entry number to update: ");
    scanf("%d", &index);
    getchar(); // Clear the input buffer

    if (index < 1 || index > diaryCount)
    {
        printf("\nInvalid entry number!\n");
    }
    else
    {
        if (confirmAction("update this diary entry"))
        {
            index--; // Adjust for array indexing
            printf("Enter New Date (leave empty to keep current): ");
            char newDate[15];
            fgets(newDate, 15, stdin);
            if (strlen(newDate) > 1)
            {
                newDate[strcspn(newDate, "\n")] = 0;
                strcpy(diaryEntries[index].date, newDate);
            }

            printf("Enter New Diary Entry (leave empty to keep current): ");
            char newEntry[200];
            fgets(newEntry, 200, stdin);
            if (strlen(newEntry) > 1)
            {
                newEntry[strcspn(newEntry, "\n")] = 0;
                strcpy(diaryEntries[index].entry, newEntry);
            }

            printf("\nDiary entry updated successfully!\n");
            saveDiaryEntriesToFile(); // Save immediately
        }
        else
        {
            printf("\nAction cancelled.\n");
        }
    }
}

// Load diary entries from file
void loadDiaryEntriesFromFile()
{
    FILE *file = fopen("diary.txt", "r");
    if (file == NULL)
    {
        printf("No existing diary file found. Starting fresh.\n");
        return;
    }

    while (fscanf(file, " %[^\n] %[^\n]",
                  diaryEntries[diaryCount].date,
                  diaryEntries[diaryCount].entry) != EOF)
    {
        diaryCount++;
    }
    fclose(file);
}

// Save diary entries to file
void saveDiaryEntriesToFile()
{
    FILE *file = fopen("diary.txt", "w");
    if (file == NULL)
    {
        printf("Error saving diary entries to file!\n");
        return;
    }

    for (int i = 0; i < diaryCount; i++)
    {
        fprintf(file, "%s\n%s\n",
                diaryEntries[i].date,
                diaryEntries[i].entry);
    }
    fclose(file);
}

// Function to confirm actions
int confirmAction(const char *action)
{
    char confirm;
    printf("\nAre you sure you want to %s? (y/n): ", action);
    scanf(" %c", &confirm);
    getchar(); // Clear the input buffer
    if (confirm == 'y' || confirm == 'Y')
    {
        return 1;
    }
    return 0;
}

// Name validation (only checks for empty name)
int isValidName(char *name)
{
    if (strlen(name) == 0)
    {
        printf("\nName cannot be empty.\n");
        return 0;
    }
    return 1;
}

// Email validation (simple validation)
int isValidEmail(char *email)
{
    if (strchr(email, '@') == NULL || strchr(email, '.') == NULL)
    {
        printf("\nInvalid email format.\n");
        return 0;
    }
    return 1;
}

// Date validation (DD-MM-YYYY format)
int isValidDate(char *date)
{
    if (strlen(date) != 10 || date[2] != '-' || date[5] != '-')
    {
        printf("\nInvalid date format. Please use DD-MM-YYYY format.\n");
        return 0;
    }
    return 1;
}
