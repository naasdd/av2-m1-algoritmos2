#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <struct.h>
#include <ctime>
#include "./include/dateNow.h"
using namespace std;

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
