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
    /*for(int i = 20; i <= 126; i++ ){
        aux = (char)i;
        tabela->inserir(aux);
    }

    tabela->inserir("\n");*/
    for(int i = 0; i <= 255; i++){
        aux = (char)i;
        tabela->inserir(aux);
    }

    ///salvo a entrada em uma variavel
    vector<char> conteudo((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));

    int posicao = 0, posicaoProximo = 0, posicaoNaTabela = 0;
    string letrasAtual = "", proximos = "";
    bool naoTemOsProximos = false;

    string saida = "", posicaoBinario = "";
    unsigned int quantBitsParaONumero = 0;

    ///loop principal
    while( posicao < (int)conteudo.size() )
    {
        //cout << "i: " << posicao << " ";
        letrasAtual = conteudo.at(posicao);
        //cout << "letra: " << letrasAtual << endl;

        ///procuro, se tenho a letra atual, mais o proximo
        naoTemOsProximos = false;

        while( !naoTemOsProximos ){

            if( (posicao + 1) < (int)conteudo.size() ){

                posicaoProximo = posicao + 1;
                proximos = letrasAtual + conteudo.at(posicaoProximo);

                //cout << "procurando: " << proximos << endl;

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

        //cout << "palavra a procurar: " << letrasAtual << endl;

        posicaoNaTabela = tabela->verificarExistencia(letrasAtual);
        quantBitsParaONumero = 1 + trunc(log2(tabela->tamBibli()-1));

        posicaoBinario = bitset<64>(posicaoNaTabela).to_string();

        posicaoBinario = posicaoBinario.substr(64-quantBitsParaONumero);

        //cout << "posicaoNaTabela: " << posicaoNaTabela;
        //cout << "\nquantBitsParaONumero: " << quantBitsParaONumero;
        //cout << "\nposicaoBinario: " << posicaoBinario << endl;

        saida += posicaoBinario;


        if( (posicao + 1) < (int)conteudo.size() ){
            proximos = letrasAtual + conteudo.at(posicaoProximo);
            tabela->inserir(proximos);
            //cout << "inserindo: " << proximos << endl;
        }

        ///e verifico a aucasiao especial
        posicao++;
    }

    cout << saida << endl;

    ofstream fileCodificado;
    fileCodificado.open("codificado.teoria", std::ios::binary);
    string auxVar, test;
    int intVar, is = 0;
    char charVar;

    for(is = 0; is < (saida.size() - 8); is += 8){

        auxVar = saida.substr(is,8);
        intVar = stoi(auxVar,nullptr,2);
        charVar = intVar;
        fileCodificado << charVar;

    }

    int bitsQueSobra = ( (trunc(saida.size() / 8)) * 8) - (saida.size() % 8);

    if( bitsQueSobra != ( (trunc(saida.size() / 8)) * 8) ){

        string auxSobra = "";
        int diferenca = ( (trunc(saida.size() / 8)) * 8) - bitsQueSobra;

        for(int i = 0; i < (8 - diferenca); i++)
            auxSobra += "0";

        auxSobra += saida.substr((trunc(saida.size() / 8)) * 8);

        intVar = stoi(auxSobra,nullptr,2);
        charVar = intVar;

        fileCodificado << charVar;

    }
    else{
        auxVar = saida.substr(saida.size()-8);
        intVar = stoi(auxVar,nullptr,2);
        charVar = intVar;
        fileCodificado << charVar;
    }
    //0's 48 -> 53

    cout << "Arquivo Codificado!!!" << endl;

    fileCodificado.close();

    return 0;

}
