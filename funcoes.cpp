#include "funcoes.h"

Tabela::Tabela(){

    this->tamanho = 0;
}

void Tabela::inserir(string palavra){

    this->biblioteca.push_back(palavra);
    this->tamanho++;
}

void Tabela::imprimirTudo(){

    unsigned int posicao = 0;
    for(auto i = this->biblioteca.begin(); i != this->biblioteca.end(); ++i){

        cout << "posicao " << posicao << " : " << *i << endl;
        posicao++;
    }

}

void Tabela::imprimiPosicao(unsigned int posicao){

    if(posicao <= (this->tamanho - 1) )
        cout << "posicao " << posicao << " : " << this->biblioteca.at(posicao) << endl;
    else
        cout << "valor da posicao desejada, nao existe" << endl;
}

int Tabela::verificarExistencia(string palavraDesejada){

    int posicao = 0;
    bool encontrou = false;

    for(auto i = this->biblioteca.begin(); i != this->biblioteca.end(); ++i){

        if(palavraDesejada.compare(*i) == 0){
            encontrou = true;
            break;
        }

        posicao++;
    }

    if(!encontrou){
        return -1;
    }

    return posicao;

}

unsigned int Tabela::tamBibli(){
    return this->tamanho;
}
