#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <stdio.h>
#include <conio.h>
#include <unistd.h>

using namespace std;

// �����ͻ
// �  �  �
// �����͹
// �  �  �
// �����ͼ

struct record {
    char fullname[32];
    char street[18];
    short int numberHouse;
    short int numberApartment;
    char dateSettle[10];
};

const int N = 4000;

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

struct BTree {
    vector<MyQueue> data;
    int balance;
    BTree* left;
    BTree* right;
};

void InsertInBTree(MyQueue& data, BTree*& root, bool& VR, bool& HR)
{
    if (root == nullptr) {
        root = new BTree;
        root->data.push_back(data);
        root->balance = 0;
        root->left = nullptr;
        root->right = nullptr;
        VR = false;
        HR = false;
    } else {
        if (data.front()->numberApartment < root->data.front().head->data->numberApartment) {
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
                }
                else {
                    root->balance = 0;
                    HR = true;
                }
            }
            else {
                HR = false;
            }
        }
        else if (data.front()->numberApartment > root->data.front().head->data->numberApartment) {
            InsertInBTree(data, root->right, VR, HR);
            if (VR) {
                root->balance = 1;
                VR = false;
                HR = true;
            }
            else if (HR) {
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
                }
                else {
                    HR = false;
                }
            }
            else {
                HR = false;
            }
        }
        else {
            root->data.push_back(data);
        }
    }
}

void InOrderTraversal(BTree* root) {
    if (root == nullptr) {
        return;
    }

    InOrderTraversal(root->left);
    for (size_t i = 0; i < root->data.size(); i++) {
        cout << root->data[i].head->data->fullname
                  << "\t"
                  << root->data[i].head->data->street
                  << "\t"
                  << root->data[i].head->data->numberHouse
                  << "\t\t"
                  << root->data[i].head->data->numberApartment
                  << "\t\t"
                  << root->data[i].head->data->dateSettle
                  << endl;
    }
    InOrderTraversal(root->right);
}

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
         << "�  4. �뢮� ����筮�� B-��ॢ� ���᪠                          �"
         << "\n"
         << "�  5. ����஢����                                              �"
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

short int numTree() {
    system("cls");
    short int num = 0;
    cout << "������ ����� ������� : ";
    cin >> num; 

    return num;
}

void printTreeKey(BTree* root, int num) {
    if (root == nullptr) {
        return;
    }

    printTreeKey(root->left, num);
    for (size_t i = 0; i < root->data.size(); i++) {
        if (root->data[i].head->data->numberApartment == num) {
            cout << root->data[i].head->data->fullname 
                 << "\t"
                 << root->data[i].head->data->street
                 << "\t"
                 << root->data[i].head->data->numberHouse 
                 << "\t\t "
                 << root->data[i].head->data->numberApartment 
                 << "\t\t "
                 << root->data[i].head->data->dateSettle 
                 << endl;
        }
    }
    printTreeKey(root->right, num);
}

bool checkKeyTree(BTree* root) {
    char key = getch();
    bool flag = true;
    short int num;
    switch (key) {
        case 'q' :
            system("cls");
            flag = false;
            break;
        case 'r' :
            num = numTree();
            system("cls");
            printTreeKey(root, num);
            system("pause");
            break;
    }

    return flag;
}

void printControlMenuTree() {
    cout << "<q> : ��� �� �ணࠬ��" 
         << "\t"
         << "<r> : ���� �� �����"
         << endl;
}

int up(int n, double q, double *array, double chance[]) {
    int i = 0, j = 0;
    for (i = n - 2; i >= 1; i--) {
        if (array[i - 1] < q) array[i] = array[i - 1];
        else {
            j = i;
            break;
        }
        if ((i - 1) == 0 && chance[i - 1] < q) {
            j = 0;
            break;
        }
    }
    array[j] = q;
    return j;
}

void down(int n, int j, int Length[], char c[][20]) {
    char pref[20];
    for (int i = 0; i < 20; i++) pref[i] = c[j][i];
    int l = Length[j];
    for (int i = j; i < n - 2; i++) {
        for (int k = 0; k < 20; k++)
            c[i][k] = c[i+1][k];
        Length[i] = Length[i+1];
    }
    for (int i = 0; i < 20; i++) {
        c[n-2][i] = pref[i];
        c[n-1][i] = pref[i];
    }
    c[n-1][l] = '1';
    c[n-2][l] = '0';
    Length[n-1] = l + 1;
    Length[n-2] = l + 1;
}

void huffman(int n, double *array, int Length[], char c[][20], double chance[]) {
    if (n == 2) {
        c[0][0] = '0';
        Length[0] = 1;
        c[1][0] = '1';
        Length[1] = 1;
    } else {
        double q = array[n - 2] + array[n - 1];
        int j = up(n, q, array, chance);
        huffman(n - 1, array, Length, c, chance);
        down(n, j, Length, c);
    }
}

unordered_map<char, int> get_char_counts_from_file(const string &file_name, int &file_size, int n = N) {
    ifstream file(file_name, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("�� 㤠���� ������ 䠩�!");
    }
    char ch_arr[sizeof(record) * n];
    file.read((char *) ch_arr, sizeof(record) * n);
    file.close();

    unordered_map<char, int> counter_map;
    file_size = 0;
    for (auto ch : ch_arr) {
        counter_map[ch]++;
        file_size++;
    }
    return counter_map;
}

vector<pair<double, char>> calc_probabilities(const unordered_map<char, int> &counter_map, int count) {
    vector<pair<double, char>> probabilities;
    probabilities.reserve(counter_map.size());
    for (auto i : counter_map) {
        probabilities.emplace_back(make_pair((double) i.second / count, i.first));
    }
    return probabilities;
}

void coding() {
    int file_size;
    unordered_map<char, int> counter_map;
    counter_map = get_char_counts_from_file("testBase4.dat", file_size);

    auto probabilities = calc_probabilities(counter_map, file_size);
    counter_map.clear();

    sort(probabilities.begin(), probabilities.end(), greater<pair<double, char>>());
    cout << "\nProbabil.  �har\n";
    for (auto i : probabilities) {
        cout << fixed << i.first << " | " << i.second << '\n';
    }

    const int n = (int) probabilities.size();

    char c[n][20];
    int Length[n];
    for (auto &i : Length) {
        i = 0;
    }

    auto p = new double[n];
    double chance_l[n];
    for (int i = 0; i < n; ++i) {
        p[i] = probabilities[i].first;
        chance_l[i] = p[i];

    }

    huffman(n, chance_l, Length, c, p);
    cout << "\n��� ���䬥��:\n";
    cout << "\nCh  Prob      Code\n";
    double avg_len = 0;
    double entropy = 0;
    for (int i = 0; i < n; i++) {
        avg_len += Length[i] * p[i];
        entropy -= p[i] * log2(p[i]);
        printf("%c | %.5lf | ", probabilities[i].second, p[i]);
        for (int j = 0; j < Length[i]; ++j) {
            printf("%c", c[i][j]);
        }
        cout << '\n';
    }
    cout << "\n"
         << "�।��� ����� = " << avg_len << '\n'
         << "���ய�� = " << entropy << '\n'
         << "�।��� ����� < ��ய�� + 1\n"
         << "N = " << n << "\n\n";
}

// �஢�ઠ ������ ����
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
            if(!result.empty()) {
                root = nullptr;
                while(!(result.head == nullptr)) {
                    InsertInBTree(result, root, VR, HR);
                    result.head = result.head->next;
                }
                while(flag) {
                    system("cls");
                    InOrderTraversal(root);
                    cout << endl;
                    printControlMenuTree();
                    flag = checkKeyTree(root);
                }
            } else {
                cout << "��।� �����! ������� ᭠砫� �㭪� 3!";
                sleep(5);
            }
            break;
        case '5':
            while(flag) {
                coding();
                cout << "\n\n<q> : ��� �� �ணࠬ��" << endl;
                if (getch() == 'q') {
                    system("cls");
                    flag = false;
                }
            }
            //sleep(10);
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