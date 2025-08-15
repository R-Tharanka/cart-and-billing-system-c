/**
 * Shopping Cart and Bill Generator
 * Terminal-based management system implemented in C
 * 
 * This program allows users to:
 * - Manage a list of items for purchase (cart management)
 * - Add, update, and remove products
 * - Compute item subtotals, total bill, and discounts
 * - Persist transactions into a text file
 * - Generate formatted receipts for customers
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ITEMS 200
#define MAX_CODE_LEN 10
#define MAX_NAME_LEN 50
#define FILENAME "bills.txt"
#define DISCOUNT_THRESHOLD 500.0
#define DISCOUNT_RATE 0.10  // 10% discount
#define TAX_RATE 0.05       // 5% VAT (optional)

// Structure to represent an item in the shopping cart
typedef struct {
    char code[MAX_CODE_LEN];
    char name[MAX_NAME_LEN];
    int quantity;
    float unitPrice;
    float subtotal;
} Item;

// Function prototypes
void displayMenu();
void loadFromFile(const char *filename, Item arr[], int *count);
void saveToFile(const char *filename, Item arr[], int count);
void addItem(Item arr[], int *count);
void viewCart(Item arr[], int count);
int findItemIndexByCode(Item arr[], int count, const char *code);
void searchItem(Item arr[], int count);
void updateItem(Item arr[], int count);
void removeItem(Item arr[], int *count);
float calculateSubtotal(int qty, float price);
float calculateFinalBill(Item arr[], int count, float *discount);
void generateReceipt(Item arr[], int count);
void clearInputBuffer();
int getValidInteger(const char *prompt);
float getValidFloat(const char *prompt);
void getString(const char *prompt, char *buffer, int maxLength);

int main() {
    Item cart[MAX_ITEMS];
    int itemCount = 0;
    int choice;
    
    // Load existing data from file (if any)
    loadFromFile(FILENAME, cart, &itemCount);
    
    do {
        displayMenu();
        printf("Enter your choice: ");
        choice = getValidInteger("");
        
        switch (choice) {
            case 1:
                addItem(cart, &itemCount);
                break;
            case 2:
                viewCart(cart, itemCount);
                break;
            case 3:
                searchItem(cart, itemCount);
                break;
            case 4:
                updateItem(cart, itemCount);
                break;
            case 5:
                removeItem(cart, &itemCount);
                break;
            case 6:
                generateReceipt(cart, itemCount);
                break;
            case 7:
                saveToFile(FILENAME, cart, itemCount);
                printf("\nCart saved successfully. Exiting...\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
        
        printf("\nPress Enter to continue...");
        clearInputBuffer();
        getchar(); // Wait for user to press Enter
        system("cls"); // Clear screen (Windows)
        // For cross-platform compatibility, you can use:
        // #ifdef _WIN32
        //     system("cls");
        // #else
        //     system("clear");
        // #endif
        
    } while (choice != 7);
    
    return 0;
}

// Display the main menu
void displayMenu() {
    printf("----------------------------------\n");
    printf("   SHOPPING CART AND BILL GENERATOR\n");
    printf("----------------------------------\n");
    printf("1. Add Item\n");
    printf("2. View Cart\n");
    printf("3. Search Item\n");
    printf("4. Update Item\n");
    printf("5. Remove Item\n");
    printf("6. Generate Bill\n");
    printf("7. Save & Exit\n");
    printf("----------------------------------\n");
}

// Load cart data from file
void loadFromFile(const char *filename, Item arr[], int *count) {
    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("No existing cart data found. Starting with an empty cart.\n");
        *count = 0;
        return;
    }
    
    *count = 0;
    char line[256];
    
    // Skip any receipt headers or previous bill information
    while (fgets(line, sizeof(line), file) != NULL) {
        // Check if this is a data line with pipe separators
        if (strchr(line, '|') != NULL) {
            char code[MAX_CODE_LEN];
            char name[MAX_NAME_LEN];
            int qty;
            float price, subtotal;
            
            // Parse the line
            sscanf(line, "%[^|]|%[^|]|%d|%f|%f", 
                  code, name, &qty, &price, &subtotal);
            
            // Add to cart
            strcpy(arr[*count].code, code);
            strcpy(arr[*count].name, name);
            arr[*count].quantity = qty;
            arr[*count].unitPrice = price;
            arr[*count].subtotal = subtotal;
            
            (*count)++;
            
            if (*count >= MAX_ITEMS) {
                printf("Warning: Maximum items reached. Some items may not be loaded.\n");
                break;
            }
        }
    }
    
    fclose(file);
    printf("Loaded %d items from %s.\n", *count, filename);
}

// Save cart data to file
void saveToFile(const char *filename, Item arr[], int count) {
    FILE *file = fopen(filename, "w");
    
    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%s|%d|%.2f|%.2f\n", 
               arr[i].code, arr[i].name, arr[i].quantity, 
               arr[i].unitPrice, arr[i].subtotal);
    }
    
    fclose(file);
    printf("Cart data saved to %s successfully.\n", filename);
}

// Add an item to the cart
void addItem(Item arr[], int *count) {
    if (*count >= MAX_ITEMS) {
        printf("Error: Cart is full. Cannot add more items.\n");
        return;
    }
    
    printf("\n--- Add New Item ---\n");
    
    // Get item code
    getString("Enter item code: ", arr[*count].code, MAX_CODE_LEN);
    
    // Check for duplicate code
    if (findItemIndexByCode(arr, *count, arr[*count].code) != -1) {
        printf("Error: Item with this code already exists. Use Update option instead.\n");
        return;
    }
    
    // Get other details
    getString("Enter item name: ", arr[*count].name, MAX_NAME_LEN);
    arr[*count].quantity = getValidInteger("Enter quantity (positive number): ");
    arr[*count].unitPrice = getValidFloat("Enter unit price (positive number): ");
    
    // Calculate subtotal
    arr[*count].subtotal = calculateSubtotal(arr[*count].quantity, arr[*count].unitPrice);
    
    (*count)++;
    printf("Item added successfully!\n");
}

// View all items in the cart
void viewCart(Item arr[], int count) {
    if (count == 0) {
        printf("\nCart is empty.\n");
        return;
    }
    
    float total = 0.0;
    
    printf("\n--- Shopping Cart ---\n");
    printf("%-10s %-30s %-8s %-10s %-10s\n", "Code", "Name", "Qty", "Price", "Subtotal");
    printf("-------------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-10s %-30s %-8d $%-9.2f $%-9.2f\n", 
              arr[i].code, arr[i].name, arr[i].quantity, 
              arr[i].unitPrice, arr[i].subtotal);
        
        total += arr[i].subtotal;
    }
    
    printf("-------------------------------------------------------------------\n");
    printf("%-50s Total: $%.2f\n", "", total);
}

// Find an item by its code
int findItemIndexByCode(Item arr[], int count, const char *code) {
    for (int i = 0; i < count; i++) {
        if (strcmp(arr[i].code, code) == 0) {
            return i;
        }
    }
    return -1; // Item not found
}

// Search for an item by code
void searchItem(Item arr[], int count) {
    if (count == 0) {
        printf("\nCart is empty.\n");
        return;
    }
    
    char searchCode[MAX_CODE_LEN];
    getString("Enter item code to search: ", searchCode, MAX_CODE_LEN);
    
    int index = findItemIndexByCode(arr, count, searchCode);
    
    if (index == -1) {
        printf("Item with code '%s' not found.\n", searchCode);
    } else {
        printf("\n--- Item Found ---\n");
        printf("Code: %s\n", arr[index].code);
        printf("Name: %s\n", arr[index].name);
        printf("Quantity: %d\n", arr[index].quantity);
        printf("Unit Price: $%.2f\n", arr[index].unitPrice);
        printf("Subtotal: $%.2f\n", arr[index].subtotal);
    }
}

// Update an existing item
void updateItem(Item arr[], int count) {
    if (count == 0) {
        printf("\nCart is empty.\n");
        return;
    }
    
    char updateCode[MAX_CODE_LEN];
    getString("Enter item code to update: ", updateCode, MAX_CODE_LEN);
    
    int index = findItemIndexByCode(arr, count, updateCode);
    
    if (index == -1) {
        printf("Item with code '%s' not found.\n", updateCode);
        return;
    }
    
    printf("\n--- Update Item ---\n");
    printf("Current details:\n");
    printf("Code: %s\n", arr[index].code);
    printf("Name: %s\n", arr[index].name);
    printf("Quantity: %d\n", arr[index].quantity);
    printf("Unit Price: $%.2f\n", arr[index].unitPrice);
    
    printf("\nWhat would you like to update?\n");
    printf("1. Quantity\n");
    printf("2. Unit Price\n");
    printf("3. Both\n");
    printf("4. Cancel\n");
    
    int choice = getValidInteger("Enter your choice: ");
    
    switch (choice) {
        case 1:
            arr[index].quantity = getValidInteger("Enter new quantity (positive number): ");
            break;
        case 2:
            arr[index].unitPrice = getValidFloat("Enter new unit price (positive number): ");
            break;
        case 3:
            arr[index].quantity = getValidInteger("Enter new quantity (positive number): ");
            arr[index].unitPrice = getValidFloat("Enter new unit price (positive number): ");
            break;
        case 4:
            printf("Update cancelled.\n");
            return;
        default:
            printf("Invalid choice.\n");
            return;
    }
    
    // Recalculate subtotal
    arr[index].subtotal = calculateSubtotal(arr[index].quantity, arr[index].unitPrice);
    printf("Item updated successfully!\n");
}

// Remove an item from the cart
void removeItem(Item arr[], int *count) {
    if (*count == 0) {
        printf("\nCart is empty.\n");
        return;
    }
    
    char removeCode[MAX_CODE_LEN];
    getString("Enter item code to remove: ", removeCode, MAX_CODE_LEN);
    
    int index = findItemIndexByCode(arr, *count, removeCode);
    
    if (index == -1) {
        printf("Item with code '%s' not found.\n", removeCode);
        return;
    }
    
    // Confirm removal
    printf("Are you sure you want to remove '%s - %s'? (1 for Yes, 0 for No): ", 
           arr[index].code, arr[index].name);
    int confirm = getValidInteger("");
    
    if (confirm != 1) {
        printf("Removal cancelled.\n");
        return;
    }
    
    // Shift all elements after the removed item
    for (int i = index; i < *count - 1; i++) {
        arr[i] = arr[i + 1];
    }
    
    (*count)--;
    printf("Item removed successfully!\n");
}

// Calculate subtotal for an item
float calculateSubtotal(int qty, float price) {
    return qty * price;
}

// Calculate final bill amount with discount
float calculateFinalBill(Item arr[], int count, float *discount) {
    float total = 0.0;
    
    for (int i = 0; i < count; i++) {
        total += arr[i].subtotal;
    }
    
    // Apply discount if total meets threshold
    if (total >= DISCOUNT_THRESHOLD) {
        *discount = total * DISCOUNT_RATE;
    } else {
        *discount = 0.0;
    }
    
    return total - *discount;
}

// Generate and save receipt
void generateReceipt(Item arr[], int count) {
    if (count == 0) {
        printf("\nCart is empty. Cannot generate bill.\n");
        return;
    }
    
    float discount = 0.0;
    float total = 0.0;
    
    for (int i = 0; i < count; i++) {
        total += arr[i].subtotal;
    }
    
    float finalBill = calculateFinalBill(arr, count, &discount);
    
    // Print receipt to console
    printf("\n");
    printf("----------------------------------\n");
    printf("        SHOP RECEIPT\n");
    printf("----------------------------------\n");
    printf("%-6s %-20s %-4s %-8s %-10s\n", "Code", "Name", "Qty", "Price", "Subtotal");
    printf("----------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-6s %-20s %-4d $%-7.2f $%-9.2f\n", 
              arr[i].code, arr[i].name, arr[i].quantity, 
              arr[i].unitPrice, arr[i].subtotal);
    }
    
    printf("----------------------------------\n");
    printf("Total: $%.2f\n", total);
    printf("Discount: $%.2f\n", discount);
    printf("Final Bill: $%.2f\n", finalBill);
    printf("----------------------------------\n");
    printf("Thank you for shopping!\n");
    
    // Save receipt to file
    FILE *file = fopen(FILENAME, "a");
    
    if (file == NULL) {
        printf("Error: Unable to save receipt to file.\n");
        return;
    }
    
    // Add a separator before the receipt
    fprintf(file, "\n----------------------------------\n");
    fprintf(file, "        SHOP RECEIPT\n");
    fprintf(file, "----------------------------------\n");
    
    // Write receipt items
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s|%s|%d|%.2f|%.2f\n", 
               arr[i].code, arr[i].name, arr[i].quantity, 
               arr[i].unitPrice, arr[i].subtotal);
    }
    
    // Write receipt summary
    fprintf(file, "----------------------------------\n");
    fprintf(file, "Total: $%.2f\n", total);
    fprintf(file, "Discount: $%.2f\n", discount);
    fprintf(file, "Final Bill: $%.2f\n", finalBill);
    fprintf(file, "----------------------------------\n");
    fprintf(file, "Thank you for shopping!\n");
    
    fclose(file);
    printf("\nReceipt saved to %s successfully!\n", FILENAME);
}

// Clear input buffer to prevent issues with input functions
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Get a valid integer from the user
int getValidInteger(const char *prompt) {
    int value;
    char input[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        
        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';
        
        // Check if empty input and prompt is empty (for initial menu choice)
        if (input[0] == '\0' && prompt[0] == '\0') {
            continue;
        }
        
        // Check if input is valid
        char *endptr;
        value = strtol(input, &endptr, 10);
        
        if (*endptr != '\0') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value <= 0 && strcmp(prompt, "") != 0) {
            printf("Please enter a positive number.\n");
            continue;
        }
        
        break;
    }
    
    return value;
}

// Get a valid float from the user
float getValidFloat(const char *prompt) {
    float value;
    char input[100];
    
    while (1) {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        
        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';
        
        // Check if input is valid
        char *endptr;
        value = strtof(input, &endptr);
        
        if (*endptr != '\0') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (value <= 0.0) {
            printf("Please enter a positive number.\n");
            continue;
        }
        
        break;
    }
    
    return value;
}

// Get a string from the user
void getString(const char *prompt, char *buffer, int maxLength) {
    printf("%s", prompt);
    if (fgets(buffer, maxLength, stdin) == NULL) {
        printf("Error reading input.\n");
        buffer[0] = '\0';
        return;
    }
    
    // Remove trailing newline
    buffer[strcspn(buffer, "\n")] = '\0';
    
    // Validate non-empty input
    while (buffer[0] == '\0') {
        printf("Input cannot be empty. %s", prompt);
        if (fgets(buffer, maxLength, stdin) == NULL) {
            printf("Error reading input.\n");
            buffer[0] = '\0';
            return;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}
