#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <cstring>
#include <ctime>
#include <string.h>
#include "../headers/review.h"
#include "../headers/track.h"

using namespace std;

Review::Review(){
  
}

Review::Review(string dir){
  dirArq = dir;
}

Review::~Review(){

}

/**
 * @brief  Função responsável por entrar no repositório passado como argumento da main 
 *         e verificar a existência do arquivo tiktok_app_reviews.bin.
 * 
 */
void Review::verificaDir(){
  cout << "Iniciando verificação do diretório:" << endl;
  cout << endl;

  string dirReview = dirArq + "/tiktok_app_reviews.bin";

  ifstream reviewBin(dirReview, ios::in);

  // Se o arquivo tiktok_app_reviews.bin não existe, inicia-se o processamento do tiktok_app_reviews.csv.
  if(!reviewBin.is_open()){
    cout << "O arquivo 'tiktok_app_reviews.bin' ainda não existe." << endl;
    cout << "Iniciando processamento e criação do arquivo binário:" << endl;

    processaReview();  // Chamada da função de processamento do arquivo .csv.

    fstream arq(dirArq+"/tiktok_app_reviews.bin", ios::in | ios::binary);  // Abertura do arquivo .bin para leitura.
    arq.seekg(0);  // Posicionando o ponteiro do arquivo no primeiro byte.
    iniciar(arq);  // Chamada do menu iniciar.
    arq.close();   // Fechando arquivo .bin.
  }
  // Caso contrário, abre o arquivo binário e segue para a próxima etapa.
  else{
    cout << "O arquivo 'tiktok_app_reviews.bin' já existe." << endl;

    fstream arq(dirArq+"/tiktok_app_reviews.bin", ios::in | ios::binary);  // Abertura do arquivo .bin para leitura.
    arq.seekg(0);  // Posicionando o ponteiro do arquivo no primeiro byte.
    iniciar(arq);  // Chamada do menu iniciar.
    arq.close();   // Fechando arquivo .bin.
  }
}

/**
 * @brief  Função utilizada para fazer o processamento do arquivo .csv
 * 
 */
void Review::processaReview(){
  cout << "Iniciando processamento do arquivo .csv:" << endl;

  string dirReview = dirArq + "/tiktok_app_reviews.csv";

  // Realizando a abertura dos arquivos para as devidas manipulações.
  fstream arq1(dirReview, ios::in | ios::out | ios::app);
  fstream arq2(dirArq+"/tiktok_app_reviews.bin", ios:: out | ios::binary);

  arq1.seekg(0);  // Posicionando o ponteiro no início do arquivo .csv.
  arq2.seekg(0);  // Posicionando o ponteiro no início do arquivo .bin.

  string line = "";  // String para capturar a linha inteira do arquivo arq1.
  string aux = "";   // String auxiliar para armazenar o caractere ','.

  // String auxiliares para armazenar os campos de criação da track.
  string idAux = "";   
  string textAux = "";
  string votesAux = "";
  string versionAux = "";
  string dateAux = "";

  // Vetores de char para fazer a criação do objeto do tipo Track.
  char id[100];
  char text[2000];
  char votes[5];
  char version[7];
  char date[20];

  long int cont = 0;  // Contador para verificar a linha a ser lida.

  while(arq1.good() && cont < 55000){
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

    // Armazenando as informações das strings nos vetores de char correspondentes para a criação da track.
    strcpy(id, idAux.c_str());
    strcpy(text, textAux.c_str());
    strcpy(votes, votesAux.c_str());
    strcpy(version, versionAux.c_str());
    strcpy(date, dateAux.c_str());

    criaTrack(arq2, id, text, votes, version, date);  // Chamada a função para criar o objeto track.
    cont++;
  }

  arq1.close();  // Fechando arq1.
  arq2.close();  // Fechando arq2.

  cout << endl;
  cout << "Arquivo processado com sucesso!" << endl;
  cout << "----------------------------------------------------------------------------------" << endl;
  cout << endl;
}

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
  
  Track track = returnTrack(arq, index); // Criação do objeto track.

  cout << "Imprimindo review selecionado: " << endl;
  cout << endl;
  cout << "Id: " << track.review_id << endl;
  cout << "Text: " << track.review_text << endl;
  cout << "UpVotes: " << track.upvotes << endl;
  cout << "Version: " << track.app_version << endl;
  cout << "Date: " << track.posted_date << endl;
  cout << "----------------------------------------------------------------" << endl;
  cout << endl;
}

void Review::testeImportacao(ifstream *arqBin, ofstream *outFile){
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
    cout << "Saída no console (n = 10)" << endl;
    limit = 10;
  } else {
    cout << "Saída em arquivo (n = 100)" << endl;
    limit = 100;
  }
}

void Review::iniciar(fstream &arq){

  bool continua = true;

  do{
    cout << "Digite uma das opções abaixo: " << endl;
    cout << "1 - Acessar Registro" << endl;
    cout << "2 - Testar Importação" << endl;
    cout << "3 - Sair do programa" << endl;
    
    int escolha;
    cin >> escolha;

    while(escolha < 1 || escolha > 3){
      cout << "Digite uma opção válida:\n1 - Acessar Registro\n2 - Testar Importação\n3 - Sair do programa" << endl;
      cin >> escolha;
    }

    if(escolha == 1){
      int i;
      cout << "\nDigite o índice do parâmetro" << endl;
      cin >> i;
      acessaRegistro(arq, i); // imprime na tela
    } else if (escolha == 2) {
      //testeImportacao(); Descomentar essa linha depois.
    } else {
      continua = false;
    }
  } while (continua);  

}

/**
 * @brief        Função utilizada para escrever a track no arquivo binário.
 * 
 * @param arq    Arquivo binário no qual será feita a escrita das tracks.
 * @param track  Objeto do tipo Track que será armazenado.
 */
void Review::escreveTrack(fstream &arq, Track track){

  // Verificando se o arquivo foi aberto corretamente.
  if(!arq.is_open()){
    cout << "ERRO: O arquivo não foi aberto corretamente." << endl;
    return;
  }
  else{
    arq.write(reinterpret_cast<const char*>(&track), sizeof(Track));  // Realiza a escrita da track no arquivo binário.
  }
}


/**
 * @brief          Função responsável por criar o objeto do tipo Track.
 * 
 * @param arq      Arquivo binário no qual será escrito a track.
 * @param id       Id do review.
 * @param text     Texto do review.
 * @param votes    Votos favoráveis.
 * @param version  Versão do app.
 * @param date     Data da postagem.
 */
void Review::criaTrack(fstream &arq, char id[], char text[], char votes[], char version[], char date[]){
  try{
    Track track;

    // Armazenando os valores passados como parâmetro nos campos da track.
    strcpy(track.review_id, id);
    strcpy(track.review_text, text);
    strcpy(track.upvotes, votes);
    strcpy(track.app_version, version);
    strcpy(track.posted_date, date);

    escreveTrack(arq, track);  // Chamada da função para escrever a track criada no arquvio binário.
  }
  catch(const exception& e){
    cout << "ERRO: Algo deu errado ao criar a track." << endl;
    cout << e.what() << '\n';
  }
}

/**
 * @brief  Função responsável por ler um trecho específico do arquivo binário
 *         e retornar o objeto do tipo Track correspondente.
 * 
 * @param arq     Arquivo binário para a leitura.
 * @param index   Índice do review a ser lido.
 * @return Track  Objeto a ser retornado. 
 */
Track Review::returnTrack(fstream &arq, int index){
  Track track;

  // Se o arquivo não foi aberto, retorna Null.
  if(!arq.is_open()){
    cout << "ERRO! O arquivo não foi aberto corretamente." << endl;
    return track;
  }
  // Caso contrário, leia o arquivo e retorna a track.
  else{
    arq.seekg(0);                              // Posicionando ponteiro no início do arquivo.
    arq.seekg(index*sizeof(Track), ios::beg);  // Posicionando ponteiro no índice desejado.

    arq.read(reinterpret_cast <char*>(&track), sizeof(Track));  // Leitura da track.

    return track;
  }
}
