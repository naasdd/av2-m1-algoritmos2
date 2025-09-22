#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>

using namespace std;

struct Date
{
    int month, day, year;
};

struct Product
{
    string productName;
    float amountProduct;
    float productPrice;
};

string dateNow()
{
    time_t timestamp = time(NULL);
    tm *data = localtime(&timestamp);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", data);

    return string(buffer);
}

void createProduct()
{
    Product temp;

    cout << "Nome do produto: ";
    cin >> temp.productName;

    cout << "Quantidade: ";
    cin >> temp.amountProduct;

    cout << "Valor de venda";
    cin >> temp.productPrice;

    // A fazer parte de verificar se já existe;

    ofstream dataBaseTXT;
    dataBaseTXT.open("./database.txt");
    dataBaseTXT << temp.productName << "," << temp.amountProduct << "," << temp.productPrice << endl;
    dataBaseTXT.close();
}

void readDataBase()
{
}

void registerProduct()
{
}

void sellProduct()
{
    // A fazer
}

void sellMenu()
{
    int option;
    do
    {

        cout << "\n\n--- Menu de venda ---\n";
        cout << "    - [1] Formas de Pagamento \n";
        cout << "    - [2] Datas de Parcelas \n";
        cout << "    - [0] Voltar\n";

        cout << "\nEscolha uma opção: ";

        cin >> option;

        switch (option)
        {
        case 1:
            break;
        case 2:
            break;
        case 0:
            break;
        default:
            cout << "Opção inválida!\n";
            break;
        }
    } while (option != 0);
}

void mainMenu()
{
    cout << dateNow();

    int option;
    do
    {

        cout << "\n\n--- Menu ---\n";
        cout << "  - Menu principal:\n";
        cout << "    - [1] Cadastrar produto\n";
        cout << "    - [2] Vender produto\n";
        cout << "    - [0] Sair\n";

        cout << "\nEscolha uma opção: ";
        cin >> option;

        switch (option)
        {
        case 1:
            registerProduct();
            break;
        case 2:
            sellMenu();
            break;
        case 0:
            cout << "\nEncerrando sessão...\n";
            break;
        default:
            cout << "Opção inválida!\n";
            break;
        }
    } while (option != 0);
}

int main()
{
    system("chcp 65001");

    mainMenu();

    return 0;
}