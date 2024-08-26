#include <iostream>
#include <cstdlib>
#include <ctime>
#include "./BigFile/BigFile.h"
#include "./ExternalSorting/Sorting.h"
#include "./BigFile/BigFile.cpp"
#include "./ExternalSorting/Sorting.cpp"

using namespace std;

int main(int argc, char **argv)
{
    unsigned long int entriesAmount;
    unsigned long int memoryLimit;
    unsigned long int outBufferSize;
    int div;
    clock_t time;

    if (argc != 4)
    {
        entriesAmount = 512000;
        memoryLimit = 67108864;
        outBufferSize = memoryLimit / 2;
    }
    else
    {
        entriesAmount = static_cast<unsigned long int>(atoi(argv[1]));
        memoryLimit = static_cast<unsigned long int>(atoi(argv[2]));
        div = atoi(argv[3]);
        outBufferSize = memoryLimit / div;
    }

    array_generator("teste.dat", entriesAmount, 42);
    cout << "Arquivo de teste gerado\n";

    time = clock();
    ordenacao_externa("teste.dat", memoryLimit, outBufferSize, "saida");
    time = clock() - time;

    cout << "\nTempo total da ordenação: " << static_cast<float>(time) / CLOCKS_PER_SEC << " segundos\n";

    return 0;
}
