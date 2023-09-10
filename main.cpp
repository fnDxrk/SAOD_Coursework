#include <fstream>
#include <iostream>
#include <cstring>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <unistd.h>

int sizeLocality = 4000;


// Структура записи БД
struct record {
    char fullname[32];
    char street[18];
    short int numberHouse;
    short int numberApartment;
    char dateSettle[10];
};

//Пирамидальная сортировка
void heapify(record* arr, record** indexArr, int size, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && strcmp(indexArr[left]->street, indexArr[largest]->street) > 0) 
        largest = left;

    if (right < size && strcmp(indexArr[right]->street, indexArr[largest]->street) > 0)
        largest = right;

    if (largest != i) {
        std::swap(indexArr[i], indexArr[largest]);
        heapify(arr, indexArr, size, largest);
    }
}

void heapSort(record* arr, record** indexArr, int size)
{
    for (int i = size / 2 - 1; i >= 0; i--)
    heapify(arr, indexArr, size, i);

    for (int i = size - 1; i >= 0; i--) {
        std::swap(indexArr[0], indexArr[i]);
        heapify(arr, indexArr, i, 0);
    }
}

// Вывод начальной линии
void printStartLine()
{
    std::cout << "\tFull Name"
              << "\t\t\t   "
              << "Street"
              << "\t  "
              << "Number of house"
              << "\t"
              << "Number of apartment"
              << "\t    "
              << "Settlement date" << std::endl;
}

// Вывод базы данных
void printRecord(record* locality, int& currentPage)
{
    for (int i = currentPage * 20; i < currentPage * 20 + 20; i++) {
        std::cout << i + 1
                  << ". "
                  << locality[i].fullname
                  << "\t"
                  << locality[i].street
                  << "\t" << locality[i].numberHouse
                  << "\t\t\t"
                  << locality[i].numberApartment
                  << "\t\t\t"
                  << locality[i].dateSettle << std::endl;
    }
    std::cout << std::endl;
}

// Вывод сортированной базы данных
void printSortRecord(record** indexArr, int& currentPage)
{
    for (int i = currentPage * 20; i < currentPage * 20 + 20; i++) {
        std::cout << i + 1
                  << ". "
                  << indexArr[i]->fullname
                  << "\t"
                  << indexArr[i]->street
                  << "\t" 
                  << indexArr[i]->numberHouse
                  << "\t\t\t"
                  << indexArr[i]->numberApartment
                  << "\t\t\t"
                  << indexArr[i]->dateSettle << std::endl;
    }
    std::cout << std::endl;
}

// Вывод меню управления
void printMenu()
{
    std::cout << "<q> : Quit programs"
              << "\t\t"
              << "<h> : Last page"
              << "\t\t\t"
              << "<l> : Next page" 
              << std::endl
              << "<r> : Search record"
              << "\t\t"
              << "<j> : Last 10 page"
              << "\t\t"
              << "<k> : Next 10 page" 
              << std::endl
              << "<d> : Default record"
              << "\t\t"
              << "<s> : Sort"
              << std::endl;
}

void checkKey(char& key, int& currentPage, record* locality, record** indexArr, int &keySort) {
    key = getch();
    switch (key) {
        case 'q':;
            exit(0);
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
            std::cout << "Enter the record : ";
            std::cin >> numberPage;
            if (numberPage > 0 && numberPage < 4000) currentPage = floor((numberPage - 1) / 20);
            else {
                std::cout << "Error!" << std::endl;
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
        case 'd' :
            keySort = 0;
            break;
        case 's' :
            keySort = 1;
            heapSort(locality, indexArr, sizeLocality);
            break;
    }  
}

int main()
{
    //Динамический массив для записей БД
    record* locality = new record[sizeLocality];
    //Массив указателей
    record** indexArr = new record*[sizeLocality];
    for (int i = 0; i < sizeLocality; i++) {
        indexArr[i] = &locality[i];
    }

    int currentPage = 0;
    int keySort = 0;
    char key;

    // Чтение файла
    std::ifstream fileDateBase("testBase4.dat", std::ios::binary);

    // Проверка чтения файла
    if (!fileDateBase) {
        std::cout << "Не удалось открыть файл!" << std::endl;
        return 1;
    }

    // Заполнения БД в память
    for (int i = 0; !fileDateBase.read((char*)&locality[i], sizeof(record)).eof(); i++);

    while(true) {
        printStartLine();
        if (keySort == 0) printRecord(locality, currentPage);
        if (keySort == 1) printSortRecord(indexArr, currentPage);
        printMenu();
        checkKey(key, currentPage, locality, indexArr, keySort);
        system("cls");
    }

    fileDateBase.close();

    return 0;
}
