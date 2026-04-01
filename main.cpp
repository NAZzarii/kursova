#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

struct Ingredient {
    string name;
    double price;
};

class Pizza {
protected:
    string name;
    vector<Ingredient> ingredients;

public:
    Pizza(string n) {
        name = n;
    }

    virtual ~Pizza() {}

    void addIngredient(Ingredient ing) {
        ingredients.push_back(ing);
    }

    string getName() const {
        return name;
    }

    vector<Ingredient> getIngredients() const {
        return ingredients;
    }

    virtual double getPrice() const {
        double total = 50.0; 
        for (size_t i = 0; i < ingredients.size(); i++) {
            total += ingredients[i].price;
        }
        return total;
    }

    virtual void show() const {
        cout << "Піца: " << name << " | Склад: ";
        if (ingredients.empty()) {
            cout << "основа";
        } else {
            for (size_t i = 0; i < ingredients.size(); i++) {
                cout << ingredients[i].name << (i == ingredients.size() - 1 ? "" : ", ");
            }
        }
        cout << " | Вартість: " << getPrice() << " грн" << endl;
    }
    
    bool containsIngredient(const string& ingName) const {
        for (const auto& ing : ingredients) {
            if (ing.name == ingName) return true;
        }
        return false;
    }
};

class StandardPizza : public Pizza {
public:
    StandardPizza(string n) : Pizza(n) {}
};

class CustomPizza : public Pizza {
public:
    CustomPizza() : Pizza("Кастомна піца (Зібрана клієнтом)") {}
};

class Pizzeria {
private:
    vector<Pizza*> menu;
    vector<Ingredient> availableIngredients;
    vector<Pizza*> order;

public:
    Pizzeria() {
        availableIngredients.push_back({"Сир", 20.0});
        availableIngredients.push_back({"Ковбаса", 30.0});
        availableIngredients.push_back({"Гриби", 15.0});
        availableIngredients.push_back({"Помідори", 10.0});
    }

    ~Pizzeria() {
        for (auto p : menu) delete p;
        for (auto p : order) delete p;
    }

    void addPizzaToMenu(Pizza* p) {
        menu.push_back(p);
    }

    void showMenu() const {
        cout << "\n--- МЕНЮ ПІЦЕРІЇ ---" << endl;
        if (menu.empty()) cout << "Меню порожнє." << endl;
        for (size_t i = 0; i < menu.size(); i++) {
            cout << i + 1 << ". ";
            menu[i]->show();
        }
    }

    void showIngredients() const {
        cout << "\n--- ДОСТУПНІ ІНГРЕДІЄНТИ ---" << endl;
        for (size_t i = 0; i < availableIngredients.size(); i++) {
            cout << i + 1 << ". " << availableIngredients[i].name 
                 << " (" << availableIngredients[i].price << " грн)" << endl;
        }
    }

    void addIngredientDB() {
        string name;
        double price;
        cout << "Введіть назву інгредієнта: ";
        cin >> name;
        cout << "Введіть ціну: ";
        cin >> price;
        availableIngredients.push_back({name, price});
        cout << "Інгредієнт додано!" << endl;
    }

    void searchByIngredient() const {
        string searchItem;
        cout << "Введіть назву інгредієнта для пошуку: ";
        cin >> searchItem;
        
        bool found = false;
        cout << "\nРезультати пошуку:" << endl;
        for (const auto& p : menu) {
            if (p->containsIngredient(searchItem)) {
                p->show();
                found = true;
            }
        }
        if (!found) cout << "Піц з таким інгредієнтом не знайдено." << endl;
    }

    void orderStandardPizza() {
        showMenu();
        if (menu.empty()) return;
        
        int choice;
        cout << "Оберіть номер піци для замовлення: ";
        cin >> choice;
        
        if (choice > 0 && choice <= menu.size()) {
            Pizza* newPizza = new StandardPizza(*dynamic_cast<StandardPizza*>(menu[choice - 1]));
            order.push_back(newPizza);
            cout << "Піцу додано до замовлення!" << endl;
        } else {
            cout << "Невірний вибір." << endl;
        }
    }

    void orderCustomPizza() {
        CustomPizza* cp = new CustomPizza();
        int choice = -1;
        while (choice != 0) {
            showIngredients();
            cout << "Оберіть номер інгредієнта (0 - завершити складання): ";
            cin >> choice;
            if (choice > 0 && choice <= availableIngredients.size()) {
                cp->addIngredient(availableIngredients[choice - 1]);
                cout << "Додано: " << availableIngredients[choice - 1].name << endl;
            }
        }
        order.push_back(cp);
        cout << "Кастомну піцу додано до замовлення!" << endl;
    }

    void checkout() {
        if (order.empty()) {
            cout << "Ваше замовлення порожнє!" << endl;
            return;
        }

        double totalSum = 0;
        cout << "\n================ ЧЕК ================" << endl;
        for (const auto& p : order) {
            p->show();
            totalSum += p->getPrice();
        }
        cout << "-------------------------------------" << endl;
        cout << "ДО СПЛАТИ: " << totalSum << " грн" << endl;
        cout << "=====================================" << endl;

        double payment;
        cout << "Внесіть суму для оплати: ";
        cin >> payment;

        if (payment >= totalSum) {
            cout << "Оплата успішна! Ваша решта: " << payment - totalSum << " грн." << endl;
            cout << "Смачного!" << endl;
            for (auto p : order) delete p;
            order.clear();
        } else {
            cout << "Недостатньо коштів! Замовлення скасовано." << endl;
        }
    }

    void saveMenuToFile(const string& filename) const {
        ofstream out(filename);
        if (out.is_open()) {
            for (const auto& p : menu) {
                out << p->getName() << "|";
                vector<Ingredient> ings = p->getIngredients();
                for (size_t i = 0; i < ings.size(); i++) {
                    out << ings[i].name << "-" << ings[i].price;
                    if (i < ings.size() - 1) out << ",";
                }
                out << "\n";
            }
            out.close();
            cout << "Меню збережено!" << endl;
        }
    }

    void loadMenuFromFile(const string& filename) {
        ifstream in(filename);
        if (in.is_open()) {
            for (auto p : menu) delete p;
            menu.clear();

            string line;
            while (getline(in, line)) {
                stringstream ss(line);
                string pizzaName, ingsStr;
                
                getline(ss, pizzaName, '|');
                StandardPizza* sp = new StandardPizza(pizzaName);
                
                getline(ss, ingsStr);
                stringstream ssIngs(ingsStr);
                string ingData;
                
                while (getline(ssIngs, ingData, ',')) {
                    stringstream ssSingle(ingData);
                    string ingName, ingPriceStr;
                    getline(ssSingle, ingName, '-');
                    getline(ssSingle, ingPriceStr);
                    if (!ingName.empty() && !ingPriceStr.empty()) {
                        sp->addIngredient({ingName, stod(ingPriceStr)});
                    }
                }
                menu.push_back(sp);
            }
            in.close();
            cout << "Меню завантажено!" << endl;
        } else {
            cout << "Файл не знайдено." << endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Ukrainian");

    Pizzeria myPizzeria;

    StandardPizza* p1 = new StandardPizza("Маргарита");
    p1->addIngredient({"Сир", 20.0});
    p1->addIngredient({"Помідори", 10.0});
    myPizzeria.addPizzaToMenu(p1);

    StandardPizza* p2 = new StandardPizza("М'ясна");
    p2->addIngredient({"Сир", 20.0});
    p2->addIngredient({"Ковбаса", 30.0});
    myPizzeria.addPizzaToMenu(p2);

    int choice = -1;
    while (choice != 0) {
        cout << "\n====== СИСТЕМА ПІЦЕРІЇ ======" << endl;
        cout << "1. Показати меню піц" << endl;
        cout << "2. Знайти піцу за інгредієнтом" << endl;
        cout << "3. Додати новий інгредієнт в базу" << endl;
        cout << "4. Замовити стандартну піцу" << endl;
        cout << "5. Зібрати свою піцу (Кастомна)" << endl;
        cout << "6. Оплатити замовлення (Чек)" << endl;
        cout << "7. Зберегти меню у файл" << endl;
        cout << "8. Завантажити меню з файлу" << endl;
        cout << "0. Вихід" << endl;
        cout << "Оберіть дію: ";
        cin >> choice;

        if (choice == 1) myPizzeria.showMenu();
        else if (choice == 2) myPizzeria.searchByIngredient();
        else if (choice == 3) myPizzeria.addIngredientDB();
        else if (choice == 4) myPizzeria.orderStandardPizza();
        else if (choice == 5) myPizzeria.orderCustomPizza();
        else if (choice == 6) myPizzeria.checkout();
        else if (choice == 7) myPizzeria.saveMenuToFile("pizzeria_db.txt");
        else if (choice == 8) myPizzeria.loadMenuFromFile("pizzeria_db.txt");
    }

    return 0;
}
