
#include <iostream> 
#include <fstream> 
#include <string> 

using namespace std;

const int MAX_PRODUCTS = 10;  // Количество товаров 
const int MAX_CART_ITEMS = 10; // Максимальное количество товаров в корзине 

// Структура для товара 
struct Product
{
    string name;
    double price;
    int quantity;
};

// Структура для пользователя 
struct User
{
    string username;
    string password;
};

// Класс магазина 
class Store
{
private:
    Product products[MAX_PRODUCTS];
    Product cart[MAX_CART_ITEMS];
    int cartSize = 0;
    User currentUser;
    bool isLoggedIn = false;
    const double deliveryCost = 200.0; // Стоимость доставки 

public:
    Store()
    {
        products[0] = { "Хлеб", 50, 10 };
        products[1] = { "Молоко", 80, 15 };
        products[2] = { "Яблоки", 100, 20 };
        products[3] = { "Сахар", 90, 12 };
        products[4] = { "Чай", 120, 10 };
        products[5] = { "Кофе", 250, 8 };
        products[6] = { "Мясо", 450, 5 };
        products[7] = { "Рыба", 400, 7 };
        products[8] = { "Картофель", 30, 30 };
        products[9] = { "Макароны", 70, 14 };
    }

    void registerUser()
    {
        User user;
        cout << "Регистрация\n\nВведите имя пользователя: ";
        cin >> user.username;
        cout << "Введите пароль: ";
        cin >> user.password;

        ofstream file("users.txt", ios::app);
        file << user.username << " " << user.password << endl;
        file.close();

        cout << "Регистрация успешна!\n\n";
    }

    bool loginUser()
    {
        string username, password;
        cout << "Вход в аккаунт\n\nВведите имя пользователя: ";
        cin >> username;
        cout << "\nВведите пароль: ";
        cin >> password;

        ifstream file("users.txt");
        string u, p;
        while (file >> u >> p)
        {
            if (u == username && p == password)
            {
                currentUser = { username, password };
                isLoggedIn = true;
                cout << "Вход выполнен успешно!\n\n";
                loadCart();
                return true;
            }
        }
        cout << "Ошибка входа! Неверные данные.\n\n";
        return false;
    }

    void showProducts()
    {
        cout << "\n\nСписок товаров:\n\n";
        for (int i = 0; i < MAX_PRODUCTS; i++)
        {
            cout << i + 1 << ". " << products[i].name << " - "
                << products[i].price << " руб. (Остаток: "
                << products[i].quantity << ")\n\n";
        }
    }

    void addToCart()
    {
        if (!isLoggedIn)
        {
            cout << "Сначала войдите в аккаунт!\n\n";
            return;
        }

        int choice, qty;
        showProducts();
        cout << "\nВведите номер товара: ";
        cin >> choice;
        cout << "\nВведите количество: ";
        cin >> qty;

        if (choice < 1 || qty > products[choice - 1].quantity)
        {
            cout << "\nОшибка! Некорректный выбор или недостаточно товара.\n\n";
            return;
        }

        cart[cartSize] = { products[choice - 1].name, products[choice - 1].price, qty };
        cartSize++;
        products[choice - 1].quantity -= qty;

        saveCart();
        cout << "\nТовар добавлен в корзину!\n";
    }

    void viewCart()
    {
        if (!isLoggedIn)
        {
            cout << "\nСначала войдите в аккаунт!\n";
            return;
        }

        double total = 0;
        cout << "\n\nВаша корзина:\n\n";
        for (int i = 0; i < cartSize; i++) {
            cout << cart[i].name << " - " << cart[i].price << " руб. x "
                << cart[i].quantity << " = " << cart[i].price * cart[i].quantity << " руб.\n";
            total += cart[i].price * cart[i].quantity;
        }

        cout << "Итого: " << total << " руб.\n";
    }

    void checkout()
    {
        if (!isLoggedIn)
        {
            cout << "\nСначала войдите в аккаунт!\n"; 
                return;
        }

        if (cartSize == 0)
        {
            cout << "\nВаша корзина пуста!\n";
            return;
        }
        int deliveryOption;
        double finalCost = 0;
        cout << "\n\nВыберите способ получения заказа:\n1. Доставка на дом\n2. Самовывоз\nВаш выбор: ";
        cin >> deliveryOption;

        if (deliveryOption == 1)
        {
            string address;
            cout << "\n\nВведите адрес доставки: ";
            cin.ignore();
            getline(cin, address);
            cout << "\nВаш заказ будет доставлен по адресу: " << address << endl;
            finalCost = deliveryCost;
        }
        else
        {
            cout << "\n\nСамовывоз доступен по адресу: г. Липецк, ул. Шерстобитова, д. 26\n";
        }

        double total = 0;
        cout << "\nВаш заказ оформлен!\nТовары:\n";
        for (int i = 0; i < cartSize; i++)
        {
            cout << cart[i].name << " - " << cart[i].price << " руб. x "
                << cart[i].quantity << " = " << cart[i].price * cart[i].quantity << " руб.\n";
            total += cart[i].price * cart[i].quantity;
        }

        cout << "\nСтоимость доставки: " << finalCost << " руб.\n";
        cout << "\nОбщая сумма к оплате: " << total + finalCost << " руб.\n";
        cout << "\nСпасибо за покупку!\n";

        cartSize = 0;
        saveCart();
    }

    void saveCart()
    {
        if (!isLoggedIn) return;

        ofstream file("cart_" + currentUser.username + ".txt");
        for (int i = 0; i < cartSize; i++)
        {
            file << cart[i].name << " " << cart[i].price << " " << cart[i].quantity << endl;
        }
        file.close();
    }

    void loadCart()
    {
        if (!isLoggedIn) return;

        ifstream file("cart_" + currentUser.username + ".txt");
        if (!file) return;

        cartSize = 0;
        while (file >> cart[cartSize].name >> cart[cartSize].price >> cart[cartSize].quantity)
        {
            cartSize++;
        }
        file.close();
    }

    bool isUserLoggedIn()
    {
        return isLoggedIn;
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");

    Store store;
    int choice;

    do
    {
        if (!store.isUserLoggedIn())
        {
            cout << "\nМеню:\n1. Регистрация\n2. Вход\n0. Выход\n\nВыберите действие: ";
        }
        else
        {
            cout << "\nМеню:\n3. Просмотр товаров\n4. Добавить в корзину\n5. Посмотреть корзину\n6. Оформить заказ\n0. Выход\n\nВыберите действие: ";
        }

        cin >> choice;

        if (!store.isUserLoggedIn())
        {
            if (choice == 1) store.registerUser();
            else if (choice == 2) store.loginUser();
            else if (choice == 0) cout << "Выход из программы.\n";
            else cout << "Некорректный ввод. Попробуйте снова.\n";
        }

        else
        {
            switch (choice)
            {
            case 3: store.showProducts(); break;
            case 4: store.addToCart(); break;
            case 5: store.viewCart(); break;
            case 6: store.checkout(); break;
            case 0: cout << "Выход из программы.\n"; break;
            default: cout << "Некорректный ввод. Попробуйте снова.\n";
            }
        }
    } while (choice != 0);

    return 0;
}

