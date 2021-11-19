#ifndef REVIEW_H
#define REVIEW_H

#include <iostream>
#include <string.h>
#include <cstring>
#include <vector>
#include "../headers/track.h"

using namespace std;

class Review{
  public:
    Review();
    Review(string dir);
    ~Review();

    void verificaDir();  // Função para verificar a existência do arquivo binário no diretório.

    // Funções principais -----------------------------------------------
    void processaReview();
    void importaRegistros();
    void acessaRegistro(fstream &arq, int index); // imprime na tela
    void testeImportacao(ifstream *arqBin, ofstream *outFile);
    void iniciar(fstream &arq);
  

    // Funções Auxiliares -----------------------------------------------
    void escreveTrack(fstream &arq, Track track);
    void criaTrack(fstream &arq, char id[], char text[], char votes[], char version[], char date[]);
    Track returnTrack(fstream &arq, int index);

  private:
    string dirArq;       // String para armazenar o diretório do arquivo.
};

#endif
