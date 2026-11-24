#include <stdio.h>

#define MAX_STUDENTS 100
#define SUBJECTS 3
#define FILE_NAME "students.txt"

typedef struct {
    int rollNumber;
    char name[50];
    float marks[SUBJECTS];
} Student;

float calculateTotal(const Student *student) {
    float total = 0;

    for (int i = 0; i < SUBJECTS; i++) {
        total += student->marks[i];
    }

    return total;
}

float calculateAverage(const Student *student) {
    return calculateTotal(student) / SUBJECTS;
}

int findStudent(const Student students[], int count, int rollNumber) {
    for (int i = 0; i < count; i++) {
        if (students[i].rollNumber == rollNumber) {
            return i;
        }
    }

    return -1;
}

void displayStudent(const Student *student) {
    printf("\nRoll number: %d\n", student->rollNumber);
    printf("Name: %s\n", student->name);
    printf("Marks: %.2f, %.2f, %.2f\n",
           student->marks[0], student->marks[1], student->marks[2]);
    printf("Total: %.2f\n", calculateTotal(student));
    printf("Average: %.2f\n", calculateAverage(student));
}

void displayAllStudents(const Student students[], int count) {
    if (count == 0) {
        printf("\nNo student records found.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        displayStudent(&students[i]);
    }
}

int addStudent(Student students[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("\nStudent list is full.\n");
        return 0;
    }

    Student newStudent;

    printf("Enter roll number: ");
    if (scanf("%d", &newStudent.rollNumber) != 1) {
        printf("Invalid roll number.\n");
        return 0;
    }

    if (findStudent(students, *count, newStudent.rollNumber) != -1) {
        printf("A student with that roll number already exists.\n");
        return 0;
    }

    printf("Enter name: ");
    scanf(" %49[^\n]", newStudent.name);

    for (int i = 0; i < SUBJECTS; i++) {
        printf("Enter marks for subject %d (0-100): ", i + 1);

        if (scanf("%f", &newStudent.marks[i]) != 1 ||
            newStudent.marks[i] < 0 || newStudent.marks[i] > 100) {
            printf("Invalid marks. Please enter a value from 0 to 100.\n");
            return 0;
        }
    }

    students[*count] = newStudent;
    (*count)++;

    printf("\nStudent record added successfully.\n");
    return 1;
}

void searchStudent(const Student students[], int count) {
    int rollNumber;

    printf("Enter roll number to search: ");
    if (scanf("%d", &rollNumber) != 1) {
        printf("Invalid roll number.\n");
        return;
    }

    int index = findStudent(students, count, rollNumber);

    if (index == -1) {
        printf("\nStudent not found.\n");
    } else {
        displayStudent(&students[index]);
    }
}

void saveStudents(const Student students[], int count) {
    FILE *file = fopen(FILE_NAME, "w");

    if (file == NULL) {
        printf("\nError: Could not open the file for saving.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d|%s|%.2f|%.2f|%.2f\n",
                students[i].rollNumber,
                students[i].name,
                students[i].marks[0],
                students[i].marks[1],
                students[i].marks[2]);
    }

    fclose(file);
    printf("\nRecords saved to %s.\n", FILE_NAME);
}

int loadStudents(Student students[]) {
    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL) {
        return 0;  // The file may not exist the first time the program runs.
    }

    int count = 0;

    while (count < MAX_STUDENTS &&
           fscanf(file, " %d|%49[^|]|%f|%f|%f",
                  &students[count].rollNumber,
                  students[count].name,
                  &students[count].marks[0],
                  &students[count].marks[1],
                  &students[count].marks[2]) == 5) {
        count++;
    }

    fclose(file);
    return count;
}

int main(void) {
    Student students[MAX_STUDENTS];
    int count = loadStudents(students);
    int choice;

    printf("Loaded %d student record(s).\n", count);

    do {
        printf("\n--- Student Record and Result Management ---\n");
        printf("1. Add student\n");
        printf("2. Display all students\n");
        printf("3. Search student\n");
        printf("4. Save records\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid choice. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                if (addStudent(students, &count)) {
                    saveStudents(students, count);
                }
                break;

            case 2:
                displayAllStudents(students, count);
                break;

            case 3:
                searchStudent(students, count);
                break;

            case 4:
                saveStudents(students, count);
                break;

            case 5:
                saveStudents(students, count);
                printf("Goodbye!\n");
                break;

            default:
                printf("Please choose an option from 1 to 5.\n");
        }
    } while (choice != 5);

    return 0;
}