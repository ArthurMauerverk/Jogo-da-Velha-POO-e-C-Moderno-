#include "MotorJogo.h"
#include <iostream>


#include "JogadorHumano.h"
#include "JogadorComputador.h"

MotorJogo::MotorJogo() : jogadorAtual(0), jogoAtivo(false), modoJogo(1) {}

void MotorJogo::iniciarPartida(int modo, bool humanoInicia) {
    modoJogo = modo;
    jogadores.clear();  // unique_ptr libera automaticamente a memória
    
    // Criar jogadores baseado no modo - TODOS com make_unique (sem new/delete)
    switch(modo) {
        case 1: // Humano vs Humano
            jogadores.push_back(std::make_unique<JogadorHumano>('X', "Jogador 1"));
            jogadores.push_back(std::make_unique<JogadorHumano>('O', "Jogador 2"));
            break;
            
        case 2: // Humano vs Computador
            if (humanoInicia) {
                jogadores.push_back(std::make_unique<JogadorHumano>('X', "Humano"));
                jogadores.push_back(std::make_unique<JogadorComputador>('O', "Computador", 2));
            } else {
                jogadores.push_back(std::make_unique<JogadorComputador>('X', "Computador", 2));
                jogadores.push_back(std::make_unique<JogadorHumano>('O', "Humano"));
            }
            break;
            
        case 3: // Computador vs Computador
            jogadores.push_back(std::make_unique<JogadorComputador>('X', "Computador 1", 2));
            jogadores.push_back(std::make_unique<JogadorComputador>('O', "Computador 2", 2));
            break;
    }
    
    tabuleiro.reset();
    jogadorAtual = 0;
    jogoAtivo = true;
}

void MotorJogo::alternarTurno() {
    jogadorAtual = (jogadorAtual + 1) % jogadores.size();
}

void MotorJogo::executarTurno() {
    if (!jogoAtivo || jogadores.empty()) return;
    
    tabuleiro.imprimir();
    
    // Chamada polimórfica - cada tipo de jogador implementa seu próprio método
    Jogador* jogador = jogadores[jogadorAtual].get();
    int posicao = jogador->escolherJogada(tabuleiro);
    
    if (tabuleiro.aplicarJogada(posicao, jogador->getSimbolo())) {
        if (!verificarFimDeJogo()) {
            alternarTurno();
        }
    } else {
        std::cout << "Jogada inválida! Tente novamente.\n";
    }
}

bool MotorJogo::verificarFimDeJogo() {
    char vencedor = tabuleiro.verificarVencedor();
    
    if (vencedor != ' ') {
        // Atualizar pontuação do vencedor
        for (auto& jogador : jogadores) {
            if (jogador->getSimbolo() == vencedor) {
                jogador->incrementarPontuacao();
            }
        }
        jogoAtivo = false;
        return true;
    }
    
    if (tabuleiro.isCheio()) {
        jogoAtivo = false;  // Empate
        return true;
    }
    
    return false;
}

void MotorJogo::exibirResultado() const {
    char vencedor = tabuleiro.verificarVencedor();
    
    if (vencedor != ' ') {
        for (const auto& jogador : jogadores) {
            if (jogador->getSimbolo() == vencedor) {
                std::cout << "\n🏆 " << jogador->getNome() << " venceu! 🏆\n";
                return;
            }
        }
    } else {
        std::cout << "\n🤝 Empate! 🤝\n";
    }
}

void MotorJogo::reset() {
    tabuleiro.reset();
    jogadorAtual = 0;
    jogoAtivo = false;
}