#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip> // Para setprecision e setw

using namespace std;

struct Date
{
    int day, month, year;
};

struct Product
{
    string productName;
    float amountProduct;
    float productPrice;
};

Product productArray[20];

int lerNumero()
{
    int numero;
    while (!(cin >> numero))
    {
        cin.clear();            // limpa erro
        cin.ignore(1000, '\n'); // descarta entrada inválida
        cout << "Valor inválido! Digite um número inteiro: ";
    }
    cin.ignore(1000, '\n'); // limpa resto da linha
    return numero;
}

float lerDecimal()
{
    float numero;
    while (!(cin >> numero))
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Valor inválido! Digite um número decimal: ";
    }
    cin.ignore(1000, '\n');
    return numero;
}

string getCurrentDate()
{
    // pega o horário atual em segundos desde 1 jan 1970
    time_t atualTime = time(0);

    // converte esse valor para uma struct com dia/mes/ano/hora
    tm *now = localtime(&atualTime);

    // cria um fluxo de string para montar a data formatada
    stringstream ss;

    // formata: dia/mês/ano (com 2 dígitos pro dia e mês, e zeros à esquerda se precisar)
    ss << setw(2) << setfill('0') << now->tm_mday << "/"
       << setw(2) << setfill('0') << (now->tm_mon + 1) << "/"
       << (now->tm_year + 1900);

    // retorna a data como string no formato "dd/mm/yyyy"
    return ss.str();
}

// converte string de data "dd/mm/yyyy" para struct Date
Date stringToDate(string dateStr)
{
    Date d;
    stringstream ss(dateStr);
    char delimiter; // para capturar as barras "/"

    ss >> d.day >> delimiter >> d.month >> delimiter >> d.year;

    return d;
}

// converte struct Date para string formatada "dd/mm/yyyy"
string dateToString(Date d)
{
    stringstream ss;
    ss << setw(2) << setfill('0') << d.day << "/"
       << setw(2) << setfill('0') << d.month << "/"
       << d.year;
    return ss.str();
}

// verifica se um ano é bissexto
bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// adiciona dias a uma data (calcula parcelas)
Date addDays(Date d, int days)
{
    // dias em cada mês (jan=31, fev=28, mar=31, etc)
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // verifica se o ano é bissexto e ajusta fevereiro (utilizando operador ternário)
    daysInMonth[2] = isLeapYear(d.year) ? 29 : 28;

    d.day += days; // adiciona os dias

    // enquanto o dia ultrapassar o limite do mês
    while (d.day > daysInMonth[d.month])
    {
        d.day -= daysInMonth[d.month]; // subtrai os dias do mês atual
        d.month++;                     // avança para o próximo mês

        // se passou de dezembro, vai para janeiro do próximo ano
        if (d.month > 12)
        {
            d.month = 1;
            d.year++;
            // recalcula fevereiro para o novo ano
            daysInMonth[2] = isLeapYear(d.year) ? 29 : 28;
        }
    }

    return d;
}

// mostra as datas calculadas de cada parcela
void showInstallmentDates(int numParcelas, float valorParcela)
{
    // pega a data atual e converte para struct Date
    Date currentDate = stringToDate(getCurrentDate());

    cout << "\nDatas das parcelas:\n";
    cout << "+-----------------------------+\n";

    // para cada parcela, calcula a data (30 dias * número da parcela)
    for (int i = 1; i <= numParcelas; i++)
    {
        Date parcelaDate = addDays(currentDate, 30 * i);
        cout << "Parcela " << i << ": R$ " << fixed << setprecision(2)
             << valorParcela << " - " << dateToString(parcelaDate) << "\n";
    }

    cout << "+-----------------------------+\n";
}

// Após o usuario cadastrar, essa função vê se ja tinha o produto adicionado antes
Product updateDatabase(Product userInput)
{
    ifstream readDataBaseTXT("./database.txt");
    string line;
    Product temp;
    vector<string> lines;

    int i = 0;

    cout << "\n[DEBBUG] Iniciando leitura de arquivo txt\n";
    while (getline(readDataBaseTXT, line))
    {
        if (line.empty())
            continue; // pula linhas vazias

        string nameSTR, quantitySTR, priceSTR;
        stringstream ss(line); // stringstream é um tipo string manipulável

        getline(ss, nameSTR, ',');
        getline(ss, quantitySTR, ',');
        getline(ss, priceSTR, ',');

        if (userInput.productName == nameSTR)
        {
            cout << "[DEBBUG] Produto já cadastrado no banco de dados" << endl;

            userInput.amountProduct += stof(quantitySTR);
        }
        else
        {
            lines.push_back(line);
        }
    }

    // Reescreve database
    ofstream overwriteDataBaseTXT("./database.txt", ios::trunc); // começa a reescrever todo o banco

    overwriteDataBaseTXT << userInput.productName << "," << userInput.amountProduct << "," << userInput.productPrice << endl;

    for (int i = 0; i < lines.size(); i++)
    {
        string l = lines[i];
        overwriteDataBaseTXT << l << "\n";
    }

    return userInput;
}

void createProduct()
{
    system("cls");
    Product temp;

    cout << "\n+-----------------------------+\n";
    cout << "|      Cadastrar Produto      |\n";
    cout << "+-----------------------------+\n";

    cout << "Nome do produto: ";
    cin >> temp.productName;

    cout << "Quantidade: ";
    temp.amountProduct = lerDecimal();

    cout << "Valor de venda: ";
    temp.productPrice = lerDecimal();

    temp = updateDatabase(temp);

    cout << "\n*** PRODUTO CADASTRADO COM SUCESSO! ***\n";
    cout << "Pressione Enter para continuar...";
    // cin.ignore();
    cin.get();
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
        if (line.empty())
            continue; // pula linhas vazias

        string nameSTR, quantitySTR, priceSTR;
        stringstream ss(line); // stringstream é um tipo string manipulável

        getline(ss, nameSTR, ','); // pega a string SS, guarda em nameSTR, e lê até chegar na ","
        getline(ss, quantitySTR, ',');
        getline(ss, priceSTR, ',');

        // FAZER A LÓGICA DE VALIDAÇÃO DE CAMPOS ANTES DE CONVERTER

        if (quantitySTR.empty() || priceSTR.empty())
            continue; // pula linha inválida

        try
        {
            temp.productName = nameSTR;
            temp.amountProduct = stof(quantitySTR); // stof() transforma string em float
            temp.productPrice = stof(priceSTR);
        }
        catch (const std::exception &)
        {
            continue; // pula linha inválida
        }

        cout << temp.productName << " "
             << temp.amountProduct << " "
             << temp.productPrice << "\n";

        if (i < 20)
        {
            productArray[i] = temp;
            i += 1;
        }
    }

    return temp;
}

void paymentOptionsMenu()
{
    float totalCompra = 0.0f; // FAZER A LÓGICA PARA CALCULAR TOTAL DA COMPRA
    int option;
    do
    {
        system("cls");
        cout << "\n+-----------------------------+\n";
        cout << "|      Formas de Pagamento    |\n";
        cout << "+-----------------------------+\n";
        cout << "\n*** AQUI VAI A LÓGICA DAS FORMAS DE PAGAMENTO ***\n";
        cout << "\nTotal da compra: R$ " << totalCompra << "\n";
        cout << "+-----------------------------+\n";
        cout << "  [1] À vista (5% desconto)\n";
        cout << "  [2] Até 3x sem juros\n";
        cout << "  [3] Até 12x com 10% juros\n";
        cout << "  [0] Voltar\n";
        cout << "Escolha uma opção: ";
        option = lerNumero();

        switch (option)
        {
        case 1:
            system("cls");
            cout << "\n+-----------------------------+\n";
            cout << "|      Resumo da Compra       |\n";
            cout << "+-----------------------------+\n";
            cout << "Total: R$ " << fixed << setprecision(2) << totalCompra << "\n";
            cout << "Desconto (5%): R$ " << (totalCompra * 0.05f) << "\n";
            cout << "Total a pagar: R$ " << (totalCompra * 0.95f) << "\n";
            cout << "Forma: À VISTA\n";

            // PARTE 4: Mostra data de vencimento (pagamento à vista = 1 parcela)
            showInstallmentDates(1, totalCompra * 0.95f);

            cout << "\nVenda finalizada!\n";
            cout << "Pressione Enter para continuar...";
            cin.get();
            return;
        case 2:
        {
            system("cls");
            cout << "\n+-----------------------------+\n";
            cout << "|      Parcelamento sem Juros |\n";
            cout << "+-----------------------------+\n";
            cout << "Total: R$ " << fixed << setprecision(2) << totalCompra << "\n";
            cout << "Escolha o número de parcelas (1-3): ";
            int parcelas = lerNumero();

            // validação: parcelas deve estar entre 1 e 3
            if (parcelas < 1 || parcelas > 3)
            {
                cout << "\nNúmero de parcelas inválido! Deve ser entre 1 e 3.\n";
                cout << "Pressione Enter para continuar...";
                cin.get();
                break;
            }

            float valorParcela = totalCompra / parcelas;
            cout << "\n" << parcelas << "x de R$ " << valorParcela << "\n";
            
            // PARTE 6: Mostra datas de cada parcela
            showInstallmentDates(parcelas, valorParcela);

            cout << "\nVenda finalizada!\n";
            cout << "Pressione Enter para continuar...";
            cin.get();
            return;
        }
        case 3:
        {
            system("cls");
            cout << "\n+-----------------------------+\n";
            cout << "|      Parcelamento com Juros |\n";
            cout << "+-----------------------------+\n";
            cout << "\n*** AQUI VAI A LÓGICA DO PARCELAMENTO COM JUROS ***\n";
            cout << "Total: R$ " << totalCompra << "\n";
            cout << "Juros (10%): R$ " << (totalCompra * 0.10f) << "\n";
            float totalComJuros = totalCompra * 1.10f;
            cout << "Total com juros: R$ " << totalComJuros << "\n";
            cout << "Escolha o número de parcelas (4-12): ";
            int parcelasJuros = lerNumero();

            cout << "\n"
                 << parcelasJuros << "x de R$ " << (totalComJuros / parcelasJuros) << "\n";
            cout << "\nVenda finalizada!\n";
            cout << "Pressione Enter para continuar...";
            cin.ignore();
            cin.get();
            return;
        }
        case 0:
            break;
        default:
            cout << "Opção inválida!\n";
            cout << "Pressione Enter para continuar...";
            cin.ignore();
            cin.get();
            break;
        }
    } while (option != 0);
}

void installmentDatesMenu()
{
    system("cls");
    cout << "\n+-----------------------------+\n";
    cout << "|        Datas de Parcelas    |\n";
    cout << "+-----------------------------+\n";
    cout << "\n*** AQUI VAI A LÓGICA DE CALCULAR DATAS DAS PARCELAS ***\n";
    cout << "\nParcelas com datas calculadas:\n";
    cout << "Parcela 1: R$ 21.83 - 24/10/2025\n";
    cout << "Parcela 2: R$ 21.83 - 24/11/2025\n";
    cout << "Parcela 3: R$ 21.84 - 24/12/2025\n";
    cout << "+-----------------------------+\n";
    cout << "\nPressione Enter para continuar...";
    cin.ignore();
    cin.get();
}

void sellProduct()
{
    // FAZER A LÓGICA DE VENDER PRODUTO
}

void sellMenu()
{
    int option;
    do
    {
        system("cls");
        cout << "\n+-----------------------------+\n";
        cout << "|          Menu de Venda      |\n";
        cout << "+-----------------------------+\n";
        cout << "  [1] Formas de Pagamento \n";
        cout << "  [2] Datas de Parcelas \n";
        cout << "  [0] Voltar\n";
        cout << "Escolha uma opção: ";
        option = lerNumero();

        switch (option)
        {
        case 1:
            paymentOptionsMenu();
            break;
        case 2:
            installmentDatesMenu();
            break;
        case 0:
            break;
        default:
            cout << "Opção inválida!\n";
            cout << "Pressione Enter para continuar...";
            cin.ignore();
            cin.get();
            break;
        }
    } while (option != 0);
}

void mainMenu()
{
    int option;
    do
    {
        system("cls");
        cout << "\n+-----------------------------+\n";
        cout << "|       Data: " << getCurrentDate() << "      |\n";
        cout << "+-----------------------------+\n";
        cout << "|        Menu Principal       |\n";
        cout << "+-----------------------------+\n";
        cout << "  [1] Cadastrar produto\n"; // LOGICA PARA VERIFICAR SE É NUMERO
        cout << "  [2] Vender produto\n";
        cout << "  [0] Sair\n";
        cout << "Escolha uma opção: ";
        option = lerNumero();

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
            cout << "Pressione Enter para continuar...";
            cin.ignore();
            cin.get();
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