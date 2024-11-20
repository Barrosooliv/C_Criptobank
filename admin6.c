#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define TAM 50
#define TAM_INDETERMINADO 5000
#define ARQUIVO_ADM "administradores.txt"
#define ARQUIVO_MOEDAS "moedas.txt"
#define ARQUIVO_USUARIOS "usuarios.txt"

int usuario_logAdm;

typedef struct
{
    char cpf[TAM];
    char senha[TAM];
    char nome[TAM];

} Administradores;
Administradores *adms = NULL;
int num_adm = 0;

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
Usuario *usuariosAdm = NULL;
int num_usuariosAdm = 0;

typedef struct  
{
    char nomeMoeda[TAM];
    double cotacaoMoeda;
    double taxaVenda;
    double taxaCompra;

} Moeda;
Moeda *moedaAdm = NULL;
int num_moedasAdm = 0;

//FUNÇÕES DA APLICAÇÃO DO USUÀRIO

int salvarUsuariosAdm()
{
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "wb");
    if (arquivo == NULL)
    {
        return -1;
    }

    fwrite(&num_usuariosAdm, sizeof(int), 1, arquivo);

    for (int i = 0; i < num_usuariosAdm; i++)
    {
        fwrite(&usuariosAdm[i], sizeof(Usuario), 1, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int carregarUsuariosAdm()
{
    FILE *arquivo = fopen(ARQUIVO_USUARIOS, "rb");
    if (arquivo == NULL)
    {
        return -1;
    }

    fread(&num_usuariosAdm, sizeof(int), 1, arquivo);

    usuariosAdm = malloc(num_usuariosAdm * sizeof(Usuario));

    for (int i = 0; i < num_usuariosAdm; i++)
    {
        fread(&usuariosAdm[i], sizeof(Usuario), 1, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int cadastroAdm()      // Cadastra user
{
    if (num_usuariosAdm >= 10)
    {
        printf("Nao e possivel cadastrar mais usuarios! (Limite de 10 usuarios atingido)\n");
        return -1;
    }

    int CPFjaexiste = 0;
    usuariosAdm = realloc(usuariosAdm, (num_usuariosAdm + 1) * sizeof(Usuario));

    printf("Digite o nome: ");

    fgets(usuariosAdm[num_usuariosAdm].nome, TAM, stdin);
    usuariosAdm[num_usuariosAdm].nome[strcspn(usuariosAdm[num_usuariosAdm].nome, "\n")] = '\0';

    printf("Digite seu CPF: ");
    fgets(usuariosAdm[num_usuariosAdm].cpf, TAM, stdin);
    usuariosAdm[num_usuariosAdm].cpf[strcspn(usuariosAdm[num_usuariosAdm].cpf, "\n")] = '\0';

    for (int i = 0; i < num_usuariosAdm; i++)
    {
        if (strcmp(usuariosAdm[i].cpf, usuariosAdm[num_usuariosAdm].cpf) == 0)
        {
            CPFjaexiste = 1;
            break;
        }
    }

    while (strlen(usuariosAdm[num_usuariosAdm].cpf) != 11 || CPFjaexiste == 1)
    {
        if (strlen(usuariosAdm[num_usuariosAdm].cpf) != 11)
        {
            printf("=> O CPF deve conter 11 digitos!\n");
        }
        else if (CPFjaexiste == 1)
        {
            printf("=> Este CPF ja esta cadastrado!\n");
        }

        printf("Digite novamente seu CPF: ");
        fgets(usuariosAdm[num_usuariosAdm].cpf, TAM, stdin);
        usuariosAdm[num_usuariosAdm].cpf[strcspn(usuariosAdm[num_usuariosAdm].cpf, "\n")] = '\0';

        for (int i = 0; i < num_usuariosAdm; i++)
        {
            if (strcmp(usuariosAdm[i].cpf, usuariosAdm[num_usuariosAdm].cpf) == 0)
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
    fgets(usuariosAdm[num_usuariosAdm].senha, TAM, stdin);
    while (strlen(usuariosAdm[num_usuariosAdm].senha) < 6)
    {
        printf("Digite uma senha superior a 6 digitos ou letras!\n");
        printf("Digite novamente sua senha: ");
        fgets(usuariosAdm[num_usuariosAdm].senha, TAM, stdin);
    }
    usuariosAdm[num_usuariosAdm].senha[strcspn(usuariosAdm[num_usuariosAdm].senha, "\n")] = '\0';

    usuariosAdm[num_usuariosAdm].saldoReal = 0;
    usuariosAdm[num_usuariosAdm].saldoBit = 0;
    usuariosAdm[num_usuariosAdm].saldoEthereum = 0;
    usuariosAdm[num_usuariosAdm].saldoRipple = 0;

    strcpy(usuariosAdm[num_usuariosAdm].extrato, "Nova conta sem transacoes ate o momento.");

    num_usuariosAdm++;
    printf("\nUsuario cadastrado com sucesso!\n");

    salvarUsuariosAdm();
    return 0;
}

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

int salvarMoeda() {
    FILE *arquivo = fopen(ARQUIVO_MOEDAS, "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo das moedas.\n");
        return -1;
    }

    int moedas_adicionais = num_moedasAdm - 3;
    fwrite(&moedas_adicionais, sizeof(int), 1, arquivo);

    for (int i = 3; i < num_moedasAdm; i++) {
        fwrite(&moedaAdm[i], sizeof(Moeda), 1, arquivo);
    }

    fclose(arquivo);
    return 1;
}

int carregarMoedas() {
    FILE *arquivo = fopen(ARQUIVO_MOEDAS, "rb");
    if (arquivo == NULL) {
        printf("Arquivo de moedas não encontrado. Inicializando apenas moedas padrão.\n");

        num_moedasAdm = 3; 
        moedaAdm = malloc(num_moedasAdm * sizeof(Moeda));
        if (!moedaAdm) {
            printf("Erro ao alocar memória para moedas.\n");
            return -1;
        }

        strcpy(moedaAdm[0].nomeMoeda, "Bitcoin");
        moedaAdm[0].cotacaoMoeda = 350925.72;
        moedaAdm[0].taxaCompra = 0.02;
        moedaAdm[0].taxaVenda = 0.01;

        strcpy(moedaAdm[1].nomeMoeda, "Ethereum");
        moedaAdm[1].cotacaoMoeda = 12980.41;
        moedaAdm[1].taxaCompra = 0.025;
        moedaAdm[1].taxaVenda = 0.015;

        strcpy(moedaAdm[2].nomeMoeda, "Ripple");
        moedaAdm[2].cotacaoMoeda = 3.26;
        moedaAdm[2].taxaCompra = 0.03;
        moedaAdm[2].taxaVenda = 0.02;

        return 1;
    }

    int moedas_adicionais = 0;
    fread(&moedas_adicionais, sizeof(int), 1, arquivo);

    num_moedasAdm = 3 + moedas_adicionais;

    moedaAdm = realloc(moedaAdm, num_moedasAdm * sizeof(Moeda));
    if (!moedaAdm) {
        fclose(arquivo);
        printf("Erro ao alocar memória para moedas.\n");
        return -1;
    }

    strcpy(moedaAdm[0].nomeMoeda, "Bitcoin");
    moedaAdm[0].cotacaoMoeda = 350925.72;
    moedaAdm[0].taxaCompra = 0.02;
    moedaAdm[0].taxaVenda = 0.01;

    strcpy(moedaAdm[1].nomeMoeda, "Ethereum");
    moedaAdm[1].cotacaoMoeda = 12980.41;
    moedaAdm[1].taxaCompra = 0.025;
    moedaAdm[1].taxaVenda = 0.015;

    strcpy(moedaAdm[2].nomeMoeda, "Ripple");
    moedaAdm[2].cotacaoMoeda = 3.26;
    moedaAdm[2].taxaCompra = 0.03;
    moedaAdm[2].taxaVenda = 0.02;

    fread(&moedaAdm[3], sizeof(Moeda), moedas_adicionais, arquivo);
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

void op_excluir() {//REVER ESTA FUNÇÂO TESTAR FUNCIONAMENTO
    if (carregarUsuariosAdm() == -1) {
        printf("Erro ao carregar usuarios!\n");
        return;
    }

    if (num_usuariosAdm == 0) {
        printf("Nenhum investidor cadastrado.\n");
        return;
    }

    char cpf[TAM];
    printf("Informe o CPF do investidor para exclusão: ");
    fgets(cpf, TAM, stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    int indiceEncontrado = -1;
    for (int i = 0; i < num_usuariosAdm; i++) {
        if (strcmp(usuariosAdm[i].cpf, cpf) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Investidor com CPF '%s' não encontrado.\n", cpf);
        return;
    }

    Usuario encontrado = usuariosAdm[indiceEncontrado];
    printf("\nInvestidor encontrado:\n");
    printf("Nome: %s\n", encontrado.nome);
    printf("CPF: %s\n", encontrado.cpf);
    printf("Saldo em Real: %.2f\n", encontrado.saldoReal);
    printf("Saldo em Bitcoin: %.6f\n", encontrado.saldoBit);
    printf("Saldo em Ethereum: %.6f\n", encontrado.saldoEthereum);
    printf("Saldo em Ripple: %.6f\n", encontrado.saldoRipple);

    char confirmacao;
    printf("\nDeseja realmente excluir este investidor? (s/n): ");
    scanf(" %c", &confirmacao);
    getchar(); 
    if (confirmacao != 's' && confirmacao != 'S') {
        printf("Exclusão cancelada.\n");
        return;
    }

    for (int i = indiceEncontrado; i < num_usuariosAdm - 1; i++) {
        usuariosAdm[i] = usuariosAdm[i + 1];
    }
    num_usuariosAdm--;

    if (salvarUsuariosAdm() == -1) {
        printf("Erro ao salvar as alterações no arquivo!\n");
    } else {
        printf("Investidor excluído com sucesso!\n");
    }
}

int op_cadastraCripto() {
    carregarMoedas();

    double cotacaoinput;
    double taxaVendainput;
    double taxaComprainput;
    int MoedajaExiste = 0;

    moedaAdm = realloc(moedaAdm, (num_moedasAdm + 1) * sizeof(Moeda));
    if (!moedaAdm) {
        printf("Erro ao alocar memória.\n");
        return -1;
    }

    printf(":::: Cadastre as Criptos ::::\n");

    do {
        printf("Informe o nome da Criptomoeda: ");
        fgets(moedaAdm[num_moedasAdm].nomeMoeda, TAM, stdin);
        moedaAdm[num_moedasAdm].nomeMoeda[strcspn(moedaAdm[num_moedasAdm].nomeMoeda, "\n")] = '\0';

        MoedajaExiste = 0;
        for (int i = 0; i < num_moedasAdm; i++) {
            if (strcmp(moedaAdm[i].nomeMoeda, moedaAdm[num_moedasAdm].nomeMoeda) == 0) {
                printf("Moeda ja cadastrada. Tente outro nome.\n");
                MoedajaExiste = 1;
                break;
            }
        }
    } while (MoedajaExiste || strlen(moedaAdm[num_moedasAdm].nomeMoeda) == 0);

    do {
        printf("Informe a cotação da Criptomoeda: ");
        if (scanf("%lf", &cotacaoinput) != 1 || cotacaoinput <= 0) {
            printf("Entrada invalida.\n");
            clearbufferAdm();
        } else {
            break;
        }
    } while (1);
    moedaAdm[num_moedasAdm].cotacaoMoeda = cotacaoinput;

    do {
        printf("Informe a taxa de compra da Criptomoeda(em forma decimal): ");
        if (scanf("%lf", &taxaComprainput) != 1 || taxaComprainput <= 0) {
            printf("Entrada invalida.\n");
            clearbufferAdm();
        } else {
            break;
        }
    } while (1);
    moedaAdm[num_moedasAdm].taxaCompra = taxaComprainput;

    do {
        printf("Informe a taxa de venda da Criptomoeda(em forma decimal): ");
        if (scanf("%lf", &taxaVendainput) != 1 || taxaVendainput <= 0) {
            printf("Entrada invalida.\n");
            clearbufferAdm();
        } else {
            break;
        }
    } while (1);
    moedaAdm[num_moedasAdm].taxaVenda = taxaVendainput;

    num_moedasAdm++;
    printf("Moeda cadastrada com sucesso!\n");

    salvarMoeda(); 
    return 0;
}

void op_excluirMoeda() {
    if (carregarMoedas() == -1) {
        printf("Erro ao carregar moedas!\n");
        return;
    }

    if (num_moedasAdm == 0) {
        printf("Nenhuma moeda cadastrada.\n");
        return;
    }

    char nomeMoeda[TAM];
    printf("Informe o nome da moeda para exclusao: ");
    fgets(nomeMoeda, TAM, stdin);
    nomeMoeda[strcspn(nomeMoeda, "\n")] = '\0';

    int indiceEncontrado = -1;
    for (int i = 0; i < num_moedasAdm; i++) {
        if (strcmp(moedaAdm[i].nomeMoeda, nomeMoeda) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Moeda '%s' nao encontrada.\n", nomeMoeda);
        return;
    }

    Moeda encontrada = moedaAdm[indiceEncontrado];
    printf("\nMoeda encontrada:\n");
    printf("Nome: %s\n", encontrada.nomeMoeda);
    printf("Cotacao: %.2f\n", encontrada.cotacaoMoeda);
    printf("Taxa de Venda: %.2f\n", encontrada.taxaVenda);
    printf("Taxa de Compra: %.2f\n", encontrada.taxaCompra);

    char confirmacao;
    printf("\nDeseja realmente excluir esta moeda? (s/n): ");
    scanf(" %c", &confirmacao);
    getchar(); 
    if (confirmacao != 's' && confirmacao != 'S') {
        printf("Exclusao cancelada.\n");
        return;
    }

    for (int i = indiceEncontrado; i < num_moedasAdm - 1; i++) {
        moedaAdm[i] = moedaAdm[i + 1];
    }
    num_moedasAdm--;

    if (salvarMoeda() == -1) {
        printf("Erro ao salvar as alteracoes no arquivo!\n");
    } else {
        printf("Moeda excluida com sucesso!\n");
    }
}

void op_consultarSaldo() {
    if (carregarUsuariosAdm() == -1) {
        printf("Erro ao carregar usuários!\n");
        return;
    }

    if (num_usuariosAdm == 0) {
        printf("Nenhum investidor cadastrado.\n");
        return;
    }

    char cpf[TAM];
    printf("Informe o CPF do investidor: ");
    fgets(cpf, TAM, stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < num_usuariosAdm; i++) {
        if (strcmp(usuariosAdm[i].cpf, cpf) == 0) {
            printf("\nInvestidor encontrado:\n");
            printf("Nome: %s\n", usuariosAdm[i].nome);
            printf("CPF: %s\n", usuariosAdm[i].cpf);
            printf("Saldo em Real: %.2f\n", usuariosAdm[i].saldoReal);
            printf("Saldo em Bitcoin: %.6f\n", usuariosAdm[i].saldoBit);
            printf("Saldo em Ethereum: %.6f\n", usuariosAdm[i].saldoEthereum);
            printf("Saldo em Ripple: %.6f\n", usuariosAdm[i].saldoRipple);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Investidor com CPF '%s' não encontrado.\n", cpf);
    }
}

int op_consultarExtrato() {
    if (carregarUsuariosAdm() == -1) {
        printf("Erro ao carregar usuários!\n");
        return -1;
    }

    puts("\n::::: Consultar Extrato :::::\n");

    if (num_usuariosAdm == 0) {
        printf("Nenhum investidor cadastrado.\n");
        return -1;
    }

    char cpf[TAM];
    printf("Informe o CPF do investidor: ");
    fgets(cpf, TAM, stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    int usuario_encontrado = -1;
    for (int i = 0; i < num_usuariosAdm; i++) {
        if (strcmp(usuariosAdm[i].cpf, cpf) == 0) {
            usuario_encontrado = i;
            break;
        }
    }

    if (usuario_encontrado == -1) {
        printf("Investidor com CPF '%s' nao encontrado.\n", cpf);
        return -1;
    }

    char *operacoes[300];
    int totalop = 0;
    char *extrato_copia = strdup(usuariosAdm[usuario_encontrado].extrato);
    char *token = strtok(extrato_copia, "/");

    printf("\nExtrato de operacoes para CPF: %s\n", cpf);
    while (token != NULL) {
        operacoes[totalop] = token;
        totalop++;
        token = strtok(NULL, "/");
    }

    for (int i = 0; i < totalop; i++) {
        if (i > 100) {
            printf("Limite de operacoes atingido.\n");
            free(extrato_copia);
            return -1;
        }
        printf("%s\n", operacoes[i]);
    }

    free(extrato_copia);
    return 0;
}

int menuAdm(){
    int selecao;

    printf("\n::::: Selecione uma opcao :::::\n");

    puts("1. Cadastra usuario\n");
    puts("2. Exclusao de usuario\n");
    puts("3. cadastroAdm de criptomoeda\n");
    puts("4. Excluir Criptomoeda\n");
    puts("5. Consultar saldo de investidor\n");
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
        cadastroAdm();
        break;
    case 2:
        op_excluir();
        break;
    case 3:
        op_cadastraCripto();
        break;
    case 4:
        op_excluirMoeda();
        break;
    case 5:
        op_consultarSaldo();
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