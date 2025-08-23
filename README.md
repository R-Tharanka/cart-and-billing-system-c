# SHOPPING CART AND BILL GENERATOR


## Project Description
------------------
This is a terminal-based Shopping Cart and Bill Generator system implemented in C.
It allows users to manage a list of items for purchase, compute subtotals and totals,
apply discounts, and generate formatted receipts. The system is designed to demonstrate
proficiency in C programming fundamentals including functions, conditional logic, 
loops, arrays, and text file handling.

## Features
--------
1. **Item Management**
   - Add items to the cart with unique code, name, quantity, and price
   - View all items in the cart with a formatted tabular display
   - Search for items by their unique code
   - Update item quantities or prices with automatic subtotal recalculation
   - Remove items from the cart with confirmation

2. **Bill Processing**
   - Calculate item subtotals (quantity × unit price)
   - Compute total bill amount with running total
   - Apply automatic discounts (10% for bills ≥ 500)
   - Generate formatted receipts with detailed item breakdown

3. **Data Persistence**
   - Save cart data to a text file (bills.txt)
   - Load previous cart data on program startup
   - Append formatted receipts to bills.txt for record keeping

4. **Input Validation**
   - Validate numeric inputs for quantity and price
   - Prevent duplicate item codes
   - Handle invalid inputs with appropriate error messages

## Technical Implementation
--------------------
- Written in C (ISO C99 compatible)
- Modular design with 10+ user-defined functions
- Structured data storage using arrays of structs
- Text-based file I/O with pipe-delimited format
- Memory-efficient implementation handling up to 200 items
- Cross-platform compatibility (Windows, Linux, macOS)

## Compilation Instructions
-----------------------
To compile the program using gcc:

```
gcc -o shopping_cart shopping_cart.c
```

For additional warnings and strict compilation:
```
gcc -Wall -Wextra -pedantic -std=c99 -o shopping_cart shopping_cart.c
```

## Running the Program
------------------
After compilation, run the program using:

```
./shopping_cart    # Linux/macOS
```

On Windows:
```
shopping_cart.exe
```

## Usage Guide
-----------
1. **Main Menu Navigation**
   - Select options by entering the corresponding number (1-7)
   - Follow on-screen prompts for each operation
   - Press Enter after operations to return to the main menu

2. **Managing Items**
   - Add items (Option 1): Enter a unique code, name, quantity, and price
   - View cart (Option 2): See a formatted table of all items with subtotals
   - Search items (Option 3): Look up an item using its code
   - Update items (Option 4): Modify quantity or price of existing items
   - Remove items (Option 5): Delete an item from the cart

3. **Billing Operations**
   - Generate Bill (Option 6): Create a formatted receipt with totals
   - A 10% discount is automatically applied for bills totaling 500 or more
   
4. **Exiting the Program**
   - Always use Save & Exit (Option 7) to ensure data is properly saved

## Project Structure
----------------
```
cart-and-billing-system-c/
├── shopping_cart.c     # Main source code
├── bills.txt           # Data file (auto-generated on first run)
└── README.txt          # This documentation file

```

## Data Format
----------
Items are stored in bills.txt using pipe-delimited format:
```
<code>|<name>|<quantity>|<unit_price>|<subtotal>
```

Example:
```
101|Rice|2|50.00|100.00
102|Milk|1|75.00|75.00
```

## Implementation Details
---------------------
- **Discount Logic**: Automatically applies 10% discount for bills ≥ 500
- **Data Loading**: Loads previous cart data from bills.txt on startup
- **Receipt Storage**: Appends formatted receipts to bills.txt
- **Memory Management**: Efficiently handles up to 200 items
- **Input Validation**: Robust validation for all user inputs
- **Error Handling**: Graceful handling of invalid inputs and file operations

## Requirements Fulfillment
-----------------------
This implementation satisfies all project requirements including:
- Multiple user-defined functions
- Two distinct calculations (subtotal, final bill with discount)
- Reporting functionality (formatted receipts)
- File I/O operations (load/save data)
- Use of arrays, loops, and conditional logic
- Proper input validation and error handling
