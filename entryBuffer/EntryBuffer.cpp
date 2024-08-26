#include "EntryBuffer.h"
#include <cstdio>
#include <cstdlib>

EntryBuffer::EntryBuffer(const char *nome_arquivo, size_t num_registros)
    : posicao_atual(0), tamanho_buffer(num_registros), buffer_vazio(false)
{
    arquivo = fopen(nome_arquivo, "rb");
    if (!arquivo)
    {
        perror("Erro ao abrir arquivo de entrada");
        exit(1);
    }
    buffer.resize(num_registros);
    carregar_buffer();
}

EntryBuffer::~EntryBuffer()
{
    if (arquivo)
    {
        fclose(arquivo);
    }
}

void EntryBuffer::carregar_buffer()
{
    size_t lidos = fread(buffer.data(), sizeof(ITEM_VENDA), tamanho_buffer, arquivo);
    posicao_atual = 0;
    if (lidos < tamanho_buffer)
    {
        buffer.resize(lidos);
        buffer_vazio = true;
    }
}

ITEM_VENDA EntryBuffer::proximo()
{
    return buffer[posicao_atual];
}

ITEM_VENDA EntryBuffer::consumir()
{
    ITEM_VENDA item = buffer[posicao_atual];
    posicao_atual++;
    if (posicao_atual >= buffer.size())
    {
        carregar_buffer();
    }
    return item;
}

bool EntryBuffer::vazio()
{
    return buffer_vazio && posicao_atual >= buffer.size();
}
