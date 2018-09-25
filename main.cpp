#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>

#include "funcoes.h"

using namespace std;

int main(int argc, char** argv){

    //Tabela principal
    Tabela *tabela = new Tabela();
    //tabela->inserir("a");
    //cout << tabela->verificarExistencia("ab") << endl;

    string nomeArquivo = "";
    cout << "Insira o nome do arquivo a ser compactado: ";
    getline(cin, nomeArquivo);

    ifstream input(nomeArquivo.c_str(), std::ios::binary);

    if(!input)
    {
        cout << "nao encontrou o arquivo" << endl;
        return -1;
    }else{
        ofstream nomeArquivoParaDecodificador;
        nomeArquivoParaDecodificador.open("nomeArquivoParaDecodificador.txt");
        nomeArquivoParaDecodificador << nomeArquivo.c_str();
        nomeArquivoParaDecodificador.close();
    }

    string aux = "";

    ///insero na tabela a biblioteca
    for(int i = 20; i <= 126; i++ ){
        aux = (char)i;
        tabela->inserir(aux);
    }

    tabela->inserir("\n");

    ///salvo a entrada em uma variavel
    vector<char> conteudo((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));

    int posicao = 0, posicaoProximo = 0, posicaoNaTabela = 0;
    string letrasAtual = "", proximos = "";
    bool naoTemOsProximos = false;

    string saida = "";

    ///loop principal
    while( posicao < (int)conteudo.size() )
    {
        cout << "i: " << posicao << " ";
        letrasAtual = conteudo.at(posicao);
        cout << "letra: " << letrasAtual << endl;

        ///procuro, se tenho a letra atual, mais o proximo
        naoTemOsProximos = false;

        while( !naoTemOsProximos ){

            if( (posicao + 1) < (int)conteudo.size() ){

                posicaoProximo = posicao + 1;
                proximos = letrasAtual + conteudo.at(posicaoProximo);

                if ( tabela->verificarExistencia(proximos) != -1){

                    letrasAtual = proximos;
                    posicao++;

                }else{
                    naoTemOsProximos = true;
                }
            }else{
                naoTemOsProximos = true;
            }
        }

        cout << "palavra a procurar: " << letrasAtual;

        posicaoNaTabela = tabela->verificarExistencia(letrasAtual);
        saida += to_string(posicaoNaTabela);

        cout << " salvou com o cod: " << posicaoNaTabela << endl;

        if( (posicao + 1) < (int)conteudo.size() ){
            proximos = letrasAtual + conteudo.at(posicaoProximo);
            tabela->inserir(proximos);
            cout << "inserindo: " << proximos << endl;
        }

        ///e verifico a aucasiao especial
        posicao++;
    }

    cout << "resultado: " << saida << endl;

    return 0;

}
