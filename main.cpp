#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <conio.h>
#include <unistd.h>

using namespace std;

const int MAX_KEYS = 2; // ���ᨬ��쭮� ������⢮ ���祩 � 㧫�

struct record {
    char fullname[32];
    char street[18];
    short int numberHouse;
    short int numberApartment;
    char dateSettle[10];
};

// �����ͻ
// �  �  �
// �����͹
// �  �  �
// �����ͼ

// ������� 㧫�
struct node {
    record* data;
    node* next;
};

// ������� ��।�
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

// ������ ���� �� �����
MyQueue BinarySearch(record** indexArr, const string& targetKey) {
    MyQueue result; // ��।� ��� �࠭���� ����ᥩ � ��������� ���箬

    int arrSize = 4000;
    int left = 0;
    int right = arrSize - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        
        // ��������� ���� �� 䠬����
        string key(indexArr[mid]->fullname, 3);

        if (key == targetKey) {
            // �᫨ ���� ᮢ������, ������塞 ������ � ��।�
            result.push(indexArr[mid]);
            
            // ���� ��� ����ᥩ � ⠪�� �� ���箬 �����
            int leftIndex = mid - 1; // �������� ���祭��
            while (leftIndex >= 0 && strncmp(indexArr[leftIndex]->fullname, targetKey.c_str(), 3) == 0) { // ��ࠢ���� �ࠢ�����
                result.push(indexArr[leftIndex]);
                leftIndex--;
            }
            
            // ���� ��� ����ᥩ � ⠪�� �� ���箬 ��ࠢ�
            int rightIndex = mid + 1; // �������� ���祭��
            while (rightIndex < arrSize && strncmp(indexArr[rightIndex]->fullname, targetKey.c_str(), 3) == 0) { // ��ࠢ���� �ࠢ�����
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

// ���� ���� 
string inputKey() {
    string key;
    cout << "������ 3 �㪢� 䠬���� : ";
    cin >> key;

    // ��१��� ��ப�, �⮡� ��⠢��� ⮫쪮 ���� 3 ᨬ����
    if (key.length() > 3) {
        key = key.substr(0, 3);
    }

    return key;
}

struct BTreeNode {
    vector<record*> keys; // ���᮪ 㪠��⥫�� �� ����� � 㧫�
    vector<BTreeNode*> children; // ���᮪ 㪠��⥫�� �� ��⮬���

    BTreeNode(bool isLeaf = true) : isLeaf(isLeaf) {}

    bool isLeaf; // �ਧ��� ���⮢��� 㧫�
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
            cout << "B-��ॢ� ����." << endl;
        }
    }
    void insertFromQueue(MyQueue& queue) {
        while (!queue.empty()) {
            record* tempData = queue.front();
            //cout << "�������� ������ � ��ॢ�: " << tempData->fullname << endl; // �⫠���� �뢮�
            queue.pop();
            insert(tempData);
        }
    }

private:
    const int MAX_KEYS; // ���ᨬ��쭮� ������⢮ ���祩 � 㧫�
    BTreeNode* root; // ��୥��� 㧥� ��ॢ�

    void printBTreeNode(BTreeNode* node, int indent) const {
        if (node) {
            // �뢮��� �ࠢ�� �����ॢ� � ����㯮�
            printBTreeNode(node->children.back(), indent + 4);
            // �뢮��� ���� � ⥪�饬 㧫�
            for (const auto& key : node->keys) {
                cout << string(indent, ' ') << key->fullname << endl;
            }
            // �뢮��� ����� �����ॢ� � ����㯮�
            printBTreeNode(node->children.front(), indent + 4);
        }
    }
    // ��⠢�� ����� � �� ����� 㧥�
    void insertNonFull(BTreeNode* x, record* rec) {
        // �뢮��� ���ଠ�� � ��⠢�塞�� �����
        cout << "Inserting record: " << rec->fullname << " Apartment: " << rec->numberApartment << endl;
        
        // �뢮��� ���� � 㧫� ��। ��⠢���
        cout << "Keys in node before insertion: ";
        for (const auto& key : x->keys) {
            cout << key->fullname << " ";
        }
        cout << endl;

        // ��⠢�塞 ������ � 㧥�

        // �뢮��� ���� � 㧫� ��᫥ ��⠢��
        cout << "Keys in node after insertion: ";
        for (const auto& key : x->keys) {
            cout << key->fullname << " ";
        }
        cout << endl;
    }


    // ���������� 㧫�
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

    // ���� � 㧫�
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

//��ࠬ����쭠� ���஢��
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

// �㭪�� ��� �ࠢ����� ���� ����ᥩ �� 䠬����
int compareByLastName(record* a, record* b) {
    // // ��������� 䠬���� �� ���
    // char* lastNameA = strtok(a->fullname, " ");
    // char* lastNameB = strtok(b->fullname, " ");

    // // �ࠢ������ 䠬����
    return strcmp(a->fullname, b->fullname);
}

// ��ࠬ����쭠� ���஢�� ��� 䠬����
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

//�뢮� ����
void printMenu() {
    cout << "��������������������������������������������������������������ͻ"
         << "\n"
         << "�                             ����                             �"
         << "\n"
         << "��������������������������������������������������������������͹"
         << "\n"
         << "�  1. ��ᬮ���� ���� ������                                   �"
         << "\n"
         << "�  2. ��ᬮ���� �����஢����� ���� ������                   �"
         << "\n"
         << "�  3. ���� �� �����                                           �"
         << "\n"
         << "�  4. �뢥�� ����筮� ����୮� ��ॢ� ���᪠                  �"
         << "\n"
         << "��������������������������������������������������������������ͼ"
         << "\n\n"
         << ">>> ";
}

// �뢮� ��砫쭮� �����
void printStartLine()
{
    cout << "����������������������������������������������������������������������������������������������������������������������ͻ"
              << "\n"
              << "�\t\t   "
              << "���"
              << "\t\t        �       "
              << "����"
              << "      �    "
              << "����� ����"
              << "   �   "
              << "����� �������"
              << "   �   "
              << "��� ��ᥫ����   �" 
              << endl;
}

// �뢮� ����歮� �����
void printEndLine()
{
    cout << "����������������������������������������������������������������������������������������������������������������������ͼ" << "\n\n";
}

// �뢮� ���� ������
void printRecord(record* locality, int& currentPage)
{
    for (int i = currentPage * 20; i < currentPage * 20 + 20; i++) {
        cout << "����������������������������������������������������������������������������������������������������������������������͹"
             << "\n"
             << "�"
             << i + 1
             << ". "
             << locality[i].fullname
             << "\t� "
             << locality[i].street
             << "�\t    " 
             << locality[i].numberHouse
             << "\t     �\t       "
             << locality[i].numberApartment
             << "\t  �      "
             << locality[i].dateSettle 
             << "     �"
             << endl;
    }
}

// �뢮� ���஢����� ���� ������
void printSortRecord(record** indexArr, int& currentPage)
{
    for (int i = currentPage * 20; i < currentPage * 20 + 20; i++) {
        cout << "����������������������������������������������������������������������������������������������������������������������͹"
             << "\n"
             << "�"
             << i + 1
             << ". "
             << indexArr[i]->fullname
             << "\t� "
             << indexArr[i]->street
             << "�\t    " 
             << indexArr[i]->numberHouse
             << "\t     �\t       "
             << indexArr[i]->numberApartment
             << "\t  �      "
             << indexArr[i]->dateSettle
             << "     �"
             << endl;
    }
}

// �뢮� ��।�
void printQueue(const MyQueue& result, int currentPage)
{
    node* currentNode = result.head;
    int counter = 0;
    int startIndex = currentPage * 20;
    
    // �ய�᪠�� �����, �⮡� ����� � �㦭��� ������ �� ⥪�饩 ��࠭��
    while (counter < startIndex && currentNode) {
        currentNode = currentNode->next;
        counter++;
    }

    // �뢮��� �� 20 ����ᥩ �� ⥪�饩 ��࠭��
    while (currentNode && counter < startIndex + 20) {
        record* recordPtr = currentNode->data;
        cout << "����������������������������������������������������������������������������������������������������������������������͹"
             << "\n"
             << "�"
             << counter + 1
             << ". "
             << recordPtr->fullname
             << "\t� "
             << recordPtr->street
             << "�\t    "
             << recordPtr->numberHouse
             << "\t     �\t       "
             << recordPtr->numberApartment
             << "\t  �      "
             << recordPtr->dateSettle
             << "     �"
             << endl;

        currentNode = currentNode->next;
        counter++;
    }
}

// �뢮� ���� �ࠢ�����
void printControlMenu()
{
    cout << "<q> : ��� �� �ணࠬ��"
              << "\t\t    "
              << "<h> : �।���� ��୨�"
              << "\t\t\t"
              << "<l> : �������� ��࠭��" 
              << "\n"
              << "<r> : ���� �����"
              << "\t\t\t    "
              << "<j> : ���� 10 ����ᥩ"
              << "\t\t\t"
              << "<k> : ������騥 10 ����ᥩ" 
              << endl;
}

// �஢�ઠ ������ �ࠢ�����
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
            cout << "������ ����� ����� : ";
            cin >> numberPage;
            if (numberPage > 0 && numberPage <= totalRecords) {
                currentPage = floor((numberPage - 1) / 20);
            } else {
                cout << "�訡��!" << endl;
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

// �஢�ઠ ������ ����
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
                    std::cout << "����� � ���箬 " << targetKey << " �� �������." << std::endl;
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
                cout << "B-��ॢ�:" << endl;
                bTree.printBTree();
            } else {
                cout << "��।� ����. ������� ����� � ��।� � ��砥 3 ��। ��ᬮ�஬ B-��ॢ�." << endl;
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
        cout << "�� 㤠���� ������ 䠩�!" << endl;
        return 1;
    }

    for (int i = 0; !fileDateBase.read((char*)&locality[i], sizeof(record)).eof(); i++);
    
    // ������� ���ᨢ ��� ���஢�� �� 㫨� � ������ ����
    record** indexArr = new record*[sizeLocality];
    for (int i = 0; i < sizeLocality; i++) {
        indexArr[i] = &locality[i];
    }
    heapSort(indexArr, sizeLocality);

    // ������� ���ᨢ ��� ���஢�� �� 䠬����
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