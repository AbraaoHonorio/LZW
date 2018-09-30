#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <bitset>

#include "funcoes.h"

using namespace std;

int main(int argc, char** argv){


    string nomeArquivoSaida = "deco_";
    ifstream nomeSaida("nomeArquivoParaDecodificador.txt");

    if(!nomeSaida){

        cout << "nao encontrou nomeArquivoParaDecodificador" << endl;
        return -1;
    }

    string auxEntrada = "";
    int bytesAdicionados = 0;

    getline(nomeSaida,auxEntrada); //pego o nome do arquivo decodificado
    nomeArquivoSaida += auxEntrada;
    getline(nomeSaida,auxEntrada); //pego a quantidades de bits, adicionado na esquerda no ultimo byte
    bytesAdicionados = stoi(auxEntrada,nullptr,10);

    ifstream arquivo("codificado.teoria", std::ios::binary); //abro o arquivo

    if(!arquivo){

        cout << "nao encontrou o arquivo codificado" << endl;
        return -1;
    }

    vector<char> buffer((istreambuf_iterator<char>(arquivo)),(istreambuf_iterator<char>()));
    string auxF, conteudoCodificado = "";

    //salvo os bits na variavel
    for(int i = 0; i < ((int)buffer.size()-1); i++){

        bitset<8> varBit((int)buffer.at(i));
        conteudoCodificado += varBit.to_string();
    }

    //retiro os 0's a esquerda adicionados no ultimo byte
    bitset<8> varBit((int)buffer.at((int)buffer.size()-1));
    string ultimo = varBit.to_string();
    conteudoCodificado += ultimo.substr(bytesAdicionados);

    //criando a biblioteca
    Tabela *tabela = new Tabela();
    string aux = "";
    for(int i = 0; i <= 255; i++){
        aux = (char)i;
        tabela->inserir(aux);
    }

    int posicao = 0, posicaoNaTabela = 0;
    string palavrabin = "", palavraAnterior = "", palavraAtual = "";

    unsigned int quantBitsParaONumero = 0;

    bool inicio = true;

    string saida = "";
    string primeiraLetraAnterior = "";
    string palavraAtualASerInserida = "";
    ofstream fileDecodificado;
    fileDecodificado.open(nomeArquivoSaida.c_str(),std::ios::binary);

    //loop principal;
    while( posicao < conteudoCodificado.size() ) {

        if(!inicio){

            //pego a quantidade de bits, que foi utilizado para aquela palavra
            quantBitsParaONumero = 1 + trunc(log2(tabela->tamBibli()));
            //salvo os bits na variavel
            palavrabin = conteudoCodificado.substr(posicao,quantBitsParaONumero);
            //passo para inteiro
            posicaoNaTabela = stoi(palavrabin,nullptr,2);
            
            //verifico se ela existe na tabela,
            //caso especial
            if(!tabela->retornarPalavra(posicaoNaTabela,&palavraAtual)){
                primeiraLetraAnterior = palavraAnterior[0];
                palavraAtual = palavraAnterior + primeiraLetraAnterior;
            }

            //pego a primeira letra da palavra
            if(palavraAtual.size() > 1)
                palavraAtualASerInserida = palavraAtual.substr(0,1);
            else
                palavraAtualASerInserida = palavraAtual;

            //e salvo somando com a palavra anterior
            tabela->inserir(palavraAnterior + palavraAtualASerInserida);

            palavraAnterior = palavraAtual;
            
            //salvo no arquivo
            fileDecodificado << palavraAnterior;

            //desloco a posicao inicial da proxima palavra
            posicao += quantBitsParaONumero;

        }else{

            inicio = false;

            //pego a quantidade de bits, que foi utilizado para aquela palavra
            quantBitsParaONumero = 1 + trunc(log2(tabela->tamBibli()-1));
            //salvo os bits na variavel
            palavrabin = conteudoCodificado.substr(posicao,quantBitsParaONumero);
            //passo para inteiro
            posicaoNaTabela = stoi(palavrabin,nullptr,2);
            
            //pego a palavra naquela posicao na tabela
            tabela->retornarPalavra(posicaoNaTabela,&palavraAnterior);
            
            //salvo no arquivo
            fileDecodificado << palavraAnterior;

            //desloco a posicao inicial da proxima palavra
            posicao += quantBitsParaONumero;
        }

    }

    fileDecodificado.close();

    cout << "Arquivo decodificado!!!" << endl;

    return 0;

}
