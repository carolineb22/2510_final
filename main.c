#include <stdio.h>
#include <string.h>

#define MAX_PATIENTS 50

int totalPatients = 0;

struct Patient
{
    int patientID;
    char name[30];
    int age;
    char diagnosis[100];
    int roomNumber;
};

struct Patient patientList[MAX_PATIENTS];

void menu(void);
void addPatient();
void viewPatients();
void searchPatients();
void dischargePatients();
void manageDoctorSchedule();
int patientIdExists();

int main(void)
{
    menu();
    return 0;
}

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

void addPatient()
{
    if (totalPatients >= MAX_PATIENTS)
    {
        printf("Hospital is full! cannot add more patients.\n");
        return;
    }
    struct Patient newPatient;

    printf("\nEnter patient ID: ");
    scanf("%d", &newPatient.patientID);
    while (getchar() != '\n') {}

    if (newPatient.patientID <= 0 ||
        newPatient.patientID > MAX_PATIENTS ||
        patientIdExists(patientList, totalPatients, newPatient.patientID) != -1)
    {
        printf("Invalid or duplicate ID!\n");
        return;
    }

    printf("Please enter patients full name: ");
    fgets(newPatient.name, sizeof(newPatient.name), stdin);
    newPatient.name[strcspn(newPatient.name, "\n")] = 0;

    printf("Please enter patient's age: ");
    scanf("%d", &newPatient.age);
    while (getchar() != '\n') {}

    if (newPatient.age < 0)
    {
        printf("Age cannot be less than 0.\n");
    }

    printf("Please enter patient's room number: ");
    scanf("%d", &newPatient.roomNumber);
    while (getchar() != '\n') {}

    if (newPatient.roomNumber < 0)
    {
        printf("Room Number cannot be less than 0.\n");
    }

    printf("Please enter patient's diagnosis name or reason for admission: ");
    fgets(newPatient.diagnosis, sizeof(newPatient.diagnosis), stdin);
    newPatient.diagnosis[strcspn(newPatient.diagnosis, "\n")] = 0;

    patientList[totalPatients] = newPatient;
    totalPatients++;

    printf("Patient added successfully!\n");
}

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

void searchPatients()
{
    if (totalPatients == 0)
    {
        printf("No patients booked!\n");
        return;
    }

    int choice, id, index = -1;
    char name[30];

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
        printf("Enter Patient Name: ");
        fgets(name, 30, stdin);
        name[strcspn(name, "\n")] = 0;

        for (int i = 0; i < totalPatients; i++)
        {
            if (strcmp(patientList[i].name, name) == 0)
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

void dischargePatients()
{
    if (totalPatients == 0)
    {
        printf("No patients booked!\n");
        return;
    }

    int id, index;

    printf("Enter Patient ID to Discharge: ");
    scanf("%d", &id);
    while (getchar() != '\n') {}

    index = patientIdExists(patientList, totalPatients, id);

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

void manageDoctorSchedule()
{
  char choice;
  int choice2;
  int choice3;
  int dayChoice;
  int shiftChoice;
  char name[50];


    char string_array[8][4][256] = { {"SCHEDULE", "\tMorning", "\t\tAfternoon", "\tEvening"},
                                     {"Sunday\t", "\t[EMPTY]", "\t\t[EMPTY]", "\t\t[EMPTY]"},
                                     {"Monday\t", "\t[EMPTY]", "\t\t[EMPTY]", "\t\t[EMPTY]"},
                                     {"Tuesday\t", "\t[EMPTY]", "\t\t[EMPTY]", "\t\t[EMPTY]"},
                                     {"Wednesday", "\t[EMPTY]", "\t\t[EMPTY]", "\t\t[EMPTY]"},
                                     {"Thursday", "\t[EMPTY]", "\t\t[EMPTY]", "\t\t[EMPTY]"},
                                     {"Friday\t", "\t[EMPTY]", "\t\t[EMPTY]", "\t\t[EMPTY]"},
                                     {"Saturday", "\t[EMPTY]", "\t\t[EMPTY]", "\t\t[EMPTY]"}};

    int i, j;
    for (i = 0; i < 8; i++) {
        printf("\n");
        for (j = 0; j < 4; j++) {
            printf("%s", string_array[i][j]);
        }
    }
    printf("\nWould you like to edit the schedule? (Y/N): ");
    scanf(" %c", &choice); // Note the space before %c to consume any leftover newline
    if (choice == 'N' || choice == 'n')
    {
      printf("Closing schedule...\n");
    }
    else if (choice == 'Y' || choice == 'y')
    {
      printf("Would you like to (1) add or (2) delete from the schedule?: ");
      scanf("%d", &choice2);
      if (choice2 == 1) //add to schedule
      {
        printf("Please enter the name of the doctor you would like to add to the schedule: ");
              while ((getchar()) != '\n');
              scanf("%[^\n]%*c", name);


            printf("Which day would you like to schedule %s ?\n", name);
            printf("1. Sunday \n2. Monday \n3. Tuesday \n4. Wednesday \n5. Thursday \n6. Friday \n7. Saturday\n");

            while ((getchar()) != '\n');
            scanf("%d", &dayChoice);
            printf("Which shift would you like to schedule %s ?\n", name);
          printf("1. Morning \n2. Afternoon \n3. Evening\n");
          while ((getchar()) != '\n');
            scanf("%d", &shiftChoice);

          char temp[256]; // Temporary string to hold the formatted name
          sprintf(temp, "\t%s", name); // Format the name
          strcpy(string_array[dayChoice][shiftChoice], temp); // Copy the formatted string
          printf("Shift added successfully!\n");
        }
        else if (choice == 2) //delete from schedule
        {
            printf("do this");
        }
        else
        {
            printf("Invalid choice!"); //TODO Loop back to choice
        }
    }
    else
    {
      printf("Invalid choice!"); //TODO loop back to choice
    }
}

int patientIdExists(struct Patient arr[], int size, int id)
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