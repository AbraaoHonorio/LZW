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

    string nomeArquivo = "";
    cout << "Insira o nome do arquivo a ser compactado: ";
    getline(cin, nomeArquivo);

    //abrindo o arquivo desejado!
    ifstream input(nomeArquivo.c_str(), std::ios::binary);
    ofstream nomeArquivoParaDecodificador;

    if(!input)
    {
        cout << "nao encontrou o arquivo" << endl;
        return -1;
    }else{
        nomeArquivoParaDecodificador.open("nomeArquivoParaDecodificador.txt");
        nomeArquivoParaDecodificador << nomeArquivo.c_str() << "\n";

    }

    string aux = "";

    ///insero na tabela a biblioteca
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
        //pego a ultima letra lida
        letrasAtual = conteudo.at(posicao);
        
        naoTemOsProximos = false;

        while( !naoTemOsProximos ){

            if( (posicao + 1) < (int)conteudo.size() ){

                posicaoProximo = posicao + 1;
                proximos = letrasAtual + conteudo.at(posicaoProximo);

                //verifico se a tabela possui a proxima letra,
                //se sim, continuo no while e procuro a seguinte da proxima
                //senão saiu do while, com o máximo de palavras encontradas
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

        //pego a posicao da palavra na tabela,
        // e vejo quantos bits que ela irá utilizar no arquivo
        posicaoNaTabela = tabela->verificarExistencia(letrasAtual);
        quantBitsParaONumero = 1 + trunc(log2(tabela->tamBibli()-1));
        // transformo a posicao em binario
        posicaoBinario = bitset<64>(posicaoNaTabela).to_string();
        // retiro os bits, desnecessários
        posicaoBinario = posicaoBinario.substr(64-quantBitsParaONumero);
        // salvo na variavel que irá guardar a codificacao
        saida += posicaoBinario;

        //verifico se o arquivo ainda nao acabou
        if( (posicao + 1) < (int)conteudo.size() ){
            proximos = letrasAtual + conteudo.at(posicaoProximo); //pego o proxima letra
            tabela->inserir(proximos); //e adiciono na tabela
        }


        posicao++;
    }
    
    //salvando no arquivo...

    ofstream fileCodificado;
    fileCodificado.open("codificado.teoria", std::ios::binary);
    string auxVar, test;
    int intVar, is = 0;
    char charVar;

    //loop que salva no arquivo em 8 em 8 bits, mais nao o ultimo
    for(is = 0; is < (saida.size() - 8); is += 8){

        auxVar = saida.substr(is,8);
        intVar = stoi(auxVar,nullptr,2);
        charVar = intVar;
        fileCodificado << charVar;

    }

    int bitsQueSobra = ( (trunc(saida.size() / 8)) * 8) - (saida.size() % 8);

    //verifico que, o que sobrou não é divisivel por 8
    //se sim, salvo com '0's adicionados na esquerda
    //senão, salvo normalmente o byte
    if( bitsQueSobra != ( (trunc(saida.size() / 8)) * 8) ){

        string auxSobra = "";
        int diferenca = ( (trunc(saida.size() / 8)) * 8) - bitsQueSobra;

        for(int i = 0; i < (8 - diferenca); i++)
            auxSobra += "0";

        auxSobra += saida.substr((trunc(saida.size() / 8)) * 8);

        intVar = stoi(auxSobra,nullptr,2);
        charVar = intVar;

        fileCodificado << charVar;
        nomeArquivoParaDecodificador << (8 - diferenca);
    }
    else{
        auxVar = saida.substr(saida.size()-8);
        intVar = stoi(auxVar,nullptr,2);
        charVar = intVar;
        fileCodificado << charVar;
        nomeArquivoParaDecodificador << 0;
    }
    
    cout << "Arquivo Codificado!!!" << endl;

    fileCodificado.close();
    nomeArquivoParaDecodificador.close();

    return 0;

}
