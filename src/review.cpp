#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <cstring>
#include <ctime>
#include <string.h>
#include "../headers/review.h"
#include "../headers/registro.h"
#include "../headers/ordenacao.h"
//#include "../headers/hash.h"

using namespace std;

//? Constructors ------------------------------------------------------------------------------
Review::Review(){
  
}

Review::Review(string dir){
  dirArq = dir;
}

//? Destructor --------------------------------------------------------------------------------
Review::~Review(){

}

//? Funções de Processamento ------------------------------------------------------------------
/**
 * @brief  Função responsável por entrar no repositório passado como argumento da main 
 *         e verificar a existência do arquivo tiktok_app_reviews.bin.
 * 
 */
void Review::verificaDir(){
  cout << "Iniciando verificação do diretório:" << endl;
  cout << endl;

  int escolha;
  int limit;

  string dirReview = dirArq + "/tiktok_app_reviews.bin";

  ifstream reviewBin(dirReview, ios::in);

  // Se o arquivo tiktok_app_reviews.bin não existe, inicia-se o processamento do tiktok_app_reviews.csv.
  if(!reviewBin.is_open()){

    cout << "O arquivo 'tiktok_app_reviews.bin' ainda não existe." << endl;
    cout << endl;
    cout << "Iniciando processamento e criação do arquivo binário:" << endl;
    cout << endl;
    cout << "Digite:" << endl;
    cout << "  - 1: Para processar o arquivo inteiro." << endl;
    cout << "  - 2: Para processar apenas uma parte do arquivo (55000 primeiras linhas)." << endl;
    cout << "Escolha: " << endl;
    cin >> escolha;
    cout << "------------------------------------------------------------------------------" << endl;

    if(escolha == 1){
      limit = 2400000;
    }
    else{
      limit = 55000;
    }

    processaReview(limit);  // Chamada da função de processamento do arquivo .csv.

    cout << "Arquivo .csv processado com sucesso!" << endl;
    cout << "------------------------------------------------------------------------------" << endl;

    fstream arq(dirArq+"/tiktok_app_reviews.bin", ios::in | ios::binary);  // Abertura do arquivo .bin para leitura.
    arq.seekg(0);  // Posicionando o ponteiro do arquivo no primeiro byte.
    iniciar(arq, limit);  // Chamada do menu iniciar.
    arq.close();   // Fechando arquivo .bin.
  }
  // Caso contrário, abre o arquivo binário e segue para a próxima etapa.
  else{
    cout << "O arquivo 'tiktok_app_reviews.bin' já existe." << endl;
    cout << endl;
    cout << "Defina o intervalo de acesso aos registros:" << endl;
    cout << "  - 1: Para o arquivo inteiro." << endl;
    cout << "  - 2: Para apenas uma parte do arquivo (55000 primeiros registros)." << endl;
    cout << endl;
    cout << "Atenção: Caso defina o intervalo para o arquivo inteiro e ele não tenha sido processado por completo," << endl;
    cout << "poderá haver corrupção nas informações exibidas." << endl;
    cout << endl;
    cout << "Escolha: " << endl;
    cin >> escolha;
    cout << "------------------------------------------------------------------------------" << endl;

    if(escolha == 1){
      limit = 2400000;
    }
    else{
      limit = 55000;
    }

    fstream arq(dirArq+"/tiktok_app_reviews.bin", ios::in | ios::binary);  // Abertura do arquivo .bin para leitura.
    arq.seekg(0);  // Posicionando o ponteiro do arquivo no primeiro byte.
    iniciar(arq, limit);  // Chamada do menu iniciar.
    arq.close();   // Fechando arquivo .bin.
  }
}

/**
 * @brief  Função utilizada para fazer o processamento do arquivo .csv
 * 
 */
void Review::processaReview(int limit){
  cout << "Iniciando processamento do arquivo .csv:" << endl;

  string dirReview = dirArq + "/tiktok_app_reviews.csv";

  // Realizando a abertura dos arquivos para as devidas manipulações.
  fstream arq1(dirReview, ios::in | ios::out | ios::app);
  fstream arq2(dirArq+"/tiktok_app_reviews.bin", ios:: out | ios::binary);

  arq1.seekg(0);  // Posicionando o ponteiro no início do arquivo .csv.
  arq2.seekg(0);  // Posicionando o ponteiro no início do arquivo .bin.

  string line = "";  // String para capturar a linha inteira do arquivo arq1.
  string aux = "";   // String auxiliar para armazenar o caractere ','.

  // String auxiliares para armazenar os campos de criação da registro.
  string idAux = "";   
  string textAux = "";
  string votesAux = "";
  string versionAux = "";
  string dateAux = "";

  // Vetores de char para fazer a criação do objeto do tipo registro.
  char id[100];
  char text[2000];
  char text2[50];
  int votes;
  char version[7];
  char date[20];

  long int cont = 0;  // Contador para verificar a linha a ser lida.

  // Para processar o .csv completamente, é necessário alterar o parâmetro do while
  // para percorrer o arquivo por completo.
  while(cont < 1000000){
    // Ignorar a primeira linha do .csv.
    if(cont == 0){
      getline(arq1, line);
    }
    else{
      getline(arq1, line);         // Captura da linha completa.
      std::stringstream ss(line);  // Transformando em stringstream.

      getline(ss, idAux, ',');

      // Se o texto de comentário for delimitado por '"', a quebra de linha será feita com '"'.
      if(line[90] == '"'){
        getline(ss, aux, '"');
        getline(ss, textAux, '"');
        textAux = '"' + textAux + '"';

        getline(ss, aux, ',');
        getline(ss, votesAux, ',');
        getline(ss, versionAux, ',');
        getline(ss, dateAux, '\n');
      }
      // Caso contrário, a quebra de linha será feita com ','.
      else{
        getline(ss, textAux, ',');
        textAux = '"' + textAux + '"';

        getline(ss, votesAux, ',');
        getline(ss, versionAux, ',');
        getline(ss, dateAux, '\n');
      }
    }

    // Armazenando as informações das strings nos vetores de char correspondentes para a criação da registro.
    strcpy(id, idAux.c_str());
    strcpy(text, textAux.c_str());

    try{
      votes = stoi(votesAux);
    }
    catch(const exception& e){
      votes = 0;
    }
    
    strcpy(version, versionAux.c_str());
    strcpy(date, dateAux.c_str());

    criaRegistro(arq2, id, text2, votes, version, date);  // Chamada a função para criar o objeto registro.
    cont++;
  }

  arq1.close();  // Fechando arq1.
  arq2.close();  // Fechando arq2.

  cout << endl;
  cout << "Arquivo processado com sucesso!" << endl;
  cout << "----------------------------------------------------------------------------------" << endl;
  cout << endl;
}

//? Operações Principais ----------------------------------------------------------------------
void Review::importaRegistros(){
  cout << "Importa os registros do arquivo" << endl;
}

/**
 * @brief        Função utilizada para acessar o arquivo .bin e exibir na tela o review do índice
 *               especificado pelo usuário.
 * 
 * @param arq    Arquivo binário a ser lido.
 * @param index  Índice especificado pelo usuário.
 */
void Review::acessaRegistro(fstream &arq, int index){

  Registro registro = returnRegistro(arq, index); // Criação do objeto registro.

  cout << endl;
  cout << "Id: " << registro.review_id << endl;
  //cout << "Text: " << registro.review_text << endl;
  cout << "UpVotes: " << registro.upvotes << endl;
  cout << "Version: " << registro.app_version << endl;
  cout << "Date: " << registro.posted_date << endl;
  cout << "----------------------------------------------------------------" << endl;
  cout << endl;
}

/**
 * @brief          Função utilizada para escrever o registro no arquivo de saída.
 * 
 * @param arq      Arquivo binário para leitura.
 * @param outFile  Arquivo de saída em formato .txt. 
 * @param index    Indice para a cessar o registro.
 */
void Review::acessaRegistroEscreveArquivo(fstream &arq, ofstream &outFile, int index){
  
  Registro registro = returnRegistro(arq, index); // Criação do objeto registro.

  outFile << endl;
  outFile << "Id: " << registro.review_id << endl;
  //outFile << "Text: " << registro.review_text << endl;
  outFile << "UpVotes: " << registro.upvotes << endl;
  outFile << "Version: " << registro.app_version << endl;
  outFile << "Date: " << registro.posted_date << endl;
  outFile << "----------------------------------------------------------------" << endl;
  outFile << endl;
}

//? Módulo de Teste --------------------------------------------------------------------------
/**
 * @brief          Módulo de teste para escrita e acesso dos registros no arquivo binário.
 * 
 * @param arqBin   Arquivo binário para leitura.
 * @param outFile  Arquivo de saída em modo .txt 
 */
void Review::testeImportacao(fstream &arqBin, int intervalo){
  int escolha;
  int limit;
  int cont = 0;

  cout << "\nDigite:\n1 - Saída no console\n2 - Saída em arquivo" << endl;
  cin >> escolha;

  while(escolha != 1 && escolha != 2){
    cout << "Digite uma opção válida:\n1 - Saída em console\n2 - Saída no arquivo" << endl;
    cin >> escolha;
  }

  if(escolha == 1){
    cout << "\n-> SAÍDA NO CONSOLE <-\n" << endl;
    limit = 10;

    for(int i = 0; i < limit; i++) {
      acessaRegistro( arqBin, (rand() % ( intervalo - 1 ) ));
    }
    cout << endl;
  } else {
    cout << "\n -> SAÍDA EM ARQUIVO <-\n" << endl;
    limit = 100;
    ofstream outFile(dirArq+"/testeImportacao.txt");

    if(!arqBin.is_open() || !outFile.is_open()){
      cout << "ERRO: Um dos arquivos não está aberto corretamente!" << endl;
      return; 
    } 

    for(int i = 0; i < limit; i++){
      acessaRegistroEscreveArquivo(arqBin, outFile, (rand() % ( intervalo -1 ) ));
    }
    cout << "------------------------------------------------------------------------" << endl;
    cout << "Escrito no arquivo que está no mesmo diretório do arquivo .csv original!" << endl;
    cout << "------------------------------------------------------------------------\n" << endl;
    outFile.close();
  }
}

//? Menu Iniciar -----------------------------------------------------------------------------
/**
 * @brief      Função que implementa o menu iniciar.
 * 
 * @param arq  Arquivo binário para leitura. 
 */
void Review::iniciar(fstream &arq, int limit){

  bool continua = true;

  do{
    cout << "Digite uma das opções abaixo:" << endl;
    cout << endl;
    cout << "  Operações da etapa 1: " << endl;
    cout << "    1 - Acessar Registro" << endl;
    cout << "    2 - Testar Importação" << endl;
    cout << endl;
    cout << "  Operações da etapa 2: " << endl;
    cout << "    3 - Ordenação" << endl;
    cout << "    4 - Hash" << endl;
    cout << "    5 - Módulo de Teste" << endl;
    cout << endl;
    cout << "  Outras operações: " << endl;
    cout << "    6 - Sair do Programa" << endl;
    cout << "Escolha: " << endl;
    
    int escolha;
    cin >> escolha;

    cout << "------------------------------------------------------------------------------" << endl;

    while(escolha < 1 || escolha > 6){
      cout << "Digite uma das opções abaixo:" << endl;
      cout << endl;
      cout << "  Operações da etapa 1: " << endl;
      cout << "    1 - Acessar Registro" << endl;
      cout << "    2 - Testar Importação" << endl;
      cout << endl;
      cout << "  Operações da etapa 2: " << endl;
      cout << "    3 - Ordenação" << endl;
      cout << "    4 - Hash" << endl;
      cout << "    5 - Módulo de Teste" << endl;
      cout << endl;
      cout << "  Outras operações: " << endl;
      cout << "    6 - Sair do Programa" << endl;
      cout << "Escolha: " << endl;
      cin >> escolha;
      cout << "------------------------------------------------------------------------------" << endl;
    }

    if(escolha == 1){
      int i;
      do{
        cout << "\nDigite um índice válido para acessar o registro:" << endl;
        cin >> i;
      }while(i<=0 || i>limit);
      acessaRegistro(arq, i);
    }
    else{
      if(escolha == 2){
        testeImportacao(arq, limit);
      }
      else{
        if(escolha == 3){
          Ordenacao *o;
          o->medeDesempenho(dirArq, arq, 10);
        }
        else{
          if(escolha == 4){
            //Hash *h;
            //h->versionFrequentes(dirArq, arq);
          }
          else{
            if(escolha == 5){

            }
            else{
              continua = false;
            }
          }
        }
      }
    } 
  } while (continua); 

}


//? Funções Auxiliares -----------------------------------------------------------------------
/**
 * @brief        Função utilizada para escrever a registro no arquivo binário.
 * 
 * @param arq    Arquivo binário no qual será feita a escrita das registros.
 * @param registro  Objeto do tipo registro que será armazenado.
 */
void Review::escreveRegistro(fstream &arq, Registro registro, char text[]){

  // Verificando se o arquivo foi aberto corretamente.
  if(!arq.is_open()){
    cout << "ERRO: O arquivo não foi aberto corretamente." << endl;
    return;
  }
  else{
    fstream arqText(dirArq+"/tiktok_app_reviews_text.bin", ios:: out | ios::binary);
    arq.write(reinterpret_cast<const char*>(text), 50);
    arqText.close();
    arq.write(reinterpret_cast<const char*>(&registro), sizeof(registro));  // Realiza a escrita da registro no arquivo binário.
  }
}

/**
 * @brief          Função responsável por criar o objeto do tipo registro.
 * 
 * @param arq      Arquivo binário no qual será escrito a registro.
 * @param id       Id do review.
 * @param text     Texto do review.
 * @param votes    Votos favoráveis.
 * @param version  Versão do app.
 * @param date     Data da postagem.
 */
void Review::criaRegistro(fstream &arq, char id[], char text[], int votes, char version[], char date[]){
  try{
    Registro registro;

    // Armazenando os valores passados como parâmetro nos campos da registro.
    strcpy(registro.review_id, id);
    strcpy(registro.review_text, text);
    registro.upvotes = votes;
    strcpy(registro.app_version, version);
    strcpy(registro.posted_date, date);

    escreveRegistro(arq, registro, text);  // Chamada da função para escrever a registro criada no arquvio binário.
  }
  catch(const exception& e){
    cout << "ERRO: Algo deu errado ao criar a registro." << endl;
    cout << e.what() << '\n';
  }
}

/**
 * @brief  Função responsável por ler um trecho específico do arquivo binário
 *         e retornar o objeto do tipo registro correspondente.
 * 
 * @param arq     Arquivo binário para a leitura.
 * @param index   Índice do review a ser lido.
 * @return registro  Objeto a ser retornado. 
 */
Registro Review::returnRegistro(fstream &arq, int index){
  Registro registro;

  // Se o arquivo não foi aberto, retorna Null.
  if(!arq.is_open()){
    cout << "ERRO! O arquivo não foi aberto corretamente." << endl;
    return registro;
  }
  // Caso contrário, leia o arquivo e retorna a registro.
  else{
    arq.seekg(0);                              // Posicionando ponteiro no início do arquivo.
    arq.seekg(index*sizeof(Registro), ios::beg);  // Posicionando ponteiro no índice desejado.

    arq.read(reinterpret_cast <char*>(&registro), sizeof(Registro));  // Leitura da registro.

    return registro;
  }
}
