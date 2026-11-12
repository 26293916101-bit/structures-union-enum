#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Student Structure
typedef struct {
    int roll_no;
    char name[50];
    int age;
    float gpa;
} Student;

// Function prototypes
void display_menu();
int add_student(Student **students, int *count, int *capacity);
void display_students(const Student *students, int count);
void search_student(const Student *students, int count);
void update_student(Student *students, int count);
void delete_student(Student *students, int *count);
void calculate_analytics(const Student *students, int count);
void sort_students(Student *students, int count);

// Helper function to clear input buffer
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int capacity = 0;
    int count = 0;

    printf("Enter initial capacity of student records: ");
    if (scanf("%d", &capacity) != 1 || capacity <= 0) {
        printf("Invalid capacity! Defaulting to capacity of 3.\n");
        capacity = 3;
    }
    clear_input_buffer();

    Student *students = (Student*) malloc(capacity * sizeof(Student));
    if (students == NULL) {
        printf("Memory allocation failed! Exiting program.\n");
        return 1;
    }

    int choice = 0;
    do {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1:
                add_student(&students, &count, &capacity);
                break;
            case 2:
                display_students(students, count);
                break;
            case 3:
                search_student(students, count);
                break;
            case 4:
                update_student(students, count);
                break;
            case 5:
                delete_student(students, &count);
                break;
            case 6:
                calculate_analytics(students, count);
                break;
            case 7:
                sort_students(students, count);
                break;
            case 8:
                printf("Exiting program and freeing allocated memory...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 8);

    free(students);
    return 0;
}

void display_menu() {
    printf("\n=== Student Records & Academic Analytics System ===\n");
    printf("1. Add Student\n");
    printf("2. Display All Students\n");
    printf("3. Search by Roll Number\n");
    printf("4. Update Student Record\n");
    printf("5. Delete Student Record\n");
    printf("6. View GPA Analytics (Highest, Lowest, Average)\n");
    printf("7. Sort Students by GPA (Descending)\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

int add_student(Student **students, int *count, int *capacity) {
    if (*count >= *capacity) {
        int new_capacity = *capacity * 2;
        Student *temp = (Student*) realloc(*students, new_capacity * sizeof(Student));
        if (temp == NULL) {
            printf("Error: Reallocation failed! Cannot add more students.\n");
            return 0;
        }
        *students = temp;
        *capacity = new_capacity;
        printf("Collection capacity expanded to %d.\n", *capacity);
    }

    Student new_s;

    printf("Enter roll number (positive integer): ");
    if (scanf("%d", &new_s.roll_no) != 1 || new_s.roll_no <= 0) {
        printf("Invalid roll number!\n");
        clear_input_buffer();
        return 0;
    }

    // Check for duplicate roll number
    for (int i = 0; i < *count; i++) {
        if ((*students)[i].roll_no == new_s.roll_no) {
            printf("Error: Roll number already exists!\n");
            clear_input_buffer();
            return 0;
        }
    }
    clear_input_buffer();

    printf("Enter name (max 49 chars): ");
    if (fgets(new_s.name, sizeof(new_s.name), stdin) != NULL) {
        size_t len = strlen(new_s.name);
        if (len > 0 && new_s.name[len - 1] == '\n') {
            new_s.name[len - 1] = '\0';
        }
    }

    printf("Enter age (16-100): ");
    if (scanf("%d", &new_s.age) != 1 || new_s.age < 16 || new_s.age > 100) {
        printf("Invalid age! Must be between 16 and 100.\n");
        clear_input_buffer();
        return 0;
    }

    printf("Enter GPA (0.00-4.00): ");
    if (scanf("%f", &new_s.gpa) != 1 || new_s.gpa < 0.00f || new_s.gpa > 4.00f) {
        printf("Invalid GPA! Must be between 0.00 and 4.00.\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    (*students)[*count] = new_s;
    (*count)++;
    printf("Student added successfully with Roll No: %d\n", new_s.roll_no);
    return 1;
}

void display_students(const Student *students, int count) {
    if (count == 0) {
        printf("No student records found.\n");
        return;
    }

    printf("\n--- Student Records List (%d students) ---\n", count);
    for (int i = 0; i < count; i++) {
        printf("Roll No: %-5d | Name: %-20s | Age: %-3d | GPA: %.2f\n",
               students[i].roll_no, students[i].name, students[i].age, students[i].gpa);
    }
}

void search_student(const Student *students, int count) {
    if (count == 0) {
        printf("No records to search.\n");
        return;
    }

    int roll;
    printf("Enter roll number to search: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    for (int i = 0; i < count; i++) {
        if (students[i].roll_no == roll) {
            printf("Student Found:\n");
            printf("Roll No: %d, Name: %s, Age: %d, GPA: %.2f\n",
                   students[i].roll_no, students[i].name, students[i].age, students[i].gpa);
            return;
        }
    }
    printf("Student with roll number %d not found.\n", roll);
}

void update_student(Student *students, int count) {
    if (count == 0) {
        printf("No records to update.\n");
        return;
    }

    int roll;
    printf("Enter roll number of student to update: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    for (int i = 0; i < count; i++) {
        if (students[i].roll_no == roll) {
            printf("Updating record for %s (Roll No: %d)\n", students[i].name, roll);
            
            printf("Enter new name: ");
            if (fgets(students[i].name, sizeof(students[i].name), stdin) != NULL) {
                size_t len = strlen(students[i].name);
                if (len > 0 && students[i].name[len - 1] == '\n') {
                    students[i].name[len - 1] = '\0';
                }
            }

            printf("Enter new age (16-100): ");
            scanf("%d", &students[i].age);

            printf("Enter new GPA (0.00-4.00): ");
            scanf("%f", &students[i].gpa);
            clear_input_buffer();

            printf("Student record updated successfully!\n");
            return;
        }
    }
    printf("Student with roll number %d not found.\n", roll);
}

void delete_student(Student *students, int *count) {
    if (*count == 0) {
        printf("No records to delete.\n");
        return;
    }

    int roll;
    printf("Enter roll number of student to delete: ");
    if (scanf("%d", &roll) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int found_index = -1;
    for (int i = 0; i < *count; i++) {
        if (students[i].roll_no == roll) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        printf("Student with roll number %d not found.\n", roll);
        return;
    }

    // Shift elements to fill the gap
    for (int i = found_index; i < *count - 1; i++) {
        students[i] = students[i + 1];
    }
    (*count)--;
    printf("Student with roll number %d deleted successfully.\n", roll);
}

void calculate_analytics(const Student *students, int count) {
    if (count == 0) {
        printf("Collection is empty. No analytics available.\n");
        return;
    }

    int highest_idx = 0;
    int lowest_idx = 0;
    float sum_gpa = 0.0f;

    for (int i = 0; i < count; i++) {
        sum_gpa += students[i].gpa;
        if (students[i].gpa > students[highest_idx].gpa) {
            highest_idx = i;
        }
        if (students[i].gpa < students[lowest_idx].gpa) {
            lowest_idx = i;
        }
    }

    float avg_gpa = sum_gpa / count;

    printf("\n--- Academic Analytics ---\n");
    printf("Highest GPA: %.2f (%s, Roll No: %d)\n", students[highest_idx].gpa, students[highest_idx].name, students[highest_idx].roll_no);
    printf("Lowest GPA:  %.2f (%s, Roll No: %d)\n", students[lowest_idx].gpa, students[lowest_idx].name, students[lowest_idx].error ? "" : students[lowest_idx].name, students[lowest_idx].roll_no);
    printf("Average GPA across all records: %.2f\n", avg_gpa);
}

void sort_students(Student *students, int count) {
    if (count < 2) {
        printf("Not enough records to sort.\n");
        return;
    }

    // Bubble sort: Descending GPA, tie-break by ascending Roll No
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int swap_needed = 0;
            if (students[j].gpa < students[j + 1].gpa) {
                swap_needed = 1;
            } else if (students[j].gpa == students[j + 1].gpa) {
                if (students[j].roll_no > students[j + 1].roll_no) {
                    swap_needed = 1;
                }
            }

            if (swap_needed) {
                Student temp = students[j];
                students[j] = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }
    printf("Students sorted successfully by GPA (descending) and Roll No (ascending)!\n");
}
