#include <iostream>
#include <fstream>
#include <stdio.h>

struct record {
    char fullname[32];
    char street[18];
    short int numberHouse;
    short int numberApartment;
    char dateSettle[10];
};

int main() {
    record* locality = new record[4000];
    
    int currentPage = 0;

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
        for (int i = currentPage * 20; i < currentPage * 20 + 20; i++) {
            std::cout << locality[i].fullname << "\t" << locality[i].street << "\t" << locality[i].numberHouse << "\t\t\t" << locality[i].numberApartment << "\t\t\t\t" << locality[i].dateSettle << std::endl;
        }
        char key;
        std::cin >> key;
        if (key == 'q') return 2;
    }

    fileDateBase.close();

    return 0;
}





        // std::cout << "Full Name" << "\t\t\t" << "Street" << "\t\t" << "Number of house" << "\t" << "\tNumber of apartment" << "\t" << "Settlement date" << std::endl;
        // for(int i = 0; !fileDateBase.read((char*)&locality[i],sizeof(record)).eof(); i++) {
        //     //std::cout << locality[i].fullname << "\t" << locality[i].street << "\t" << locality[i].numberHouse << "\t\t\t" << locality[i].numberApartment << "\t\t\t\t" << locality[i].dateSettle << std::endl;
        // }