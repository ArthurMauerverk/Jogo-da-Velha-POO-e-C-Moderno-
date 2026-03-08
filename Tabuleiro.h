#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <array>
#include <iostream>

class Tabuleiro {
private:
    std::array<char, 9> posicoes;  // Matriz 3x3 representada como vetor de 9 posições
    int jogadasRealizadas;

public:
    Tabuleiro();
    
    bool aplicarJogada(int posicao, char simbolo);
    char verificarVencedor() const;
    bool isCheio() const;
    void imprimir() const;
    void reset();
    
    // Métodos para persistência (getters)
    const std::array<char, 9>& getPosicoes() const { return posicoes; }
    void setPosicoes(const std::array<char, 9>& novasPosicoes);
    int getJogadasRealizadas() const { return jogadasRealizadas; }
};

#endif