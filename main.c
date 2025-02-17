#include <stdio.h>
#include "patient.c"

int main(void) {
    struct Patient patientList[50];
    int count = 0;
    int flag = 1;
    char again;

  while (flag == 1 )
  {

    printf("\n1. Add Patient Record\n"
           "2. View All Patients\n"
           "3. Search Patient by ID\n"
           "4. Discharge Patient\n"
           "5. Manage Doctor Schedule\n"
           "6. Exit");

    int selection;
    printf("\nEnter your selection : ");
    scanf("%d", &selection);

    switch (selection)
    {
        case 1:
            printf("You have chosen 'Add Patient Record'.");
            struct Patient newPatient;
            AddNewPatient();
            addElement(newPatient, patientList);
            printf("\nPatient record added successfully.");
            break;
        case 2:
            printf("You have chosen 'View All Patients'.");
            printArray(count, patientList);
            break;
        case 3:
            printf("You have chosen 'Search Patient by ID'.");
            // run method to search array for patient by ID number from patient class
            break;
        case 4:
            printf("You have chosen 'Discharge Patient'.");
            // run method to discharge patient from patient class
            break;
        case 5:
            printf("You have chosen 'Manage Doctor Schedule'.");
            // run method to see and edit the doctor's schedule from doctor class
            break;
        case 6:
            printf("You have chosen 'Exit'");
            // Exit program
            break;
        default:
            printf("You have chosen 'Invalid Choice'. Please input a valid selection.");
            main();
    }

      printf("\nWould you like to continue? y/n\n");
      getchar();
      scanf("%c", &again);
      if (again == 'y'|| again == 'Y') {
          flag = 1;
      }
      else
      {
          flag = 0;
          printf("\nThank you, the program will now close.");
      }
  }
    return 0;
}
