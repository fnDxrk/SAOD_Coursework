#include <fstream>
#include <iostream>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <conio.h>
#include <unistd.h>

using namespace std;

struct record {
    char fullname[32];
    char street[18];
    short int numberHouse;
    short int numberApartment;
    char dateSettle[10];
};

// ษออหออป
// บ  บ  บ
// ฬออฮออน
// บ  บ  บ
// ศออสออผ

// ‘โเใชโใเ  ใงซ 
struct node {
    record* data;
    node* next;
};

// ‘โเใชโใเ  ฎ็ฅเฅคจ
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

// ๋แโเ๋ฉ ฏฎจแช ฏฎ ชซ๎็ใ
MyQueue BinarySearch(record** indexArr, const string& targetKey) {
    MyQueue result; // ็ฅเฅค์ คซ๏ ๅเ ญฅญจ๏ ง ฏจแฅฉ แ ฎคจญ ชฎข๋ฌ ชซ๎็ฎฌ

    int arrSize = 4000;
    int left = 0;
    int right = arrSize - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        
        // งขซฅช ฅฌ ชซ๎็ จง ไ ฌจซจจ
        string key(indexArr[mid]->fullname, 3);

        if (key == targetKey) {
            // …แซจ ชซ๎็ แฎขฏ ค ฅโ, คฎก ขซ๏ฅฌ ง ฏจแ์ ข ฎ็ฅเฅค์
            result.push(indexArr[mid]);
            
            // ฎจแช ขแฅๅ ง ฏจแฅฉ แ โ ชจฌ ฆฅ ชซ๎็ฎฌ ขซฅขฎ
            int leftIndex = mid - 1; // งฌฅญฅญฎ งญ ็ฅญจฅ
            while (leftIndex >= 0 && strncmp(indexArr[leftIndex]->fullname, targetKey.c_str(), 3) == 0) { // แฏเ ขซฅญฎ แเ ขญฅญจฅ
                result.push(indexArr[leftIndex]);
                leftIndex--;
            }
            
            // ฎจแช ขแฅๅ ง ฏจแฅฉ แ โ ชจฌ ฆฅ ชซ๎็ฎฌ ขฏเ ขฎ
            int rightIndex = mid + 1; // งฌฅญฅญฎ งญ ็ฅญจฅ
            while (rightIndex < arrSize && strncmp(indexArr[rightIndex]->fullname, targetKey.c_str(), 3) == 0) { // แฏเ ขซฅญฎ แเ ขญฅญจฅ
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

// ขฎค ชซ๎็  
string inputKey() {
    string key;
    cout << "ขฅคจโฅ 3 กใชข๋ ไ ฌจซจจ : ";
    cin >> key;

    // กเฅง ฅฌ แโเฎชใ, ็โฎก๋ ฎแโ ขจโ์ โฎซ์ชฎ ฏฅเข๋ฅ 3 แจฌขฎซ 
    if (key.length() > 3) {
        key = key.substr(0, 3);
    }

    return key;
}

//จเ ฌจค ซ์ญ ๏ แฎเโจเฎขช 
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

// ”ใญชๆจ๏ คซ๏ แเ ขญฅญจ๏ คขใๅ ง ฏจแฅฉ ฏฎ ไ ฌจซจจ
int compareByLastName(record* a, record* b) {
    // // งขซฅช ฅฌ ไ ฌจซจจ จง ”
    // char* lastNameA = strtok(a->fullname, " ");
    // char* lastNameB = strtok(b->fullname, " ");

    // // ‘เ ขญจข ฅฌ ไ ฌจซจจ
    return strcmp(a->fullname, b->fullname);
}

// จเ ฌจค ซ์ญ ๏ แฎเโจเฎขช  คซ๏ ไ ฌจซจจ
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

//๋ขฎค ฌฅญ๎
void printMenu() {
    cout << "ษออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออป"
         << "\n"
         << "บ                             ฅญ๎                             บ"
         << "\n"
         << "ฬออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออน"
         << "\n"
         << "บ  1. ฎแฌฎโเฅโ์ ก งใ ค ญญ๋ๅ                                   บ"
         << "\n"
         << "บ  2. ฎแฌฎโเฅโ์ ฎโแฎเโจเฎข ญญใ๎ ก งใ ค ญญ๋ๅ                   บ"
         << "\n"
         << "บ  3. ฎจแช ฏฎ ชซ๎็ใ                                           บ"
         << "\n"
         << "ศออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออผ"
         << "\n\n"
         << ">>> ";
}

// ๋ขฎค ญ ็ ซ์ญฎฉ ซจญจจ
void printStartLine()
{
    cout << "ษอออออออออออออออออออออออออออออออออออออออหออออออออออออออออออหอออออออออออออออออหออออออออออออออออออออหออออออออออออออออออออป"
              << "\n"
              << "บ\t\t   "
              << "”"
              << "\t\t        บ       "
              << "“ซจๆ "
              << "      บ    "
              << "ฎฌฅเ คฎฌ "
              << "   บ   "
              << "ฎฌฅเ ชข เโจเ๋"
              << "   บ   "
              << " โ  ฏฎแฅซฅญจ๏   บ" 
              << endl;
}

// ๋ขฎค ชฎญฅๆญฎฉ ซจญจจ
void printEndLine()
{
    cout << "ศอออออออออออออออออออออออออออออออออออออออสออออออออออออออออออสอออออออออออออออออสออออออออออออออออออออสออออออออออออออออออออผ" << "\n\n";
}

// ๋ขฎค ก ง๋ ค ญญ๋ๅ
void printRecord(record* locality, int& currentPage)
{
    for (int i = currentPage * 20; i < currentPage * 20 + 20; i++) {
        cout << "ฬอออออออออออออออออออออออออออออออออออออออฮออออออออออออออออออฮอออออออออออออออออฮออออออออออออออออออออฮออออออออออออออออออออน"
             << "\n"
             << "บ"
             << i + 1
             << ". "
             << locality[i].fullname
             << "\tบ "
             << locality[i].street
             << "บ\t    " 
             << locality[i].numberHouse
             << "\t     บ\t       "
             << locality[i].numberApartment
             << "\t  บ      "
             << locality[i].dateSettle 
             << "     บ"
             << endl;
    }
}

// ๋ขฎค แฎเโจเฎข ญญฎฉ ก ง๋ ค ญญ๋ๅ
void printSortRecord(record** indexArr, int& currentPage)
{
    for (int i = currentPage * 20; i < currentPage * 20 + 20; i++) {
        cout << "ฬอออออออออออออออออออออออออออออออออออออออฮออออออออออออออออออฮอออออออออออออออออฮออออออออออออออออออออฮออออออออออออออออออออน"
             << "\n"
             << "บ"
             << i + 1
             << ". "
             << indexArr[i]->fullname
             << "\tบ "
             << indexArr[i]->street
             << "บ\t    " 
             << indexArr[i]->numberHouse
             << "\t     บ\t       "
             << indexArr[i]->numberApartment
             << "\t  บ      "
             << indexArr[i]->dateSettle
             << "     บ"
             << endl;
    }
}

// ๋ขฎค ฎ็ฅเฅคจ
void printQueue(const MyQueue& result, int currentPage)
{
    node* currentNode = result.head;
    int counter = 0;
    int startIndex = currentPage * 20;
    
    // เฎฏใแช ฅฌ ง ฏจแจ, ็โฎก๋ ญ ็ โ์ แ ญใฆญฎฃฎ จญคฅชแ  ญ  โฅชใ้ฅฉ แโเ ญจๆฅ
    while (counter < startIndex && currentNode) {
        currentNode = currentNode->next;
        counter++;
    }

    // ๋ขฎคจฌ คฎ 20 ง ฏจแฅฉ ญ  โฅชใ้ฅฉ แโเ ญจๆฅ
    while (currentNode && counter < startIndex + 20) {
        record* recordPtr = currentNode->data;
        cout << "ฬอออออออออออออออออออออออออออออออออออออออฮออออออออออออออออออฮอออออออออออออออออฮออออออออออออออออออออฮออออออออออออออออออออน"
             << "\n"
             << "บ"
             << counter + 1
             << ". "
             << recordPtr->fullname
             << "\tบ "
             << recordPtr->street
             << "บ\t    "
             << recordPtr->numberHouse
             << "\t     บ\t       "
             << recordPtr->numberApartment
             << "\t  บ      "
             << recordPtr->dateSettle
             << "     บ"
             << endl;

        currentNode = currentNode->next;
        counter++;
    }
}

// ๋ขฎค ฌฅญ๎ ใฏเ ขซฅญจ๏
void printControlMenu()
{
    cout << "<q> : ๋ฉโจ จง ฏเฎฃเ ฌฌ๋"
              << "\t\t    "
              << "<h> : เฅค๋คใ้ ๏ แโเญจๆ "
              << "\t\t\t"
              << "<l> : ‘ซฅคใ๎้ ๏ แโเ ญจๆ " 
              << "\n"
              << "<r> : ฎจแช ง ฏจแจ"
              << "\t\t\t    "
              << "<j> : เฎ่ซ๋ฅ 10 ง ฏจแฅฉ"
              << "\t\t\t"
              << "<k> : ‘ซฅคใ๎้จฅ 10 ง ฏจแฅฉ" 
              << endl;
}

// เฎขฅเช  ชซ ขจ่ ใฏเ ขซฅญจ๏
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
            cout << "ขฅคจโฅ ญฎฌฅเ ง ฏจแจ : ";
            cin >> numberPage;
            if (numberPage > 0 && numberPage <= totalRecords) {
                currentPage = floor((numberPage - 1) / 20);
            } else {
                cout << "่จกช !" << endl;
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

// เฎขฅเช  ชซ ขจ่ ฌฅญ๎
void checkKeyMenu(record* locality, record** indexArr, record** indexArrLastName, int& currentPage) 
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
            MyQueue result = BinarySearch(indexArrLastName, targetKey);
                if (result.empty()) {
                    std::cout << " ฏจแจ แ ชซ๎็ฎฌ " << targetKey << " ญฅ ญ ฉคฅญ๋." << std::endl;
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
        default:
            exit(0);
            break;
    }
}

int main()
{    
    system("chcp 866 > nul");
    
    int sizeLocality = 4000;
    int currentPage = 0;
    
    record* locality = new record[sizeLocality]; 

    ifstream fileDateBase("testBase4.dat", ios::binary);

    if (!fileDateBase) {
        cout << "ฅ ใค ซฎแ์ ฎโชเ๋โ์ ไ ฉซ!" << endl;
        return 1;
    }

    for (int i = 0; !fileDateBase.read((char*)&locality[i], sizeof(record)).eof(); i++);
    
    // ญคฅชแญ๋ฉ ฌ แแจข คซ๏ แฎเโจเฎขชจ ฏฎ ใซจๆฅ จ ญฎฌฅเใ คฎฌ 
    record** indexArr = new record*[sizeLocality];
    for (int i = 0; i < sizeLocality; i++) {
        indexArr[i] = &locality[i];
    }
    heapSort(indexArr, sizeLocality);

    // ญคฅชแญ๋ฉ ฌ แแจข คซ๏ แฎเโจเฎขชจ ฏฎ ไ ฌจซจจ
    record** indexArrLastName = new record*[sizeLocality];
    for (int i = 0; i < sizeLocality; i++) {
        indexArrLastName[i] = &locality[i];
    }
    heapSortLastName(indexArrLastName, sizeLocality);

    while(true) {
        checkKeyMenu(locality, indexArr, indexArrLastName, currentPage);
        system("cls");
    }

    fileDateBase.close();

    return 0;
}