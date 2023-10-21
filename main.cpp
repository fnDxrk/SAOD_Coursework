#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <conio.h>
#include <unistd.h>

using namespace std;

const int MAX_KEYS = 2; // Максимальное количество ключей в узле

struct record {
    char fullname[32];
    char street[18];
    short int numberHouse;
    short int numberApartment;
    char dateSettle[10];
};

// ╔══╦══╗
// ║  ║  ║
// ╠══╬══╣
// ║  ║  ║
// ╚══╩══╝

// Структура узла
struct node {
    record* data;
    node* next;
};

// Структура очереди
struct MyQueue {
    node* head;
    node* tail;

    MyQueue() : head(nullptr), tail(nullptr) {}

    void push(record* data) {
        node* newNode = new node;
        newNode->data = data;
        newNode->next = nullptr;

        if (!head) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    bool empty() const {
        return !head;
    }

    record* front() const {
        if (head) {
            return head->data;
        }
        throw runtime_error("Queue is empty");
    }

    void pop() {
        if (head) {
            node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    int size() const {
        int count = 0;
        node* current = head;
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }
};

// Быстрый поиск по ключу
MyQueue BinarySearch(record** indexArr, const string& targetKey) {
    MyQueue result; // Очередь для хранения записей с одинаковым ключом

    int arrSize = 4000;
    int left = 0;
    int right = arrSize - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        
        // Извлекаем ключ из фамилии
        string key(indexArr[mid]->fullname, 3);

        if (key == targetKey) {
            // Если ключ совпадает, добавляем запись в очередь
            result.push(indexArr[mid]);
            
            // Поиск всех записей с таким же ключом влево
            int leftIndex = mid - 1; // Изменено значение
            while (leftIndex >= 0 && strncmp(indexArr[leftIndex]->fullname, targetKey.c_str(), 3) == 0) { // Исправлено сравнение
                result.push(indexArr[leftIndex]);
                leftIndex--;
            }
            
            // Поиск всех записей с таким же ключом вправо
            int rightIndex = mid + 1; // Изменено значение
            while (rightIndex < arrSize && strncmp(indexArr[rightIndex]->fullname, targetKey.c_str(), 3) == 0) { // Исправлено сравнение
                result.push(indexArr[rightIndex]);
                rightIndex++;
            }
            
            return result;
        } else if (key < targetKey) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}

// Ввод ключа 
string inputKey() {
    string key;
    cout << "Введите 3 буквы фамилии : ";
    cin >> key;

    // Обрезаем строку, чтобы оставить только первые 3 символа
    if (key.length() > 3) {
        key = key.substr(0, 3);
    }

    return key;
}

struct BTreeNode {
    vector<record*> keys; // Список указателей на записи в узле
    vector<BTreeNode*> children; // Список указателей на потомков

    BTreeNode(bool isLeaf = true) : isLeaf(isLeaf) {}

    bool isLeaf; // Признак листового узла
};

class BTree {
public:
    BTree(int maxKeys) : MAX_KEYS(maxKeys), root(new BTreeNode()) {}

    void buildTreeFromQueue(MyQueue& queue) {
        while (!queue.empty()) {
            record* rec = queue.front();
            queue.pop();
            insert(rec);
        }
    }

    void insert(record* rec) {
        insertNonFull(root, rec);
    }

    void search(int targetApartment, vector<record*>& result) {
        searchInNode(root, targetApartment, result);
    }
    void printBTree() const {
        if (root) {
            printBTreeNode(root, 0);
        } else {
            cout << "B-дерево пусто." << endl;
        }
    }
    void insertFromQueue(MyQueue& queue) {
        while (!queue.empty()) {
            record* tempData = queue.front();
            //cout << "Добавляю запись в дерево: " << tempData->fullname << endl; // Отладочный вывод
            queue.pop();
            insert(tempData);
        }
    }

private:
    const int MAX_KEYS; // Максимальное количество ключей в узле
    BTreeNode* root; // Корневой узел дерева

    void printBTreeNode(BTreeNode* node, int indent) const {
        if (node) {
            // Выводим правое поддерево с отступом
            printBTreeNode(node->children.back(), indent + 4);
            // Выводим ключи в текущем узле
            for (const auto& key : node->keys) {
                cout << string(indent, ' ') << key->fullname << endl;
            }
            // Выводим левое поддерево с отступом
            printBTreeNode(node->children.front(), indent + 4);
        }
    }
    // Вставка записи в не полный узел
    void insertNonFull(BTreeNode* x, record* rec) {
        // Выводим информацию о вставляемой записи
        cout << "Inserting record: " << rec->fullname << " Apartment: " << rec->numberApartment << endl;
        
        // Выводим ключи в узле перед вставкой
        cout << "Keys in node before insertion: ";
        for (const auto& key : x->keys) {
            cout << key->fullname << " ";
        }
        cout << endl;

        // Вставляем запись в узел

        // Выводим ключи в узле после вставки
        cout << "Keys in node after insertion: ";
        for (const auto& key : x->keys) {
            cout << key->fullname << " ";
        }
        cout << endl;
    }


    // Разделение узла
    void splitChild(BTreeNode* x, int i) {
        BTreeNode* y = x->children[i];
        BTreeNode* z = new BTreeNode(y->isLeaf);

        x->keys.insert(x->keys.begin() + i, y->keys[MAX_KEYS / 2]);

        for (int j = MAX_KEYS / 2 + 1; j < MAX_KEYS; j++) {
            z->keys.push_back(y->keys[j]);
        }

        y->keys.erase(y->keys.begin() + MAX_KEYS / 2, y->keys.end());

        if (!y->isLeaf) {
            for (int j = MAX_KEYS / 2 + 1; j <= MAX_KEYS; j++) {
                z->children.push_back(y->children[j]);
            }

            y->children.erase(y->children.begin() + MAX_KEYS / 2 + 1, y->children.end());
        }

        x->children.insert(x->children.begin() + i + 1, z);
    }

    // Поиск в узле
    void searchInNode(BTreeNode* x, int targetApartment, vector<record*>& result) {
        int i = 0;
        while (i < x->keys.size() && targetApartment > x->keys[i]->numberApartment) {
            i++;
        }

        if (i < x->keys.size() && targetApartment == x->keys[i]->numberApartment) {
            result.push_back(x->keys[i]);
        }

        if (!x->isLeaf) {
            for (int j = 0; j < x->children.size(); j++) {
                searchInNode(x->children[j], targetApartment, result);
            }
        }
    }
};

//Пирамидальная сортировка
void heapify(record** indexArr, int size, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && (strcmp(indexArr[left]->street, indexArr[largest]->street) > 0 
        || (strcmp(indexArr[left]->street, indexArr[largest]->street) == 0 
        && indexArr[left]->numberHouse > indexArr[largest]->numberHouse))) 
        largest = left;

    if (right < size && (strcmp(indexArr[right]->street, indexArr[largest]->street) > 0 
        || (strcmp(indexArr[right]->street, indexArr[largest]->street) == 0 
        && indexArr[right]->numberHouse > indexArr[largest]->numberHouse)))
        largest = right;

    if (largest != i) {
        swap(indexArr[i], indexArr[largest]);
        heapify(indexArr, size, largest);
    }
}

void heapSort(record** indexArr, int size)
{
    for (int i = size / 2 - 1; i >= 0; i--)
    heapify(indexArr, size, i);

    for (int i = size - 1; i >= 0; i--) {
        swap(indexArr[0], indexArr[i]);
        heapify(indexArr, i, 0);
    }
}

// Функция для сравнения двух записей по фамилии
int compareByLastName(record* a, record* b) {
    // // Извлекаем фамилии из ФИО
    // char* lastNameA = strtok(a->fullname, " ");
    // char* lastNameB = strtok(b->fullname, " ");

    // // Сравниваем фамилии
    return strcmp(a->fullname, b->fullname);
}

// Пирамидальная сортировка для фамилии
void heapifyLastName(record** arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && compareByLastName(arr[left], arr[largest]) > 0)
        largest = left;

    if (right < n && compareByLastName(arr[right], arr[largest]) > 0)
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapifyLastName(arr, n, largest);
    }
}

void heapSortLastName(record** arr, int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapifyLastName(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapifyLastName(arr, i, 0);
    }
}

//Вывод меню
void printMenu() {
    cout << "╔══════════════════════════════════════════════════════════════╗"
         << "\n"
         << "║                             Меню                             ║"
         << "\n"
         << "╠══════════════════════════════════════════════════════════════╣"
         << "\n"
         << "║  1. Посмотреть базу данных                                   ║"
         << "\n"
         << "║  2. Посмотреть отсортированную базу данных                   ║"
         << "\n"
         << "║  3. Поиск по ключу                                           ║"
         << "\n"
         << "║  4. Вывести двоичное бинарное дерево поиска                  ║"
         << "\n"
         << "╚══════════════════════════════════════════════════════════════╝"
         << "\n\n"
         << ">>> ";
}

// Вывод начальной линии
void printStartLine()
{
    cout << "╔═══════════════════════════════════════╦══════════════════╦═════════════════╦════════════════════╦════════════════════╗"
              << "\n"
              << "║\t\t   "
              << "ФИО"
              << "\t\t        ║       "
              << "Улица"
              << "      ║    "
              << "Номер дома"
              << "   ║   "
              << "Номер квартиры"
              << "   ║   "
              << "Дата поселения   ║" 
              << endl;
}

// Вывод конецной линии
void printEndLine()
{
    cout << "╚═══════════════════════════════════════╩══════════════════╩═════════════════╩════════════════════╩════════════════════╝" << "\n\n";
}

// Вывод базы данных
void printRecord(record* locality, int& currentPage)
{
    for (int i = currentPage * 20; i < currentPage * 20 + 20; i++) {
        cout << "╠═══════════════════════════════════════╬══════════════════╬═════════════════╬════════════════════╬════════════════════╣"
             << "\n"
             << "║"
             << i + 1
             << ". "
             << locality[i].fullname
             << "\t║ "
             << locality[i].street
             << "║\t    " 
             << locality[i].numberHouse
             << "\t     ║\t       "
             << locality[i].numberApartment
             << "\t  ║      "
             << locality[i].dateSettle 
             << "     ║"
             << endl;
    }
}

// Вывод сортированной базы данных
void printSortRecord(record** indexArr, int& currentPage)
{
    for (int i = currentPage * 20; i < currentPage * 20 + 20; i++) {
        cout << "╠═══════════════════════════════════════╬══════════════════╬═════════════════╬════════════════════╬════════════════════╣"
             << "\n"
             << "║"
             << i + 1
             << ". "
             << indexArr[i]->fullname
             << "\t║ "
             << indexArr[i]->street
             << "║\t    " 
             << indexArr[i]->numberHouse
             << "\t     ║\t       "
             << indexArr[i]->numberApartment
             << "\t  ║      "
             << indexArr[i]->dateSettle
             << "     ║"
             << endl;
    }
}

// Вывод очереди
void printQueue(const MyQueue& result, int currentPage)
{
    node* currentNode = result.head;
    int counter = 0;
    int startIndex = currentPage * 20;
    
    // Пропускаем записи, чтобы начать с нужного индекса на текущей странице
    while (counter < startIndex && currentNode) {
        currentNode = currentNode->next;
        counter++;
    }

    // Выводим до 20 записей на текущей странице
    while (currentNode && counter < startIndex + 20) {
        record* recordPtr = currentNode->data;
        cout << "╠═══════════════════════════════════════╬══════════════════╬═════════════════╬════════════════════╬════════════════════╣"
             << "\n"
             << "║"
             << counter + 1
             << ". "
             << recordPtr->fullname
             << "\t║ "
             << recordPtr->street
             << "║\t    "
             << recordPtr->numberHouse
             << "\t     ║\t       "
             << recordPtr->numberApartment
             << "\t  ║      "
             << recordPtr->dateSettle
             << "     ║"
             << endl;

        currentNode = currentNode->next;
        counter++;
    }
}

// Вывод меню управления
void printControlMenu()
{
    cout << "<q> : Выйти из программы"
              << "\t\t    "
              << "<h> : Предыдущая стрница"
              << "\t\t\t"
              << "<l> : Следующая страница" 
              << "\n"
              << "<r> : Поиск записи"
              << "\t\t\t    "
              << "<j> : Прошлые 10 записей"
              << "\t\t\t"
              << "<k> : Следующие 10 записей" 
              << endl;
}

// Проверка клавиш управления
bool checkKey(int& currentPage, int totalRecords) {
    char key = getch();
    bool flag = true;
    switch (key) {
        case 'q':
            system("cls");
            flag = false;
            break;
        case 'h':
            currentPage--;
            if (currentPage < 0) currentPage = 0;
            break;
        case 'l':
            if ((currentPage + 1) * 20 < totalRecords) {
                currentPage++;
            }
            break;
        case 'r':
            int numberPage;
            cout << "Введите номер записи : ";
            cin >> numberPage;
            if (numberPage > 0 && numberPage <= totalRecords) {
                currentPage = floor((numberPage - 1) / 20);
            } else {
                cout << "Ошибка!" << endl;
                sleep(1);
            }
            break;
        case 'j':
            currentPage -= 10; 
            if (currentPage < 0) currentPage = 0;
            break;
        case 'k':
            currentPage += 10; 
            if (currentPage * 20 >= totalRecords) {
                currentPage = (totalRecords - 1) / 20;
            }
            break;
    }

    return flag;
}

// Проверка клавиш меню
void checkKeyMenu(record* locality, record** indexArr, record** indexArrLastName, int& currentPage, MyQueue& result, BTree& bTree)
{
    printMenu();
    string targetKey;
    char key = getch();
    bool flag = true;
    currentPage = 0;
    switch (key) {
        case '1':
            while(flag) {
                system("cls");
                printStartLine();
                printRecord(locality, currentPage);
                printEndLine();
                printControlMenu();
                flag = checkKey(currentPage, 4000);
            }
            break;
        case '2':
            while(flag) {
                system("cls");
                printStartLine();
                printSortRecord(indexArr, currentPage);
                printEndLine();
                printControlMenu();
                flag = checkKey(currentPage, 4000);  
            }
            break;
        case '3':
            system("cls");
            targetKey = inputKey();
            while (flag) {
                result = BinarySearch(indexArrLastName, targetKey);
                if (result.empty()) {
                    std::cout << "Записи с ключом " << targetKey << " не найдены." << std::endl;
                    sleep(3);
                    break;
                } else {
                    system("cls");
                    printStartLine();
                    printQueue(result, currentPage);
                    printEndLine();
                    printControlMenu();
                    flag = checkKey(currentPage, result.size());
                }
            }
            break;
        case '4':
            system("cls");
            if (!result.empty()) {
                bTree.insertFromQueue(result);
                sleep(5);
                cout << "B-дерево:" << endl;
                bTree.printBTree();
            } else {
                cout << "Очередь пуста. Добавьте записи в очередь в случае 3 перед просмотром B-дерева." << endl;
                sleep(3);
            }
            break;
        default :
            exit(0);
            break;
    }
}

int main()
{    
    system("chcp 866 > nul");
    system("cls");
    
    int sizeLocality = 4000;
    int currentPage = 0;
    
    record* locality = new record[sizeLocality]; 

    ifstream fileDateBase("testBase4.dat", ios::binary);

    if (!fileDateBase) {
        cout << "Не удалось открыть файл!" << endl;
        return 1;
    }

    for (int i = 0; !fileDateBase.read((char*)&locality[i], sizeof(record)).eof(); i++);
    
    // Индексный массив для сортировки по улице и номеру дома
    record** indexArr = new record*[sizeLocality];
    for (int i = 0; i < sizeLocality; i++) {
        indexArr[i] = &locality[i];
    }
    heapSort(indexArr, sizeLocality);

    // Индексный массив для сортировки по фамилии
    record** indexArrLastName = new record*[sizeLocality];
    for (int i = 0; i < sizeLocality; i++) {
        indexArrLastName[i] = &locality[i];
    }
    heapSortLastName(indexArrLastName, sizeLocality);

    MyQueue recordQ;

    BTree bTree(MAX_KEYS);

    while(true) {
        checkKeyMenu(locality, indexArr, indexArrLastName, currentPage, recordQ, bTree);
        system("cls");
    }

    delete[] locality;
    delete[] indexArr;
    delete[] indexArrLastName;

    fileDateBase.close();

    return 0;
}