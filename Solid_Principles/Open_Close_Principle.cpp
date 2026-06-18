#include <iostream>
#include <vector>

using namespace std;

// Product class representing any item in eCommerce.
class Product {
public:
    string name;
    double price;

    Product(string name, double price) {
        this->name = name;
        this->price = price;
    }
};

//1. ShoppingCart: Only responsible for Cart related business logic.
class ShoppingCart {
private:
    vector<Product*> products; // Store heap-allocated products

public:
    void addProduct(Product* p) { 
        products.push_back(p);
    }

    const vector<Product*>& getProducts() { 
        return products;
    } 

    //Calculates total price in cart.
    double calculateTotal() {
        double total = 0;
        for (auto p : products) {
            total += p->price;
        }
        return total;
    }
};

// 2. ShoppingCartPrinter: Only responsible for printing invoices
class ShoppingCartPrinter {
private:
    ShoppingCart* cart; 

public:
    ShoppingCartPrinter(ShoppingCart* cart) { 
        this->cart = cart; 
    }

    void printInvoice() {
        cout << "Shopping Cart Invoice:\n";
        for (auto p : cart->getProducts()) {
            cout << p->name << " - Rs " << p->price << endl;
        }
        cout << "Total: Rs " << cart->calculateTotal() << endl;
    }
};

class Persistence{
    private: 
    ShoppingCart* cart;

    public: 
    virtual void save(ShoppingCart* cart)=0;
};

class savetoSQL : public Persistence{
    public:
    void save(ShoppingCart* cart)override{
        cout<<"Saving to sql db"<<endl;
    }
};

class mongodb : public Persistence{
    public:
    void save(ShoppingCart* cart)override{
        cout<<"Saving to mongodb db"<<endl;
    }
};

int main() {
    Product* p1 = new Product("fridge",6000);
    Product* p2 = new Product("tv",12000);
    ShoppingCart* c = new ShoppingCart();
    
    c->addProduct(p1);
    c->addProduct(p2);

    Persistence* sql = new savetoSQL();
    Persistence* mongo = new mongodb();

    sql->save(c);
    mongo->save(c);
    return 0;
}