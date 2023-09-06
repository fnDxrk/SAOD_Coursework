#include <iostream>
#include <fstream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

//Структура терминалов
struct termios oldt, newt;

//Структура записи БД
struct record {
    char fullname[32];
    char street[18];
    short int numberHouse;
    short int numberApartment;
    char dateSettle[10];
};

//Функция считывания клавиш
void readKey(int& key) {
        tcgetattr(STDIN_FILENO, &oldt );
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO );
        tcsetattr(STDIN_FILENO, TCSANOW, &newt );
        key = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt );
}

//Функция проверки нажатой клавиши
void checkKey(int& key, int &currentPage) {
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    switch (key) {
        case 'q' :
            std::cout << "Close" << std::endl;
            exit(0);
        case 'h' :
            currentPage--;
            if (currentPage < 0) {
                std::cout << "Error!" << std::endl;
                sleep(2);
                currentPage = 0;
            }
            break;
        case 'l' :
            currentPage++;
            if (currentPage > 199) {
                std::cout << "This is end!" << std::endl;
                sleep(2);
                currentPage = 199;
            }
            break;
    }
}

int main() {
    record* locality = new record[4000];
    
    int currentPage = 0;
    int key;

    //Чтение файла
    std::ifstream fileDateBase("testBase4.dat", std::ios::binary);

    //Проверка чтения файла
    if (!fileDateBase) {
        std::cout << "Не удалось открыть файл!" << std::endl;
        return 1;
    }
    
    //Заполнения БД в память
    for (int i = 0; !fileDateBase.read((char*)&locality[i],sizeof(record)).eof(); i++);

    while(true) {
        std::cout << "Full Name" << "\t\t\t" << "Street" << "\t\t" << "Number of house" << "\t" << "\tNumber of apartment" << "\t" << "Settlement date" << std::endl;
        for (int i = currentPage * 20; i < currentPage * 20 + 20; i++) {
            std::cout << i+1 << ". " << locality[i].fullname << "\t" << locality[i].street << "\t" << locality[i].numberHouse << "\t\t\t" << locality[i].numberApartment << "\t\t\t\t" << locality[i].dateSettle << std::endl;
        }
        std::cout << "<q> : Quit programs" << "\t\t" << "<h> : Last page" << "\t\t" << "<l> : Next page" << std::endl;

        readKey(key);
        checkKey(key, currentPage);
        system("clear");
    }
    fileDateBase.close();

    return 0;
}

