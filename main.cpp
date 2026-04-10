#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct Product {
    string name;
    int price;
};

class Pizza {
public:
    string name;
    vector<string> ingredients;
    int price;

    Pizza() { price = 0; }
    virtual ~Pizza() {}

    virtual void show() {
        cout << name << " | Ціна: " << price << " | Склад: ";
        for (int i = 0; i < ingredients.size(); i++) {
            cout << ingredients[i] << " ";
        }
        cout << endl;
    }
};

class StandardPizza : public Pizza {
public:
    StandardPizza(string n, int p) {
        name = n;
        price = p;
    }
};

class CustomPizza : public Pizza {
public:
    CustomPizza() {
        name = "Кастомна піца";
        price = 20; 
    }
    void addIngredient(Product p) {
        ingredients.push_back(p.name);
        price += p.price;
    }
};

class Pizzeria {
public:
    vector<Pizza*> menu;
    vector<Product> products;
    vector<Pizza*> order;

    void addProduct() {
        Product p;
        cout << "Назва: ";
        cin >> p.name;
        cout << "Ціна: ";
        cin >> p.price;
        products.push_back(p);
    }

    void removeProduct() {
        int id;
        cout << "Номер для видалення: ";
        cin >> id;
        products.erase(products.begin() + id - 1);
    }

    void editProduct() {
        int id;
        cout << "Номер для редагування: ";
        cin >> id;
        cout << "Нова назва: ";
        cin >> products[id - 1].name;
        cout << "Нова ціна: ";
        cin >> products[id - 1].price;
    }

    void addPizza() {
        string n;
        int p;
        cout << "Назва піци: ";
        cin >> n;
        cout << "Ціна: ";
        cin >> p;
        StandardPizza* sp = new StandardPizza(n, p);
        int count;
        cout << "Скільки інгредієнтів? ";
        cin >> count;
        for (int i = 0; i < count; i++) {
            string ing;
            cout << "Інгредієнт " << i + 1 << ": ";
            cin >> ing;
            sp->ingredients.push_back(ing);
        }
        menu.push_back(sp);
    }

    void removePizza() {
        int id;
        cout << "Номер для видалення: ";
        cin >> id;
        menu.erase(menu.begin() + id - 1);
    }

    void showMenu() {
        for (int i = 0; i < menu.size(); i++) {
            cout << i + 1 << ". ";
            menu[i]->show();
        }
    }

    void showProducts() {
        for (int i = 0; i < products.size(); i++) {
            cout << i + 1 << ". " << products[i].name << " (" << products[i].price << " грн)\n";
        }
    }

    void searchPizza() {
        string q;
        cout << "Який інгредієнт шукаємо: ";
        cin >> q;
        for (int i = 0; i < menu.size(); i++) {
            for (int j = 0; j < menu[i]->ingredients.size(); j++) {
                if (menu[i]->ingredients[j] == q) {
                    menu[i]->show();
                }
            }
        }
    }

    void save() {
        ofstream f("pizza.txt");
        f << menu.size() << endl;
        for (int i = 0; i < menu.size(); i++) {
            f << menu[i]->name << endl;
            f << menu[i]->price << endl;
            f << menu[i]->ingredients.size() << endl;
            for (int j = 0; j < menu[i]->ingredients.size(); j++) {
                f << menu[i]->ingredients[j] << endl;
            }
        }
        f.close();
    }

    void load() {
        ifstream f("pizza.txt");
        if (!f.is_open()) return;
        menu.clear();
        int count;
        f >> count;
        for (int i = 0; i < count; i++) {
            string n;
            int p, icount;
            f >> n >> p >> icount;
            StandardPizza* sp = new StandardPizza(n, p);
            for (int j = 0; j < icount; j++) {
                string ing;
                f >> ing;
                sp->ingredients.push_back(ing);
            }
            menu.push_back(sp);
        }
        f.close();
    }

    void orderStandard() {
        showMenu();
        int id;
        cout << "Оберіть номер піци: ";
        cin >> id;
        order.push_back(menu[id - 1]);
    }

    void orderCustom() {
        CustomPizza* cp = new CustomPizza();
        int id = -1;
        while (id != 0) {
            showProducts();
            cout << "Номер продукту (0 - завершити): ";
            cin >> id;
            if (id > 0) {
                cp->addIngredient(products[id - 1]);
            }
        }
        order.push_back(cp);
    }

    void pay() {
        int sum = 0;
        cout << "--- ЧЕК ---\n";
        for (int i = 0; i < order.size(); i++) {
            order[i]->show();
            sum += order[i]->price;
        }
        cout << "До сплати: " << sum << " грн\n";
        int money;
        cout << "Даю гроші: ";
        cin >> money;
        if (money >= sum) {
            cout << "Решта: " << money - sum << " грн\n";
            order.clear();
        }
    }
};

int main() {
    system("chcp 1251>nul");
    Pizzeria pz;
    int choice = -1;
    
    while (choice != 0) {
        cout << "\n1.Додати піцу  2.Видалити піцу  3.Меню  4.Зберегти  5.Завантажити\n";
        cout << "6.Додати продукт  7.Видалити продукт  8.Редагувати продукт\n";
        cout << "9.Пошук  10.Замовити стандартну  11.Замовити кастомну  12.Оплатити  0.Вихід\n";
        cout << "Вибір: ";
        cin >> choice;

        if (choice == 1) pz.addPizza();
        if (choice == 2) pz.removePizza();
        if (choice == 3) pz.showMenu();
        if (choice == 4) pz.save();
        if (choice == 5) pz.load();
        if (choice == 6) pz.addProduct();
        if (choice == 7) pz.removeProduct();
        if (choice == 8) pz.editProduct();
        if (choice == 9) pz.searchPizza();
        if (choice == 10) pz.orderStandard();
        if (choice == 11) pz.orderCustom();
        if (choice == 12) pz.pay();
    }
    return 0;
}
