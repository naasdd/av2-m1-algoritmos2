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
    dataBaseTXT.close()
}

void readDataBase() {

}

void registerProduct()
{
}

void sellProduct()
{
    // A fazer
}

void mainMenu()
{
    cout << "--- Menu ---\n";
    cout << "  - Menu principal:\n";
    cout << "    - [1] Cadastrar produto\n";
    cout << "    - [2] Vender produto\n";
    cout << "    - [0] Voltar\n";
    cout << "  - Menu de pagamento ao finalizar venda.\n";

    cout << "\nEscolha uma opção: ";
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
    case 0:
        void menu();
        break;
    default:
        break;
    }
}

void menu()
{

    cout << dateNow();

    cout << "--- Menu ---\n";
    cout << "    - [1] Menu principal\n";
    cout << "    - [2] Formas de Pagamento \n";
    cout << "    - [3] Datas de Parcelas \n";
    cout << "    - [0] Sair\n";
    cout << "  - Menu de pagamento ao finalizar venda.\n";

    cout << "\nEscolha uma opção: ";
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
    case 0:
        return;
        break;
    default:
        break;
    }
}

int main()
{
    system("chcp 65001");

    menu();

    return 0;
}