#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 50
#define MAX_NAME_LENGTH 30

int totalPatients = 0;

struct Patient
{
    int patientID;
    char name[MAX_NAME_LENGTH];
    int age;
    char diagnosis[100];
    int roomNumber;
};

struct Patient patientList[MAX_PATIENTS];

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
int patientIdExists();

/* Program entry point. */
int main(void)
{
    menu();
    return 0;
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
        while (getchar() != '\n') {}

        switch (selection)
        {
            case 1: addPatient(); break;
            case 2: viewPatients(); break;
            case 3: searchPatients(); break;
            case 4: dischargePatients(); break;
            case 5: manageDoctorSchedule(); break;
            case 6: printf("Exiting Program...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (selection != 6);
}

/* Creates a newPatient struct and after taking user input,
 * adds it to the array of patient structs. */
void addPatient()
{
    if (totalPatients >= MAX_PATIENTS)
    {
        printf("Hospital is full! cannot add more patients.\n");
        return;
    }
    struct Patient newPatient;

    { // Set int values to -1 so that input validation can catch char inputs.
        newPatient.patientID = -1;
        newPatient.age = -1;
        newPatient.roomNumber = -1;
    }

    // input validation flags
    int validID = 0,
        validName = 0,
        validAge = 0,
        validRoomNumber = 0,
        validDiagnosis = 0;

    do
    {
        printf("\nEnter patient ID: ");
        scanf("%d", &newPatient.patientID);
        while (getchar() != '\n') {}

        if (newPatient.patientID <= 0 ||
            newPatient.patientID > MAX_PATIENTS ||
            patientIdExists(patientList, totalPatients, newPatient.patientID) != -1)
        {
            printf("Invalid or duplicate ID!\n");
        }
        else
        {
            validID = 1;
        }
    } while (validID != 1);

    do
    {
        printf("Please enter patients full name: ");
        fgets(newPatient.name, sizeof(newPatient.name), stdin);
        newPatient.name[strcspn(newPatient.name, "\n")] = 0;

        if (isspace(newPatient.name[0]))
        {
            printf("Name cannot contain leading whitespace.\n");
        }
        else
        {
            validName = 1;
        }
    } while (validName != 1);

    do
    {
        printf("Please enter patient's age: ");
        scanf("%d", &newPatient.age);
        while (getchar() != '\n') {}

        if (newPatient.age < 0)
        {
            printf("Invalid age.\n");
        }
        else
        {
            validAge = 1;
        }
    } while (validAge != 1);

    do
    {
        printf("Please enter patient's room number: ");
        scanf("%d", &newPatient.roomNumber);
        while (getchar() != '\n') {}

        if (newPatient.roomNumber < 0)
        {
            printf("Invalid Room Number\n");
        }
        else
        {
            validRoomNumber = 1;
        }
    } while (validRoomNumber != 1);

    do
    {
        printf("Please enter patient's diagnosis name or reason for admission: ");
        fgets(newPatient.diagnosis, sizeof(newPatient.diagnosis), stdin);
        newPatient.diagnosis[strcspn(newPatient.diagnosis, "\n")] = 0;

        if (isspace(newPatient.diagnosis[0]))
        {
            printf("Diagnosis cannot contain leading whitespace.");
        }
        else
        {
            validDiagnosis = 1;
        }
    } while (validDiagnosis != 1);

    patientList[totalPatients] = newPatient;
    totalPatients++;

    printf("Patient added successfully!\n");
}

/* Prints the list of patients to console.
 * It won't print any if there aren't any patients. */
void viewPatients()
{
    if (totalPatients == 0)
    {
        printf("No patients booked!\n");
        return;
    }

    printf("\nPatients:\n");
    printf("ID\tName\t\t\t\tage\tRoom #\tDiagnosis\n");
    for (int i = 0; i < totalPatients; i++)
    {
        printf("%d\t%-30s\t%d\t%d\t%s\n",
            patientList[i].patientID,
            patientList[i].name,
            patientList[i].age,
            patientList[i].roomNumber,
            patientList[i].diagnosis);
    }
}

/* Allows the searching of patients in the system.
 * It can search by either ID or name. Name must have an exact match. */
void searchPatients()
{
    if (totalPatients == 0)
    {
        printf("No patients booked!\n");
        return;
    }

    int choice, id, index = -1;

    printf("Search by (1) ID or (2) Name: ");
    scanf("%d", &choice);
    while (getchar() != '\n') {}

    if (choice == 1)
    {
        printf("Enter Patient ID: ");
        scanf("%d", &id);
        while (getchar() != '\n') {}

        index = patientIdExists(patientList, totalPatients, id);
    }
    else if (choice == 2)
    {
        char nameToSearch[MAX_NAME_LENGTH];
        printf("Enter Patient Name: ");
        fgets(nameToSearch, sizeof(nameToSearch), stdin);
        nameToSearch[strcspn(nameToSearch, "\n")] = 0;

        for (int i = 0; i < totalPatients; i++)
        {
            if (strcmp(patientList[i].name, nameToSearch) == 0)
            {
                index = i;
                break;
            }
        }
    }
    if (index != -1)
    {
        printf("Patient found - ID: %d, Name: %s, Age: %d, Room #: %d, Diagnosis: %s\n",
                patientList[index].patientID,
                patientList[index].name,
                patientList[index].age,
                patientList[index].roomNumber,
                patientList[index].diagnosis);
    }
    else
    {
        printf("Patient not found.\n");
    }
}

/* Discharges a patient by searching by ID. */
void dischargePatients()
{
    if (totalPatients == 0)
    {
        printf("No patients booked!\n");
        return;
    }

    int id;

    printf("Enter Patient ID to Discharge: ");
    scanf("%d", &id);
    while (getchar() != '\n') {}

    const int index = patientIdExists(patientList, totalPatients, id);

    if (index != -1)
    {
        for (int i = index; i < totalPatients; i++)
        {
            patientList[i] = patientList[i + 1];
        }
        totalPatients--;
        printf("Patient %d discharged.\n", id);
    }
    else
    {
        printf("Patient ID not found.\n");
    }
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
    for (int i = 0; i < 8; i++)
    {
        printf("\n");
        for (int j = 0; j < 4; j++)
        {
            printf("%-20s", string_array[i][j]);
        }
    }

    printf("\nWould you like to edit the schedule? (Y/N): ");
    scanf(" %c", &editScheduleChoice);

    if (editScheduleChoice == 'Y' || editScheduleChoice == 'y')
    {
        printf("Would you like to (1) add or (2) delete from the schedule?: ");
        scanf("%d", &addOrDeleteChoice);
        while (getchar() != '\n') {}

        // Add to schedule
        if (addOrDeleteChoice == 1)
        {
            char doctorName[MAX_NAME_LENGTH];

            printf("Please enter the name of the doctor you would like to add to the schedule: ");
            fgets(doctorName, sizeof(doctorName), stdin);
            doctorName[strcspn(doctorName, "\n")] = 0;

            printf("Which day would you like to schedule %s?", doctorName);
            printf("\n1. Sunday \n2. Monday \n3. Tuesday \n4. Wednesday \n5. Thursday \n6. Friday \n7. Saturday\n");

            scanf("%d", &dayChoice);
            while (getchar() != '\n') {}

            printf("Which shift would you like to schedule %s?", doctorName);
            printf("\n1. Morning \n2. Afternoon \n3. Evening\n");

            scanf("%d", &shiftChoice);
            while (getchar() != '\n') {}

            // Validate choices
            if (dayChoice >= 1 && dayChoice <= 7 &&
                shiftChoice >= 1 && shiftChoice <= 3)
            {
                char formattedName[MAX_NAME_LENGTH] = {'\t'};
                strcat(formattedName, doctorName);

                strcpy(string_array[dayChoice][shiftChoice], formattedName);
                printf("Shift added successfully!\n");
            }
            else
            {
                printf("Invalid day or shift choice!\n");
            }
        }

        // Remove from schedule
        else if (addOrDeleteChoice == 2)
        {
            printf("Enter which day to clear: ");
            printf("\n1. Sunday \n2. Monday \n3. Tuesday \n4. Wednesday \n5. Thursday \n6. Friday \n7. Saturday\n");
            scanf("%d", &dayChoice);
            while (getchar() != '\n') {}

            printf("Enter the Shift to clear: ");
            printf("\n1. Morning \n2. Afternoon \n3. Evening\n");
            scanf("%d", &shiftChoice);
            while (getchar() != '\n') {}

            // Validate choices
            if (dayChoice >= 1 && dayChoice <= 7 &&
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
int patientIdExists(struct Patient arr[], const int size, const int id)
{
    for (int i = 0; i < size; i++)
    {
        if (arr[i].patientID == id)
        {
            return i;
        }
    }
    return -1;
}