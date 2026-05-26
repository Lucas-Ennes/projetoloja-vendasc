#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 10

// ======================= STRUCTS =======================

struct Produto {
    int idProduto;
    char nome[50];
    float preco;
    int quantidade;
};

struct Venda {
    int idVenda;
    char cliente[50];
    int qtdProdutos;
    int idsProdutos[MAX_ITENS];
};

// ======================= FUNÇÕES PRODUTOS =======================

void cadastrarProduto() {

    FILE *fp = fopen("produtos.dat", "ab");

    if (fp == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    struct Produto p;

    printf("\nID do Produto: ");
    scanf("%d", &p.idProduto);

    getchar();

    printf("Nome do Produto: ");
    fgets(p.nome, 50, stdin);
    p.nome[strcspn(p.nome, "\n")] = '\0';

    printf("Preco: ");
    scanf("%f", &p.preco);

    printf("Quantidade em estoque: ");
    scanf("%d", &p.quantidade);

    fwrite(&p, sizeof(struct Produto), 1, fp);

    fclose(fp);

    printf("Produto cadastrado com sucesso!\n");
}

void listarProdutos() {

    FILE *fp = fopen("produtos.dat", "rb");

    if (fp == NULL) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    struct Produto p;

    printf("\n===== PRODUTOS =====\n");

    while (fread(&p, sizeof(struct Produto), 1, fp)) {

        printf("\nID: %d\n", p.idProduto);
        printf("Nome: %s\n", p.nome);
        printf("Preco: %.2f\n", p.preco);
        printf("Quantidade: %d\n", p.quantidade);
    }

    fclose(fp);
}

void buscarProdutoPorID(int id, struct Produto *produto, int *encontrado) {

    FILE *fp = fopen("produtos.dat", "rb");

    *encontrado = 0;

    if (fp == NULL) {
        return;
    }

    struct Produto p;

    while (fread(&p, sizeof(struct Produto), 1, fp)) {

        if (p.idProduto == id) {

            *produto = p;
            *encontrado = 1;
            break;
        }
    }

    fclose(fp);
}

void atualizarProduto() {

    FILE *fp = fopen("produtos.dat", "rb+");

    if (fp == NULL) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    int id;
    struct Produto p;

    printf("Digite o ID do produto: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(struct Produto), 1, fp)) {

        if (p.idProduto == id) {

            getchar();

            printf("Novo nome: ");
            fgets(p.nome, 50, stdin);
            p.nome[strcspn(p.nome, "\n")] = '\0';

            printf("Novo preco: ");
            scanf("%f", &p.preco);

            printf("Nova quantidade: ");
            scanf("%d", &p.quantidade);

            fseek(fp, -sizeof(struct Produto), SEEK_CUR);

            fwrite(&p, sizeof(struct Produto), 1, fp);

            printf("Produto atualizado!\n");

            fclose(fp);
            return;
        }
    }

    printf("Produto nao encontrado.\n");

    fclose(fp);
}

void removerProduto() {

    FILE *fp = fopen("produtos.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("Erro nos arquivos.\n");
        return;
    }

    int id;
    struct Produto p;

    printf("Digite o ID do produto para remover: ");
    scanf("%d", &id);

    while (fread(&p, sizeof(struct Produto), 1, fp)) {

        if (p.idProduto != id) {

            fwrite(&p, sizeof(struct Produto), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("produtos.dat");
    rename("temp.dat", "produtos.dat");

    printf("Produto removido!\n");
}

// ======================= FUNÇÕES VENDAS =======================

void cadastrarVenda() {

    FILE *fv = fopen("vendas.dat", "ab");

    if (fv == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    struct Venda v;

    int i;

    printf("\nID da Venda: ");
    scanf("%d", &v.idVenda);

    getchar();

    printf("Nome do Cliente: ");
    fgets(v.cliente, 50, stdin);
    v.cliente[strcspn(v.cliente, "\n")] = '\0';

    printf("Quantidade de produtos da venda: ");
    scanf("%d", &v.qtdProdutos);

    if (v.qtdProdutos > MAX_ITENS) {

        printf("Limite maximo excedido.\n");
        fclose(fv);
        return;
    }

    for (i = 0; i < v.qtdProdutos; i++) {

        printf("ID do produto %d: ", i + 1);
        scanf("%d", &v.idsProdutos[i]);
    }

    fwrite(&v, sizeof(struct Venda), 1, fv);

    fclose(fv);

    printf("Venda cadastrada!\n");
}

void listarVendas() {

    FILE *fv = fopen("vendas.dat", "rb");

    if (fv == NULL) {
        printf("Nenhuma venda cadastrada.\n");
        return;
    }

    struct Venda v;
    struct Produto p;

    int encontrado;
    int i;

    printf("\n===== VENDAS =====\n");

    while (fread(&v, sizeof(struct Venda), 1, fv)) {

        printf("\nID Venda: %d\n", v.idVenda);
        printf("Cliente: %s\n", v.cliente);

        printf("Produtos:\n");

        for (i = 0; i < v.qtdProdutos; i++) {

            buscarProdutoPorID(v.idsProdutos[i], &p, &encontrado);

            if (encontrado) {

                printf("- %s (ID %d)\n", p.nome, p.idProduto);

            } else {

                printf("- Produto ID %d nao encontrado\n", v.idsProdutos[i]);
            }
        }
    }

    fclose(fv);
}

void atualizarVenda() {

    FILE *fv = fopen("vendas.dat", "rb+");

    if (fv == NULL) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    int id;
    int i;

    struct Venda v;

    printf("Digite o ID da venda: ");
    scanf("%d", &id);

    while (fread(&v, sizeof(struct Venda), 1, fv)) {

        if (v.idVenda == id) {

            getchar();

            printf("Novo nome do cliente: ");
            fgets(v.cliente, 50, stdin);
            v.cliente[strcspn(v.cliente, "\n")] = '\0';

            printf("Nova quantidade de produtos: ");
            scanf("%d", &v.qtdProdutos);

            for (i = 0; i < v.qtdProdutos; i++) {

                printf("ID do produto %d: ", i + 1);
                scanf("%d", &v.idsProdutos[i]);
            }

            fseek(fv, -sizeof(struct Venda), SEEK_CUR);

            fwrite(&v, sizeof(struct Venda), 1, fv);

            printf("Venda atualizada!\n");

            fclose(fv);
            return;
        }
    }

    printf("Venda nao encontrada.\n");

    fclose(fv);
}

void removerVenda() {

    FILE *fv = fopen("vendas.dat", "rb");
    FILE *temp = fopen("tempVenda.dat", "wb");

    if (fv == NULL || temp == NULL) {
        printf("Erro nos arquivos.\n");
        return;
    }

    int id;

    struct Venda v;

    printf("Digite o ID da venda para remover: ");
    scanf("%d", &id);

    while (fread(&v, sizeof(struct Venda), 1, fv)) {

        if (v.idVenda != id) {

            fwrite(&v, sizeof(struct Venda), 1, temp);
        }
    }

    fclose(fv);
    fclose(temp);

    remove("vendas.dat");
    rename("tempVenda.dat", "vendas.dat");

    printf("Venda removida!\n");
}

// ======================= MENU =======================

int main() {

    int opcao;

    do {

        printf("\n========== SISTEMA LOJA / ESTOQUE ==========\n");

        printf("\n1 - Cadastrar Produto");
        printf("\n2 - Listar Produtos");
        printf("\n3 - Atualizar Produto");
        printf("\n4 - Remover Produto");

        printf("\n5 - Cadastrar Venda");
        printf("\n6 - Listar Vendas");
        printf("\n7 - Atualizar Venda");
        printf("\n8 - Remover Venda");

        printf("\n0 - Sair");

        printf("\n\nEscolha: ");
        scanf("%d", &opcao);

        switch(opcao) {

            case 1:
                cadastrarProduto();
                break;

            case 2:
                listarProdutos();
                break;

            case 3:
                atualizarProduto();
                break;

            case 4:
                removerProduto();
                break;

            case 5:
                cadastrarVenda();
                break;

            case 6:
                listarVendas();
                break;

            case 7:
                atualizarVenda();
                break;

            case 8:
                removerVenda();
                break;

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while(opcao != 0);

    return 0;
}
