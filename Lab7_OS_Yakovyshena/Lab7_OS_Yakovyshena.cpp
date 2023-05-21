#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

using namespace std;

int f(int x) {
    int sum = 0;
    for (int i = 1; i <= x; ++i) {
        sum += i;
    }
    return sum;
}

int g(int x) {
    int factorial = 1;
    for (int i = 1; i <= x; ++i) {
        factorial *= i;
    }
    return factorial;
}

// Прапорець для зупинки обчислень
atomic<bool> stop(false);

// Функція, яка обчислює f(x)
void calculateF(int x, atomic<int>& result) {
    // Обчислення f(x)
    int fResult = f(x);

    // Збереження результату
    result.store(fResult);

    // Помітити зупинку обчислень
    stop.store(true);
}

// Функція, яка обчислює g(x)
void calculateG(int x, atomic<int>& result) {
    // Обчислення g(x)
    int gResult = g(x);

    // Збереження результату
    result.store(gResult);

    // Помітити зупинку обчислень
    stop.store(true);
}

int main() {
    int x;  // Вхідне значення x

    // Введення значення x
    cout << "Enter the value of x: ";
    cin >> x;

    // Результати обчислень f(x) та g(x)
    atomic<int> fResult(0);
    atomic<int> gResult(0);

    // Створення потоків для обчислення f(x) та g(x)
    thread fThread(calculateF, x, ref(fResult));
    thread gThread(calculateG, x, ref(gResult));

    // Очікування обчислення результатів або введення користувачем команди зупинки
    while (!stop.load()) {
        // Перевірка результатів та відповідна обробка
        if (fResult.load() || gResult.load()) {
            cout << "f(x) || g(x) is true" << endl;
            break;
        }

        // Затримка на 10 секунд
        this_thread::sleep_for(chrono::seconds(10));

        // Перевірка зупинки обчислень
        if (stop.load()) {
            cout << "No result available. What would you like to do?" << endl;
            cout << "1) Continue calculations" << endl;
            cout << "2) Stop" << endl;
            cout << "3) Continue without further prompts" << endl;

            int choice;
            cin >> choice;

            if (choice == 2) {
                // Зупинка обчислень
                break;
            }
            else if (choice == 3) {
                // Продовження обчислень без подальших запитів
                stop.store(false);
            }
        }
    }

    // Завершення потоків
    fThread.join();
    gThread.join();

    return 0;
}
