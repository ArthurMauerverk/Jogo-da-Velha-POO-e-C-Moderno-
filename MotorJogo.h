#ifndef MOTORJOGO_H
#define MOTORJOGO_H

#include <memory>
#include <vector>
#include "Tabuleiro.h"
#include "Jogador.h"

class MotorJogo {
private:
    Tabuleiro tabuleiro;
    std::vector<std::unique_ptr<Jogador>> jogadores;
    int jogadorAtual;
    bool jogoAtivo;
    int modoJogo;
    int nivelBot;

public:
    MotorJogo();
    
    // Método atualizado para receber o nível
    void iniciarPartida(int modo, bool humanoInicia = true, int nivel = 2);
    void alternarTurno();
    void executarTurno();
    bool verificarFimDeJogo();
    void exibirResultado() const;
    void reset();
    
    // Getters/Setters
    const Tabuleiro& getTabuleiro() const { return tabuleiro; }
    void setTabuleiro(const Tabuleiro& tab) { tabuleiro = tab; }
    
    const std::vector<std::unique_ptr<Jogador>>& getJogadores() const { return jogadores; }
    void setJogadores(std::vector<std::unique_ptr<Jogador>> novosJogadores) {
        jogadores = std::move(novosJogadores);
    }
    
    int getJogadorAtual() const { return jogadorAtual; }
    void setJogadorAtual(int jogador) { jogadorAtual = jogador; }
    
    bool isJogoAtivo() const { return jogoAtivo; }
    void setJogoAtivo(bool ativo) { jogoAtivo = ativo; }
    
    int getModoJogo() const { return modoJogo; }
    void setModoJogo(int modo) { modoJogo = modo; }
};

#endif