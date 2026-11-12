#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.00001f

typedef struct {
    int id;
    float width;
    float height;
    float area;
    float perimeter;
} Rectangle;

void display_menu();
int add_rectangle(Rectangle **rectangles, int *count, int *capacity);
void display_rectangles(const Rectangle *rectangles, int count);
void search_rectangle(const Rectangle *rectangles, int count);
void update_rectangle(Rectangle *rectangles, int count);
void delete_rectangle(Rectangle *rectangles, int *count);
void calculate_analytics(const Rectangle *rectangles, int count);
void sort_and_filter_rectangles(const Rectangle *rectangles, int count);

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    int capacity = 0;
    int count = 0;

    printf("Enter initial capacity of rectangles (1-1000): ");
    if (scanf("%d", &capacity) != 1 || capacity <= 0 || capacity > 1000) {
        printf("Invalid capacity! Defaulting to capacity of 3.\n");
        capacity = 3;
    }
    clear_input_buffer();

    Rectangle *rectangles = (Rectangle*) malloc(capacity * sizeof(Rectangle));
    if (rectangles == NULL) {
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
                add_rectangle(&rectangles, &count, &capacity);
                break;
            case 2:
                display_rectangles(rectangles, count);
                break;
            case 3:
                search_rectangle(rectangles, count);
                break;
            case 4:
                update_rectangle(rectangles, count);
                break;
            case 5:
                delete_rectangle(rectangles, &count);
                break;
            case 6:
                calculate_analytics(rectangles, count);
                break;
            case 7:
                sort_and_filter_rectangles(rectangles, count);
                break;
            case 8:
                printf("Exiting program and freeing allocated memory...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 8);

    free(rectangles);
    return 0;
}

void display_menu() {
    printf("\n=== Rectangle Inventory & Geometry Analysis System ===\n");
    printf("1. Add Rectangle\n");
    printf("2. Display All Rectangles & Classifications\n");
    printf("3. Search Rectangle by ID\n");
    printf("4. Update Rectangle Dimensions by ID\n");
    printf("5. Delete Rectangle by ID\n");
    printf("6. View Geometry Analytics (Area/Perimeter Summary)\n");
    printf("7. Sort & Filter Rectangles by Area\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

int add_rectangle(Rectangle **rectangles, int *count, int *capacity) {
    if (*count >= *capacity) {
        int new_capacity = *capacity * 2;
        Rectangle *temp = (Rectangle*) realloc(*rectangles, new_capacity * sizeof(Rectangle));
        if (temp == NULL) {
            printf("Error: Reallocation failed! Cannot add more rectangles.\n");
            return 0;
        }
        *rectangles = temp;
        *capacity = new_capacity;
        printf("Inventory capacity expanded to %d.\n", *capacity);
    }

    Rectangle new_r;

    printf("Enter unique rectangle ID (positive integer): ");
    if (scanf("%d", &new_r.id) != 1 || new_r.id <= 0) {
        printf("Invalid ID!\n");
        clear_input_buffer();
        return 0;
    }

    for (int i = 0; i < *count; i++) {
        if ((*rectangles)[i].id == new_r.id) {
            printf("Error: Rectangle ID already exists!\n");
            clear_input_buffer();
            return 0;
        }
    }

    printf("Enter width (finite positive float): ");
    if (scanf("%f", &new_r.width) != 1 || new_r.width <= 0.0f) {
        printf("Invalid width!\n");
        clear_input_buffer();
        return 0;
    }

    printf("Enter height (finite positive float): ");
    if (scanf("%f", &new_r.height) != 1 || new_r.height <= 0.0f) {
        printf("Invalid height!\n");
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();

    new_r.area = new_r.width * new_r.height;
    new_r.perimeter = 2.0f * (new_r.width + new_r.height);

    (*rectangles)[*count] = new_r;
    (*count)++;
    printf("Rectangle added successfully with ID: %d\n", new_r.id);
    return 1;
}

void display_rectangles(const Rectangle *rectangles, int count) {
    if (count == 0) {
        printf("Rectangle inventory is empty.\n");
        return;
    }

    printf("\n--- Rectangle Inventory (%d rectangles) ---\n", count);
    for (int i = 0; i < count; i++) {
        int is_square = (fabsf(rectangles[i].width - rectangles[i].height) < EPSILON);
        printf("ID: %-4d | Width: %-6.2f | Height: %-6.2f | Area: %-7.2f | Perimeter: %-7.2f | Type: %s\n",
               rectangles[i].id, rectangles[i].width, rectangles[i].height,
               rectangles[i].area, rectangles[i].perimeter,
               is_square ? "Square" : "Non-Square");
    }
}

void search_rectangle(const Rectangle *rectangles, int count) {
    if (count == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    int id;
    printf("Enter rectangle ID to search: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    for (int i = 0; i < count; i++) {
        if (rectangles[i].id == id) {
            int is_square = (fabsf(rectangles[i].width - rectangles[i].height) < EPSILON);
            printf("Rectangle Found:\n");
            printf("ID: %d, Width: %.2f, Height: %.2f, Area: %.2f, Perimeter: %.2f, Type: %s\n",
                   rectangles[i].id, rectangles[i].width, rectangles[i].height,
                   rectangles[i].area, rectangles[i].perimeter, is_square ? "Square" : "Non-Square");
            return;
        }
    }
    printf("Rectangle with ID %d not found.\n", id);
}

void update_rectangle(Rectangle *rectangles, int count) {
    if (count == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    int id;
    printf("Enter rectangle ID to update dimensions: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    for (int i = 0; i < count; i++) {
        if (rectangles[i].id == id) {
            float w, h;
            printf("Enter new width: ");
            if (scanf("%f", &w) != 1 || w <= 0.0f) {
                printf("Invalid width!\n");
                clear_input_buffer();
                return;
            }
            printf("Enter new height: ");
            if (scanf("%f", &h) != 1 || h <= 0.0f) {
                printf("Invalid height!\n");
                clear_input_buffer();
                return;
            }
            clear_input_buffer();

            rectangles[i].width = w;
            rectangles[i].height = h;
            rectangles[i].area = w * h;
            rectangles[i].perimeter = 2.0f * (w + h);

            printf("Rectangle ID %d dimensions updated successfully!\n", id);
            return;
        }
    }
    printf("Rectangle with ID %d not found.\n", id);
}

void delete_rectangle(Rectangle *rectangles, int *count) {
    if (*count == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    int id;
    printf("Enter rectangle ID to delete: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int found_index = -1;
    for (int i = 0; i < *count; i++) {
        if (rectangles[i].id == id) {
            found_index = i;
            break;
        }
    }

    if (found_index == -1) {
        printf("Rectangle with ID %d not found.\n", id);
        return;
    }

    for (int i = found_index; i < *count - 1; i++) {
        rectangles[i] = rectangles[i + 1];
    }
    (*count)--;
    printf("Rectangle with ID %d deleted successfully.\n", id);
}

void calculate_analytics(const Rectangle *rectangles, int count) {
    if (count == 0) {
        printf("Inventory is empty. No analytics available.\n");
        return;
    }

    int largest_idx = 0;
    int smallest_idx = 0;
    float total_area = 0.0f;

    for (int i = 0; i < count; i++) {
        total_area += rectangles[i].area;
        if (rectangles[i].area > rectangles[largest_idx].area) {
            largest_idx = i;
        }
        if (rectangles[i].area < rectangles[smallest_idx].area) {
            smallest_idx = i;
        }
    }

    float avg_area = total_area / count;

    printf("\n--- Geometry Analytics ---\n");
    printf("Largest Area:  %.2f (ID: %d)\n", rectangles[largest_idx].area, rectangles[largest_idx].id);
    printf("Smallest Area: %.2f (ID: %d)\n", rectangles[smallest_idx].area, rectangles[smallest_idx].id);
    printf("Total Area:    %.2f\n", total_area);
    printf("Average Area:  %.2f\n", avg_area);
}

void sort_and_filter_rectangles(const Rectangle *rectangles, int count) {
    if (count == 0) {
        printf("Inventory is empty.\n");
        return;
    }

    Rectangle *copy = (Rectangle*) malloc(count * sizeof(Rectangle));
    if (copy == NULL) {
        printf("Memory allocation failed for sorting.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        copy[i] = rectangles[i];
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int swap_needed = 0;
            if (copy[j].area > copy[j + 1].area) {
                swap_needed = 1;
            } else if (fabsf(copy[j].area - copy[j + 1].area) < EPSILON) {
                if (copy[j].id > copy[j + 1].id) {
                    swap_needed = 1;
                }
            }
            if (swap_needed) {
                Rectangle temp = copy[j];
                copy[j] = copy[j + 1];
                copy[j + 1] = temp;
            }
        }
    }

    printf("\n--- Rectangles Sorted by Area (Ascending) ---\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %-4d | Area: %.2f | Perimeter: %.2f\n", copy[i].id, copy[i].area, copy[i].perimeter);
    }

    float threshold;
    printf("\nEnter area threshold to filter rectangles (shows area > threshold): ");
    if (scanf("%f", &threshold) == 1) {
        clear_input_buffer();
        printf("\n--- Filtered Rectangles (Area > %.2f) ---\n", threshold);
        int matched = 0;
        for (int i = 0; i < count; i++) {
            if (copy[i].area > threshold) {
                printf("ID: %-4d | Area: %.2f\n", copy[i].id, copy[i].area);
                matched++;
            }
        }
        if (matched == 0) {
            printf("No rectangles exceed the specified threshold.\n");
        }
    } else {
        clear_input_buffer();
    }

    free(copy);
}
