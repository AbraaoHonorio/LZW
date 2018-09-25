#ifndef FUNCOES_H_
#define FUNCOES_H_

#include <iostream>
#include <vector>

using namespace std;

class Tabela{

public:

    Tabela();
    void inserir(string palavra);
    void imprimirTudo();
    void imprimiPosicao(unsigned int posicao);
    int verificarExistencia(string palavraDesejada);

    unsigned int tamanho;
    vector<string> biblioteca;

};


#endif
