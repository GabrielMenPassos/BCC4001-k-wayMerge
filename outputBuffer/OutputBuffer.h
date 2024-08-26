#pragma once

#include "../BigFile/BigFile.h"
#include <cstdio>
#include <vector>

class OutputBuffer
{
public:
    OutputBuffer(const char *nome_arquivo, size_t num_registros);
    ~OutputBuffer();

    void inserir(const ITEM_VENDA &item);
    void despejar();

private:
    FILE *arquivo;
    std::vector<ITEM_VENDA> buffer;
    size_t tamanho_buffer;

    void escrever_buffer();
};
