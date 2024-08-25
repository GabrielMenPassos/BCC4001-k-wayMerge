#include <iostream>
#include <cstdlib>
#include <ctime>
#include "./big_file/big_file.h"
#include "./ordenacaoExterna/ordenacao.h"
#include "./big_file/big_file.cpp"
#include "./ordenacaoExterna/ordenacao.cpp"

using namespace std;

int main(int argc, char** argv) {
    unsigned long int qtdRegistros;
    unsigned long int maximoMemoria;
    unsigned long int tamBufferSaida;
    int div;
    clock_t tempo;

    if (argc != 4) {
        qtdRegistros = 512000;
        maximoMemoria = 67108864;
        tamBufferSaida = maximoMemoria / 2;
    } else {
        qtdRegistros = static_cast<unsigned long int>(atoi(argv[1]));
        maximoMemoria = static_cast<unsigned long int>(atoi(argv[2]));
        div = atoi(argv[3]);
        tamBufferSaida = maximoMemoria / div;
    }

    cout << "----------------------------------------------\n";
    cout << "-               INICIANDO                    -\n";
    cout << "----------------------------------------------\n\n";

    cout << "[1]: Gerando arquivo de dados 'teste.dat'...\n";
    gerar_array_iv("teste.dat", qtdRegistros, 42);

    cout << "[2]: Executando ordenação externa...\n";
    cout << "   - Configurações:\n";
    cout << "      > Quantidade de registros: " << qtdRegistros << endl;
    cout << "      > Memória máxima: " << maximoMemoria << " bytes\n";
    cout << "      > Tamanho do buffer de saída: " << tamBufferSaida << " bytes\n";
    
    tempo = clock();
    ordenacao_externa("teste.dat", maximoMemoria, tamBufferSaida, "saida");
    tempo = clock() - tempo;

    cout << "\n[3]: Ordenação finalizada! Verificando integridade...\n";

    int resposta = isSaidaOrdenada("saida");
    cout << "\n----------------------------------------------\n";
    if (resposta) {
        cout << "- Resultado: O arquivo 'saida' está ordenado -\n";
    } else {
        cout << "- Resultado: O arquivo 'saida' NÃO está ordenado -\n";
    }
    cout << "----------------------------------------------\n";

    cout << "\nTempo total: " << static_cast<float>(tempo) / CLOCKS_PER_SEC << " segundos\n";

    cout << "\n---------- FINALIZAÇÃO ----------\n";

    return 0;
}
