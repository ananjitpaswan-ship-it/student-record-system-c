#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 50
#define MAX_STUDENTS 100
#define FILENAME "students.dat"


// Structure to store student data
typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    float grade;
    int attendance; // percentage
    int isActive;   // 1 for active, 0 for deleted
} Student;

// Function prototypes
void displayMenu();
void addStudent();
void displayAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void calculateStatistics();
void saveToFile(Student students[], int count);
int loadFromFile(Student students[]);
int getNextId(Student students[], int count);
void clearInputBuffer();

int main() {
    int choice;
    
    printf("========================================\n");
    printf("    STUDENT RECORD MANAGEMENT SYSTEM    \n");
    printf("========================================\n");
    
    do {
        displayMenu();
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            continue;
        }
        
        clearInputBuffer(); // Clear any leftover input
        
        switch(choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                calculateStatistics();
                break;
            case 7:
                printf("Exiting system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
        
        printf("\n");
    } while(choice != 7);
    
    return 0;
}

void displayMenu() {
    printf("\n----------- MAIN MENU -----------\n");
    printf("1. Add New Student\n");
    printf("2. Display All Students\n");
    printf("3. Search Student\n");
    printf("4. Update Student Record\n");
    printf("5. Delete Student Record\n");
    printf("6. View Statistics\n");
    printf("7. Exit\n");
    printf("---------------------------------\n");
}

void addStudent() {
    Student students[MAX_STUDENTS];
    int count = loadFromFile(students);
    
    if (count >= MAX_STUDENTS) {
        printf("Error: Maximum student limit reached!\n");
        return;
    }
    
    Student newStudent;
    
    // Get student ID
    newStudent.id = getNextId(students, count);
    
    // Get student name
    printf("Enter student name: ");
    fgets(newStudent.name, MAX_NAME_LENGTH, stdin);
    
    // Remove newline character if present
    newStudent.name[strcspn(newStudent.name, "\n")] = '\0';
    
    // Validate and get grade
    do {
        printf("Enter grade (0.0 to 100.0): ");
        if (scanf("%f", &newStudent.grade) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            newStudent.grade = -1; // Invalid value to retry
            continue;
        }
        
        if (newStudent.grade < 0 || newStudent.grade > 100) {
            printf("Grade must be between 0.0 and 100.0!\n");
        }
    } while (newStudent.grade < 0 || newStudent.grade > 100);
    
    clearInputBuffer();
    
    // Validate and get attendance
    do {
        printf("Enter attendance percentage (0 to 100): ");
        if (scanf("%d", &newStudent.attendance) != 1) {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer();
            newStudent.attendance = -1; // Invalid value to retry
            continue;
        }
        
        if (newStudent.attendance < 0 || newStudent.attendance > 100) {
            printf("Attendance must be between 0 and 100!\n");
        }
    } while (newStudent.attendance < 0 || newStudent.attendance > 100);
    
    clearInputBuffer();
    
    newStudent.isActive = 1; // Mark as active
    
    // Add to array
    students[count] = newStudent;
    count++;
    
    // Save to file
    saveToFile(students, count);
    
    printf("\nStudent added successfully!\n");
    printf("Student ID: %d\n", newStudent.id);
}

void displayAllStudents() {
    Student students[MAX_STUDENTS];
    int count = loadFromFile(students);
    
    if (count == 0) {
        printf("No student records found!\n");
        return;
    }
    
    int activeCount = 0;
    
    printf("\n%-10s %-20s %-10s %-15s\n", 
           "ID", "Name", "Grade", "Attendance(%)");
    printf("--------------------------------------------------------\n");
    int i;
    for (i = 0; i < count; i++) {
        if (students[i].isActive) {
            printf("%-10d %-20s %-10.2f %-15d\n", 
                   students[i].id, 
                   students[i].name, 
                   students[i].grade, 
                   students[i].attendance);
            activeCount++;
        }
    }
    
    printf("--------------------------------------------------------\n");
    printf("Total active students: %d\n", activeCount);
}

void searchStudent() {
    Student students[MAX_STUDENTS];
    int count = loadFromFile(students);
    
    if (count == 0) {
        printf("No student records found!\n");
        return;
    }
    
    int choice;
    printf("\nSearch by:\n");
    printf("1. Student ID\n");
    printf("2. Student Name\n");
    printf("Enter your choice: ");
    
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input!\n");
        clearInputBuffer();
        return;
    }
    
    clearInputBuffer();
    
    int found = 0;
    
    if (choice == 1) {
        int searchId;
        printf("Enter student ID to search: ");
        scanf("%d", &searchId);
        clearInputBuffer();
        
        int i;
        for (i = 0; i < count; i++) {
            if (students[i].id == searchId && students[i].isActive) {
                printf("\nStudent Found!\n");
                printf("ID: %d\n", students[i].id);
                printf("Name: %s\n", students[i].name);
                printf("Grade: %.2f\n", students[i].grade);
                printf("Attendance: %d%%\n", students[i].attendance);
                found = 1;
                break;
            }
        }
    } 
    else if (choice == 2) {
        char searchName[MAX_NAME_LENGTH];
        printf("Enter student name to search: ");
        fgets(searchName, MAX_NAME_LENGTH, stdin);
        searchName[strcspn(searchName, "\n")] = '\0';
        
        printf("\nSearch Results:\n");
        printf("%-10s %-20s %-10s %-15s\n", 
               "ID", "Name", "Grade", "Attendance(%)");
        printf("--------------------------------------------------------\n");
        
        int i;
        for (i = 0; i < count; i++) {
            if (students[i].isActive && 
                strstr(students[i].name, searchName) != NULL) {
                printf("%-10d %-20s %-10.2f %-15d\n", 
                       students[i].id, 
                       students[i].name, 
                       students[i].grade, 
                       students[i].attendance);
                found = 1;
            }
        }
    }
    else {
        printf("Invalid choice!\n");
        return;
    }
    
    if (!found) {
        printf("No matching student found!\n");
    }
}

void updateStudent() {
    Student students[MAX_STUDENTS];
    int count = loadFromFile(students);
    
    if (count == 0) {
        printf("No student records found!\n");
        return;
    }
    
    int id, index = -1;
    printf("Enter student ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    // Find student
    int i;
    for (i = 0; i < count; i++) {
        if (students[i].id == id && students[i].isActive) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Student with ID %d not found or has been deleted!\n", id);
        return;
    }
    
    printf("\nUpdating record for Student ID: %d\n", id);
    printf("Current Name: %s\n", students[index].name);
    printf("Current Grade: %.2f\n", students[index].grade);
    printf("Current Attendance: %d%%\n", students[index].attendance);
    
    printf("\nEnter new details (press Enter to keep current value):\n");
    
    // Update name
    char newName[MAX_NAME_LENGTH];
    printf("Enter new name: ");
    fgets(newName, MAX_NAME_LENGTH, stdin);
    newName[strcspn(newName, "\n")] = '\0';
    
    if (strlen(newName) > 0) {
        strcpy(students[index].name, newName);
    }
    
    // Update grade
    char gradeInput[20];
    printf("Enter new grade (current: %.2f): ", students[index].grade);
    fgets(gradeInput, sizeof(gradeInput), stdin);
    
    if (strlen(gradeInput) > 1) { // More than just newline
        float newGrade = atof(gradeInput);
        if (newGrade >= 0 && newGrade <= 100) {
            students[index].grade = newGrade;
        } else {
            printf("Invalid grade! Keeping current grade.\n");
        }
    }
    
    // Update attendance
    char attendanceInput[20];
    printf("Enter new attendance (current: %d%%): ", students[index].attendance);
    fgets(attendanceInput, sizeof(attendanceInput), stdin);
    
    if (strlen(attendanceInput) > 1) { // More than just newline
        int newAttendance = atoi(attendanceInput);
        if (newAttendance >= 0 && newAttendance <= 100) {
            students[index].attendance = newAttendance;
        } else {
            printf("Invalid attendance! Keeping current attendance.\n");
        }
    }
    
    // Save to file
    saveToFile(students, count);
    
    printf("\nStudent record updated successfully!\n");
}

void deleteStudent() {
    Student students[MAX_STUDENTS];
    int count = loadFromFile(students);
    
    if (count == 0) {
        printf("No student records found!\n");
        return;
    }
    
    int id, index = -1;
    printf("Enter student ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    // Find student
    int i;
    for (i = 0; i < count; i++) {
        if (students[i].id == id && students[i].isActive) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Student with ID %d not found or already deleted!\n", id);
        return;
    }
    
    // Confirm deletion
    char confirm;
    printf("Are you sure you want to delete student '%s' (ID: %d)? (y/n): ", 
           students[index].name, students[index].id);
    scanf("%c", &confirm);
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        students[index].isActive = 0; // Soft delete
        saveToFile(students, count);
        printf("Student record deleted successfully!\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}

void calculateStatistics() {
    Student students[MAX_STUDENTS];
    int count = loadFromFile(students);
    
    if (count == 0) {
        printf("No student records found!\n");
        return;
    }
    
    int activeCount = 0;
    float totalGrade = 0;
    float totalAttendance = 0;
    float highestGrade = -1;
    float lowestGrade = 101;
    char topStudent[MAX_NAME_LENGTH] = "";
    char lowestStudent[MAX_NAME_LENGTH] = "";
    
    int i;
    for (i = 0; i < count; i++) {
        if (students[i].isActive) {
            activeCount++;
            totalGrade += students[i].grade;
            totalAttendance += students[i].attendance;
            
            if (students[i].grade > highestGrade) {
                highestGrade = students[i].grade;
                strcpy(topStudent, students[i].name);
            }
            
            if (students[i].grade < lowestGrade) {
                lowestGrade = students[i].grade;
                strcpy(lowestStudent, students[i].name);
            }
        }
    }
    
    if (activeCount == 0) {
        printf("No active student records found!\n");
        return;
    }
    
    float avgGrade = totalGrade / activeCount;
    float avgAttendance = totalAttendance / activeCount;
    
    printf("\n========== STATISTICS ==========\n");
    printf("Total Active Students: %d\n", activeCount);
    printf("Average Grade: %.2f\n", avgGrade);
    printf("Average Attendance: %.2f%%\n", avgAttendance);
    printf("Highest Grade: %.2f (%s)\n", highestGrade, topStudent);
    printf("Lowest Grade: %.2f (%s)\n", lowestGrade, lowestStudent);
    
    // Grade distribution
    printf("\nGrade Distribution:\n");
    printf("A (90-100): ");
    int aCount = 0, bCount = 0, cCount = 0, dCount = 0, fCount = 0;
    
    for (i = 0; i < count; i++) {
        if (!students[i].isActive) continue;
        
        if (students[i].grade >= 90) aCount++;
        else if (students[i].grade >= 80) bCount++;
        else if (students[i].grade >= 70) cCount++;
        else if (students[i].grade >= 60) dCount++;
        else fCount++;
    }
    
    printf("%d students\n", aCount);
    printf("B (80-89):  %d students\n", bCount);
    printf("C (70-79):  %d students\n", cCount);
    printf("D (60-69):  %d students\n", dCount);
    printf("F (0-59):   %d students\n", fCount);
    printf("================================\n");
}

void saveToFile(Student students[], int count) {
    FILE *file = fopen(FILENAME, "wb");
    
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    
    // Write count first
    fwrite(&count, sizeof(int), 1, file);
    
    // Write all students
    fwrite(students, sizeof(Student), count, file);
    
    fclose(file);
}

int loadFromFile(Student students[]) {
    FILE *file = fopen(FILENAME, "rb");
    
    if (file == NULL) {
        return 0; // No file exists yet
    }
    
    int count;
    fread(&count, sizeof(int), 1, file);
    
    // Make sure we don't read more than MAX_STUDENTS
    if (count > MAX_STUDENTS) {
        count = MAX_STUDENTS;
    }
    
    fread(students, sizeof(Student), count, file);
    
    fclose(file);
    return count;
}

int getNextId(Student students[], int count) {
    int maxId = 0;
    
    int i;
    for (i = 0; i < count; i++) {
        if (students[i].id > maxId) {
            maxId = students[i].id;
        }
    }
    
    return maxId + 1;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
