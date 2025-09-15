#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <struct.h>
#include <ctime>
#include "./include/dateNow.h"
using namespace std;

struct Date{
    int month, day , year;
};


struct Product
{
    string productName;
    float amountProduct;
    float productPrice;
};

std::string dateNow()
{
    time_t timestamp = time(NULL);
    tm *data = localtime(&timestamp);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", data);

    return std::string(buffer);
}

int main()
{

    cout << "--- Programa iniciado ---\n";
    cout << "Selecione uma opção\n";
    cout << "1 - Cadastrar um produto\n";
    cout << "2 - Vender um produto\n";
    cout << dateNow();

    int option;
    cin >> option;

    switch (option)
    {
    case 1:
        void registerProduct();
        break;
    case 2:
        void sellProduct();
        break;


    default:
        break;
    }

    return 0;
}
