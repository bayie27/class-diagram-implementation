#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <regex>

using namespace std;

// -- Validation Functions

// Validates if the input is an integer and converts it to an integer if valid.
bool isValidInteger(const string& input, int& output) {
    try {
        size_t pos;
        output = stoi(input, &pos);
        if (pos == input.length()) { // Ensures entire string is converted
            return true;
        }
    } catch (const exception&) {
        return false; // If conversion fails, return false
    }
    return false;
}

// Validates if the input is within a specified menu number range.
bool isValidMenuNumber(const string& input, int& output, int min, int max) {
    if (input.find(' ') != string::npos) return false; // Rejects input with spaces
    if (isValidInteger(input, output)) {
        return output >= min && output <= max; // Ensures number is within range
    }
    return false;
}

// Validates if the input is a valid numeric payment amount.
bool isValidPayment(const string& input) {
    regex realNumberRegex("^-?\\d*\\.?\\d+$"); // Regular expression for real numbers
    return regex_match(input, realNumberRegex);
}

// Validates if the input is a valid 'yes' or 'no' choice.
bool isValidChoice(const string& input) {
    return input == "y" || input == "Y" || input == "n" || input == "N";
}

// -- Classes

// Product Class: Represents a product with an ID, name, and price.
class Product {
public:
    int id;
    string name;
    double price;
    
    // Constructor to initialize a product.
    Product(int id, string name, double price) : id(id), name(name), price(price) {}
};

// Payment Class: Handles processing of customer payments.
class Payment {
public:
    static void processPayment(double amount) {
        string input;
        double paidAmount;
        do {
            cout << "Enter payment amount: ₱";
            getline(cin, input);
            
            // Validate if input is a valid numeric amount.
            if (!isValidPayment(input)) {
                cout << "Invalid input. Please enter a valid amount." << endl;
                continue;
            }
            
            paidAmount = stod(input); // Convert string to double
            
            // Ensure the payment is sufficient.
            if (paidAmount < amount) {
                cout << "Insufficient amount. Please enter at least ₱" << amount << endl;
            }
        } while (paidAmount < amount);
        
        // Payment successful message and change calculation.
        cout << "Payment successful!" << endl;
        cout << "Your change is: ₱" << (paidAmount - amount) << endl;
    }
};

// Order Class: Manages order processing and history.
class Order {
private:
    static vector<vector<pair<Product, int>>> orderHistory; // Stores past orders
    
public:
    // Processes an order by displaying details, calculating the total, and handling payment.
    static void processOrder(vector<pair<Product, int>> items) {
        if (items.empty()) {
            cout << "No items to checkout." << endl;
            return;
        }
        
        static int orderId = 1;
        double total = 0;
        
        cout << "\nOrder ID: " << orderId++ << endl;
        cout << "Order Details:" << endl;
        cout << "----------------------------------------" << endl;
        cout << "ID   | Name             | Price   | Qty" << endl;
        cout << "----------------------------------------" << endl;
        
        // Display order items and compute total cost.
        for (const auto &item : items) {
            cout << setw(4) << item.first.id << " | "
                 << setw(16) << item.first.name << " | ₱"
                 << setw(6) << item.first.price << " | "
                 << setw(3) << item.second << endl;
            total += item.first.price * item.second;
        }
        cout << "----------------------------------------" << endl;
        cout << "Total Amount: ₱" << total << endl;
        
        // Process the payment.
        Payment::processPayment(total);
        
        cout << "You have successfully checked out the products!" << endl;
        orderHistory.push_back(items); // Store order in history
    }
    
    // Displays all previous orders.
    static void viewOrders() {
        if (orderHistory.empty()) {
            cout << "No previous orders found." << endl;
            return;
        }
        
        int orderNum = 1;
        
        // Iterate through each order in history.
        for (const auto &order : orderHistory) {
            double total = 0;
            
            cout << "\nOrder " << orderNum++ << ":" << endl;
            cout << "Order Details:" << endl;
            cout << "----------------------------------------" << endl;
            cout << "ID   | Name             | Price   | Qty" << endl;
            cout << "----------------------------------------" << endl;
            for (const auto &item : order) {
                cout << setw(4) << item.first.id << " | "
                     << setw(16) << item.first.name << " | ₱"
                     << setw(6) << item.first.price << " | "
                     << setw(3) << item.second << endl;
                total += item.first.price * item.second;
            }
            cout << "----------------------------------------" << endl;
            cout << "Total Amount: ₱" << total << endl;
        }
    }
};

// Initialize the static order history vector.
vector<vector<pair<Product, int>>> Order::orderHistory;

// ShoppingCart class: Manages the shopping cart operations.
class ShoppingCart {
    private:
        // Vector to store products in the cart along with their quantity
        vector<pair<Product, int>> cart;
        
    public:
        // Function to add a product to the shopping cart
        void addProduct(const Product &product) {
            // Check if the product already exists in the cart
            for (auto &item : cart) {
                if (item.first.id == product.id) {
                    item.second++; // Increase quantity if the product exists
                    cout << "Product added successfully!" << endl;
                    return;
                }
            }
            // If the product is not in the cart, add it with quantity 1
            cart.push_back({product, 1});
            cout << "Product added successfully!" << endl;
        }
        
        // Function to display the contents of the shopping cart
        void viewCart() {
            if (cart.empty()) { // Check if cart is empty
                cout << "Your shopping cart is empty." << endl;
                return;
            }
            
            // Display cart header
            cout << "\nShopping Cart:" << endl;
            cout << "----------------------------------------" << endl;
            cout << "ID   | Name             | Price   | Qty" << endl;
            cout << "----------------------------------------" << endl;
            
            // Iterate through cart and display product details
            for (const auto &item : cart) {
                cout << setw(4) << item.first.id << " | "
                     << setw(16) << item.first.name << " | ₱"
                     << setw(6) << item.first.price << " | "
                     << setw(3) << item.second << endl;
            }
            cout << "----------------------------------------" << endl;
            
            // Prompt user for checkout
            string checkoutChoice;
            do {
                cout << "Do you want to check out all the products? (Y/N): ";
                getline(cin, checkoutChoice);
                
                if (!isValidChoice(checkoutChoice)) {
                    cout << "Invalid input. Please enter 'Y' or 'N' only." << endl;
                }
            } while (!isValidChoice(checkoutChoice));
            
            // Process order if user confirms checkout
            if (checkoutChoice == "Y" || checkoutChoice == "y") {
                Order::processOrder(cart);
                clearCart(); // Clear cart after checkout
            }        
        }
        
        // Function to clear the shopping cart
        void clearCart() {
            cart.clear();
        }
    };
    
    // Function to display available products in the store
    void displayProducts(const vector<Product> &products) {
        cout << "\nAvailable Products:" << endl;
        cout << "--------------------------------" << endl;
        cout << "ID   | Name             | Price" << endl;
        cout << "--------------------------------" << endl;
        
        // Iterate through product list and display product details
        for (const auto &product : products) {
            cout << setw(4) << product.id << " | "
                 << setw(16) << product.name << " | ₱"
                 << setw(3) << product.price << endl;
        }
        cout << "--------------------------------" << endl;
    }
    
int main() {
    // Initialize list of products available for purchase
    vector<Product> products = {
        {1, "Kopiko Lucky Day", 24.0},
        {2, "Nescafé Black", 57.0},
        {3, "Minute Maid", 38.0},
        {4, "C2 Apple", 32.0},
        {5, "Pocari Sweat", 51.0}
    };
        
    ShoppingCart cart; // Create a shopping cart instance
    string choice;
        
    do {
        // Display main menu
        cout << "\n --- DANIBOY'S ONLINE SARI-SARI STORE --- \n";
        cout << "Menu:\n";
        cout << "1. View Products\n";
        cout << "2. View Shopping Cart\n";
        cout << "3. View Orders\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        getline(cin, choice);
            
        int option;
        if (isValidMenuNumber(choice, option, 1, 4)) {
            switch (option) {
                case 1: {
                    displayProducts(products);
                    string productIdInput, addMoreInput;
                    int productId;
                    bool found;
                    
                    do {
                        found = false;
                            
                        // Validate product ID input
                        do {
                            cout << "Enter the ID of the product you want to add: ";
                            getline(cin, productIdInput);
                            
                            if (isValidInteger(productIdInput, productId)) {
                                for (const auto &product : products) {
                                    if (product.id == productId) {
                                        cart.addProduct(product);
                                        found = true;
                                        break;
                                }
                                }
                                if (!found) {
                                    cout << "Invalid product ID. Please enter a valid one." << endl;
                                }
                            } else {
                                cout << "Invalid input. Please enter a numeric product ID." << endl;
                            }
                        } while (!found); // Repeat until a valid product ID is entered
                        
                        // Validate user input for adding more products
                        do {
                            cout << "Do you want to add another product? (Y/N): ";
                            getline(cin, addMoreInput);
                                
                            if (!isValidChoice(addMoreInput)) {
                                cout << "Invalid input. Please enter 'Y' or 'N' only." << endl;
                            }
                        } while (!isValidChoice(addMoreInput));
                        
                     } while (addMoreInput == "Y" || addMoreInput == "y"); // Continue only if 'Y' is entered
                    break;
                }
                case 2:
                    cart.viewCart(); // View items in the cart
                    break;
                case 3:
                    Order::viewOrders(); // View past orders
                    break;
                case 4:
                    cout << "Thank you for shopping with us!" << endl;                        
                    break;
            }
        } else {
            cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
         }
    } while (choice != "4"); // Repeat menu until user exits
        
    return 0;
}