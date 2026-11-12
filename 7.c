// Problem 4 — Extended: Interactive Command-Driven Record Manager
// Compiled with: -std=c11 -Wall -Wextra -Wpedantic

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Task Status Enum
typedef enum {
    STATUS_PENDING,
    STATUS_DONE
} TaskStatus;

// Task Structure
typedef struct {
    int id;
    char description[100];
    TaskStatus status;
} Task;

// Extended Command Enumeration
typedef enum {
    CMD_ADD,
    CMD_DELETE,
    CMD_VIEW,
    CMD_UPDATE,
    CMD_COMPLETE,
    CMD_FIND,
    CMD_HELP,
    CMD_EXIT,
    CMD_UNKNOWN
} CommandType;

// Helper function for case-insensitive command matching
void to_upper_str(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0' && src[i] != '\n' && src[i] != '\r') {
        dest[i] = (char)toupper((unsigned char)src[i]);
        i++;
    }
    dest[i] = '\0';
}

CommandType get_command_type(const char* command_str) {
    char upper[50];
    to_upper_str(upper, command_str);

    if (strcmp(upper, "ADD") == 0) return CMD_ADD;
    if (strcmp(upper, "DELETE") == 0) return CMD_DELETE;
    if (strcmp(upper, "VIEW") == 0) return CMD_VIEW;
    if (strcmp(upper, "UPDATE") == 0) return CMD_UPDATE;
    if (strcmp(upper, "COMPLETE") == 0) return CMD_COMPLETE;
    if (strcmp(upper, "FIND") == 0) return CMD_FIND;
    if (strcmp(upper, "HELP") == 0) return CMD_HELP;
    if (strcmp(upper, "EXIT") == 0) return CMD_EXIT;
    return CMD_UNKNOWN;
}

void print_help() {
    printf("\n=== Interactive Task-List Manager Commands ===\n");
    printf("  ADD      - Add a new task (ID and description)\n");
    printf("  DELETE   - Delete a task by ID\n");
    printf("  VIEW     - List all tasks\n");
    printf("  UPDATE   - Update an existing task's description\n");
    printf("  COMPLETE - Mark a task as DONE\n");
    printf("  FIND     - Find and display a task by ID\n");
    printf("  HELP     - Show this help menu\n");
    printf("  EXIT     - Exit the program\n\n");
}

int main() {
    int capacity = 2;
    int count = 0;
    Task *tasks = (Task*) malloc(capacity * sizeof(Task));
    if (tasks == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    char input_line[256];
    print_help();

    while (1) {
        printf("\nEnter command (ADD, DELETE, VIEW, UPDATE, COMPLETE, FIND, HELP, EXIT): ");
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) {
            printf("\nInput terminated (EOF). Exiting...\n");
            break;
        }

        input_line[strcspn(input_line, "\r\n")] = '\0';
        if (strlen(input_line) == 0) {
            continue;
        }

        CommandType cmd = get_command_type(input_line);

        switch (cmd) {
            case CMD_ADD: {
                int id;
                char desc[100];

                printf("Enter Task ID (positive integer): ");
                if (scanf("%d", &id) != 1 || id <= 0) {
                    printf("Invalid Task ID!\n");
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                int c;
                while ((c = getchar()) != '\n' && c != EOF);

                // Check duplicate ID
                int duplicate = 0;
                for (int i = 0; i < count; i++) {
                    if (tasks[i].id == id) {
                        duplicate = 1;
                        break;
                    }
                }
                if (duplicate) {
                    printf("Error: Task ID %d already exists!\n", id);
                    break;
                }

                printf("Enter task description: ");
                if (fgets(desc, sizeof(desc), stdin) == NULL) {
                    break;
                }
                desc[strcspn(desc, "\r\n")] = '\0';

                // Reallocate if full
                if (count >= capacity) {
                    int new_cap = capacity * 2;
                    Task *temp = (Task*) realloc(tasks, new_cap * sizeof(Task));
                    if (temp == NULL) {
                        printf("Error: Reallocation failed!\n");
                        break;
                    }
                    tasks = temp;
                    capacity = new_cap;
                }

                tasks[count].id = id;
                strncpy(tasks[count].description, desc, sizeof(tasks[count].description) - 1);
                tasks[count].description[sizeof(tasks[count].description) - 1] = '\0';
                tasks[count].status = STATUS_PENDING;
                count++;

                printf("Success: Task ID %d added successfully.\n", id);
                break;
            }
            case CMD_DELETE: {
                if (count == 0) {
                    printf("Task list is empty.\n");
                    break;
                }
                int id;
                printf("Enter Task ID to delete: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid input.\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    break;
                }
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                int found_idx = -1;
                for (int i = 0; i < count; i++) {
                    if (tasks[i].id == id) {
                        found_idx = i;
                        break;
                    }
                }

                if (found_idx == -1) {
                    printf("Task ID %d not found.\n", id);
                    break;
                }

                for (int i = found_idx; i < count - 1; i++) {
                    tasks[i] = tasks[i + 1];
                }
                count--;
                printf("Success: Task ID %d deleted successfully.\n", id);
                break;
            }
            case CMD_VIEW: {
                if (count == 0) {
                    printf("Task list is empty.\n");
                    break;
                }
                printf("\n--- Task List (%d tasks) ---\n", count);
                for (int i = 0; i < count; i++) {
                    printf("ID: %-5d | Status: %-8s | Description: %s\n",
                           tasks[i].id,
                           tasks[i].status == STATUS_PENDING ? "PENDING" : "DONE",
                           tasks[i].description);
                }
                break;
            }
            case CMD_UPDATE: {
                if (count == 0) {
                    printf("Task list is empty.\n");
                    break;
                }
                int id;
                printf("Enter Task ID to update description: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid input.\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    break;
                }
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                int found_idx = -1;
                for (int i = 0; i < count; i++) {
                    if (tasks[i].id == id) {
                        found_idx = i;
                        break;
                    }
                }

                if (found_idx == -1) {
                    printf("Task ID %d not found.\n", id);
                    break;
                }

                printf("Enter new description: ");
                char new_desc[100];
                if (fgets(new_desc, sizeof(new_desc), stdin) == NULL) {
                    break;
                }
                new_desc[strcspn(new_desc, "\r\n")] = '\0';

                strncpy(tasks[found_idx].description, new_desc, sizeof(tasks[found_idx].description) - 1);
                tasks[found_idx].description[sizeof(tasks[found_idx].description) - 1] = '\0';
                printf("Success: Task ID %d description updated successfully.\n", id);
                break;
            }
            case CMD_COMPLETE: {
                if (count == 0) {
                    printf("Task list is empty.\n");
                    break;
                }
                int id;
                printf("Enter Task ID to mark as complete: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid input.\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    break;
                }
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                int found_idx = -1;
                for (int i = 0; i < count; i++) {
                    if (tasks[i].id == id) {
                        found_idx = i;
                        break;
                    }
                }

                if (found_idx == -1) {
                    printf("Task ID %d not found.\n", id);
                    break;
                }

                tasks[found_idx].status = STATUS_DONE;
                printf("Success: Task ID %d marked as DONE.\n", id);
                break;
            }
            case CMD_FIND: {
                if (count == 0) {
                    printf("Task list is empty.\n");
                    break;
                }
                int id;
                printf("Enter Task ID to find: ");
                if (scanf("%d", &id) != 1) {
                    printf("Invalid input.\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    break;
                }
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF);

                int found_idx = -1;
                for (int i = 0; i < count; i++) {
                    if (tasks[i].id == id) {
                        found_idx = i;
                        break;
                    }
                }

                if (found_idx == -1) {
                    printf("Task ID %d not found.\n", id);
                    break;
                }

                printf("Task Found -> ID: %d | Status: %s | Description: %s\n",
                       tasks[found_idx].id,
                       tasks[found_idx].status == STATUS_PENDING ? "PENDING" : "DONE",
                       tasks[found_idx].description);
                break;
            }
            case CMD_HELP:
                print_help();
                break;
            case CMD_EXIT:
                printf("Exiting program and freeing allocated memory...\n");
                free(tasks);
                return 0;
            default:
                printf("Unknown command. Type HELP for a list of valid commands.\n");
        }
    }

    free(tasks);
    return 0;
}
