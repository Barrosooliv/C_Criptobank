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
int menu();      // protótipo da funcao

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

void clearbuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
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
    puts("\n::::: Saque :::::\n");
    
    menu();
    return 0;
}

int op_Compramoeda()    
{
    puts("\n::::: Comprar criptomoeda :::::\n");
   
    menu();
    return 0;
}

int op_Vendemoeda()
{
    puts("\n::::: Vender criptomoeda :::::\n");

    menu();
    return 0;
}

double op_atlCotacao()
{
    puts("\n::::: Atualização da Cotacao :::::\n");

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