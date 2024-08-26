#include "OutputBuffer.h"
#include <cstdio>
#include <cstdlib>

OutputBuffer::OutputBuffer(const char *nome_arquivo, size_t num_registros)
    : tamanho_buffer(num_registros)
{
    arquivo = fopen(nome_arquivo, "wb");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo de saída");
        exit(1);
    }
    buffer.reserve(num_registros);
}

OutputBuffer::~OutputBuffer()
{
    despejar();
    if (arquivo)
    {
        fclose(arquivo);
    }
}

void OutputBuffer::inserir(const ITEM_VENDA &item)
{
    buffer.push_back(item);
    if (buffer.size() >= tamanho_buffer)
    {
        escrever_buffer();
    }
}

void OutputBuffer::despejar()
{
    if (!buffer.empty())
    {
        escrever_buffer();
    }
}

void OutputBuffer::escrever_buffer()
{
    fwrite(buffer.data(), sizeof(ITEM_VENDA), buffer.size(), arquivo);
    buffer.clear();
}
