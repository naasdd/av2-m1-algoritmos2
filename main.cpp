#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
#include <cstdlib> // funções utilitárias (system)
#include <string>  // tipo string e operações
#include <sstream> // manipulação de string streams (stringstream)
#include <iomanip> // formatação de I/O (setw, setfill, setprecision, fixed)

using namespace std;

const float cashDiscount = 0.05f;
const float installmentInterest = 0.10f;

struct Date
{
    int day, month, year;
};

struct Product
{
    string name;
    float quantity;
    float unitPrice;
};

struct CartItem
{
    string name;
    float quantity;
    float unitPrice;
    float subtotal;
};

vector<Product> productArray; // produtos (dinâmico com vector)
vector<CartItem> cart;        // carrinho de compras (dinâmico com vector)

// Pega a data atual formatada
string getCurrentDate()
{
    time_t nowTime = time(0);                                   // pega tempo atual em segundos desde 1º de janeiro de 1970 (Epoch Time)
    tm *now = localtime(&nowTime);                              // converte para data local
    stringstream text;                                          // stringstream serve para "montar" um texto aos poucos, como se fosse cout
    text << setw(2) << setfill('0') << now->tm_mday << "/"      // setw(2) define largura mínima de 2 caracteres (ex: 7 -> " 7")
         << setw(2) << setfill('0') << (now->tm_mon + 1) << "/" // setfill('0') preenche com zero à esquerda se faltar espaço (ex: 7 -> "07")
         << (now->tm_year + 1900);                              // ano atual
    return text.str();
}

// Lê string com validação
string readString()
{
    string input;
    getline(cin, input);
    while (input.empty())
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
    while (!(cin >> number))
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
    while (!(cin >> number))
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
    Date date;
    stringstream text(dateStr); // stringstream serve para "montar" um texto aos poucos, como se fosse cout
    char delimiter;             // delimiter nesse caso é o caracter /
    text >> date.day >> delimiter >> date.month >> delimiter >> date.year;
    return date;
}

// Converte uma data em string (ex: 05/10/2025)
string dateToString(Date date)
{
    stringstream text; // stringstream serve para "montar" um texto aos poucos, como se fosse cout

    text << setw(2) << setfill('0') << date.day << "/"   // setw(2) define largura mínima de 2 caracteres (ex: 7 -> " 7")
         << setw(2) << setfill('0') << date.month << "/" // setfill('0') preenche com zero à esquerda se faltar espaço (ex: 7 -> "07")
         << date.year;                                   // ano normal

    return text.str(); // transforma o que foi montado em uma string normal
}

// Observação: A versão com stringstream é mais preferível porque permite
// formatação automática (zeros à esquerda, largura mínima) e é mais legível

// Alternativa usando apenas string (funciona, mas menos elegante e flexível)
/* string dateToString(Date d)
{
    string day = (d.day < 10 ? "0" : "") + to_string(d.day); // utilizando operador ternário
    string month = (d.month < 10 ? "0" : "") + to_string(d.month);
    string year = to_string(d.year);
    return day + "/" + month + "/" + year;
} */

// Verifica ano bissexto
bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Adiciona dias a uma data
Date addDays(Date date, int days)
{
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    daysInMonth[2] = isLeapYear(date.year) ? 29 : 28; // ajusta fevereiro se for bissexto
    date.day += days;
    while (date.day > daysInMonth[date.month]) // rola para próximo mês
    {
        date.day -= daysInMonth[date.month];
        date.month++;
        if (date.month > 12) // rola para próximo ano
        {
            date.month = 1;
            date.year++;
            daysInMonth[2] = isLeapYear(date.year) ? 29 : 28; // ajusta fevereiro
        }
    }
    return date;
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
        cout << "Parcela " << i << ": R$ " << fixed << setprecision(2) // fixed  força o número a ser mostrado em ponto fixo (1234.5 será mostrado como 1234.50 e não 1.2345e+03)
             << installmentValue << " - " << dateToString(installmentDate) << "\n";
    }
    cout << "+-------------------------------------------+\n";
}

// Salva produtos no arquivo database.txt
void saveDatabase()
{
    ofstream file("database.txt", ios::trunc); // abre o arquivo e apaga todo o conteúdo existente antes de escrever
    for (size_t i = 0; i < productArray.size(); i++) // size() retorna o tamanho atual do vector
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
    for (size_t i = 0; i < productArray.size(); i++) // size() retorna o tamanho atual do vector
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
    if (!found)
    {
        productArray.push_back(newProduct); // adiciona o produto ao final do vector
    }
    saveDatabase();
    return newProduct;
}

// Cadastro de novo produto
void createProduct()
{
    system("cls");
    Product temp;
    cout << "\n+-------------------------------------------+\n";
    cout << "| Bem-vindo ao Bug & Buy | Data: " << getCurrentDate() << " |\n";
    cout << "+-------------------------------------------+\n";
    cout << "| Cadastrar Produto                         |\n";
    cout << "+-------------------------------------------+\n";
    cout << "Nome do produto: ";
    temp.name = readString(); // valida as entradas
    cout << "Quantidade: ";
    temp.quantity = readFloat();
    cout << "Valor de venda: ";
    temp.unitPrice = readFloat();
    temp = updateDatabase(temp); // salva no vector + arquivo
    cout << "\n*** PRODUTO CADASTRADO COM SUCESSO! ***\n";
    cin.get(); // pausa até o usuário pressionar Enter
}

// Lê o arquivo database.txt e carrega produtos
void loadDatabase()
{
    ifstream file("database.txt");
    if (!file.is_open())
    {
        cout << "Aviso: arquivo database.txt não encontrado...\n";
        return;
    }

    string line;
    Product temp;

    productArray.clear(); // limpa o vector antes de carregar novos dados

    while (getline(file, line)) // lê linha por linha
    {
        if (line.empty())
            continue;

        // separa os campos da linha
        string nameStr, quantityStr, priceStr;
        stringstream text(line); // stringstream serve para "montar" um texto aos poucos, como se fosse cout
        getline(text, nameStr, ',');
        getline(text, quantityStr, ',');
        getline(text, priceStr, ',');

        if (quantityStr.empty() || priceStr.empty())
            continue;

        try // evita erro se conversão string -> float falhar
        {
            temp.name = nameStr;
            temp.quantity = stof(quantityStr); // stof converte string para float.
            temp.unitPrice = stof(priceStr);
        }
        catch (const std::exception &) // captura erros de conversão, sem copiar nem alterar o erro
        {
            continue; // ignora linha inválida
        }

        productArray.push_back(temp); // adiciona o produto ao final do vector
    }
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

    if (productArray.size() == 0) // size() retorna o tamanho atual do vector
    {
        cout << "|                 Nenhum produto cadastrado!                     |\n";
    }
    else
    {
        for (size_t i = 0; i < productArray.size(); i++) // preenche por linha
        {
            cout << "| " << setw(2) << left << (i + 1) << " | "                                  // setw define largura mínima de 2 caracteres (7 vira " 7")
                 << setw(25) << left << productArray[i].name << " | "                            // left alinha o valor à esquerda no espaço definido por setw ("7 ")
                 << setw(11) << fixed << setprecision(2) << productArray[i].quantity << " | R$ " // fixed mostra em notacao normal, ao inves de cientifica (5 vira 5.00)
                 << setw(12) << fixed << setprecision(2) << productArray[i].unitPrice << " |\n"; // setprecision mostra 2 casas decimais (123.456 vira 123.46)
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

    float total = 0.0f; // f indica float, senão seria double
    for (size_t i = 0; i < cart.size(); i++)
    {
        cout << "| " << setw(25) << left << cart[i].name << " | "                    // setw define largura mínima de 2 caracteres (7 vira " 7")
                                                                                     // e left alinha o valor à esquerda no espaço definido por setw ("7 ")
             << setw(10) << fixed << setprecision(2) << cart[i].quantity << " | R$ " // fixed mostra em notacao normal, ao inves de cientifica (5 vira 5.00)
                                                                                     // setprecision mostra 2 casas decimais (123.456 vira 123.46)
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
    float total = 0.0f; // f indica float, senão seria double
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
        cout << "  [1] À vista (" << (cashDiscount * 100) << "% desconto)\n";
        cout << "  [2] Até 3x sem juros\n";
        cout << "  [3] Até 12x com " << (installmentInterest * 100) << "% juros\n";
        cout << "  [0] Voltar\n";
        cout << "Escolha uma opção: ";
        option = readInt();
        switch (option)
        {
        case 1:
            system("cls");
            cout << "\nTotal: R$ " << totalPurchase << "\n";
            cout << "Desconto (" << (cashDiscount * 100) << "%): R$ " << (totalPurchase * cashDiscount) << "\n";
            cout << "Total a pagar: R$ " << (totalPurchase * (1 - cashDiscount)) << "\n";
            showInstallmentDates(1, totalPurchase * (1 - cashDiscount)); // 1 parcela
            cart.clear();                                                // limpa o carrinho
            saveDatabase();                                              // atualiza estoque
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
            float totalWithInterest = totalPurchase * (1 + installmentInterest); // porcentagem de juros
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
        listProducts();

        if (productArray.size() == 0) // size() retorna o tamanho atual do vector
        {
            cout << "\nNao ha produtos cadastrados para vender!\n";
            cout << "Pressione Enter para voltar...";
            cin.get();
            return;
        }

        cout << "\nDigite o ID do produto (ou 0 para finalizar): ";
        option = readInt();

        // verifica se há produtos no carrinho
        if (option == 0)
        {
            if (!cart.empty())
            {
                system("cls");
                showCart();

                // Menu de confirmação de venda
                cout << "\n[1] Finalizar venda\n[2] Cancelar venda\n[0] Voltar\nEscolha uma opção: ";
                int confirm = readInt();

                if (confirm == 1) // finalizar venda
                {
                    paymentMenu();
                    return;
                }

                if (confirm == 2) // cancelar venda
                {
                    // devolve produtos do carrinho ao estoque
                    for (size_t i = 0; i < cart.size(); i++)
                    {
                        for (size_t j = 0; j < productArray.size(); j++)
                        {
                            if (productArray[j].name == cart[i].name)
                            {
                                productArray[j].quantity += cart[i].quantity; // retorna quantidade ao estoque
                                break;
                            }
                        }
                    }
                    cart.clear();   // limpa o carrinho
                    saveDatabase(); // salva o estoque atualizado no arquivo
                    cout << "\n*** VENDA CANCELADA! ***\nPressione Enter para continuar...";
                    cin.get();
                    return;
                }

                if (confirm == 0) // voltar sem finalizar ou cancelar
                    return;
            }
            else
                return; // se o carrinho estiver vazio, apenas retorna
        }
        // Se o usuário digitou um ID válido de produto
        else if (option > 0 && option <= (int)productArray.size()) // converte size_t para int na comparação
        {
            int index = option - 1; // ID para índice do vector
            cout << "\nProduto selecionado: " << productArray[index].name << "\n";
            cout << "Estoque disponivel: " << productArray[index].quantity << "\n";
            cout << "Preco unitario: R$ " << productArray[index].unitPrice << "\n";
            cout << "Quantidade a comprar: ";

            float quantity = readFloat(); // lê a quantidade desejada

            // quantidade é válida
            if (quantity <= 0 || quantity > productArray[index].quantity)
            {
                cout << "\nQuantidade invalida ou estoque insuficiente!\n";
                cin.get();
                continue;
            }

            // cria um item para o carrinho
            CartItem item;
            item.name = productArray[index].name;
            item.quantity = quantity;
            item.unitPrice = productArray[index].unitPrice;
            item.subtotal = quantity * productArray[index].unitPrice;

            cart.push_back(item);                     // adiciona o item ao final do carrinho com o push_back
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

int main()
{
    system("chcp 65001"); // encoding UTF-8 no Windows
    loadDatabase();
    mainMenu();
    return 0;
}