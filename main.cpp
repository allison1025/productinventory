//purpose of program: inventory management system for an electronics store. reads in a txt file that has information on various electronic products such as product condition, quanity, and price. it'll then show the items that are defective.
//revision history: 10/21 - defined class data members (InventorySystem, Product, InventoryItem) . 10/23 - wrote methods for the necessary functions in the classes.
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
using namespace std;


int makeProductID () {
    int productId = 0;
    productId = rand() % 10000;
    return productId;
}

class InventoryItem {
public:
    InventoryItem();
    InventoryItem(string name, int quantity);
    ~InventoryItem();
    string get_itemName() const;
    int get_itemQuantity() const;
    void set_itemName(string name);
    void set_itemQuantity(int quantity);
    void Display() const;
    
protected:
    string itemName_;
    int itemQuantity_;
};

InventoryItem::InventoryItem() {
    itemQuantity_ = 0;
}

InventoryItem::InventoryItem(string name, int quantity) {
    itemName_ = name;
    itemQuantity_ = quantity;
}

void InventoryItem::Display() const {
    cout << "Name:" << itemName_ << endl;
    cout << "Quantity:" << itemQuantity_ << endl;
}

string InventoryItem::get_itemName() const {
    return itemName_;
}

int InventoryItem::get_itemQuantity() const {
    return itemQuantity_;
}

InventoryItem::~InventoryItem() {
    cout << "InventoryItem " << itemName_ << " with " << itemQuantity_ << " items destroyed..." << endl;
}

class Product:
public InventoryItem {
public:
    Product();
    Product(string name, int quantity, double price, string condition);
    int get_productID() const;
    double get_productPrice() const;
    string get_productCondition() const;
    void set_productID();
    void set_productPrice(double price);
    void set_productCondition(string condition);
    void DisplayProduct() const;
    ~Product();
    
private:
    int productID_;
    double productPrice_;
    string productCondition_;
};

Product::Product() {
    productID_ = 0;
    productPrice_ = 0.0;
}

Product::Product(string name, int quantity, double price, string condition) {
    itemName_ = name;
    itemQuantity_ = quantity;
    productPrice_ = price;
    productCondition_ = condition;
    productID_ = makeProductID();
}

void Product::set_productID() {
    productID_ = makeProductID();
}

void Product::set_productPrice(double price) {
    productPrice_ = price;
}

void Product::set_productCondition(string condition) {
    productCondition_ = condition;
}

int Product::get_productID() const {
    return productID_;
}

double Product::get_productPrice() const {
    return productPrice_;
}

string Product::get_productCondition() const {
    return productCondition_;
}

void Product::DisplayProduct() const {
    Display();
    cout << "Product #" << productID_ << endl;
    cout << "Price: " << setprecision(2) << fixed << productPrice_ << endl; //floating data w 2 digits after decimal
    cout << "Condition: ";
    if (productCondition_ == "D") {
        cout << "Defective" << endl;
    }
    if (productCondition_ == "U") {
        cout << "Used" << endl;
    }
    if (productCondition_ == "R") {
        cout << "Refurbished" << endl;
    }
    if (productCondition_ == "N") {
        cout << "New" << endl;
    }
    cout << "////////////////////////" << endl;
}

class InventorySystem {
public:
    InventorySystem();
    InventorySystem(string store_name, int store_id);
    void BuildInventory();
    void ShowInventory() const;
    void ShowDefectInventory() const;
    void Terminate() const;
    void set_store_name(string name);
    void set_store_id(int id);
    void set_item_count(int count);
    string get_store_name() const;
    int get_store_id() const;
    int get_item_count() const;
    ~InventorySystem();
    
private:
    string store_name_;
    int store_id_;
    InventoryItem* items_[512];
    int item_count_;
};

InventorySystem::InventorySystem() {
    store_name_ = "";
    store_id_ = 0;
    item_count_ = 0;
    for (int i = 0; i< 512; i++) {
        items_[i] = NULL;
    }
}

InventorySystem::InventorySystem(string store_name, int store_id) {
    store_name_ = store_name;
    store_id_ = store_id;
    item_count_ = 0;
    for (int i = 0; i < 512; i++) {
        items_[i] = NULL;
    }
}

//function - read in txt file that has product on each line --> store in ItemList, update Item Count
void InventorySystem::BuildInventory() {
    ifstream file ("inventory.txt");
    InventoryItem* i = NULL;
    Product* p = NULL;
    string name;
    int quantity = 0;
    double price = 0.0;
    string condition;
    string buffer;
    if (!file) {
        cout << "can't open file";
        exit(-1);
    }
    
    while (getline(file, buffer, ';') ) {
        name = buffer;
        getline(file, buffer, ';');
        quantity= atol(buffer.c_str());
        getline (file, buffer, ';') ;
        price = atof(buffer.c_str());
        getline (file, buffer, '\n') ;
        condition = buffer.c_str();
        i = new Product(name, quantity, price, condition);
        p = static_cast <Product*>(i);
        p->set_productCondition(condition);
        p->set_productID();
        p->set_productPrice(price);
        items_[item_count_] = p;
        ++item_count_;
    }
}

void InventorySystem::ShowInventory() const {
    cout << "Inventory at " << store_name_ << endl;
    cout << "////////////////////////" << endl;
    for (int i = 0; i < item_count_; ++i) {
        Product* p = static_cast <Product*>(items_[i]);
        p->DisplayProduct();
    }
}

void InventorySystem::ShowDefectInventory() const {
    cout << endl << endl << "defect inventory: " << endl;
    cout << "///////////////////////" << endl;
    for (int i = 0; i<item_count_; ++i) {
        Product* p = static_cast <Product*>(items_[i]);
        if (p->get_productCondition() == "D") {
            p->DisplayProduct();
        }
    }
}

void InventorySystem::Terminate() const {
    ofstream myfile ("end.txt");
    if (myfile.is_open()) {
        for (int i = 0; i < item_count_; ++i) {
            Product* p = static_cast <Product*>(items_[i]);
            myfile << p->get_itemName() << ";" << p->get_itemQuantity() << ";" << p->get_productPrice() << ";"
            << p->get_productCondition()<< endl;
        }
    }
    else {
        throw("can't write to the file");
    }
}

InventorySystem::~InventorySystem() {
    for (int i = 0; i < 512; i++) {
        delete items_[i];
    }
}

int main() {
    InventorySystem* p_system = NULL;
    p_system = new InventorySystem("Frys Electronics", 1025); //constructor
    p_system->BuildInventory();
    p_system->ShowInventory();
    p_system->ShowDefectInventory();
    p_system->Terminate();
    delete p_system;
    exit(0);
}

/* output:
 Inventory at Best Buy Auto-Generated Report:
 ////////////////////////
 Name: Panasonic DVD Player
 Quantity: 5
 Product #5249
 Price: 125.99
 Condition: ////////////////////////
 Name:
 Panasonic DVD Player
 Quantity: 43
 Product #3658
 Price: 125.99
 Condition: New
 ////////////////////////
 Name:
 Sony Camcoder
 Quantity: 125
 Product #1272
 Price: 395.99
 Condition: Used
 ////////////////////////
 Name:
 Samsung TV
 Quantity: 15
 Product #878
 Price: 298.99
 Condition: ////////////////////////
 Name:
 Apple iPhone 6
 Quantity: 1200
 Product #7709
 Price: 399.00
 Condition: New
 ////////////////////////
 Name:
 Apple iPhone 7
 Quantity: 1220
 Product #8165
 Price: 339.00
 Condition: Defective
 ////////////////////////
 Name:
 Apple iPad
 Quantity: 100
 Product #3042
 Price: 319.00
 Condition: Used
 ////////////////////////
 Name:
 Motorola Razr
 Quantity: 122
 Product #2503
 Price: 401.00
 Condition: Refurbished
 ////////////////////////
 Name:
 Apple iPhone 9
 Quantity: 121
 Product #1729
 Price: 309.00
 Condition: Defective
 ////////////////////////
 Name:
 Apple MacBook
 Quantity: 1500
 Product #2612
 Price: 219.00
 Condition: Used
 ////////////////////////
 Name:
 Motorola flipphone
 Quantity: 122
 Product #3169
 Price: 50.00
 Condition: Refurbished
 ////////////////////////
 Name:
 Amazon Alexa
 Quantity: 10
 Product #7157
 Price: 309.00
 Condition: New
 ////////////////////////
 Name:
 Beats Headphones
 Quantity: 150
 Product #933
 Price: 300.00
 Condition: Used
 ////////////////////////
 Name:
 Beats Earbuds
 Quantity: 22
 Product #278
 Price: 90.00
 Condition: Refurbished
 ////////////////////////
 Name:
 Huawei cellPhone
 Quantity: 80
 Product #5335
 Price: 42.00
 Condition: Defective
 ////////////////////////
 Name:
 HTC cellPhone
 Quantity: 1500
 Product #7826
 Price: 219.00
 Condition: New
 ////////////////////////
 
 defect inventory:
 ///////////////////////
 Name:
 Apple iPhone 7
 Quantity: 1220
 Product #8165
 Price: 339.00
 Condition: Defective
 ////////////////////////
 Name:
 Apple iPhone 9
 Quantity: 121
 Product #1729
 Price: 309.00
 Condition: Defective
 ////////////////////////
 Name:
 Huawei cellPhone
 Quantity: 80
 Product #5335
 Price: 42.00
 Condition: Defective
 ////////////////////////
 InventoryItem Panasonic DVD Player with 5 items destroyed...
 InventoryItem
 Panasonic DVD Player with 43 items destroyed...
 InventoryItem
 Sony Camcoder with 125 items destroyed...
 InventoryItem
 Samsung TV with 15 items destroyed...
 InventoryItem
 Apple iPhone 6 with 1200 items destroyed...
 InventoryItem
 Apple iPhone 7 with 1220 items destroyed...
 InventoryItem
 Apple iPad with 100 items destroyed...
 InventoryItem
 Motorola Razr with 122 items destroyed...
 InventoryItem
 Apple iPhone 9 with 121 items destroyed...
 InventoryItem
 Apple MacBook with 1500 items destroyed...
 InventoryItem
 Motorola flipphone with 122 items destroyed...
 InventoryItem
 Amazon Alexa with 10 items destroyed...
 InventoryItem
 Beats Headphones with 150 items destroyed...
 InventoryItem
 Beats Earbuds with 22 items destroyed...
 InventoryItem
 Huawei cellPhone with 80 items destroyed...
 InventoryItem
 HTC cellPhone with 1500 items destroyed...
 */
