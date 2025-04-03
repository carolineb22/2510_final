#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INITIAL_PATIENTS 50
#define MAX_NAME_LENGTH 30
#define EXPANSION_FACTOR 2
#define PATIENT_FILE "patients.txt"
#define SCHEDULE_FILE "schedule.txt"

#define BACKUP_PATIENT_FILE "backup_patients.txt"
#define BACKUP_SCHEDULE_FILE "backup_schedule.txt"

int totalPatients = 0;

struct Patient
{
    int patientID;
    char name[MAX_NAME_LENGTH];
    int age;
    char diagnosis[250];
    int roomNumber;
    struct Patient* next;  // Pointer to next patient
};

// Head of the patient linked list
struct Patient* patientList = NULL;;

/* 3D array to store our formatted doctor schedule. */
char string_array[8][4][MAX_NAME_LENGTH] = {
    {"SCHEDULE", "\tMorning", "\tAfternoon", "\tEvening"},
    {"Sunday\t", "\t[EMPTY]", "\t[EMPTY]", "\t[EMPTY]"},
    {"Monday\t", "\t[EMPTY]", "\t[EMPTY]", "\t[EMPTY]"},
    {"Tuesday\t", "\t[EMPTY]", "\t[EMPTY]", "\t[EMPTY]"},
    {"Wednesday", "\t[EMPTY]", "\t[EMPTY]", "\t[EMPTY]"},
    {"Thursday", "\t[EMPTY]", "\t[EMPTY]", "\t[EMPTY]"},
    {"Friday\t", "\t[EMPTY]", "\t[EMPTY]", "\t[EMPTY]"},
    {"Saturday", "\t[EMPTY]", "\t[EMPTY]", "\t[EMPTY]"}
};

// Function prototypes.
void menu(void);

void addPatient();
void viewPatients();
void searchPatients();
void dischargePatients();
void manageDoctorSchedule();

struct Patient* findPatientById(int id);
void saveToFile(const char *fileName);
void loadFromFile(const char *fileName);
void backupData();
void restoreData();
void freePatientList();
int validIntegerInput(int *value);
void getValidStringInput(char *buffer, int maxLength, const char *prompt);


int patientIdExists();

/* Program entry point. */
int main() {
    loadFromFile(PATIENT_FILE);
    menu();
    saveToFile(PATIENT_FILE);
    freePatientList();
    return 0;
}

// Function to save patient records and doctor schedules to files.
void saveToFile(const char* fileName) {
    FILE* file = fopen(fileName, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    struct Patient* current = patientList;
    while (current) {
        fprintf(file, "%d,%s,%d,%d,%s\n",
                current->patientID,
                current->name,
                current->age,
                current->roomNumber,
                current->diagnosis);
        current = current->next;
    }

    fclose(file);
}

// Function to load patient records and doctor schedules from files.
void loadFromFile(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (!file) {
        printf("No existing patient records found or error opening file.\n");
        return;
    }

    struct Patient* newPatient;
    while ((newPatient = malloc(sizeof(struct Patient))) &&
           fscanf(file, "%d,%29[^,],%d,%d,%249[^\n]\n",
                  &newPatient->patientID,
                  newPatient->name,
                  &newPatient->age,
                  &newPatient->roomNumber,
                  newPatient->diagnosis) == 5) {
        if (findPatientById(newPatient->patientID)) {  // Prevent duplicate IDs
            printf("Warning: Duplicate patient ID %d found in file. Skipping entry.\n", newPatient->patientID);
            free(newPatient);
            continue;
        }
        newPatient->next = patientList;
        patientList = newPatient;
                  }

    fclose(file);
}

// Call saveToFile function with Backup File paths
void backupData() {
    saveToFile(BACKUP_PATIENT_FILE);
}

// Call loadFromFile function with Backup File paths
void restoreData() {
    freePatientList();
    loadFromFile(BACKUP_PATIENT_FILE);
}

/* When called, asks the user for which action they would like to do.
 * Calls functions to the respective actions and acts as the fallback if
 * any actions fail due to invalid inputs. */
void menu()
{
    int selection;
    do
    {
        printf("\n1. Add Patient Record\n"
            "2. View All Patients\n"
            "3. Search Patient\n"
            "4. Discharge Patient\n"
            "5. Manage Doctor Schedule\n"
            "6. Exit");
        printf("\nEnter your selection : ");
        scanf("%d", &selection);
        while(getchar() != '\n'){}

        switch(selection)
        {
        case 1: addPatient();
            break;
        case 2: viewPatients();
            break;
        case 3: searchPatients();
            break;
        case 4: dischargePatients();
            break;
        case 5: manageDoctorSchedule();
            break;
        case 6: printf("Exiting Program...\n");
            break;
        default: printf("Invalid choice! Try again.\n");
        }
    }
    while(selection != 6);
}

/* Add a new patient to the linked list */
void addPatient() {
    struct Patient* newPatient = (struct Patient*)malloc(sizeof(struct Patient));
    if (!newPatient) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter patient ID: ");
    if (scanf("%d", &newPatient->patientID) != 1) {  // Invalid input check
        printf("Invalid ID input! Please enter a number.\n");
        free(newPatient);
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');  // Clear input buffer


    // Ensure ID is unique
    if (findPatientById(newPatient->patientID) != NULL) {
        printf("Patient ID already exists!\n");
        free(newPatient);
        return;
    }

    printf("Enter patient full name: ");
    fgets(newPatient->name, sizeof(newPatient->name), stdin);
    newPatient->name[strcspn(newPatient->name, "\n")] = 0;
    if (strlen(newPatient->name) == 0) {  // Empty input check
        printf("Name cannot be empty!\n");
        free(newPatient);
        return;
    }

    printf("Enter patient age: ");
    if (scanf("%d", &newPatient->age) != 1 || newPatient->age <= 0 || newPatient->age > 120) {  // Age validation
        printf("Invalid age! Please enter a valid number between 1 and 120.\n");
        free(newPatient);
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    printf("Enter room number: ");
    if (scanf("%d", &newPatient->roomNumber) != 1 || newPatient->roomNumber <= 0) {  // Room number validation
        printf("Invalid room number! Please enter a positive integer.\n");
        free(newPatient);
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    printf("Enter diagnosis: ");
    fgets(newPatient->diagnosis, sizeof(newPatient->diagnosis), stdin);
    newPatient->diagnosis[strcspn(newPatient->diagnosis, "\n")] = 0;
    if (strlen(newPatient->diagnosis) == 0) {  // Empty input check
        printf("Diagnosis cannot be empty!\n");
        free(newPatient);
        return;
    }

    // Add patient to the list (Insert at Head)
    newPatient->next = patientList;
    patientList = newPatient;
    totalPatients++;

    printf("Patient added successfully!\n");
}

/* Prints the list of patients to console.
 * It won't print any if there aren't any patients. */
void viewPatients() {
    if (!patientList) {
        printf("No patients booked!\n");
        return;
    }

    struct Patient* current = patientList;
    printf("\nPatients:\n");
    printf("ID\tName\t\tAge\tRoom #\tDiagnosis\n");

    while (current) {
        printf("%d\t%-30s\t%d\t%d\t%s\n",
               current->patientID,
               current->name,
               current->age,
               current->roomNumber,
               current->diagnosis);
        current = current->next;
    }
}

/* Allows the searching of patients in the system.
 * It can search by either ID or name. Name must have an exact match. */
void searchPatients() {
    if (!patientList) {
        printf("No patients booked!\n");
        return;
    }

    int choice, id;
    printf("Search by (1) ID or (2) Name: ");
    scanf("%d", &choice);
    while(getchar() != '\n');

    if (choice == 1) {
        printf("Enter Patient ID: ");
        scanf("%d", &id);
        while(getchar() != '\n');

        struct Patient* found = findPatientById(id);
        if (found) {
            printf("Patient found - ID: %d, Name: %s, Age: %d, Room #: %d, Diagnosis: %s\n",
                   found->patientID, found->name, found->age, found->roomNumber, found->diagnosis);
        } else {
            printf("Patient not found.\n");
        }
    } else {
        char nameToSearch[MAX_NAME_LENGTH];
        printf("Enter Patient Name: ");
        fgets(nameToSearch, sizeof(nameToSearch), stdin);
        nameToSearch[strcspn(nameToSearch, "\n")] = 0;

        struct Patient* current = patientList;
        while (current) {
            if (strcmp(current->name, nameToSearch) == 0) {
                printf("Patient found - ID: %d, Name: %s, Age: %d, Room #: %d, Diagnosis: %s\n",
                       current->patientID, current->name, current->age, current->roomNumber, current->diagnosis);
                return;
            }
            current = current->next;
        }
        printf("Patient not found.\n");
    }
}

// Find patient by ID
struct Patient* findPatientById(int id) {
    struct Patient* current = patientList;
    while (current) {
        if (current->patientID == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/* Discharges a patient by searching by ID. */
    void dischargePatients() {
        if (!patientList) {
            printf("No patients booked!\n");
            return;
        }

        int id;
        printf("Enter Patient ID to Discharge: ");
        if (scanf("%d", &id) != 1) {  // Validate ID input
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            return;
        }
        while (getchar() != '\n');

        struct Patient* current = patientList;
        struct Patient* previous = NULL;

        while (current) {
            if (current->patientID == id) {
                if (previous) {
                    previous->next = current->next;
                } else {
                    patientList = current->next;
                }
                free(current);
                printf("Patient %d discharged.\n", id);
                return;
            }
            previous = current;
            current = current->next;
        }

        printf("Patient ID not found.\n");
    }

/* All in one function to manage doctor schedule. First prints the entire
 * schedule then gives the user the option to edit or remove a doctor from
 * the schedule. */
void manageDoctorSchedule()
{
    char editScheduleChoice;
    int addOrDeleteChoice;
    int dayChoice;
    int shiftChoice;

    // Display the current schedule
    for (int i = 0; i < 8; i++) {
        printf("\n");
        for (int j = 0; j < 4; j++) {
            printf("%-20s", string_array[i][j]);
        }
    }

    printf("\nWould you like to edit the schedule? (Y/N): ");
    scanf(" %c", &editScheduleChoice);
    while(getchar() != '\n'){}

    if(editScheduleChoice == 'Y' || editScheduleChoice == 'y')
    {
        printf("Would you like to (1) add or (2) delete from the schedule?: ");
        if (scanf("%d", &addOrDeleteChoice) != 1) {
            printf("Invalid input! Please enter 1 or 2.\n");
            while (getchar() != '\n');
            return;
        }
        while(getchar() != '\n'){}

        // Add to schedule
        if(addOrDeleteChoice == 1)
        {
            char doctorName[MAX_NAME_LENGTH];
            char formattedName[MAX_NAME_LENGTH] = "";

            printf("Please enter the name of the doctor you would like to add to the schedule: ");
            fgets(doctorName, sizeof(doctorName), stdin);
            doctorName[strcspn(doctorName, "\n")] = 0;

            if (strlen(doctorName) == 0) {  // Empty name check
                printf("Doctor name cannot be empty!\n");
                return;
            }

            if(strlen(doctorName) < MAX_NAME_LENGTH - 1)
            {
                strcat(formattedName, doctorName);
            }
            else
            {
                printf("Error: Doctor's name is too long! Please enter a shorter name.\n");
                return; // Exit the function if the name is too long
            }

            printf("Which day would you like to schedule %s?", doctorName);
            printf("\n1. Sunday \n2. Monday \n3. Tuesday \n4. Wednesday \n5. Thursday \n6. Friday \n7. Saturday\n");
            if (scanf("%d", &dayChoice) != 1 || dayChoice < 1 || dayChoice > 7) {
                printf("Invalid day! Please enter a number from 1 to 7.\n");
                while (getchar() != '\n');
                return;
            }
            while(getchar() != '\n'){}

            printf("Which shift would you like to schedule %s?", doctorName);
            printf("\n1. Morning \n2. Afternoon \n3. Evening\n");
            if (scanf("%d", &shiftChoice) != 1 || shiftChoice < 1 || shiftChoice > 3) {
                printf("Invalid shift! Please enter 1, 2, or 3.\n");
                while (getchar() != '\n');
                return;
            }
            while(getchar() != '\n'){}
        }

        // Remove from schedule
        else if(addOrDeleteChoice == 2)
        {
            printf("Enter which day to clear: ");
            printf("\n1. Sunday \n2. Monday \n3. Tuesday \n4. Wednesday \n5. Thursday \n6. Friday \n7. Saturday\n");
            scanf("%d", &dayChoice);
            while(getchar() != '\n')
            {
            }

            printf("Enter the Shift to clear: ");
            printf("\n1. Morning \n2. Afternoon \n3. Evening\n");
            scanf("%d", &shiftChoice);
            while(getchar() != '\n')
            {
            }

            // Validate choices
            if(dayChoice >= 1 && dayChoice <= 7 &&
                shiftChoice >= 1 && shiftChoice <= 3)
            {
                strcpy(string_array[dayChoice][shiftChoice], "\t[EMPTY]");
                printf("Shift removed successfully!\n");
            }
            else
            {
                printf("Invalid day or shift choice!\n");
            }
        }
        else
        {
            printf("Invalid choice.\n");
        }
    }
    else
    {
        printf("Closing schedule...\n");
    }
}

/* Helper function to see if a given ID exists in the given array.
 * If the ID exists, the function returns the given ID. If not, returns -1. */
int patientIdExists(struct Patient* head, int id) {
    struct Patient* current = head;
    while (current) {
        if (current->patientID == id) {
            return 1; // True, ID exists
        }
        current = current->next;
    }
    return 0; // False, ID does not exist
}


// Free the linked list
void freePatientList() {
    struct Patient* current = patientList;
    while (current) {
        struct Patient* temp = current;
        current = current->next;
        free(temp);
    }
}

int validIntegerInput(int *value) {
    char buffer[50];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    if (sscanf(buffer, "%d", value) != 1) {
        printf("Invalid input! Please enter a number.\n");
        return 0;
    }
    return 1;
}

void getValidStringInput(char *buffer, int maxLength, const char *prompt) {
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, maxLength, stdin)) {
            buffer[strcspn(buffer, "\n")] = 0;  // Remove newline
            if (strlen(buffer) > 0) {
                return;
            }
        }
        printf("Input cannot be empty! Try again.\n");
    }
}

