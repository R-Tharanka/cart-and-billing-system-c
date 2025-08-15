SHOPPING CART AND BILL GENERATOR
================================

Project Description
------------------
This is a terminal-based Shopping Cart and Bill Generator system implemented in C.
It allows users to manage a list of items for purchase, compute subtotals and totals,
apply discounts, and generate formatted receipts.

Features
--------
1. Add items to the cart with code, name, quantity, and price
2. View all items in the cart
3. Search for items by code
4. Update item quantities or prices
5. Remove items from the cart
6. Generate formatted receipts
7. Save cart data and receipts to a text file

Compilation Instructions
-----------------------
To compile the program using gcc:

```
gcc -o shopping_cart shopping_cart.c
```

Running the Program
------------------
After compilation, run the program using:

```
shopping_cart
```

On Windows:
```
shopping_cart.exe
```

Usage
-----
1. Follow the on-screen menu to navigate through options
2. Add items using option 1
3. View your cart using option 2
4. Search for specific items using option 3
5. Update quantities or prices using option 4
6. Remove items using option 5
7. Generate a receipt using option 6
8. Save and exit using option 7

File Structure
-------------
- shopping_cart.c: Main source code
- bills.txt: Data file for storing cart items and receipts

Notes
-----
- The program automatically loads previous cart data from bills.txt on startup
- All receipts are appended to bills.txt
- A 10% discount is applied to bills totaling 500 or more
