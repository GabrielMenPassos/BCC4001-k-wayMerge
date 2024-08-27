#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include "../entryBuffer/EntryBuffer.h"
#include "../outputBuffer/OutputBuffer.h"
#include "../entryBuffer/EntryBuffer.cpp"
#include "../outputBuffer/OutputBuffer.cpp"

// Realiza a intercalação k-vias de buffers de entrada em um buffer de saída.
// Seleciona o menor elemento entre os buffers de entrada e o insere no buffer de saída.
// Continua até que todos os buffers de entrada estejam vazios, e então despeja o conteúdo do buffer de saída no arquivo.
void intercalacao_k_vias(std::vector<EntryBuffer *> &buffers_entrada, OutputBuffer &buffer_saida)
{
    int qtd_buffers_vazios = 0;
    int qtd_buffer_entrada = buffers_entrada.size();

    while (qtd_buffers_vazios < qtd_buffer_entrada)
    {
        EntryBuffer *menor = nullptr;
        uint32_t auxmenor = UINT32_MAX;

        qtd_buffers_vazios = 0;
        for (int i = 0; i < qtd_buffer_entrada; i++)
        {
            if (!buffers_entrada[i]->vazio())
            {
                if (auxmenor > buffers_entrada[i]->proximo().id)
                {
                    auxmenor = buffers_entrada[i]->proximo().id;
                    menor = buffers_entrada[i];
                }
            }
            else
            {
                qtd_buffers_vazios++;
            }
        }

        if (auxmenor != UINT32_MAX)
        {
            ITEM_VENDA menor_item = menor->consumir();
            buffer_saida.inserir(menor_item);
        }
    }
    buffer_saida.despejar();
}

// Realiza a ordenação externa de um arquivo grande dividindo-o em partes menores.
// Cada parte é ordenada individualmente e salva em arquivos temporários.
// Em seguida, os arquivos temporários são intercalados para produzir o arquivo de saída ordenado.
void ordenacao_externa(const char *entrada, size_t bytes_registros, size_t bytes_buffer_saida, const char *nome_saida)
{
    FILE *arq = fopen(entrada, "rb");
    fseek(arq, 0, SEEK_END);
    long int e = ftell(arq);
    fclose(arq);

    int k = std::ceil((float)e / bytes_registros);
    size_t qtd_registro_entrada = std::floor(((float)(bytes_registros - bytes_buffer_saida) / k) / sizeof(ITEM_VENDA));

    std::vector<std::string> arquivos_temporarios;
    for (int i = 0; i < k; ++i)
    {
        std::vector<ITEM_VENDA> buffer(bytes_registros / sizeof(ITEM_VENDA));
        FILE *arquivo = fopen(entrada, "rb");
        fseek(arquivo, i * bytes_registros, SEEK_SET);
        size_t lidos = fread(buffer.data(), sizeof(ITEM_VENDA), buffer.size(), arquivo);
        fclose(arquivo);
        buffer.resize(lidos);

        std::sort(buffer.begin(), buffer.end(), [](const ITEM_VENDA &a, const ITEM_VENDA &b)
                  { return a.id < b.id; });

        std::string nome_temp = "temp_" + std::to_string(i) + ".dat";
        FILE *temp = fopen(nome_temp.c_str(), "wb");
        fwrite(buffer.data(), sizeof(ITEM_VENDA), buffer.size(), temp);
        fclose(temp);
        arquivos_temporarios.push_back(nome_temp);
    }

    std::vector<EntryBuffer *> buffers_entrada;
    for (const auto &nome_temp : arquivos_temporarios)
    {
        buffers_entrada.push_back(new EntryBuffer(nome_temp.c_str(), qtd_registro_entrada));
    }
    OutputBuffer buffer_saida(nome_saida, bytes_buffer_saida / sizeof(ITEM_VENDA));

    intercalacao_k_vias(buffers_entrada, buffer_saida);

    for (auto buffer : buffers_entrada)
    {
        delete buffer;
    }

    for (const auto &nome_temp : arquivos_temporarios)
    {
        remove(nome_temp.c_str());
    }
}
