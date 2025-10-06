#include <iostream>
#include <cstdlib> // funções utilitárias (system)
#include <ctime>   // usado para funções de data e hora
#include <string>  // tipo string e operações
#include <fstream> // leitura e escrita de arquivos
#include <sstream> // manipulação de string streams (stringstream)
#include <vector>  // contêiner vector
#include <iomanip> // formatação de I/O (setw, setfill, setprecision, fixed)

using namespace std;

// Estrutura para datas
struct Date
{
    int day, month, year; // dia, mês e ano
};

// Estrutura de produto no estoque
struct Product
{
    string name;     // nome do produto
    float quantity;  // quantidade disponível
    float unitPrice; // preço unitário
};

// Estrutura para itens no carrinho de compras
struct CartItem
{
    string name;     // nome do produto
    float quantity;  // quantidade escolhida pelo cliente
    float unitPrice; // preço unitário do produto
    float subtotal;  // subtotal = quantity * unitPrice
};

Product productArray[20]; // até 20 produtos cadastrados
int totalProducts = 0;    // contador de produtos no array
vector<CartItem> cart;    // carrinho de compras (dinâmico com vector)

// Pega a data atual formatada
string getCurrentDate()
{
    time_t nowTime = time(0);      // pega tempo atual em segundos
    tm *now = localtime(&nowTime); // converte para data local
    stringstream ss;
    ss << setw(2) << setfill('0') << now->tm_mday << "/"      // setw(2) -> largura mínima de 2 caracteres
       << setw(2) << setfill('0') << (now->tm_mon + 1) << "/" // setfill('0') -> preenche com zero à esquerda
       << (now->tm_year + 1900);                              // ano atual
    return ss.str();
}

// Lê string com validação
string readString()
{
    string input;
    getline(cin, input);
    while (input.empty()) // impede entrada vazia
    {
        cout << "Entrada invalida! Digite um nome valido: ";
        getline(cin, input);
    }
    return input;
}

// Lê número inteiro com validação
int readInt()
{
    int number;
    while (!(cin >> number)) // verifica se é inteiro
    {
        cin.clear();            // limpa flags de erro
        cin.ignore(1000, '\n'); // descarta buffer
        cout << "Entrada invalida! Digite um numero inteiro: ";
    }
    cin.ignore(1000, '\n'); // limpa buffer do teclado
    return number;
}

// Lê número decimal com validação
float readFloat()
{
    float number;
    while (!(cin >> number)) // verifica se é decimal
    {
        cin.clear();            // limpa flags de erro
        cin.ignore(1000, '\n'); // descarta buffer
        cout << "Entrada invalida! Digite um numero decimal: ";
    }
    cin.ignore(1000, '\n'); // limpa buffer
    return number;
}

// Converte string em Date (dd/mm/yyyy)
Date stringToDate(string dateStr)
{
    Date d;
    stringstream ss(dateStr);
    char delimiter;
    ss >> d.day >> delimiter >> d.month >> delimiter >> d.year;
    return d;
}

// Converte Date em string
string dateToString(Date d)
{
    stringstream ss;
    ss << setw(2) << setfill('0') << d.day << "/"
       << setw(2) << setfill('0') << d.month << "/"
       << d.year;
    return ss.str();
}

// Verifica ano bissexto
bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Adiciona dias a uma data
Date addDays(Date d, int days)
{
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    daysInMonth[2] = isLeapYear(d.year) ? 29 : 28; // ajusta fevereiro se for bissexto
    d.day += days;
    while (d.day > daysInMonth[d.month]) // rola para próximo mês
    {
        d.day -= daysInMonth[d.month];
        d.month++;
        if (d.month > 12) // rola para próximo ano
        {
            d.month = 1;
            d.year++;
            daysInMonth[2] = isLeapYear(d.year) ? 29 : 28; // ajusta fevereiro
        }
    }
    return d;
}

// Mostra datas das parcelas
void showInstallmentDates(int numInstallments, float installmentValue)
{
    Date currentDate = stringToDate(getCurrentDate());
    cout << "\nDatas das parcelas:\n";
    cout << "+-------------------------------------------+\n";
    for (int i = 1; i <= numInstallments; i++)
    {
        Date installmentDate = addDays(currentDate, 30 * i);           // cada parcela +30 dias
        cout << "Parcela " << i << ": R$ " << fixed << setprecision(2) // fixed -> força ponto fixo (não notação científica)
             << installmentValue << " - " << dateToString(installmentDate) << "\n";
    }
    cout << "+-------------------------------------------+\n";
}

// Salva produtos no arquivo database.txt
void saveDatabase()
{
    ofstream file("../database.txt", ios::trunc); // sobrescreve arquivo
    for (int i = 0; i < totalProducts; i++)
    {
        if (productArray[i].quantity > 0)
        {
            file << productArray[i].name << ","
                 << productArray[i].quantity << ","
                 << productArray[i].unitPrice << endl;
        }
    }
    file.close();
}

// Atualiza ou adiciona produto no estoque
Product updateDatabase(Product newProduct)
{
    bool found = false;
    for (int i = 0; i < totalProducts; i++)
    {
        if (productArray[i].name == newProduct.name) // produto já existe
        {
            productArray[i].quantity += newProduct.quantity;  // soma quantidade
            productArray[i].unitPrice = newProduct.unitPrice; // atualiza preço
            found = true;
            newProduct = productArray[i];
            break;
        }
    }
    if (!found && totalProducts < 20) // novo produto
    {
        productArray[totalProducts] = newProduct;
        totalProducts++;
    }
    saveDatabase();
    return newProduct;
}

// Cadastro de novo produto
void createProduct()
{
    system("cls"); // limpa tela
    Product temp;
    cout << "\n+-------------------------------------------+\n";
    cout << "| Bem-vindo ao Bug & Buy | Data: " << getCurrentDate() << " |\n";
    cout << "+-------------------------------------------+\n";
    cout << "| Cadastrar Produto                         |\n";
    cout << "+-------------------------------------------+\n";
    cout << "Nome do produto: ";
    temp.name = readString();
    cout << "Quantidade: ";
    temp.quantity = readFloat();
    cout << "Valor de venda: ";
    temp.unitPrice = readFloat();
    temp = updateDatabase(temp); // salva no array + arquivo
    cout << "\n*** PRODUTO CADASTRADO COM SUCESSO! ***\n";
    cin.get(); // pausa até o usuário pressionar Enter
}

// Lê o arquivo database.txt e carrega produtos
void loadDatabase()
{
    ifstream file("../database.txt");
    string line;
    Product temp;
    int i = 0;
    while (getline(file, line)) // lê linha por linha
    {
        if (line.empty())
            continue;
        string nameStr, quantityStr, priceStr;
        stringstream ss(line);
        getline(ss, nameStr, ',');
        getline(ss, quantityStr, ',');
        getline(ss, priceStr, ',');

        if (quantityStr.empty() || priceStr.empty())
            continue;

        try // evita erro se conversão string -> float falhar
        {
            temp.name = nameStr;
            temp.quantity = stof(quantityStr); // string -> float
            temp.unitPrice = stof(priceStr);
        }
        catch (const std::exception &) // captura exceção se conversão falhar
        {
            continue; // ignora linha inválida
        }

        if (i < 20) // limite do array
        {
            productArray[i] = temp;
            i++;
        }
    }
    totalProducts = i;
    file.close();
}

// Lista todos os produtos cadastrados
void listProducts()
{
    system("cls");
    cout << "\n+----------------------------------------------------------------+\n";
    cout << "| Bem-vindo ao Bug & Buy | Data: " << getCurrentDate() << "                      |\n";
    cout << "+----------------------------------------------------------------+\n";
    cout << "| ID | Produto                  | Quantidade   | Preco Unit.     |\n";
    cout << "+----------------------------------------------------------------+\n";

    if (totalProducts == 0)
    {
        cout << "|                 Nenhum produto cadastrado!                     |\n";
    }
    else
    {
        for (int i = 0; i < totalProducts; i++)
        {
            cout << "| " << setw(2) << left << (i + 1) << " | " // setw(2) -> largura 2, left -> alinha à esquerda
                 << setw(25) << left << productArray[i].name << " | "
                 << setw(11) << fixed << setprecision(2) << productArray[i].quantity << " | R$ " // fixed + setprecision(2) -> sempre 2 casas decimais
                 << setw(12) << fixed << setprecision(2) << productArray[i].unitPrice << " |\n";
        }
    }
    cout << "+----------------------------------------------------------------+\n";
}

// Mostra o carrinho
void showCart()
{
    cout << "\n+----------------------------------------------------------------+\n";
    cout << "| Bem-vindo ao Bug & Buy | Data: " << getCurrentDate() << "                      |\n";
    cout << "+----------------------------------------------------------------+\n";
    cout << "| Produto                  | Qtd         | Preco Unit  | Subtotal|\n";
    cout << "+----------------------------------------------------------------+\n";

    float total = 0.0f; // inicializa total do carrinho
    for (size_t i = 0; i < cart.size(); i++)
    {
        cout << "| " << setw(25) << left << cart[i].name << " | "                    // nome do produto, alinhado à esquerda
             << setw(10) << fixed << setprecision(2) << cart[i].quantity << " | R$ " // quantidade com 2 casas decimais
             << setw(9) << cart[i].unitPrice << " | R$ "
             << setw(9) << cart[i].subtotal << " |\n"; // subtotal formatado
        total += cart[i].subtotal;                     // acumula total
    }
    cout << "+----------------------------------------------------------------+\n";
    cout << "| TOTAL: R$ " << setw(53) << fixed << setprecision(2) << total << "|\n"; // mostra total final
    cout << "+----------------------------------------------------------------+\n";
}

// Calcula total do carrinho
float getCartTotal()
{
    float total = 0.0f;
    for (size_t i = 0; i < cart.size(); i++)
        total += cart[i].subtotal; // soma todos os subtotais
    return total;
}

// Menu de pagamento
void paymentMenu()
{
    float totalPurchase = getCartTotal(); // total do carrinho
    int option;
    do
    {
        system("cls");
        cout << "\n+-------------------------------------------+\n";
        cout << "| Bem-vindo ao Bug & Buy | Data: " << getCurrentDate() << " |\n";
        cout << "+-------------------------------------------+\n";
        cout << "| Formas de Pagamento                       |\n";
        cout << "+-------------------------------------------+\n";
        cout << "\nTotal da compra: R$ " << fixed << setprecision(2) << totalPurchase << "\n";
        cout << "+-------------------------------------------+\n";
        cout << "  [1] À vista (5% desconto)\n";
        cout << "  [2] Até 3x sem juros\n";
        cout << "  [3] Até 12x com 10% juros\n";
        cout << "  [0] Voltar\n";
        cout << "Escolha uma opção: ";
        option = readInt();
        switch (option)
        {
        case 1:
            system("cls");
            cout << "\nTotal: R$ " << totalPurchase << "\n";
            cout << "Desconto (5%): R$ " << (totalPurchase * 0.05f) << "\n";
            cout << "Total a pagar: R$ " << (totalPurchase * 0.95f) << "\n";
            showInstallmentDates(1, totalPurchase * 0.95f); // 1 parcela
            cart.clear();                                   // limpa o carrinho (vector::clear)
            saveDatabase();                                 // atualiza estoque
            cout << "\nVenda finalizada!\n";
            cin.get(); // pausa para usuário ver o resultado
            return;
        case 2:
        {
            cout << "Escolha o numero de parcelas (1-3): ";
            int installments = readInt();
            if (installments < 1 || installments > 3)
                break;
            float installmentValue = totalPurchase / installments;
            showInstallmentDates(installments, installmentValue);
            cart.clear();   // limpa carrinho
            saveDatabase(); // atualiza estoque
            cout << "\nVenda finalizada!\n";
            cin.get(); // pausa
            return;
        }
        case 3:
        {
            float totalWithInterest = totalPurchase * 1.10f; // 10% juros
            cout << "Total com juros: R$ " << totalWithInterest << "\n";
            cout << "Escolha o numero de parcelas (4-12): ";
            int installments = readInt();
            if (installments < 4 || installments > 12)
                break;
            float installmentValue = totalWithInterest / installments;
            showInstallmentDates(installments, installmentValue);
            cart.clear();
            saveDatabase();
            cout << "\nVenda finalizada!\n";
            cin.get();
            return;
        }
        case 0:
            return; // volta
        default:
            break;
        }
    } while (option != 0);
}

// Venda de produtos
void sellProduct()
{
    int option;
    do
    {
        listProducts(); // mostra produtos
        if (totalProducts == 0)
        {
            cout << "\nNao ha produtos cadastrados para vender!\n";
            cout << "Pressione Enter para voltar...";
            cin.get();
            return;
        }
        cout << "\nDigite o ID do produto (ou 0 para finalizar): ";
        option = readInt();
        if (option == 0)
        {
            if (!cart.empty())
            {
                system("cls");
                showCart(); // mostra carrinho
                cout << "\n[1] Finalizar venda\n[2] Cancelar venda\n[0] Voltar\nEscolha uma opção: ";
                int confirm = readInt();
                if (confirm == 1)
                {
                    paymentMenu();
                    return;
                }
                if (confirm == 2)
                {
                    // devolve produtos ao estoque
                    for (size_t i = 0; i < cart.size(); i++)
                    {
                        for (int j = 0; j < totalProducts; j++)
                        {
                            if (productArray[j].name == cart[i].name)
                            {
                                productArray[j].quantity += cart[i].quantity;
                                break;
                            }
                        }
                    }
                    cart.clear(); // limpa carrinho
                    saveDatabase();
                    cout << "\n*** VENDA CANCELADA! ***\nPressione Enter para continuar...";
                    cin.get();
                    return;
                }
                if (confirm == 0)
                    return;
            }
            else
                return;
        }
        else if (option > 0 && option <= totalProducts)
        {
            int index = option - 1;
            cout << "\nProduto selecionado: " << productArray[index].name << "\n";
            cout << "Estoque disponivel: " << productArray[index].quantity << "\n";
            cout << "Preco unitario: R$ " << productArray[index].unitPrice << "\n";
            cout << "Quantidade a comprar: ";
            float quantity = readFloat();
            if (quantity <= 0 || quantity > productArray[index].quantity)
            {
                cout << "\nQuantidade invalida ou estoque insuficiente!\n";
                cin.get();
                continue;
            }
            CartItem item;
            item.name = productArray[index].name;
            item.quantity = quantity;
            item.unitPrice = productArray[index].unitPrice;
            item.subtotal = quantity * productArray[index].unitPrice;
            cart.push_back(item);                     // adiciona ao carrinho
            productArray[index].quantity -= quantity; // decrementa estoque
            cout << "\n*** PRODUTO ADICIONADO AO CARRINHO! ***\n";
            cin.get();
        }
    } while (true);
}

// Menu principal
void mainMenu()
{
    int option;
    do
    {
        system("cls");
        cout << "\n+-------------------------------------------+\n";
        cout << "| Bem-vindo ao Bug & Buy | Data: " << getCurrentDate() << " |\n";
        cout << "+-------------------------------------------+\n";
        cout << "| Menu Principal                            |\n";
        cout << "+-------------------------------------------+\n";
        cout << "  [1] Cadastrar produto\n";
        cout << "  [2] Vender produto\n";
        cout << "  [3] Listar produtos\n";
        cout << "  [0] Sair\n";
        cout << "Escolha uma opção: ";
        option = readInt();
        switch (option)
        {
        case 1:
            createProduct();
            break;
        case 2:
            sellProduct();
            break;
        case 3:
            loadDatabase();
            listProducts();
            cout << "\nPressione Enter para continuar...";
            cin.get();
            break;
        case 0:
            cout << "\nEncerrando sessão...\n";
            break;
        default:
            cout << "Opção inválida!\n";
            cin.get();
            break;
        }
    } while (option != 0);
}

// Função principal
int main()
{
    system("chcp 65001"); // encoding UTF-8 no Windows
    loadDatabase();       // carrega produtos do arquivo
    mainMenu();           // inicia menu
    return 0;
}
