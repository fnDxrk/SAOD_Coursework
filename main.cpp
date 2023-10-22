#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <conio.h>
#include <unistd.h>

using namespace std;

// ╔══╦══╗
// ║  ║  ║
// ╠══╬══╣
// ║  ║  ║
// ╚══╩══╝

struct record {
    char fullname[32];
    char street[18];
    short int numberHouse;
    short int numberApartment;
    char dateSettle[10];
};

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

struct BTree {
    MyQueue data;
    int balance;
    BTree* left;
    BTree* right;
};

void InsertInBTree(const MyQueue& data, BTree*& root, bool& VR, bool& HR) {
    if (root == nullptr) {
        root = new BTree;
        root->data = data; 
        root->balance = 0;
        root->left = nullptr;
        root->right = nullptr;
        VR = false;
        HR = false;
    } else {
        if (data.front()->numberApartment < root->data.front()->numberApartment) {
            InsertInBTree(data, root->left, VR, HR);
            if (VR) {
                if (root->balance == 0) {
                    BTree* q = root->left;
                    root->left = q->right;
                    q->right = root;
                    root = q;
                    q->balance = 1;
                    VR = false;
                    HR = true;
                } else {
                    root->balance = 0;
                    HR = true;
                }
            } else {
                HR = false;
            }
        } else if (data.front()->numberApartment > root->data.front()->numberApartment) {
            InsertInBTree(data, root->right, VR, HR);
            if (VR) {
                root->balance = 1;
                VR = false;
                HR = true;
            } else if (HR) {
                if (root->balance > 0) {
                    BTree* q = root->right;
                    root->right = q->left;
                    q->left = root;
                    root->balance = 0;
                    q->balance = 0;
                    root->left = root;
                    root = q;
                    VR = true;
                    HR = false;
                } else {
                    HR = false;
                }
            } else {
                HR = false;
            }
        }
    }
}

void InOrderTraversal(BTree* root) {
    if (root == nullptr) {
        return;
    }

    InOrderTraversal(root->left);
    cout << root->data.front()->numberApartment << endl;
    InOrderTraversal(root->right);
}

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
         << "║  4. Вывод двоичного B-дерева поиска                          ║"
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
void checkKeyMenu(record* locality, record** indexArr, record** indexArrLastName, int& currentPage, MyQueue& result, BTree*& root)
{
    printMenu();
    string targetKey;
    char key = getch();
    bool flag = true;
    currentPage = 0;
    MyQueue tempQueue;
    bool VR = false;
    bool HR = false;
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
            root = nullptr;
            // while (!result.empty()) {
            //     record* item = result.front();
            //     //cout << result.front()->fullname << endl;
            //     tempQueue.push(item);
            //     result.pop();
            // }
            // while (!tempQueue.empty()) {
            //     record* item = tempQueue.front();
            //     //cout << tempQueue.front()->fullname << endl;
            //     result.push(item);
            //     InsertInBTree(tempQueue, root, VR, HR);
            //     tempQueue.pop();
            // }
            // InOrderTraversal(root);
            // sleep(5);
            while(!(result.head == nullptr)) {
                //cout << result.front()->fullname << endl;
                InsertInBTree(result, root, VR, HR);
                result.head = result.head->next;
            }
            InOrderTraversal(root);
            sleep(5);
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
    BTree* root;

    while(true) {
        checkKeyMenu(locality, indexArr, indexArrLastName, currentPage, recordQ, root);
        system("cls");
    }

    delete[] locality;
    delete[] indexArr;
    delete[] indexArrLastName;

    fileDateBase.close();

    return 0;
}