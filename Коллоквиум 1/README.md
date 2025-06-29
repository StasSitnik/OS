# Ситник Станислав Егорович. 11 группа, вариант 2

# 1. Win API, необходимое для решения Лабораторной работы №2

- CreateThread (создание потока)
- CreateProcess (создание процесса)
- ExitThread (завершение потока)
- CreateSemaphore (создание семафора)
- ReleaseSemaphore (увеличени значения семафора)
- WaitForSingleObject (ожидание завершения потока)
- EnterCriticalSection (получение блокироки на критическую секцию)
- LeaveCriticalSection (освобождение критической секции)
- InterlockedIncrement (атомарно увелечение на единицу)
- InterlockedDecrement (атомарно уменьшение на единицу)

# 2. Что такое процесс в ОС Windows?

Процесс – это экземпляр исполняемой программы с собственным виртуальным адресным пространством, системными ресурсами и одним или несколькими потоками выполнения.

# 3. Что такое Критическая секция?

Критическая секция — механизм синхронизации потоков, позволяющий предотвратить одновременное выполнение некоторого набора операций (обычно связанных с доступом к данным) несколькими потоками.

# 4. Что такое Семафор?

Семафор – механизм синхронизации, используемый для управления доступом к общему ресурсу с возможностью ограничения числа одновременно работающих потоков.

# 5. Сравнительный анализ стандартов C++  

C++98 характеризуется жесткой типизацией, ограниченными возможностями стандартной библиотеки и ручным управлением памятью. Разработчики часто сталкиваются с необходимостью самостоятельно управлять ресурсами, что увеличивает риск возникновения ошибок, таких как утечки памяти.

С использованием библиотеки Boost, C++98 получает дополнительные возможности, включая умные указатели, поддержку многопоточности и расширенные контейнеры, что делает разработку более удобной и безопасной.

C++14 приносит ряд улучшений, таких как возможность использования std::make_unique, что упрощает создание уникальных указателей. Также введены более гибкие лямбда-выражения, которые позволяют захватывать переменные как по значению, так и по ссылке. Эти изменения делают код более лаконичным и выразительным.

С применением Boost в C++14 разработчики получают доступ к новым алгоритмам, контейнерам и улучшенным возможностям метапрограммирования.

# 1. Определение ООП

Объектно-ориентированное программирование – парадигма программирования, основанная на концепциях объектов, инкапсуляции, полиморфизма и наследования.

# 2. Магическое число 7 Миллера в IT

1. 7 уровней модели OSI.
2. 7 слоев безопасности в архитектуре ПО.
3. 7 фаз компиляции в C++.
4. 7 слотов памяти в регистровой машине.
5. 7 фаз жизненного цикла разработки ПО (SDLC).
6. 7 ключевых принципов SOLID.
7. 7 классов HTTP-статусов.

# 3. Энтропия ПО  

1. Использование строгих стайл-гайдов.
2. Нормализация баз данных.
3. Автоматическое тестирование.
4. Использование CI/CD.
5. Статический анализ кода.

## 4. 5 признаков сложной системы по Бучу

1. Иерархичность
2. Адаптивность 
3. Гетерогенность
4. Эмерджентность 
5. Эволюционность

# 5. Закон иерархических компенсаций Седова в IT

В сложной иерархически организованной системе рост разнообразия на верхнем уровне обеспечивается ограничением разнообразия на предыдущих уровнях, и наоборот, рост разнообразия на нижнем уровне разрушает верхний уровень организации.

# Задача

1 условие:

```bash
#include <iostream>
#include <vector>
#include <stdexcept>

std::vector<int> fibonacci(int n) {
    if (n <= 0) {
        throw std::invalid_argument("n должно быть больше 0!");
    }
    std::vector<int> fib_sequence;
    fib_sequence.reserve(n);
    
    if (n >= 1) fib_sequence.push_back(0);
    if (n >= 2) fib_sequence.push_back(1);
    
    for (int i = 2; i < n; ++i) {
        fib_sequence.push_back(fib_sequence[i - 1] + fib_sequence[i - 2]);
    }
    return fib_sequence;
}

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Fibonacci Sequence") {
    REQUIRE(fibonacci(1) == std::vector<int>{0});
    REQUIRE(fibonacci(2) == std::vector<int>{0, 1});
    REQUIRE(fibonacci(5) == std::vector<int>{0, 1, 1, 2, 3});
    REQUIRE_THROWS_AS(fibonacci(0), std::invalid_argument);
    REQUIRE_THROWS_AS(fibonacci(-5), std::invalid_argument);
}
```

2 условие:

```bash
bool is_palindrome(int number) {
    if (number < 0) {
        throw std::invalid_argument("Число должно быть неотрицательным");
    }
    std::string str = std::to_string(number);
    std::string reversed_str = std::string(str.rbegin(), str.rend());
    return str == reversed_str;
}

TEST_CASE("Palindrome Check") {
    REQUIRE(is_palindrome(121) == true);
    REQUIRE(is_palindrome(123) == false);
    REQUIRE(is_palindrome(0) == true);
    REQUIRE(is_palindrome(10) == false);
    REQUIRE(is_palindrome(12321) == true);
    REQUIRE_THROWS_AS(is_palindrome(-121), std::invalid_argument);
} str == reversed_str;

```

3 условие:
```bash
struct Node {
    int value;
    Node* next;
    Node(int val) : value(val), next(nullptr) {}
};

Node* reverse_linked_list(Node* head) {
    Node* prev = nullptr;
    Node* current = head;
    Node* next_node = nullptr;
    
    while (current) {
        next_node = current->next;
        current->next = prev;
        prev = current;
        current = next_node;
    }
    return prev;
}

TEST_CASE("Reverse Linked List") {
    Node* head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(3);

    Node* new_head = reverse_linked_list(head);
    REQUIRE(new_head->value == 3);
    REQUIRE(new_head->next->value == 2);
    REQUIRE(new_head->next->next->value == 1);
    REQUIRE(new_head->next->next->next == nullptr);

    delete new_head->next->next; // Delete 1
    delete new_head->next;        // Delete 2
    delete new_head;              // Delete 3

    REQUIRE(reverse_linked_list(nullptr) == nullptr);
}
```

