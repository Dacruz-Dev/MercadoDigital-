#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct telefone {
    char movel[16];
    char fixo[15];
} Telefone;

typedef struct clientes {
    int codigo;
    char nome[50];
    double cpf;
    char sexo;
    Telefone tel;
} Clientes;

typedef struct modelos {
    char marca[20];
    char modelo[20];
} Modelos;

typedef struct produtos {
    int codigo;
    char nome[20];
    double valor;
    Modelos modelo;
} Produtos;

typedef struct carrinhos {
    Produtos produto[3];
    int quantidade[3];
    double subtotal;
    double desconto;
    double total;
} Carrinhos;

typedef struct pagamento {
    char metodo[20];
    double valor;
} Pagamento;

void adicionarProduto(Produtos prod[], int *qnt);
void mostrarProdutos(Produtos prod[], int qnt);
void adicionarAoCarrinho(Carrinhos *carrinho, Produtos prod[], int qnt);
void mostrarCarrinho(Carrinhos carrinho);
void finalizarCompra(Carrinhos *carrinho);
void escolherPagamento(Pagamento *pagamento, double total);

int main() {
    Produtos produtos[3] = {
        {1, "Notebook", 3500.00, {"Dell", "Inspiron"}},
        {2, "Smartphone", 1500.00, {"Apple", "iPhone"}},
        {3, "Tablet", 800.00, {"Samsung", "Galaxy Tab"}}
    };
    Carrinhos carrinho = {{{0}}, {0}, 0, 0, 0};
    Clientes cliente = {1, "João Silva", 12345678901, 'M', {"(43)99999-9999", "(43)3333-3333"}};
    int qtdProdutos = 3, opcao;

    do {
        printf("1 - Adicionar Produto\n");
        printf("2 - Mostrar Produtos\n");
        printf("3 - Adicionar ao Carrinho\n");
        printf("4 - Mostrar Carrinho\n");
        printf("5 - Finalizar Compra\n");
        printf("6 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                adicionarProduto(produtos, &qtdProdutos);
                break;
            case 2:
                mostrarProdutos(produtos, qtdProdutos);
                break;
            case 3:
                adicionarAoCarrinho(&carrinho, produtos, qtdProdutos);
                break;
            case 4:
                mostrarCarrinho(carrinho);
                break;
            case 5:
                finalizarCompra(&carrinho);
                break;
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while(opcao != 6);

    return 0;
}

void adicionarProduto(Produtos prod[], int *qnt) {
    if (*qnt >= 3) {
        printf("Número máximo de produtos atingido. Não é possível adicionar mais produtos.\n");
        return;
    }

    prod[*qnt].codigo = *qnt + 1;
    printf("Digite o nome do produto: ");
    scanf(" %[^\n]", prod[*qnt].nome);
    printf("Digite o valor do produto: ");
    scanf("%lf", &prod[*qnt].valor);
    printf("Digite a marca do produto: ");
    scanf(" %[^\n]", prod[*qnt].modelo.marca);
    printf("Digite o modelo do produto: ");
    scanf(" %[^\n]", prod[*qnt].modelo.modelo);

    (*qnt)++;
}

void mostrarProdutos(Produtos prod[], int qnt) {
    printf("Lista de Produtos:\n");
    for (int i = 0; i < qnt; i++) {
        printf("Código: %d\n", prod[i].codigo);
        printf("Nome: %s\n", prod[i].nome);
        printf("Marca: %s\n", prod[i].modelo.marca);
        printf("Modelo: %s\n", prod[i].modelo.modelo);
        printf("Valor: %.2f\n", prod[i].valor);
        printf("-----------------------\n");
    }
}

void adicionarAoCarrinho(Carrinhos *carrinho, Produtos prod[], int qnt) {
    int codigoProduto, quantidade;
    printf("Digite o código do produto para adicionar ao carrinho: ");
    scanf("%d", &codigoProduto);

    if (codigoProduto < 1 || codigoProduto > qnt) {
        printf("Código de produto inválido.\n");
        return;
    }

    printf("Digite a quantidade do produto: ");
    scanf("%d", &quantidade);

    if (quantidade <= 0) {
        printf("Quantidade inválida.\n");
        return;
    }

    carrinho->produto[codigoProduto - 1] = prod[codigoProduto - 1];
    carrinho->quantidade[codigoProduto - 1] += quantidade;
    printf("Produto adicionado ao carrinho.\n");
}

void mostrarCarrinho(Carrinhos carrinho) {
    printf("Itens no Carrinho:\n");
    int vazio = 1;
    for (int i = 0; i < 3; i++) {
        if (carrinho.quantidade[i] > 0) {
            vazio = 0;
            printf("Produto: %s\n", carrinho.produto[i].nome);
            printf("Quantidade: %d\n", carrinho.quantidade[i]);
            printf("Valor Unitário: %.2f\n", carrinho.produto[i].valor);
            printf("Subtotal: %.2f\n", carrinho.quantidade[i] * carrinho.produto[i].valor);
            printf("-----------------------\n");
        }
    }
    if (vazio) {
        printf("O carrinho está vazio.\n");
    }
}

void escolherPagamento(Pagamento *pagamento, double total) {
    printf("Escolha o método de pagamento:\n");
    printf("1 - Cartão de Crédito\n");
    printf("2 - PayPal\n");
    printf("3 - Dinheiro\n");
    int opcao;
    scanf("%d", &opcao);

    switch(opcao) {
        case 1:
            strcpy(pagamento->metodo, "Cartão de Crédito");
            break;
        case 2:
            strcpy(pagamento->metodo, "PayPal");
            break;
        case 3:
            strcpy(pagamento->metodo, "Dinheiro");
            break;
        default:
            printf("Método de pagamento inválido. Usando Dinheiro como padrão.\n");
            strcpy(pagamento->metodo, "Dinheiro");
    }

    pagamento->valor = total;
    printf("Método de pagamento escolhido: %s\n", pagamento->metodo);
}

void finalizarCompra(Carrinhos *carrinho) {
    carrinho->subtotal = 0;
    for (int i = 0; i < 3; i++) {
        carrinho->subtotal += carrinho->quantidade[i] * carrinho->produto[i].valor;
    }

    printf("Subtotal: %.2f\n", carrinho->subtotal);

    printf("Digite o valor do desconto (em %%): ");
    scanf("%lf", &carrinho->desconto);

    if (carrinho->desconto < 0) {
        printf("Desconto inválido. O desconto deve ser maior ou igual a 0.\n");
        carrinho->desconto = 0;
    }

    carrinho->total = carrinho->subtotal - (carrinho->subtotal * (carrinho->desconto / 100));
    printf("Total após desconto: %.2f\n", carrinho->total);

    Pagamento pagamento;
    escolherPagamento(&pagamento, carrinho->total);

    double valorPago;
    printf("Digite o valor pago: ");
    scanf("%lf", &valorPago);

    if (valorPago < carrinho->total) {
        printf("Valor pago insuficiente. A compra não pode ser finalizada.\n");
        return;
    }

    double troco = valorPago - carrinho->total;
    printf("Troco: %.2f\n", troco);
    printf("Compra finalizada com sucesso!\n");

    memset(carrinho, 0, sizeof(Carrinhos));
    printf("Deseja comprar novamente? (1 - Sim, 0 - Não): ");
    int opcao;
    scanf("%d", &opcao);
    if (opcao == 1) {
        printf("Carrinho resetado. Você pode começar a comprar novamente.\n");
    } else {
        printf("Obrigado pela compra!\n");
    }
}
