#include <stdio.h>

int main(void) {
    
    printf("\n1. Add Patient Record\n"
           "2. View All Patients\n"
           "3. Search Patient by ID\n"
           "4. Discharge Patient\n"
           "5. Manage Doctor Schedule\n"
           "6. Exit");

    int selection;

    switch (selection)
    {
        case 1:
            printf("You have chosen 'Add Patient Record'.");
            // run method to add patient from patient class
            break;
        case 2:
            printf("You have chosen 'View All Patients'.");
            // run method to print array list from patient class
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
    return 0;
}
