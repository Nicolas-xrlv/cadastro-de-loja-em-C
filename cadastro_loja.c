#include <stdio.h>      // Biblioteca para entrada e saida de dados
#include <stdlib.h>     // Biblioteca para funcoes gerais
#include <string.h>     // Biblioteca para manipulacao de strings
#include <ctype.h>      // Biblioteca para converter letras maiusculas/minusculas

#define MAX_PRODUTOS 100       // Quantidade maxima de produtos cadastrados
#define ARQUIVO "produtos.txt" // Nome do arquivo principal
#define TEMP "temp.txt"        // Nome do arquivo temporario

// Estrutura que representa um produto da loja
typedef struct {
    int id;                 // Identificador automatico do produto
    char nome[50];          // Nome do produto
    char categoria[50];     // Categoria do produto
    float preco;            // Preco do produto
    int quantidade;         // Quantidade disponivel no estoque
    char fornecedor[50];    // Nome do fornecedor
} Produto;


// Funcao para ler textos com espacos
void lerTexto(char texto[], int tamanho) {
    fgets(texto, tamanho, stdin);             // Le o texto digitado
    texto[strcspn(texto, "\n")] = '\0';       // Remove o ENTER do final
}


// Funcao para ler numeros inteiros
int lerInteiro() {
    char entrada[50];                         // Armazena a entrada digitada
    int numero;                               // Armazena o numero convertido

    fgets(entrada, sizeof(entrada), stdin);   // Le a entrada como texto

    if (sscanf(entrada, "%d", &numero) != 1) {// Verifica se foi digitado um inteiro
        return -1;                            // Retorna -1 se a entrada for invalida
    }

    return numero;                            // Retorna o numero informado
}


// Funcao para ler numeros decimais
float lerFloat() {
    char entrada[50];                         // Armazena a entrada digitada
    float numero;                             // Armazena o valor convertido

    fgets(entrada, sizeof(entrada), stdin);   // Le a entrada como texto

    if (sscanf(entrada, "%f", &numero) != 1) {// Verifica se foi digitado um numero
        return -1;                            // Retorna -1 se a entrada for invalida
    }

    return numero;                            // Retorna o valor informado
}


// Funcao para comparar textos sem diferenciar maiusculas e minusculas
int compararTexto(const char *texto1, const char *texto2) {
    while (*texto1 && *texto2) {              // Percorre os dois textos
        if (tolower((unsigned char)*texto1) !=
            tolower((unsigned char)*texto2)) {// Compara os caracteres convertidos
            return 0;                         // Retorna 0 se forem diferentes
        }

        texto1++;                             // Avanca para o proximo caractere
        texto2++;                             // Avanca para o proximo caractere
    }

    return *texto1 == '\0' && *texto2 == '\0';// Retorna 1 se os textos forem iguais
}


// Funcao para verificar se um ID ja existe
int idExiste(Produto *lista, int quantidade, int id) {
    int i;                                    // Variavel de repeticao

    for (i = 0; i < quantidade; i++) {        // Percorre os produtos cadastrados
        if (lista[i].id == id) {              // Verifica se o ID ja esta sendo usado
            return 1;                         // Retorna 1 se encontrar duplicata
        }
    }

    return 0;                                 // Retorna 0 se o ID nao existir
}


// Funcao para verificar se o nome do produto ja existe
int nomeExiste(Produto *lista, int quantidade, char nome[]) {
    int i;                                    // Variavel de repeticao

    for (i = 0; i < quantidade; i++) {        // Percorre todos os produtos
        if (compararTexto(lista[i].nome, nome)) {// Compara os nomes sem diferenciar letras
            return 1;                         // Retorna 1 se encontrar o mesmo nome
        }
    }

    return 0;                                 // Retorna 0 se nao encontrar duplicatas
}


// Funcao para criar o arquivo com os produtos iniciais
void criarArquivo() {
    FILE *arquivo;                            // Ponteiro para o arquivo

    arquivo = fopen(ARQUIVO, "r");            // Tenta abrir o arquivo para leitura

    if (arquivo != NULL) {                    // Verifica se o arquivo ja existe
        fclose(arquivo);                      // Fecha o arquivo
        printf("\nO cadastro ja existe!\n");  // Informa que nao sera recriado
        return;                               // Encerra a funcao
    }

    arquivo = fopen(ARQUIVO, "w");            // Cria o arquivo para escrita

    if (arquivo == NULL) {                    // Verifica se ocorreu algum erro
        printf("\nErro ao criar o arquivo!\n");
        return;                               // Encerra a funcao
    }

    // Grava os quatro produtos fornecidos no enunciado
    fprintf(arquivo, "1|Camiseta Basica|Vestuario|49.90|30|Moda Brasil\n");
    fprintf(arquivo, "2|Mouse Optico USB|Informatica|59.99|15|TecnoShop\n");
    fprintf(arquivo, "3|Shampoo 300ml|Higiene Pessoal|22.50|40|Beleza Pura\n");
    fprintf(arquivo, "4|Cafe 500g|Alimentos|18.00|25|Graos do Sul\n");

    fclose(arquivo);                          // Fecha o arquivo
    printf("\nCadastro criado com sucesso!\n"); // Mostra a confirmacao
}


// Funcao para carregar os produtos do arquivo para o vetor
int carregarProdutos(Produto *lista) {
    FILE *arquivo;                            // Ponteiro para o arquivo
    char linha[250];                          // Armazena cada linha do arquivo
    int quantidade = 0;                       // Conta os produtos carregados

    arquivo = fopen(ARQUIVO, "r");            // Abre o arquivo para leitura

    if (arquivo == NULL) {                    // Verifica se o arquivo existe
        return -1;                            // Retorna -1 se nao encontrar
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {// Le uma linha por vez
        if (quantidade < MAX_PRODUTOS) {      // Verifica o limite do vetor

            // Separa os campos da linha e armazena na estrutura
            if (sscanf(linha, "%d|%49[^|]|%49[^|]|%f|%d|%49[^\n]",
                &lista[quantidade].id,
                lista[quantidade].nome,
                lista[quantidade].categoria,
                &lista[quantidade].preco,
                &lista[quantidade].quantidade,
                lista[quantidade].fornecedor) == 6) {

                quantidade++;                 // Conta o produto carregado
            }
        }
    }

    fclose(arquivo);                          // Fecha o arquivo
    return quantidade;                        // Retorna a quantidade de produtos
}


// Funcao para exibir os produtos em formato de tabela
void exibirTabela(Produto *lista, int quantidade) {
    int i;                                    // Variavel de repeticao

    printf("\n");
    printf("-----------------------------------------------------------------------------------------------\n");
    printf("| ID | Produto                 | Categoria          | Preco (R$) | Quantidade | Fornecedor     |\n");
    printf("-----------------------------------------------------------------------------------------------\n");

    for (i = 0; i < quantidade; i++) {        // Percorre os produtos
        printf("| %-2d | %-23s | %-18s | %10.2f | %-10d | %-14s |\n",
            lista[i].id,
            lista[i].nome,
            lista[i].categoria,
            lista[i].preco,
            lista[i].quantidade,
            lista[i].fornecedor);
    }

    printf("-----------------------------------------------------------------------------------------------\n");
}


// Funcao para listar os produtos salvos no arquivo
void listarProdutos() {
    Produto lista[MAX_PRODUTOS];              // Vetor que recebe os produtos
    int quantidade;                           // Quantidade de produtos

    quantidade = carregarProdutos(lista);     // Carrega os dados do arquivo

    if (quantidade < 0) {                     // Verifica se o arquivo nao existe
        printf("\nCrie o cadastro primeiro!\n");
        return;                               // Encerra a funcao
    }

    if (quantidade == 0) {                    // Verifica se nao existem produtos
        printf("\nNenhum produto cadastrado!\n");
        return;                               // Encerra a funcao
    }

    exibirTabela(lista, quantidade);          // Exibe os produtos na tela
}


// Funcao para inserir um novo produto
void inserirProduto() {
    Produto lista[MAX_PRODUTOS];              // Vetor de produtos
    Produto novo;                             // Estrutura do novo produto
    FILE *arquivo;                            // Ponteiro para o arquivo
    int quantidade;                           // Quantidade de produtos
    int maiorID = 0;                          // Guarda o maior ID encontrado
    int i;                                    // Variavel de repeticao

    quantidade = carregarProdutos(lista);     // Carrega os produtos existentes

    if (quantidade < 0) {                     // Verifica se o cadastro existe
        printf("\nCrie o cadastro primeiro!\n");
        return;                               // Encerra a funcao
    }

    if (quantidade >= MAX_PRODUTOS) {         // Verifica se o vetor esta cheio
        printf("\nLimite de produtos atingido!\n");
        return;                               // Encerra a funcao
    }

    // Procura o maior ID para gerar o proximo identificador
    for (i = 0; i < quantidade; i++) {
        if (lista[i].id > maiorID) {
            maiorID = lista[i].id;            // Atualiza o maior ID
        }
    }

    novo.id = maiorID + 1;                    // Gera o ID automaticamente

    // Verifica se o ID gerado ja esta cadastrado
    while (idExiste(lista, quantidade, novo.id)) {
        novo.id++;                            // Gera outro ID se houver duplicidade
    }

    printf("\n===== NOVO PRODUTO =====\n");

    printf("Nome: ");
    lerTexto(novo.nome, sizeof(novo.nome));   // Le o nome do produto

    if (strlen(novo.nome) == 0 || strchr(novo.nome, '|') != NULL) {
        printf("\nNome invalido!\n");         // Impede nome vazio ou com separador
        return;
    }

    // Verifica se ja existe produto com o mesmo nome
    if (nomeExiste(lista, quantidade, novo.nome)) {
        printf("\nEsse produto ja esta cadastrado!\n");
        return;                               // Impede a duplicidade
    }

    printf("Categoria: ");
    lerTexto(novo.categoria, sizeof(novo.categoria));

    printf("Preco: ");
    novo.preco = lerFloat();                  // Le o preco

    if (novo.preco < 0) {                     // Verifica se o preco e valido
        printf("\nPreco invalido!\n");
        return;
    }

    printf("Quantidade: ");
    novo.quantidade = lerInteiro();           // Le a quantidade

    if (novo.quantidade < 0) {                // Verifica se a quantidade e valida
        printf("\nQuantidade invalida!\n");
        return;
    }

    printf("Fornecedor: ");
    lerTexto(novo.fornecedor, sizeof(novo.fornecedor));

    // Verifica se os campos de texto possuem o separador do arquivo
    if (strchr(novo.categoria, '|') != NULL ||
        strchr(novo.fornecedor, '|') != NULL ||
        strlen(novo.categoria) == 0 ||
        strlen(novo.fornecedor) == 0) {
        printf("\nCategoria ou fornecedor invalido!\n");
        return;
    }

    arquivo = fopen(ARQUIVO, "a");            // Abre o arquivo para acrescentar dados

    if (arquivo == NULL) {                    // Verifica se o arquivo abriu
        printf("\nErro ao abrir o arquivo!\n");
        return;
    }

    // Grava o novo produto no final do arquivo
    fprintf(arquivo, "%d|%s|%s|%.2f|%d|%s\n",
        novo.id, novo.nome, novo.categoria,
        novo.preco, novo.quantidade, novo.fornecedor);

    fclose(arquivo);                          // Fecha o arquivo

    printf("\nProduto cadastrado com sucesso! ID: %d\n", novo.id);
}


// Funcao para salvar os produtos usando arquivo temporario
int salvarComTemporario(Produto *lista, int quantidade) {
    FILE *temporario;                         // Ponteiro para o arquivo temporario
    int i;                                    // Variavel de repeticao

    temporario = fopen(TEMP, "w");            // Cria o arquivo temporario

    if (temporario == NULL) {                 // Verifica se abriu corretamente
        printf("\nErro ao criar arquivo temporario!\n");
        return 0;                             // Retorna 0 em caso de erro
    }

    // Grava todos os produtos no arquivo temporario
    for (i = 0; i < quantidade; i++) {
        fprintf(temporario, "%d|%s|%s|%.2f|%d|%s\n",
            lista[i].id,
            lista[i].nome,
            lista[i].categoria,
            lista[i].preco,
            lista[i].quantidade,
            lista[i].fornecedor);
    }

    fclose(temporario);                       // Fecha o arquivo temporario

    // Faz uma copia de seguranca do arquivo original
    remove("produtos.bak");                   // Remove backup antigo, se existir

    if (rename(ARQUIVO, "produtos.bak") != 0) {// Renomeia o original para backup
        printf("\nErro ao preparar o arquivo original!\n");
        remove(TEMP);                         // Remove o temporario
        return 0;
    }

    // Coloca o arquivo temporario no lugar do arquivo original
    if (rename(TEMP, ARQUIVO) != 0) {
        rename("produtos.bak", ARQUIVO);      // Tenta restaurar o arquivo original
        printf("\nErro ao substituir o arquivo!\n");
        return 0;
    }

    remove("produtos.bak");                   // Exclui o backup apos o sucesso
    return 1;                                 // Retorna 1 se salvou corretamente
}


// Funcao para alterar um produto pelo ID
void alterarProduto() {
    Produto lista[MAX_PRODUTOS];              // Vetor de produtos
    int quantidade;                           // Quantidade de produtos
    int id;                                   // ID que sera alterado
    int i;                                    // Variavel de repeticao
    int encontrou = 0;                        // Indica se encontrou o produto

    quantidade = carregarProdutos(lista);     // Carrega os produtos do arquivo

    if (quantidade < 0) {
        printf("\nCrie o cadastro primeiro!\n");
        return;
    }

    printf("\nDigite o ID do produto que deseja alterar: ");
    id = lerInteiro();                        // Le o ID informado

    for (i = 0; i < quantidade; i++) {        // Procura o produto pelo ID
        if (lista[i].id == id) {
            encontrou = 1;                    // Marca que encontrou o produto

            printf("\nProduto encontrado: %s\n", lista[i].nome);

            printf("Novo nome: ");
            lerTexto(lista[i].nome, sizeof(lista[i].nome));

            // Verifica se o novo nome ja pertence a outro produto
            {
                int j;
                for (j = 0; j < quantidade; j++) {
                    if (j != i && compararTexto(lista[j].nome, lista[i].nome)) {
                        printf("\nJa existe outro produto com esse nome!\n");
                        return;
                    }
                }
            }

            if (strlen(lista[i].nome) == 0 || strchr(lista[i].nome, '|') != NULL) {
                printf("\nNome invalido!\n");
                return;
            }

            printf("Nova categoria: ");
            lerTexto(lista[i].categoria, sizeof(lista[i].categoria));

            printf("Novo preco: ");
            lista[i].preco = lerFloat();

            if (lista[i].preco < 0) {
                printf("\nPreco invalido!\n");
                return;
            }

            printf("Nova quantidade: ");
            lista[i].quantidade = lerInteiro();

            if (lista[i].quantidade < 0) {
                printf("\nQuantidade invalida!\n");
                return;
            }

            printf("Novo fornecedor: ");
            lerTexto(lista[i].fornecedor, sizeof(lista[i].fornecedor));

            if (strchr(lista[i].categoria, '|') != NULL ||
                strchr(lista[i].fornecedor, '|') != NULL ||
                strlen(lista[i].categoria) == 0 ||
                strlen(lista[i].fornecedor) == 0) {
                printf("\nCategoria ou fornecedor invalido!\n");
                return;
            }

            break;                            // Encerra a busca
        }
    }

    if (!encontrou) {                         // Verifica se o ID foi encontrado
        printf("\nProduto nao encontrado!\n");
        return;
    }

    // Salva as alteracoes usando o arquivo temporario
    if (salvarComTemporario(lista, quantidade)) {
        printf("\nProduto alterado com sucesso!\n");
    }
}


// Funcao para excluir um produto pelo ID
void excluirProduto() {
    Produto lista[MAX_PRODUTOS];              // Vetor de produtos
    int quantidade;                           // Quantidade de produtos
    int id;                                   // ID que sera excluido
    int i, j;                                 // Variaveis de repeticao
    int encontrou = 0;                        // Indica se encontrou o produto

    quantidade = carregarProdutos(lista);     // Carrega os produtos

    if (quantidade < 0) {
        printf("\nCrie o cadastro primeiro!\n");
        return;
    }

    printf("\nDigite o ID do produto que deseja excluir: ");
    id = lerInteiro();                        // Le o ID informado

    for (i = 0; i < quantidade; i++) {        // Percorre os produtos
        if (lista[i].id == id) {              // Verifica o ID
            encontrou = 1;                    // Marca que encontrou

            // Desloca os produtos seguintes uma posicao para a esquerda
            for (j = i; j < quantidade - 1; j++) {
                lista[j] = lista[j + 1];      // Copia o proximo produto
            }

            quantidade--;                     // Diminui a quantidade de produtos
            break;                            // Encerra a busca
        }
    }

    if (!encontrou) {                         // Verifica se encontrou o ID
        printf("\nProduto nao encontrado!\n");
        return;
    }

    // Atualiza o arquivo por meio do arquivo temporario
    if (salvarComTemporario(lista, quantidade)) {
        printf("\nProduto excluido com sucesso!\n");
    }
}


// Funcao para buscar produto pelo nome
void buscarPorNome() {
    Produto lista[MAX_PRODUTOS];              // Vetor de produtos
    int quantidade;                           // Quantidade de produtos
    char nome[50];                            // Nome que sera pesquisado
    int i;                                    // Variavel de repeticao
    int encontrou = 0;                        // Indica se encontrou resultados

    quantidade = carregarProdutos(lista);     // Carrega os produtos

    if (quantidade < 0) {
        printf("\nCrie o cadastro primeiro!\n");
        return;
    }

    printf("\nDigite o nome do produto: ");
    lerTexto(nome, sizeof(nome));             // Le o nome pesquisado

    printf("\nResultados da busca:\n");

    for (i = 0; i < quantidade; i++) {        // Percorre os produtos
        // strstr procura o texto informado dentro do nome
        // A comparacao abaixo aceita o nome completo sem diferenciar letras
        if (compararTexto(lista[i].nome, nome)) {
            exibirTabela(&lista[i], 1);       // Exibe somente o produto encontrado
            encontrou = 1;                    // Marca que encontrou
        }
    }

    if (!encontrou) {
        printf("\nProduto nao encontrado!\n");
    }
}


// Funcao para buscar produtos por categoria
void buscarPorCategoria() {
    Produto lista[MAX_PRODUTOS];              // Vetor de produtos
    int quantidade;                           // Quantidade de produtos
    char categoria[50];                       // Categoria pesquisada
    int i;                                    // Variavel de repeticao
    int encontrou = 0;                        // Indica se encontrou resultados

    quantidade = carregarProdutos(lista);     // Carrega os produtos

    if (quantidade < 0) {
        printf("\nCrie o cadastro primeiro!\n");
        return;
    }

    printf("\nDigite a categoria: ");
    lerTexto(categoria, sizeof(categoria));   // Le a categoria

    printf("\nProdutos da categoria pesquisada:\n");

    for (i = 0; i < quantidade; i++) {        // Percorre todos os produtos
        if (compararTexto(lista[i].categoria, categoria)) {
            exibirTabela(&lista[i], 1);       // Exibe o produto encontrado
            encontrou = 1;                    // Marca que encontrou
        }
    }

    if (!encontrou) {
        printf("\nNenhum produto encontrado nessa categoria!\n");
    }
}


// Funcao para ordenar os produtos usando Bubble Sort
void ordenarProdutos(Produto *lista, int quantidade, int opcao) {
    int i, j;                                 // Variaveis dos lacos
    Produto aux;                              // Estrutura auxiliar para troca

    // Percorre o vetor varias vezes
    for (i = 0; i < quantidade - 1; i++) {

        // Compara os elementos vizinhos
        for (j = 0; j < quantidade - i - 1; j++) {

            int trocar = 0;                   // Indica se sera necessario trocar

            // Opcao 1: nome em ordem crescente (A-Z)
            if (opcao == 1 && strcmp(lista[j].nome, lista[j + 1].nome) > 0) {
                trocar = 1;
            }

            // Opcao 2: nome em ordem decrescente (Z-A)
            if (opcao == 2 && strcmp(lista[j].nome, lista[j + 1].nome) < 0) {
                trocar = 1;
            }

            // Opcao 3: preco do menor para o maior
            if (opcao == 3 && lista[j].preco > lista[j + 1].preco) {
                trocar = 1;
            }

            // Opcao 4: preco do maior para o menor
            if (opcao == 4 && lista[j].preco < lista[j + 1].preco) {
                trocar = 1;
            }

            // Realiza a troca quando os elementos estao fora da ordem
            if (trocar) {
                aux = lista[j];               // Guarda o produto atual
                lista[j] = lista[j + 1];      // Coloca o proximo na posicao atual
                lista[j + 1] = aux;           // Coloca o produto guardado na proxima
            }
        }
    }
}


// Funcao para exibir os produtos ordenados sem alterar o arquivo
void exibirOrdenados(int opcao) {
    Produto lista[MAX_PRODUTOS];              // Vetor original carregado do arquivo
    Produto copia[MAX_PRODUTOS];              // Vetor que sera ordenado
    int quantidade;                           // Quantidade de produtos

    quantidade = carregarProdutos(lista);     // Carrega os dados do arquivo

    if (quantidade < 0) {
        printf("\nCrie o cadastro primeiro!\n");
        return;
    }

    // Copia os produtos para outro vetor
    memcpy(copia, lista, quantidade * sizeof(Produto));

    // Ordena somente a copia, preservando a ordem do arquivo original
    ordenarProdutos(copia, quantidade, opcao);

    // Exibe a tabela ordenada
    exibirTabela(copia, quantidade);
}


// Funcao para exportar os produtos para um arquivo CSV
void exportarCSV() {
    Produto lista[MAX_PRODUTOS];              // Vetor de produtos
    int quantidade;                           // Quantidade de produtos
    int i;                                    // Variavel de repeticao
    FILE *arquivo;                            // Ponteiro para o arquivo CSV

    quantidade = carregarProdutos(lista);     // Carrega os produtos

    if (quantidade < 0) {
        printf("\nCrie o cadastro primeiro!\n");
        return;
    }

    arquivo = fopen("produtos.csv", "w");     // Cria o arquivo CSV

    if (arquivo == NULL) {                    // Verifica se abriu corretamente
        printf("\nErro ao criar o CSV!\n");
        return;
    }

    // Grava o cabecalho do arquivo CSV
    fprintf(arquivo, "ID,Produto,Categoria,Preco,Quantidade,Fornecedor\n");

    // Grava cada produto separado por virgulas
    for (i = 0; i < quantidade; i++) {
        fprintf(arquivo, "%d,\"%s\",\"%s\",%.2f,%d,\"%s\"\n",
            lista[i].id,
            lista[i].nome,
            lista[i].categoria,
            lista[i].preco,
            lista[i].quantidade,
            lista[i].fornecedor);
    }

    fclose(arquivo);                          // Fecha o arquivo CSV
    printf("\nDados exportados para produtos.csv!\n");
}


// Funcao principal do programa
int main() {
    int opcao;                                // Armazena a opcao escolhida

    do {
        // Exibe o menu principal
        printf("\n========== CADASTRO DE LOJA ==========\n");
        printf("1  - Criar cadastro\n");
        printf("2  - Listar produtos\n");
        printf("3  - Inserir produto\n");
        printf("4  - Alterar produto\n");
        printf("5  - Excluir produto\n");
        printf("6  - Buscar por nome\n");
        printf("7  - Buscar por categoria\n");
        printf("8  - Ordenar nome (A-Z)\n");
        printf("9  - Ordenar nome (Z-A)\n");
        printf("10 - Ordenar preco (menor para maior)\n");
        printf("11 - Ordenar preco (maior para menor)\n");
        printf("12 - Exportar para CSV\n");
        printf("0  - Sair\n");
        printf("Escolha uma opcao: ");

        opcao = lerInteiro();                 // Le a opcao escolhida

        // Executa a funcao correspondente a opcao
        switch (opcao) {
            case 1:
                criarArquivo();               // Cria o arquivo inicial
                break;

            case 2:
                listarProdutos();             // Lista todos os produtos
                break;

            case 3:
                inserirProduto();             // Insere um novo produto
                break;

            case 4:
                alterarProduto();             // Altera um produto existente
                break;

            case 5:
                excluirProduto();             // Exclui um produto
                break;

            case 6:
                buscarPorNome();              // Busca pelo nome
                break;

            case 7:
                buscarPorCategoria();         // Busca pela categoria
                break;

            case 8:
                exibirOrdenados(1);           // Ordena nome de A ate Z
                break;

            case 9:
                exibirOrdenados(2);           // Ordena nome de Z ate A
                break;

            case 10:
                exibirOrdenados(3);           // Ordena preco crescente
                break;

            case 11:
                exibirOrdenados(4);           // Ordena preco decrescente
                break;

            case 12:
                exportarCSV();                // Exporta os dados para CSV
                break;

            case 0:
                printf("\nEncerrando o programa...\n");
                break;

            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

    } while (opcao != 0);                     // Repete ate escolher sair

    return 0;                                 // Finaliza o programa
}