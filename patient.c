#include <stdio.h>

int patientID = 0;

struct Patient {
    int patientID;
    char name[100];
    int age;
    char diagnosis[1000];
    int roomNumber;
};

struct Patient AddNewPatient() {
    patientID++;
    struct Patient newPatient;
    newPatient.patientID = patientID;

    printf("\nPatient ID: %d", patientID);
    getchar();

    printf("\nPlease enter patients full name: ");
    fgets(newPatient.name, sizeof(newPatient.name), stdin);

    printf("Please enter patient's age: ");
    scanf("%d", &newPatient.age);
    getchar();

    printf("Please enter patient's diagnosis name or reason for admission: ");
    fgets(newPatient.diagnosis, sizeof(newPatient.diagnosis), stdin);

    printf("Please enter patient's room number: ");
    scanf("%d", &newPatient.roomNumber);

    return newPatient;
}

void addElement(struct Patient newPatient,
                struct Patient patientList[])
{
    static int count = 0; // Keep track of the number of patients added

    if (count < 50) // Check if there's space in the array
    {
        // Add the new patient to the array
        patientList[count] = newPatient;
        count++;
    }
    else
    {
        printf("Patient list is full!\n");
    }
}

void printArray(int count,
                struct Patient patientList[])
{
    printf("\nCurrent Patient Records: ");

    for (int i = 0; i < count; i++)
    {
        printf("\n%d. Patient ID: %d"
               "\nName: %s"
               "\nAge: %d"
               "\nDiagnosis: %s"
               "\nRoom Number: %d",
               i + 1,
               patientList[i].patientID,
               patientList[i].name,
               patientList[i].age,
               patientList[i].diagnosis,
               patientList[i].roomNumber);
    }
}

