#pragma once

#include "../BigFile/BigFile.h"
#include <cstdio>
#include <vector>

class EntryBuffer
{
public:
    EntryBuffer(const char *nome_arquivo, size_t num_registros);
    ~EntryBuffer();

    ITEM_VENDA proximo();
    ITEM_VENDA consumir();
    bool vazio();

private:
    FILE *arquivo;
    std::vector<ITEM_VENDA> buffer;
    size_t posicao_atual;
    size_t tamanho_buffer;
    bool buffer_vazio;

    void carregar_buffer();
};
