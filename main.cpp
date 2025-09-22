#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>

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

Product productArray[20];

string dateNow()
{
    time_t atualTime = time(0); // pega a hora atual
    tm *now = localtime(&atualTime); // converte para o fuso local com a divisão de horas, minutos e segundos

    return to_string((now->tm_year + 1900)) + '-' + to_string((now->tm_mon + 1)) + '-' + to_string(now->tm_mday);
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

Product readDataBase()
{
    ifstream dataBaseTXT("./database.txt");
    string line;
    Product temp;

    int i = 0;

    cout << "\n[DEBBUG] Iniciando leitura de arquivo txt\n";
    while (getline(dataBaseTXT, line))
    {
        string nameSTR, quantitySTR, priceSTR;
        stringstream ss(line);

        getline(ss, nameSTR, ',');
        getline(ss, quantitySTR, ',');
        getline(ss, priceSTR, ',');

        temp.productName = nameSTR;
        temp.amountProduct = stof(quantitySTR);
        temp.productPrice = stof(priceSTR);

        cout << temp.productName << " ";
        cout << temp.amountProduct << " ";
        cout << temp.productPrice << "\n";

        productArray[i] = temp;
        i += 1;
    }

    cout << "\n[DEBBUG] Finalizado leitura de arquivo txt\n";
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
            createProduct();
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

    readDataBase();

    mainMenu();

    return 0;
}