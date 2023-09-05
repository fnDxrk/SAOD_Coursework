#include <iostream>
#include <fstream>

struct record {
    char fullname[32];
    char street[18];
    short int numberHouse;
    short int numberApartment;
    char dateSettle[10];
};

int main() {
    record* locality = new record[4000];

    std::ifstream fileDateBase("testBase4.dat", std::ios::binary);

    if (!fileDateBase) {
        std::cout << "Не удалось открыть файл!" << std::endl;
        return 1;
    }

    if (fileDateBase.is_open()) {
        for(int i = 0; !fileDateBase.read((char*)&locality[i],sizeof(record)).eof(); i++) {
            std::cout << locality[i].fullname << "\t" << locality[i].street << "\t" << locality[i].numberHouse << "\t" << locality[i].numberApartment <<  "\t" << locality[i].dateSettle << std::endl;
        }
    }

    fileDateBase.close();

    return 0;
}