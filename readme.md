# 🛒 Bug & Buy - Sistema de Supermercado

## 📌 Sobre o projeto
O **Bug & Buy** é um sistema acadêmico em **C++** para gerenciamento de supermercado.  
Permite cadastrar produtos, vender (inclusive quantidades fracionadas) e gerenciar pagamentos à vista ou parcelados.  
Os dados são salvos em `database.txt` e o sistema retorna ao menu após cada ação.  

---

## ✨ Funcionalidades
- **📦 Cadastro**: até **20 produtos**. Atualiza estoque e preço se já existir.  
- **🛍️ Vendas**: suporta fracionamento (ex.: `1.5 kg`), carrinho dinâmico, cancelamento e finalização.  
- **💳 Pagamentos**:  
  - À vista: **5% desconto**  
  - Até **3x sem juros**  
  - Até **12x com 10% de juros**, mostrando datas futuras das parcelas  
- **📋 Listagem**: mostra ID, nome, quantidade e preço.  
- **💾 Persistência**: dados salvos e carregados de `database.txt`.  

---

## ⚙️ Tecnologias
- **C++ (g++)**
- Arquivos `.txt` com `fstream`
- **Structs** e **vector**
- **ctime** (datas) e **iomanip** (formatação)
