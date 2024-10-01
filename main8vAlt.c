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

struct Moedas
{
    double bitReal;
    double ethReal;
    double ripReal;
};
struct Moedas moedas;

void clearbuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int carregarCotacoes()
{
    FILE *arquivo = fopen(ARQUIVO_MOEDAS, "rb");
    if (arquivo == NULL)
    {
        moedas.bitReal = 350925.72;
        moedas.ethReal = 12980.41;
        moedas.ripReal = 3.26;
        return 0;
    }

    fread(&moedas, sizeof(struct Moedas), 1, arquivo);
    fclose(arquivo);
    return 1;
}

int salvarCotacoes()
{
    FILE *arquivo = fopen(ARQUIVO_MOEDAS, "wb");
    if (arquivo == NULL)
    {
        printf("Erro ao salvar as cotações no arquivo.\n");
        return 0;
    }

    fwrite(&moedas, sizeof(struct Moedas), 1, arquivo);
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

int op_Compramoeda()    
{
    carregarUsuarios();
    carregarCotacoes();
    char ver_senha[TAM];
    char opcao[2];
    double investimento;
    char dataHora[50]; 

    puts("\n::::: Comprar criptomoeda :::::\n");
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

    puts("\n::::: Comprar criptomoeda :::::\n");

    puts("1. Bitcoin\n");
    puts("2. Ethereum\n");
    puts("3. Ripple\n");
    printf("Informe qual moeda deseja comprar: ");
    scanf("%s", opcao);
    opcao[strcspn(opcao, "\n")] = '\0';
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
        // Comprando BTC 🡻🡻🡻
        printf("Cotacao atual do Bitcoin: R$ %.2f\n", moedas.bitReal);
        printf("*Compra sujeita a taxa de 2%%\n");
        printf("Valor do investimento: ");
        scanf("%lf", &investimento);

        while (investimento > usuarios[usuario_log].saldoReal)
        {
            printf("Saldo insuficiente!\n");
            printf("Valor do investimento: ");
            scanf("%lf", &investimento);
        }

        usuarios[usuario_log].saldoReal -= investimento;

        double compra_taxaBit = investimento * 0.02;
        investimento -= compra_taxaBit;
        double cambioBit = investimento / moedas.bitReal;

        usuarios[usuario_log].saldoBit += cambioBit;

        printf("Voce comprou BTC %.5lf!\n", cambioBit);
        printf("Taxa aplicada de R$ %.2lf\n", compra_taxaBit);
        printf("Seu novo saldo em Bitcoin: BTC %.5lf\n", usuarios[usuario_log].saldoBit);

        char lin_extratoBit[200];
        sprintf(lin_extratoBit, "/%-19s - CompraBTC %.5lf || SaldoBTC: %.5lf || Taxa 2%%", dataHora, cambioBit, usuarios[usuario_log].saldoBit);

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
        // Comprando ETH 🡻🡻🡻
        printf("Cotacao atual do Ethereum: %.2f\n", moedas.ethReal);
        printf("*Compra sujeita a taxa de 1%%\n");
        printf("Valor do investimento: ");
        scanf("%lf", &investimento);

        while (investimento > usuarios[usuario_log].saldoReal)
        {
            printf("Saldo insuficiente!\n");
            printf("Valor do investimento: ");
            scanf("%lf", &investimento);
        }

        usuarios[usuario_log].saldoReal -= investimento;

        double compra_taxaEth = investimento * 0.01;
        investimento -= compra_taxaEth;
        double cambioEth = investimento / moedas.ethReal;

        usuarios[usuario_log].saldoEthereum += cambioEth;

        printf("Voce comprou ETH %.5lf!\n", cambioEth);
        printf("Taxa aplicada de R$ %.2lf\n", compra_taxaEth);
        printf("Seu novo saldo em Ethereum: ETH %.5lf\n", usuarios[usuario_log].saldoEthereum);

        char lin_extratoEth[200];
        sprintf(lin_extratoEth, "/%-19s - CompraETH %.5lf || SaldoETH: %.5lf || Taxa 1%%", dataHora, cambioEth, usuarios[usuario_log].saldoEthereum);

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
        // Comprando RIP 🡻🡻🡻
        printf("Cotacao atual do Ripple: %.2f\n", moedas.ripReal);
        printf("*Compra sujeita a taxa de 1%%\n");
        printf("Valor do investimento: ");
        scanf("%lf", &investimento);

        while (investimento > usuarios[usuario_log].saldoReal)
        {
            printf("Saldo insuficiente!\n");
            printf("Valor do investimento: ");
            scanf("%lf", &investimento);
        }

        usuarios[usuario_log].saldoReal -= investimento;

        double compra_taxaRip = investimento * 0.01;
        investimento -= compra_taxaRip;
        double cambioRip = investimento / moedas.ripReal;

        usuarios[usuario_log].saldoRipple += cambioRip;

        printf("Voce comprou RIP %.3lf!\n", cambioRip);
        printf("Taxa aplicada de R$ %.2lf\n", compra_taxaRip);
        printf("Seu novo saldo em Ripple: RIP %.3lf\n", usuarios[usuario_log].saldoRipple);

        char lin_extratoRip[200];
        sprintf(lin_extratoRip, "/%-19s - CompraRIP %.3lf || SaldoRIP: %.3lf || Taxa 1%%", dataHora, cambioRip, usuarios[usuario_log].saldoRipple);

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
        printf("Cotacao atual do Bitcoin: R$ %.2f\n", moedas.bitReal);
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

        double cambioBit = venda * moedas.bitReal;
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
        printf("Cotacao atual do Ethereum: R$ %.2f\n", moedas.ethReal);
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

        double cambioEth = venda * moedas.ethReal;
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
        printf("Cotacao atual do Ripple: R$ %.2f\n", moedas.ripReal);
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

        double cambioRip = venda * moedas.ripReal;
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

double op_atlCotacao()
{
    carregarCotacoes();

    double cotacaoBit = geracotacao();
    double cotacaoEth = geracotacao();
    double cotacaoRip = geracotacao();

    double a = moedas.bitReal * cotacaoBit;
    double b = moedas.ethReal * cotacaoEth;
    double c = moedas.ripReal * cotacaoRip;

    moedas.bitReal += a;
    moedas.ethReal += b;
    moedas.ripReal += c;

    puts("\n::::: Atualização da Cotacao :::::\n");
    printf("Bitcoin: %.2lf (%.2lf%%)\n", moedas.bitReal, cotacaoBit * 100);
    printf("Ethereum: %.2lf (%.2lf%%)\n", moedas.ethReal, cotacaoEth * 100);
    printf("Ripple: %.2lf (%.2lf%%)\n", moedas.ripReal, cotacaoRip * 100);

    salvarCotacoes();
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
        op_Compramoeda();
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