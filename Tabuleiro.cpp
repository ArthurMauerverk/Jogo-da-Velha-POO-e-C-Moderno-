#include "Tabuleiro.h"
#include <iostream>


Tabuleiro::Tabuleiro() {
    reset();
}

void Tabuleiro::reset() {
    posicoes.fill(' ');
    jogadasRealizadas = 0;
}

bool Tabuleiro::aplicarJogada(int posicao, char simbolo) {
    // Verifica se posição é válida (1-9) e está vazia
    if (posicao < 1 || posicao > 9 || posicoes[posicao - 1] != ' ') {
        return false;
    }
    
    posicoes[posicao - 1] = simbolo;
    jogadasRealizadas++;
    return true;
}

char Tabuleiro::verificarVencedor() const {
    // Verificar linhas
    for (int i = 0; i < 3; i++) {
        if (posicoes[i*3] != ' ' && 
            posicoes[i*3] == posicoes[i*3 + 1] && 
            posicoes[i*3] == posicoes[i*3 + 2]) {
            return posicoes[i*3];
        }
    }
    
    // Verificar colunas
    for (int i = 0; i < 3; i++) {
        if (posicoes[i] != ' ' && 
            posicoes[i] == posicoes[i + 3] && 
            posicoes[i] == posicoes[i + 6]) {
            return posicoes[i];
        }
    }
    
    // Verificar diagonais
    if (posicoes[0] != ' ' && posicoes[0] == posicoes[4] && posicoes[0] == posicoes[8]) {
        return posicoes[0];
    }
    
    if (posicoes[2] != ' ' && posicoes[2] == posicoes[4] && posicoes[2] == posicoes[6]) {
        return posicoes[2];
    }
    
    return ' '; // Sem vencedor
}

bool Tabuleiro::isCheio() const {
    return jogadasRealizadas == 9;
}

void Tabuleiro::imprimir() const {
    std::cout << "\n";
    for (int i = 0; i < 3; i++) {
        std::cout << " ";
        for (int j = 0; j < 3; j++) {
            char c = posicoes[i*3 + j];
            std::cout << (c == ' ' ? '.' : c);
            if (j < 2) std::cout << " | ";
        }
        std::cout << " ";
        if (i < 2) std::cout << "\n---+---+---\n";
    }
    std::cout << "\n\n";
}

void Tabuleiro::setPosicoes(const std::array<char, 9>& novasPosicoes) {
    posicoes = novasPosicoes;
    jogadasRealizadas = 0;
    for (char c : posicoes) {
        if (c != ' ') jogadasRealizadas++;
    }
}