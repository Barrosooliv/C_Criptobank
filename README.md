# Documentação da Exchange de Criptomoedas

**Aluno**: Daniel Barroso de Oliveira  
**R.A**: 22.124.054-2  

**Aluno**: Hendrick Gonçalves Camporese Burielo  
**R.A**: 22.124.038-5  

**Aluno**: Vinicius Alves Gimenez  
**R.A**: 22.124.088-0  

## 1. Objetivo 🎯

A exchange de criptomoedas, da qual nomeamos de **Cripto Bank**, simula a interação de um investidor com uma plataforma de compra e venda de criptomoedas, estas que são moedas digitais emitidas por nenhuma entidade governamental. Nessa versão o Cripto Bank possui funcionalidades de administração do administador para armazenamento de dados do usuário, opções de investimento (Bitcoin, Ripple, Ethereum, +outras), extrato, flutuação de cotações, exclusão de usuário, exclusão de moedas etc.

## 2. Forma de Compilação 👨‍💻

O comando `gcc -o main main.c` é usado para compilar um programa em C com o GCC (GNU Compiler Collection), que é o compilador em si. Ao utilizar o comando `gcc`, você está chamando o compilador para processar arquivos de código-fonte C e transformá-los em um programa executável. O argumento `-o main` especifica o nome do arquivo de saída; `-o` significa "output", indicando que o nome do arquivo gerado deve ser o que segue o argumento, neste caso, `main`. Sem o argumento `-o`, o GCC geraria um arquivo executável com o nome padrão `a.out` no Linux/macOS ou `a.exe` no Windows. Com o `-o main`, o arquivo executável é nomeado `main` (ou `main.exe` no Windows). O arquivo `main.c` é o código-fonte escrito em C que está sendo compilado. O GCC processa esse arquivo e, após a compilação, cria o executável. Portanto, o GCC lê o arquivo `main.c`, compila o código contido nele e gera um executável com o nome `main` em vez de usar o nome padrão. Depois disso, é possível executar o arquivo diretamente no terminal, como `.\main.exe` no Windows ou `./main` no Linux/macOS.

## 3. Funcionamento 🧩

## salvarUsuariosAdm()

A função salvarUsuariosAdm() salva os dados dos usuários administradores em um arquivo binário definido pela constante ARQUIVO_USUARIOS. Primeiro, ela tenta abrir o arquivo no modo "wb"; se falhar, retorna -1. Em seguida, grava o número total de usuários administradores (num_usuariosAdm) e, por meio de um loop, escreve as estruturas de cada usuário com fwrite. Após salvar todos os dados, o arquivo é fechado com fclose() para garantir a integridade da gravação. Por fim, a função retorna 1, indicando sucesso na operação.

## carregarUsuariosAdm()

A função carregarUsuariosAdm() carrega os dados dos usuários administradores armazenados no arquivo binário definido por ARQUIVO_USUARIOS. Ela tenta abrir o arquivo no modo "rb", retornando -1 caso falhe. Se bem-sucedida, lê o número total de usuários administradores (num_usuariosAdm) e, em seguida, aloca dinamicamente memória para armazenar os dados dos usuários. Em um loop, utiliza fread para carregar cada estrutura do arquivo para a memória. Após finalizar a leitura, o arquivo é fechado com fclose() e a função retorna 1, indicando que os dados foram carregados com sucesso.

## cadastroAdm() 

A função cadastroAdm() realiza o cadastro de novos usuários administradores no sistema. Ela solicita ao usuário informações como nome, e-mail e senha, utilizando a função `cadastroDadosUsuario()` para preencher esses dados em uma estrutura de usuário administrador. Após coletar as informações, adiciona o novo administrador ao array global de administradores, incrementando o contador `num_usuariosAdm`. Em seguida, salva os dados atualizados no arquivo binário utilizando a função `salvarUsuariosAdm()`. Por fim, a função exibe uma mensagem de sucesso ao completar o cadastro.

## clearbufferAdm()

A função clearbufferAdm() serve para limpar o buffer de entrada do teclado, garantindo que dados residuais não interfiram na leitura de futuras entradas. Ela descarta caracteres pendentes até encontrar uma quebra de linha (`\n`). É especialmente útil em programas que alternam entre diferentes métodos de entrada, como “ scanf “ e “ fgets”.

## salvaAdm()

A função salvaAdm() é responsável por salvar os dados de administradores no arquivo definido. Ela percorre uma estrutura, como um vetor de administradores, e grava cada registro no arquivo, respeitando um formato específico, geralmente estruturado com campos como ID, nome e senha. Durante o processo, o arquivo é aberto em modo de escrita, substituindo qualquer conteúdo anterior para garantir dados atualizados. Caso ocorra algum erro na abertura do arquivo, a função retorna um valor indicativo de falha. Essa abordagem assegura a persistência das informações entre execuções do programa, permitindo recuperação posterior com funções como carregarUsuariosAdm().

## salvarMoeda()

A função salvarMoeda() tem como objetivo gravar informações sobre uma moeda (ou várias) em um arquivo, garantindo a persistência dos dados. Ela normalmente aceita como parâmetros os dados das moedas ou acessa uma estrutura de dados global para coletar essas informações. O arquivo é aberto em modo de escrita para sobrescrever o conteúdo anterior, garantindo a atualização. Cada moeda é escrita em um formato específico, como texto delimitado ou binário, dependendo da implementação. Caso ocorra um erro durante a abertura ou gravação, a função pode retornar um código de erro para indicar o problema.

## carregarMoedas()

A função carregarMoedas() é responsável por ler os dados de moedas armazenados em um arquivo e carregá-los na memória para uso no programa. Ela abre o arquivo em modo de leitura e verifica se ele existe ou se há erros na abertura. Os dados são lidos linha a linha ou em blocos, sendo interpretados e armazenados em uma estrutura de dados, como um vetor ou lista. Caso o arquivo esteja vazio ou contenha erros, a função pode retornar valores específicos ou mensagens de erro. Por fim, ela garante que o arquivo seja fechado após a leitura, evitando vazamentos de recursos.

## carregaAdm()

A função carregaAdm() é responsável por carregar as informações de administradores de um arquivo previamente salvo, permitindo sua utilização no programa. Ela abre o arquivo em modo de leitura e verifica se ele existe ou está acessível. Em seguida, os dados são lidos e interpretados, sendo armazenados em uma estrutura de dados, como um vetor ou lista, para facilitar o acesso. Caso ocorra algum erro, como arquivo inexistente ou vazio, a função pode retornar mensagens de erro ou valores padrão. Por fim, o arquivo é fechado para garantir a liberação dos recursos.

## cadastroAdm_Adm()

A função cadastroAdm_Adm() permite que um administrador já registrado cadastre novos administradores no sistema. Ela solicita informações como nome, login e senha, validando os dados fornecidos para evitar duplicatas ou entradas inválidas. Após validar, os dados são adicionados à estrutura que armazena os administradores, garantindo sua persistência para futuras operações. Além disso, pode incluir verificações adicionais, como permissões específicas do administrador que realiza o cadastro. Essa função ajuda a manter a segurança e a organização do sistema de administração.

## loginAdm()

A função loginAdm() autentica administradores no sistema, solicitando login e senha. Ela percorre a lista de administradores cadastrados, comparando as credenciais fornecidas com as armazenadas. Caso encontre uma correspondência, permite o acesso, retornando um indicador de sucesso, geralmente o índice do administrador autenticado. Se as credenciais forem inválidas, informa o erro e solicita nova tentativa ou retorna uma falha. Essa função é essencial para restringir o acesso a recursos protegidos, garantindo segurança no sistema.

## op_excluir()

A função op_excluir() permite a exclusão de registros específicos no sistema, como administradores ou usuários, com base em critérios definidos pelo usuário. Inicialmente, exibe a lista de itens disponíveis e solicita que o usuário escolha qual deseja excluir, geralmente identificando-o por um ID ou índice. Em seguida, valida a entrada para evitar erros, garantindo que o registro escolhido existe. Após a confirmação da exclusão, remove o item da lista correspondente, ajustando os índices se necessário. Essa função também pode exibir mensagens de sucesso ou erro, dependendo do resultado da operação. Além disso, pode invocar outra função para salvar a lista atualizada em arquivo, mantendo a persistência dos dados. Assim, garante que o sistema permaneça organizado e atualizado após exclusões.

## op_cadastraCripto() 

A função op_cadastraCripto() é responsável por permitir o cadastro de uma nova criptomoeda no sistema. Ela solicita ao usuário informações essenciais, como o nome, código, valor inicial e outras características da criptomoeda. Após coletar os dados, valida as entradas para garantir que sejam consistentes e completas. Em seguida, armazena as informações em uma estrutura de dados, como uma lista ou vetor de moedas. Finalmente, pode chamar uma função como salvarMoeda() para persistir os dados no arquivo, garantindo que o novo cadastro fique disponível para uso futuro.

## op_excluirMoeda()

A função op_excluirMoeda() permite a exclusão de uma criptomoeda previamente cadastrada no sistema. Primeiro, solicita ao usuário o código ou nome da moeda a ser excluída, verificando se ela existe na lista ou vetor de moedas cadastradas. Caso a moeda seja encontrada, os dados correspondentes são removidos da estrutura de armazenamento. Após a remoção, a função pode atualizar o arquivo de persistência chamando, por exemplo, a função `salvarMoeda()` para garantir que a exclusão seja refletida permanentemente. Se a moeda não for encontrada, exibe uma mensagem informando o erro ao usuário.

## op_consultarSaldo()

A função op_consultarSaldo() é responsável por exibir o saldo atual de um usuário no sistema. Primeiramente, ela verifica os dados do usuário logado para acessar suas informações financeiras. Em seguida, recupera o saldo associado a esse usuário, geralmente armazenado em uma estrutura de dados ou banco de dados. A função apresenta o saldo de forma clara na interface, indicando a quantidade de criptomoedas ou valores em moeda fiduciária disponíveis. Caso ocorra algum erro, como dados não encontrados, ela notifica o usuário adequadamente. Essa funcionalidade é essencial para monitorar os ativos disponíveis no sistema.

## op_consultarExtrato()

A função op_consultarExtrato() exibe o histórico de transações de um usuário no sistema. Ela acessa os registros vinculados ao usuário logado, como compras, vendas e transferências de criptomoedas, geralmente armazenados em um banco de dados ou arquivo. Esses dados são recuperados e apresentados na tela de forma detalhada, incluindo informações como data, tipo de operação e valores envolvidos. Caso não haja transações registradas, a função exibe uma mensagem informando a ausência de movimentações. Essa funcionalidade é útil para os usuários acompanharem suas atividades financeiras e verificarem a origem e o destino de seus recursos.

## op_atlCotacaoAdm()

A função op_atlCotacaoAdm() atualiza a cotação de todas as criptmoedas, inclusive as recém criadas, mudando também a taxa dela.

## menuAdm()

A função menuAdm() é responsável por exibir o menu principal de opções para o administrador no sistema. Ela apresenta as escolhas disponíveis, como cadastrar ou excluir usuários, consultar extratos, gerenciar criptomoedas, entre outras funcionalidades administrativas. A função aguarda a entrada do administrador, processando a opção escolhida para direcioná-lo à respectiva funcionalidade. Dependendo da escolha, o sistema executa a ação correspondente, como cadastrar novos administradores ou visualizar dados financeiros. O objetivo é fornecer uma interface interativa e simples para a administração eficiente do sistema.
main()
A função main() serve como ponto de entrada para a execução do programa. Dentro dessa função, são realizadas as inicializações necessárias e a execução de funções-chave do sistema. Ela frequentemente chama a função menuAdm() para exibir as opções do menu para o administrador, permitindo a navegação por diferentes funcionalidades, como cadastro de administradores, controle de criptomoedas, ou consultas financeiras. Dependendo da escolha do administrador, o fluxo do programa segue para outras funções específicas. Ao final, a função main() coordena o fluxo geral do aplicativo, garantindo que o sistema opere corretamente.

