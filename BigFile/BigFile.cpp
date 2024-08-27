#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "BigFile.h"
#include "../prandom.h"

// Troca os valores de duas variáveis do tipo ITEM_VENDA.
// Utiliza uma variável auxiliar para realizar a troca.
static inline void handleChange(ITEM_VENDA &x, ITEM_VENDA &y)
{
    ITEM_VENDA aux;
    aux = x;
    x = y;
    y = aux;
}

// Embaralha os elementos de um array do tipo ITEM_VENDA.
// O embaralhamento ocorre no intervalo entre as posições 'start' e 'end'.
static void shuffle(ITEM_VENDA *v, int start, int end)
{
    int i, j;

    for (i = end - 1; i > start; i--)
    {
        j = (rand_() % (i + 1));
        j = j <= start ? start + 1 : j;
        handleChange(v[i], v[j]);
    }
}

// Gera um array de registros do tipo ITEM_VENDA e os salva em um arquivo binário.
// O arquivo de saída é definido por 'outFile', o número de entradas por 'entriesNumber' e a semente de aleatoriedade por 'seed'.
// Cada registro gerado contém um identificador, id de venda, desconto, data e uma descrição vazia.
void array_generator(const char *outFile, unsigned int entriesNumber, int seed)
{
    ITEM_VENDA *iv;
    FILE *output;
    uint32_t i;
    time_t t = time(NULL);

    srand(seed);

    iv = (ITEM_VENDA *)malloc(sizeof(ITEM_VENDA) * entriesNumber);

    for (i = 0; i < entriesNumber; i++)
    {
        iv[i].id = i;
        iv[i].id_venda = i + (rand() % 2);
        iv[i].desconto = (rand() % 10) / (float)100;
        iv[i].data = t + ((-1 * (rand() % 30)) * 86400);
        iv[i].desc[0] = '\0';
    }

    shuffle(iv, 0, entriesNumber);

    output = fopen(outFile, "wb");
    if (output == NULL)
    {
        exit(1);
    }

    fwrite(iv, sizeof(ITEM_VENDA), entriesNumber, output);
    fclose(output);
    free(iv);

    return;
}
