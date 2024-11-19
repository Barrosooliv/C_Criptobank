#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define TAM 50
#define TAM_INDETERMINADO 5000
#define ARQUIVO_ADM "administradores.txt"
#define ARQUIVO_MOEDAS "moedas.txt"

int usuario_logAdm;

typedef struct
{
    char cpf[TAM];
    char senha[TAM];
    char nome[TAM];

} Administradores;
Administradores *adms = NULL;
int num_adm = 0;


//FUNÇÔES ADM
void clearbufferAdm()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }

}

int salvaAdm()
{

    FILE *arquivo = fopen(ARQUIVO_ADM, "wb");
    if (arquivo == NULL)
    {
        return -1;
    }

    fwrite(&num_adm, sizeof(int), 1, arquivo);

    for (int i = 0; i < num_adm; i++)
    {
        fwrite(&adms[i], sizeof(Administradores), 1, arquivo);
    }

    fclose(arquivo);
    return 1;

}

int carregaAdm()
{

    FILE *arquivo = fopen(ARQUIVO_ADM, "rb");
    if (arquivo == NULL)
    {
        return 1;
    }

    fread(&num_adm, sizeof(int), 1, arquivo);

    adms = malloc(num_adm * sizeof(Administradores));

    for (int i = 0; i < num_adm; i++)
    {
        fread(&adms[i], sizeof(Administradores), 1, arquivo);
    }

    fclose(arquivo);
    return 0;

}

int cadastroAdm_Adm (){

    int CPFjaexiste = 0;
    adms = realloc(adms, (num_adm + 1) * sizeof(Administradores));
    if (adms == NULL)
    {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    printf("Digite o nome: ");
    fgets(adms[num_adm].nome, TAM, stdin);
    adms[num_adm].nome[strcspn(adms[num_adm].nome, "\n")] = '\0';

    printf("Digite seu CPF: ");
    fgets(adms[num_adm].cpf, TAM, stdin);
    adms[num_adm].cpf[strcspn(adms[num_adm].cpf, "\n")] = '\0';

    for (int i = 0; i < num_adm; i++)
    {
        if (strcmp(adms[i].cpf, adms[num_adm].cpf) == 0)
        {
            CPFjaexiste = 1;
            break;
        }
    }

    while (strlen(adms[num_adm].cpf) != 11 || CPFjaexiste == 1)
    {
        if (strlen(adms[num_adm].cpf) != 11)
        {
            printf("=> O CPF deve conter 11 digitos!\n");
        }
        else if (CPFjaexiste == 1)
        {
            printf("=> Este CPF ja esta cadastrado!\n");
        }

        printf("Digite novamente seu CPF: ");
        fgets(adms[num_adm].cpf, TAM, stdin);
        adms[num_adm].cpf[strcspn(adms[num_adm].cpf, "\n")] = '\0';

        for (int i = 0; i < num_adm; i++)
        {
            if (strcmp(adms[i].cpf, adms[num_adm].cpf) == 0)
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
    fgets(adms[num_adm].senha, TAM, stdin);
    while (strlen(adms[num_adm].senha) < 6)
    {
        printf("Digite uma senha superior a 6 digitos ou letras!\n");
        printf("Digite novamente sua senha: ");
        fgets(adms[num_adm].senha, TAM, stdin);
    }
    adms[num_adm].senha[strcspn(adms[num_adm].senha, "\n")] = '\0';

    num_adm++;
    printf("\nUsuario cadastrado com sucesso!\n");

    salvaAdm();
    return 0;
}

int loginAdm()
{
    carregaAdm();
    char cpf[TAM];
    char senha[TAM];
    int opcao;
    int encontrado = 0;

    while (encontrado == 0 || opcao == 0)
    {
        printf("\n::::: Bem vindo(a) ao CriptoBank :::::\n");
        printf("Gostaria de realizar login ou cadastro?\n");
        printf("1. Cadastrar Administrador\n");
        printf("2. Login\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        clearbufferAdm();

        switch (opcao)
        {
        case 1:
            cadastroAdm_Adm();
            break;
        case 2:
            printf("Digite o CPF: ");
            fgets(cpf, TAM, stdin);
            cpf[strcspn(cpf, "\n")] = '\0';

            printf("Digite a senha: ");
            fgets(senha, TAM, stdin);
            senha[strcspn(senha, "\n")] = '\0';

            for (int i = 0; i < num_adm; i++)
            {
                if (strcmp(adms[i].cpf, cpf) == 0 && strcmp(adms[i].senha, senha) == 0)
                {
                    printf("\nLogin realizado com sucesso! Bem-vindo(a), %s!\n", adms[i].nome);
                    encontrado = 1;
                    usuario_logAdm = i;
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


int menuAdm(){
    int selecao;

    printf("\n::::: Selecione uma opcao :::::\n");

    puts("1.\n");
    puts("2.\n");
    puts("3.\n");
    puts("4.\n");
    puts("5.\n");
    puts("6.\n");
    puts("0. Sair\n");

    printf("Eu desejo...");
    scanf("%d", &selecao);
    clearbufferAdm();

    while (selecao != 1 && selecao != 2 && selecao != 3 && selecao != 4 && selecao != 5 && selecao != 6 && selecao != 7 && selecao != 0)
    {
        printf("Valor invalido, insira novamente: ");
        scanf("%d", &selecao);
        clearbufferAdm();
    }

    switch (selecao)
    {
    case 0:
        loginAdm();
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    default:
        printf("Opcao invalida!\n");
        break;
    }

    return 0;
}

int main(){

    carregaAdm();

    loginAdm();

    while (1)
    {
        menuAdm();
    }

    free(adms);

    return 0;
}