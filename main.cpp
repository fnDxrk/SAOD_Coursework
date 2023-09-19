#include <fstream>
#include <iostream>
#include <cstring>
#include <math.h>
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

// ╔══╦══╗
// ║  ║  ║
// ╠══╬══╣
// ║  ║  ║
// ╚══╩══╝

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
         << "╚══════════════════════════════════════════════════════════════╝"
         << "\n\n"
         << ">>> ";
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

bool checkKey(int& currentPage) {
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
            currentPage++;
            if (currentPage > 199) currentPage = 199;
            break;
        case 'r' :
            int numberPage;
            cout << "Введите номер записи : ";
            cin >> numberPage;
            if (numberPage > 0 && numberPage <= 4000) currentPage = floor((numberPage - 1) / 20);
            else {
                cout << "Ошибка!" << endl;
                sleep(1);
            } 
            break;
        case 'j' :
            currentPage -= 10; 
            if (currentPage < 0) currentPage = 0;
            break;
        case 'k' :
            currentPage += 10; 
            if (currentPage > 199) currentPage = 199;
            break;
    }

    return flag;
}

void checkKeyMenu(record* locality, record** indexArr, int& currentPage) 
{
    printMenu();
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
                flag = checkKey(currentPage);
            }
            break;
        case '2' :
            heapSort(indexArr, 4000);
            while(flag) {
                system("cls");
                printStartLine();
                printSortRecord(indexArr, currentPage);
                printEndLine();
                printControlMenu();
                flag = checkKey(currentPage);  
            }
            break;
        case 'q':
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
   
    record** indexArr = new record*[sizeLocality];
    for (int i = 0; i < sizeLocality; i++) {
        indexArr[i] = &locality[i];
    }

    ifstream fileDateBase("testBase4.dat", ios::binary);

    if (!fileDateBase) {
        cout << "Не удалось открыть файл!" << endl;
        return 1;
    }

    for (int i = 0; !fileDateBase.read((char*)&locality[i], sizeof(record)).eof(); i++);

    while(true) {
        checkKeyMenu(locality, indexArr, currentPage);
        system("cls");
    }

    fileDateBase.close();

    return 0;
}