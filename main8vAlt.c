#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define TAM 50
#define TAM_INDETERMINADO 5000
#define ARQUIVO_USUARIOS "usuarios.txt"
#define ARQUIVO_MOEDAS "moedas.txt"
int usuario_log; 
int menu();      // protótipo da func

void DataHora(char *buffer)
{
    struct tm *data_hora_atual;
    time_t segundos;
    time(&segundos);
    data_hora_atual = localtime(&segundos);

    sprintf(buffer, "%02d-%02d-%04d %02d:%02d:%02d",
            data_hora_atual->tm_mday,
            data_hora_atual->tm_mon + 1,
            data_hora_atual->tm_year + 1900,
            data_hora_atual->tm_hour,
            data_hora_atual->tm_min,
            data_hora_atual->tm_sec);
}

typedef struct
{
    char cpf[TAM];
    char senha[TAM];
    char nome[TAM];
    double saldoReal;
    double saldoBit;
    double saldoEthereum;
    double saldoRipple;
    char extrato[TAM_INDETERMINADO];
} Usuario;
Usuario *usuarios = NULL;
int num_usuarios = 0;

typedef struct  
{
    char nomeMoeda[TAM];
    double cotacaoMoeda;
    double taxaVenda;
    double taxaCompra;

} Moeda;
Moeda *moeda = NULL;
int num_moedas = 0;

void clearbuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int carregarCotacoes() {
    FILE *arquivo = fopen(ARQUIVO_MOEDAS, "rb");
    if (arquivo == NULL) {
        printf("Arquivo de moedas não encontrado. Inicializando cotações padrão.\n");

        // Alocar memória para as moedas padrão
        num_moedas = 3;  // Três moedas padrão
        moeda = malloc(num_moedas * sizeof(Moeda));
        if (!moeda) {
            printf("Erro ao alocar memória para moedas.\n");
            return -1;
        }

        // Definir as cotações padrões
        strcpy(moeda[0].nomeMoeda, "Bitcoin");
        moeda[0].cotacaoMoeda = 350925.72;
        moeda[0].taxaCompra = 0.02;
        moeda[0].taxaVenda = 0.01;

        strcpy(moeda[1].nomeMoeda, "Ethereum");
        moeda[1].cotacaoMoeda = 12980.41;
        moeda[1].taxaCompra = 0.025;
        moeda[1].taxaVenda = 0.015;

        strcpy(moeda[2].nomeMoeda, "Ripple");
        moeda[2].cotacaoMoeda = 3.26;
        moeda[2].taxaCompra = 0.03;
        moeda[2].taxaVenda = 0.02;

        return 0;
    }

    // Libera a memória antiga, se necessário
    if (moeda != NULL) {
        free(moeda);
    }

    // Lê o número de moedas
    fread(&num_moedas, sizeof(int), 1, arquivo);

    // Aloca memória para as moedas
    moeda = malloc(num_moedas * sizeof(Moeda));
    if (!moeda) {
        fclose(arquivo);
        printf("Erro ao alocar memória para moedas.\n");
        return -1;
    }

    // Lê as moedas do arquivo
    fread(moeda, sizeof(Moeda), num_moedas, arquivo);
    fclose(arquivo);

    return 1;
}

int salvarCotacoes() {
    FILE *arquivo = fopen(ARQUIVO_MOEDAS, "wb");
    if (arquivo == NULL) {
        printf("Erro ao salvar as cotações no arquivo.\n");
        return -1;
    }

    fwrite(&num_moedas, sizeof(int), 1, arquivo);
    fwrite(moeda, sizeof(Moeda), num_moedas, arquivo);
    fclose(arquivo);
    return 1;
}

int salvarUsuarios()
{
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "wb");
    if (arquivo == NULL)
    {
        return -1;
    }

    fwrite(&num_usuarios, sizeof(int), 1, arquivo);

    for (int i = 0; i < num_usuarios; i++)
    {
        fwrite(&usuarios[i], sizeof(Usuario), 1, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int carregarUsuarios()
{
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "rb");
    if (arquivo == NULL)
    {
        return -1;
    }

    fread(&num_usuarios, sizeof(int), 1, arquivo);

    usuarios = malloc(num_usuarios * sizeof(Usuario));

    for (int i = 0; i < num_usuarios; i++)
    {
        fread(&usuarios[i], sizeof(Usuario), 1, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int cadastro()      
{
    if (num_usuarios >= 10)
    {
        printf("Não e possivel cadastrar mais usuarios! (Limite de 10 usuarios atingido)\n");
        return -1;
    }

    int CPFjaexiste = 0;
    usuarios = realloc(usuarios, (num_usuarios + 1) * sizeof(Usuario));
    if (usuarios == NULL)
    {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    printf("Digite o nome: ");

    fgets(usuarios[num_usuarios].nome, TAM, stdin);
    usuarios[num_usuarios].nome[strcspn(usuarios[num_usuarios].nome, "\n")] = '\0';

    printf("Digite seu CPF: ");
    fgets(usuarios[num_usuarios].cpf, TAM, stdin);
    usuarios[num_usuarios].cpf[strcspn(usuarios[num_usuarios].cpf, "\n")] = '\0';

    for (int i = 0; i < num_usuarios; i++)
    {
        if (strcmp(usuarios[i].cpf, usuarios[num_usuarios].cpf) == 0)
        {
            CPFjaexiste = 1;
            break;
        }
    }

    while (strlen(usuarios[num_usuarios].cpf) != 11 || CPFjaexiste == 1)
    {
        if (strlen(usuarios[num_usuarios].cpf) != 11)
        {
            printf("=> O CPF deve conter 11 digitos!\n");
        }
        else if (CPFjaexiste == 1)
        {
            printf("=> Este CPF ja esta cadastrado!\n");
        }

        printf("Digite novamente seu CPF: ");
        fgets(usuarios[num_usuarios].cpf, TAM, stdin);
        usuarios[num_usuarios].cpf[strcspn(usuarios[num_usuarios].cpf, "\n")] = '\0';

        for (int i = 0; i < num_usuarios; i++)
        {
            if (strcmp(usuarios[i].cpf, usuarios[num_usuarios].cpf) == 0)
            {
                CPFjaexiste = 1;
                break;
            }
            else
            {
                CPFjaexiste = 0;
            }
        }
    }

    printf("Digite a senha: ");
    fgets(usuarios[num_usuarios].senha, TAM, stdin);
    while (strlen(usuarios[num_usuarios].senha) < 6)
    {
        printf("Digite uma senha superior a 6 digitos ou letras!\n");
        printf("Digite novamente sua senha: ");
        fgets(usuarios[num_usuarios].senha, TAM, stdin);
    }
    usuarios[num_usuarios].senha[strcspn(usuarios[num_usuarios].senha, "\n")] = '\0';

    usuarios[num_usuarios].saldoReal = 0;
    usuarios[num_usuarios].saldoBit = 0;
    usuarios[num_usuarios].saldoEthereum = 0;
    usuarios[num_usuarios].saldoRipple = 0;

    strcpy(usuarios[num_usuarios].extrato, "Nova conta sem transacoes ate o momento.");

    num_usuarios++;
    printf("\nUsuario cadastrado com sucesso!\n");

    salvarUsuarios();
    return 0;
}

int login()
{
    carregarUsuarios();
    char cpf[TAM];
    char senha[TAM];
    int opcao;
    int encontrado = 0;

    while (encontrado == 0 || opcao == 0)
    {
        printf("\n::::: Bem vindo(a) ao CriptoBank :::::\n");
        printf("Gostaria de realizar login ou cadastro?\n");
        printf("1. Cadastrar Usuario\n");
        printf("2. Login\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        clearbuffer();

        switch (opcao)
        {
        case 1:
            cadastro();
            break;
        case 2:
            printf("Digite o CPF: ");
            fgets(cpf, TAM, stdin);
            cpf[strcspn(cpf, "\n")] = '\0';

            printf("Digite a senha: ");
            fgets(senha, TAM, stdin);
            senha[strcspn(senha, "\n")] = '\0';

            for (int i = 0; i < num_usuarios; i++)
            {
                if (strcmp(usuarios[i].cpf, cpf) == 0 && strcmp(usuarios[i].senha, senha) == 0)
                {
                    printf("\nLogin realizado com sucesso! Bem-vindo(a), %s!\n", usuarios[i].nome);
                    encontrado = 1;
                    usuario_log = i;
                    break;
                }
            }

            if (!encontrado)
            {
                printf("\nCPF ou senha incorretos.\n");
            }
            break;

        case 0:
            printf("Encerrando o programa...\n");
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("Opcao invalida.\n");
            break;
        }
    }

    return 0;
}

double geracotacao()
{
    double cotacao = (rand() % 101 - 50) / 1000.0;
    return cotacao;
}

// Opções do Menu 🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻🡻

int op_saldo()
{
    carregarUsuarios();
    puts("\n::::: Saldo :::::\n");
    printf("=> Usuario: %s\n", usuarios[usuario_log].nome);
    printf("=> CPF: %s\n", usuarios[usuario_log].cpf);
    printf("\n");
    printf("Reais: %.2lf\n", usuarios[usuario_log].saldoReal);
    printf("Bitcoin: %.5lf\n", usuarios[usuario_log].saldoBit);
    printf("Ethereum: %.5lf\n", usuarios[usuario_log].saldoEthereum);
    printf("Ripple: %.3lf\n", usuarios[usuario_log].saldoRipple);

    menu();
    return 0;
}

int op_extrato()        
{
    carregarUsuarios(); 
    puts("\n::::: Extrato :::::\n");

    char *operacoes[300];
    int totalop = 0;
    char *extrato_copia = strdup(usuarios[usuario_log].extrato);
    char *token = strtok(extrato_copia, "/");

    while (token != NULL) 
    {
        operacoes[totalop] = token;
        totalop++;
        token = strtok(NULL, "/");
    }

    for (int i = 0; i < totalop; i++)
    {
        if(i>100){
            printf("Limite de operacoes atingido\n");
            return -1;
        }
        printf("%s\n", operacoes[i]);
    }

    free(extrato_copia);
    menu();
    return 0;
}

int op_deposito()
{
    double reais;
    char dataHora[50]; 

    puts("\n::::: Deposito :::::\n");
    printf("Informe quantos reais deseja depositar: ");
    scanf("%lf", &reais);

    usuarios[usuario_log].saldoReal += reais;

    printf("Voce depositou R$ %.2lf!\n", reais);
    printf("Seu novo saldo em reais: R$ %.2lf\n", usuarios[usuario_log].saldoReal);

    DataHora(dataHora);

    char lin_extrato[200];
    sprintf(lin_extrato, "/%-19s - Deposito R$ %.2lf || Saldo: %.2lf", dataHora, reais, usuarios[usuario_log].saldoReal);

    if (strcmp(usuarios[usuario_log].extrato, "Nova conta sem transacoes ate o momento.") == 0)
    {
        strcpy(usuarios[usuario_log].extrato, lin_extrato);
    }
    else
    {
        strcat(usuarios[usuario_log].extrato, lin_extrato);
    }

    salvarUsuarios();

    menu();
    return 0;
}

int op_saque()
{
    double reais;
    char ver_senha[TAM];
    char dataHora[50];

    puts("\n::::: Saque :::::\n");
    printf("Digite sua senha: ");
    fgets(ver_senha, TAM, stdin);
    ver_senha[strcspn(ver_senha, "\n")] = '\0';

    while (strcmp(ver_senha, usuarios[usuario_log].senha) != 0)
    {
        printf("Senha incorreta. Tente novamente: ");
        fgets(ver_senha, TAM, stdin);
        ver_senha[strcspn(ver_senha, "\n")] = '\0';
    }

    printf("Informe quantos reais deseja sacar: ");
    scanf("%lf", &reais);

    while (reais > usuarios[usuario_log].saldoReal)
    {
        printf("Saldo insuficiente!\n");
        printf("Informe quantos reais deseja sacar: ");
        scanf("%lf", &reais);
    }

    usuarios[usuario_log].saldoReal -= reais;

    printf("Voce sacou R$ %.2lf!\n", reais);
    printf("Seu novo saldo em reais e: R$ %.2lf\n", usuarios[usuario_log].saldoReal);

    DataHora(dataHora);

    char lin_extrato[200];
    sprintf(lin_extrato, "/%-19s - Saque R$ %.2lf || Saldo: %.2lf", dataHora, reais, usuarios[usuario_log].saldoReal);

    if (strcmp(usuarios[usuario_log].extrato, "Nova conta sem transacoes ate o momento.") == 0)
    {
        strcpy(usuarios[usuario_log].extrato, lin_extrato);
    }
    else
    {
        strcat(usuarios[usuario_log].extrato, lin_extrato);
    }
    salvarUsuarios();

    menu();
    return 0;
}

int op_ComprarMoeda() {
    carregarUsuarios();
    carregarCotacoes();

    char ver_senha[TAM];
    double investimento;
    int opcao;
    char dataHora[50];

    puts("\n::::: Comprar criptomoeda :::::\n");

    // Verifica senha
    printf("Digite sua senha: ");
    fgets(ver_senha, TAM, stdin);
    ver_senha[strcspn(ver_senha, "\n")] = '\0';

    while (strcmp(ver_senha, usuarios[usuario_log].senha) != 0) {
        printf("Senha incorreta. Digite novamente: ");
        fgets(ver_senha, TAM, stdin);
        ver_senha[strcspn(ver_senha, "\n")] = '\0';
    }

    // Lista as moedas disponíveis
    printf("\nCriptomoedas disponíveis:\n");
    for (int i = 0; i < num_moedas; i++) {
        printf("%d. %s (Cotacao: R$ %.2f, Taxa: %.2f%%)\n",
               i + 1,
               moeda[i].nomeMoeda,
               moeda[i].cotacaoMoeda,
               moeda[i].taxaCompra * 100);
    }

    // Escolha da moeda
    do {
        printf("Informe o número da moeda que deseja comprar: ");
        if (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > num_moedas) {
            printf("Opção inválida.\n");
            clearbuffer();
        } else {
            break;
        }
    } while (1);
    clearbuffer();

    // Moeda selecionada
    int idx_moeda = opcao - 1;
    printf("Você selecionou: %s\n", moeda[idx_moeda].nomeMoeda);
    printf("Cotação atual: R$ %.2f\n", moeda[idx_moeda].cotacaoMoeda);
    printf("*Compra sujeita a taxa de %.2f%%\n", moeda[idx_moeda].taxaCompra * 100);

    // Valor do investimento
    do {
        printf("Digite o valor a investir: ");
        if (scanf("%lf", &investimento) != 1 || investimento <= 0) {
            printf("Entrada inválida. Tente novamente.\n");
            clearbuffer();
        } else if (investimento > usuarios[usuario_log].saldoReal) {
            printf("Saldo insuficiente! Seu saldo atual é R$ %.2f.\n", usuarios[usuario_log].saldoReal);
        } else {
            break;
        }
    } while (1);
    clearbuffer();

    // Processa a compra
    double taxa = investimento * moeda[idx_moeda].taxaCompra;
    double valor_liquido = investimento - taxa;
    double qtd_comprada = valor_liquido / moeda[idx_moeda].cotacaoMoeda;

    usuarios[usuario_log].saldoReal -= investimento;

    // Atualiza o saldo da criptomoeda correspondente
    if (strcmp(moeda[idx_moeda].nomeMoeda, "Bitcoin") == 0) {
        usuarios[usuario_log].saldoBit += qtd_comprada;
    } else if (strcmp(moeda[idx_moeda].nomeMoeda, "Ethereum") == 0) {
        usuarios[usuario_log].saldoEthereum += qtd_comprada;
    } else if (strcmp(moeda[idx_moeda].nomeMoeda, "Ripple") == 0) {
        usuarios[usuario_log].saldoRipple += qtd_comprada;
    }

    printf("Você comprou %.5lf %s!\n", qtd_comprada, moeda[idx_moeda].nomeMoeda);
    printf("Taxa aplicada: R$ %.2f\n", taxa);
    printf("Saldo restante: R$ %.2f\n", usuarios[usuario_log].saldoReal);

    // Registra no extrato
    DataHora(dataHora);
    char lin_extrato[200];
    sprintf(lin_extrato, "/%-19s - Compra %s %.5lf || Saldo: R$ %.2f || Taxa: R$ %.2f\n",
            dataHora,
            moeda[idx_moeda].nomeMoeda,
            qtd_comprada,
            usuarios[usuario_log].saldoReal,
            taxa);

    if (strcmp(usuarios[usuario_log].extrato, "Nova conta sem transacoes ate o momento.") == 0) {
        strcpy(usuarios[usuario_log].extrato, lin_extrato);
    } else {
        strcat(usuarios[usuario_log].extrato, lin_extrato);
    }

    salvarUsuarios();
    return 0;
}

int op_Vendemoeda()
{
    carregarUsuarios();
    carregarCotacoes();
    char ver_senha[TAM];
    char opcao[1];
    double venda;
    char dataHora[50]; 

    puts("\n::::: Vender criptomoeda :::::\n");

    printf("Digite sua senha: ");
    fgets(ver_senha, TAM, stdin);
    ver_senha[strcspn(ver_senha, "\n")] = '\0';

    while (strcmp(ver_senha, usuarios[usuario_log].senha) != 0)
    {
        printf("Digite sua senha novamente: ");
        fgets(ver_senha, TAM, stdin);
        ver_senha[strcspn(ver_senha, "\n")] = '\0';
        if (strcmp(ver_senha, usuarios[usuario_log].senha) == 0)
        {
            break;
        }
    }

    puts("\n::::: Vender criptomoeda :::::\n");

    puts("1. Bitcoin\n");
    puts("2. Ethereum\n");
    puts("3. Ripple\n");
    printf("Informe qual moeda deseja vender: ");
    scanf("%s", opcao);
    clearbuffer();
    int valor = atoi(opcao);

    while (valor != 1 && valor != 2 && valor != 3)
    {
        printf("Valor inválido, insira novamente: ");
        scanf("%s", opcao);
        clearbuffer();
        opcao[strcspn(opcao, "\n")] = '\0';

        valor = atoi(opcao);
    }

    DataHora(dataHora); 

    switch (valor)
    {
    case 1:
        // Vendendo BTC 🡻🡻🡻
        printf("Cotacao atual do Bitcoin: R$ %.2f\n", moeda[0].cotacaoMoeda);
        printf("*Venda sujeita a taxa de 3%%\n");
        printf("Vender BTC: ");
        scanf("%lf", &venda);

        while (venda > usuarios[usuario_log].saldoBit)
        {
            printf("Saldo insuficiente!\n");
            printf("Vender BTC: ");
            scanf("%lf", &venda);
        }

        usuarios[usuario_log].saldoBit -= venda;

        double cambioBit = venda * moeda[0].cotacaoMoeda;
        double venda_taxaBit = cambioBit * 0.03;
        cambioBit -= venda_taxaBit;

        usuarios[usuario_log].saldoReal += cambioBit;

        printf("Voce vendeu R$ %.2lf em BitCoin!\n", cambioBit);
        printf("Taxa aplicada de R$ %.2lf\n", venda_taxaBit);
        printf("Seu novo saldo: R$ %.2lf\n", usuarios[usuario_log].saldoReal);

        char lin_extratoBit[200];
        sprintf(lin_extratoBit, "/%-19s - VendaBTC %.5lf || SaldoBTC: %.5lf || Saldo: R$ %.2lf || Taxa 3%%", dataHora, venda, usuarios[usuario_log].saldoBit, usuarios[usuario_log].saldoReal);

        if (strcmp(usuarios[usuario_log].extrato, "Nova conta sem transacoes ate o momento.") == 0)
        {
            strcpy(usuarios[usuario_log].extrato, lin_extratoBit);
        }
        else
        {
            strcat(usuarios[usuario_log].extrato, lin_extratoBit);
        }

        break;
    case 2:
        // Vendendo ETH 🡻🡻🡻
        printf("Cotacao atual do Ethereum: R$ %.2f\n", moeda[1].cotacaoMoeda);
        printf("*Venda sujeita a taxa de 2%%\n");
        printf("Vender ETH: ");
        scanf("%lf", &venda);

        while (venda > usuarios[usuario_log].saldoEthereum)
        {
            printf("Saldo insuficiente!\n");
            printf("Vender ETH: ");
            scanf("%lf", &venda);
        }

        usuarios[usuario_log].saldoEthereum -= venda;

        double cambioEth = venda * moeda[1].cotacaoMoeda;
        double venda_taxaEth = cambioEth * 0.02;
        cambioEth -= venda_taxaEth;

        usuarios[usuario_log].saldoReal += cambioEth;

        printf("Voce vendeu R$ %.2lf em Ethereum!\n", cambioEth);
        printf("Taxa aplicada de R$ %.2lf\n", venda_taxaEth);
        printf("Seu novo saldo: R$ %.2lf\n", usuarios[usuario_log].saldoReal);

        char lin_extratoEth[200];
        sprintf(lin_extratoEth, "/%-19s - VendaETH %.5lf || SaldoETH: %.5lf || Saldo: R$ %.2lf || Taxa 2%%", dataHora, venda, usuarios[usuario_log].saldoEthereum, usuarios[usuario_log].saldoReal);

        if (strcmp(usuarios[usuario_log].extrato, "Nova conta sem transacoes ate o momento.") == 0)
        {
            strcpy(usuarios[usuario_log].extrato, lin_extratoEth);
        }
        else
        {
            strcat(usuarios[usuario_log].extrato, lin_extratoEth);
        }


        break;
    case 3:
        // Vendendo RIP 🡻🡻🡻
        printf("Cotacao atual do Ripple: R$ %.2f\n", moeda[2].cotacaoMoeda);
        printf("*Venda sujeita a taxa de 1%%\n");
        printf("Vender RIP: ");
        scanf("%lf", &venda);

        while (venda > usuarios[usuario_log].saldoRipple)
        {
            printf("Saldo insuficiente!\n");
            printf("Vender RIP: ");
            scanf("%lf", &venda);
        }

        usuarios[usuario_log].saldoRipple -= venda;

        double cambioRip = venda * moeda[2].cotacaoMoeda;
        double venda_taxaRip = cambioRip * 0.01;
        cambioRip -= venda_taxaRip;

        usuarios[usuario_log].saldoReal += cambioRip;

        printf("Voce vendeu R$ %.2lf em Ripple!\n", cambioRip);
        printf("Taxa aplicada de R$ %.2lf\n", venda_taxaRip);
        printf("Seu novo saldo: R$ %.2lf\n", usuarios[usuario_log].saldoReal);

        char lin_extratoRip[200];
        sprintf(lin_extratoRip, "/%-19s - VendaRIP %.3lf || SaldoRIP: %.3lf || Saldo: R$ %.2lf || Taxa 1%%", dataHora, venda, usuarios[usuario_log].saldoRipple, usuarios[usuario_log].saldoReal);

        if (strcmp(usuarios[usuario_log].extrato, "Nova conta sem transacoes ate o momento.") == 0)
        {
            strcpy(usuarios[usuario_log].extrato, lin_extratoRip);
        }
        else
        {
            strcat(usuarios[usuario_log].extrato, lin_extratoRip);
        }

        break;
    default:
        break;
    }

    salvarUsuarios();

    menu();
    return 0;
}

double op_atlCotacao() {
    if (carregarCotacoes() == -1) {
        return -1;
    }

    printf("\n::::: Atualizacao da Cotacao :::::\n");
    for (int i = 0; i < num_moedas; i++) {
        double variacao = geracotacao();
        double novaCotacao = moeda[i].cotacaoMoeda * (1 + variacao);
        printf("%s: %.2lf -> %.2lf (%.2lf%%)\n", moeda[i].nomeMoeda, moeda[i].cotacaoMoeda, novaCotacao, variacao * 100);

        moeda[i].cotacaoMoeda = novaCotacao;
    }

    if (salvarCotacoes() == -1) {
        printf("Erro ao salvar as novas cotações.\n");
        return -1;
    }

    return 0;
}

int menu()
{
    int selecao;

    printf("\n::::: Selecione uma opcao :::::\n");

    puts("1. Consultar Saldo");
    puts("2. Consultar Extrato");
    puts("3. Depositar Reais");
    puts("4. Saque de Reais");
    puts("5. Comprar Criptomoedas");
    puts("6. Vender Criptomoedas");
    puts("7. Atualizar Cotacao");
    puts("0. Sair");

    printf("Eu desejo...");
    scanf("%d", &selecao);
    clearbuffer();

    while (selecao != 1 && selecao != 2 && selecao != 3 && selecao != 4 && selecao != 5 && selecao != 6 && selecao != 7 && selecao != 0)
    {
        printf("Valor invalido, insira novamente: ");
        scanf("%d", &selecao);
        clearbuffer();
    }

    switch (selecao)
    {
    case 0:
        login();
        break;
    case 1:
        op_saldo();
        break;
    case 2:
        op_extrato();
        break;
    case 3:
        op_deposito();
        break;
    case 4:
        op_saque();
        break;
    case 5:
        op_ComprarMoeda();
        break;
    case 6:
        op_Vendemoeda();
        break;
    case 7:
        op_atlCotacao();
        break;
    default:
        printf("Opcao invalida!\n");
        break;
    }

    return 0;
}

int main(void)
{
    carregarUsuarios();

    login();

    while (1)
    {
        menu();
    }

    free(usuarios);
    return 0;
}