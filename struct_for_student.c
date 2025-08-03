#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//This is stucture that holds all the necessary information for a single student.

typedef struct {
    int id;         // Student ID (integer) 
    char name[50];  // Student Name (string) 
    char gender;    // Student Gender (char)   
    int age;        // Student Age (integer) 
    float gpa;      // Student GPA (float) 
} Student;


//This program manages student records, allowing users to add, display, search, save, load, and delete student records.
// Function prototypes to declare the functions used in the program.
// This allows `main` to call them before they are defined.
void addStudent(Student list[], int *count);
void displayAll(Student list[], int count);
Student* searchByID(Student list[], int count, int id);
void saveToFile(Student list[], int count);
void loadFromFile(Student list[], int *count);
void deleteStudent(Student list[], int *count, int id);


//The main function where the program execution begins.
int main() {
    //An array of Student structs to store up to 100 student records.
    Student studentList[100];
    int studentCount = 0; // Keeps track of the current number of students.
    int choice; // Variable to store the user's menu choice.

    // Automatically loads records from the file when the program starts.
    loadFromFile(studentList, &studentCount);

    //The main menu loop of the program.
    do {
        // Display menu options to the user. 
        printf("\n--- Student Record Management System ---\n");
        printf("1. Add Student Record\n");
        printf("2. Display All Records\n");
        printf("3. Search by ID\n");
        printf("4. Save to File\n");
        printf("5. Load from File\n");
        printf("6. Delete Student Record (Bonus)\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // A switch statement to handle the user's menu selection.
        switch (choice) {
            case 1:
                //Calls the function to add a new student.
                addStudent(studentList, &studentCount);
                break;
            case 2:
                // Calls the function to display all student records. 
                displayAll(studentList, studentCount);
                break;
            case 3:
                {
                    int searchId;
                    printf("Enter student ID to search: ");
                    scanf("%d", &searchId);
                    //Searches for a student by their ID.
                    Student *foundStudent = searchByID(studentList, studentCount, searchId);
                    if (foundStudent != NULL) {
                        printf("Student found:\n");
                        printf("ID: %d, Name: %s, Gender: %c, Age: %d, GPA: %.2f\n",
                               foundStudent->id, foundStudent->name, foundStudent->gender,
                               foundStudent->age, foundStudent->gpa);
                    } else {
                        printf("Student with ID %d not found.\n", searchId);
                    }
                }
                break;
            case 4:
                // Calls the function to save records to a file.
                saveToFile(studentList, studentCount);
                break;
            case 5:
                // Calls the function to load records from a file. 
                loadFromFile(studentList, &studentCount);
                break;
            case 6:
                {
                    int deleteId;
                    printf("Enter student ID to delete: ");
                    scanf("%d", &deleteId);
                    //Calls the function to delete a student record.
                    deleteStudent(studentList, &studentCount, deleteId);
                }
                break;
            case 7:
                // Exits the program. 
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}

//Function to add a new student record.
// It takes an array of Students and a pointer to the student count.
void addStudent(Student list[], int *count) {
    //Checks if the array is full.
    if (*count >= 100) {
        printf("Cannot add more students. The list is full.\n");
        return;
    }
    //Prompts the user for student details.
    printf("Enter Student ID: ");
    scanf("%d", &list[*count].id);
    printf("Enter Name: ");
    //Reads a string with spaces.
    scanf(" %[^\n]s", list[*count].name);
    printf("Enter Gender (M/F): ");
    scanf(" %c", &list[*count].gender);
    printf("Enter Age: ");
    scanf("%d", &list[*count].age);
    printf("Enter GPA: ");
    scanf("%f", &list[*count].gpa);

    //Increments the student count using a pointer.
    (*count)++;
    printf("Student added successfully.\n");
}

//Function to display all student records.
// It iterates through the array and prints each student's data.
void displayAll(Student list[], int count) {
    if (count == 0) {
        printf("No records to display.\n");
        return;
    }
    //Prints data in a tabular format for readability.
    printf("\n--- All Student Records ---\n");
    printf("%-5s %-20s %-8s %-5s %-5s\n", "ID", "Name", "Gender", "Age", "GPA");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-20s %-8c %-5d %-5.2f\n",
               list[i].id, list[i].name, list[i].gender, list[i].age, list[i].gpa);
    }
}

//Function to search for a student by ID.
// Returns a pointer to the found Student struct, or NULL if not found.
Student* searchByID(Student list[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (list[i].id == id) {
            // [Source: 31] Returns a pointer to the matching student struct.
            return &list[i];
        }
    }
    // Returns NULL if no student with the given ID is found.
    return NULL;
}

//Function to save all records to "students.txt".
void saveToFile(Student list[], int count) {
    // Opens the file in write mode, which creates or overwrites the file.
    FILE *file = fopen("students.txt", "w");
    //Error handling in case the file cannot be opened.
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }
    //Writes each student's data to the file in a readable format.
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,%s,%c,%d,%.2f\n",
                list[i].id, list[i].name, list[i].gender, list[i].age, list[i].gpa);
    }
    fclose(file); // Closes the file to save changes.
    printf("Records saved to students.txt\n");
}

//Function to load records from "students.txt".
void loadFromFile(Student list[], int *count) {
    // Opens the file in read mode.
    FILE *file = fopen("students.txt", "r");
    if (file == NULL) {
        printf("No existing data file found. Starting with an empty list.\n");
        return;
    }
    *count = 0; // Resets the count before loading from the file.
    //Reads and parses each record from the file until the end.
    while (*count < 100 && fscanf(file, "%d,%[^,],%c,%d,%f\n",
                                  &list[*count].id, list[*count].name, &list[*count].gender,
                                  &list[*count].age, &list[*count].gpa) != EOF) {
        (*count)++;
    }
    fclose(file); // Closes the file.
    printf("Records loaded from students.txt\n");
}

// [Source: 48, 49] Bonus function to delete a student record by ID.
void deleteStudent(Student list[], int *count, int id) {
    int found = -1; // Index of the student to be deleted.

    // Finds the index of the student with the matching ID.
    for (int i = 0; i < *count; i++) {
        if (list[i].id == id) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        //Shifts all subsequent elements one position to the left.
        for (int i = found; i < *count - 1; i++) {
            list[i] = list[i + 1];
        }
        (*count)--; // Decrements the total student count.
        printf("Student with ID %d has been deleted.\n", id);
    } else {
        printf("Student with ID %d not found.\n", id);
    }
}