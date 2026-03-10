#include "MotorJogo.h"
#include "JogadorHumano.h"
#include "BotAleatorio.h"
#include "BotTatico.h"
#include <iostream>

MotorJogo::MotorJogo() : jogadorAtual(0), jogoAtivo(false), modoJogo(1), nivelBot(2) {}

void MotorJogo::iniciarPartida(int modo, bool humanoInicia, int nivel) {
    modoJogo = modo;
    nivelBot = nivel;
    jogadores.clear();
    
    std::cout << "\n🎮 Iniciando partida com nível " << nivel << "...\n";
    
    switch(modo) {
        case 1: // Humano vs Humano
            jogadores.push_back(std::make_unique<JogadorHumano>('X', "Jogador 1"));
            jogadores.push_back(std::make_unique<JogadorHumano>('O', "Jogador 2"));
            std::cout << "Modo: Humano vs Humano\n";
            break;
            
        case 2: // Humano vs Computador
            if (humanoInicia) {
                jogadores.push_back(std::make_unique<JogadorHumano>('X', "Humano"));
                if (nivel == 1) {
                    jogadores.push_back(std::make_unique<BotAleatorio>('O', "Computador (Fácil)"));
                } else {
                    std::string nome = (nivel == 2) ? "Computador (Médio)" : "Computador (Difícil)";
                    jogadores.push_back(std::make_unique<BotTatico>('O', nome, nivel));
                }
            } else {
                if (nivel == 1) {
                    jogadores.push_back(std::make_unique<BotAleatorio>('X', "Computador (Fácil)"));
                } else {
                    std::string nome = (nivel == 2) ? "Computador (Médio)" : "Computador (Difícil)";
                    jogadores.push_back(std::make_unique<BotTatico>('X', nome, nivel));
                }
                jogadores.push_back(std::make_unique<JogadorHumano>('O', "Humano"));
            }
            std::cout << "Modo: Humano vs Computador (Nível " << nivel << ")\n";
            break;
            
        case 3: // Computador vs Computador
            if (nivel == 1) {
                jogadores.push_back(std::make_unique<BotAleatorio>('X', "Bot 1 (Fácil)"));
                jogadores.push_back(std::make_unique<BotAleatorio>('O', "Bot 2 (Fácil)"));
            } else {
                std::string nome1 = (nivel == 2) ? "Bot 1 (Médio)" : "Bot 1 (Difícil)";
                std::string nome2 = (nivel == 2) ? "Bot 2 (Médio)" : "Bot 2 (Difícil)";
                jogadores.push_back(std::make_unique<BotTatico>('X', nome1, nivel));
                jogadores.push_back(std::make_unique<BotTatico>('O', nome2, nivel));
            }
            std::cout << "Modo: Computador vs Computador (Nível " << nivel << ")\n";
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
    
    std::cout << "\n─────────────────────────\n";
    std::cout << "JOGADA " << (tabuleiro.getJogadasRealizadas() + 1) << "\n";
    std::cout << "─────────────────────────\n";
    
    tabuleiro.imprimir();
    
    Jogador* jogador = jogadores[jogadorAtual].get();
    std::cout << "🎯 Vez de: " << jogador->getNome() << "\n";
    
    int posicao = jogador->escolherJogada(tabuleiro);
    
    // Se jogador humano digitou 0 para suspender
    if (posicao == 0 && jogador->getTipo() == "Humano") {
        std::cout << "⏸️ Jogo suspenso pelo jogador.\n";
        jogoAtivo = false;
        return;
    }
    
    if (tabuleiro.aplicarJogada(posicao, jogador->getSimbolo())) {
        std::cout << "✅ Jogada registrada!\n";
        if (!verificarFimDeJogo()) {
            alternarTurno();
        }
    } else {
        std::cout << "❌ Jogada inválida! Tente novamente.\n";
    }
}

bool MotorJogo::verificarFimDeJogo() {
    char vencedor = tabuleiro.verificarVencedor();
    
    if (vencedor != ' ') {
        for (auto& jogador : jogadores) {
            if (jogador->getSimbolo() == vencedor) {
                jogador->incrementarPontuacao();
            }
        }
        jogoAtivo = false;
        return true;
    }
    
    if (tabuleiro.isCheio()) {
        jogoAtivo = false;
        return true;
    }
    
    return false;
}

void MotorJogo::exibirResultado() const {
    std::cout << "\n═════════════════════════\n";
    std::cout << "        RESULTADO        \n";
    std::cout << "═════════════════════════\n";
    tabuleiro.imprimir();
    
    char vencedor = tabuleiro.verificarVencedor();
    
    if (vencedor != ' ') {
        for (const auto& jogador : jogadores) {
            if (jogador->getSimbolo() == vencedor) {
                std::cout << "🏆 " << jogador->getNome() << " VENCEU! 🏆\n";
                break;
            }
        }
    } else {
        std::cout << "🤝 EMPATE! 🤝\n";
    }
    
    std::cout << "\n📊 ESTATÍSTICAS:\n";
    for (const auto& j : jogadores) {
        std::cout << "   " << j->getNome() << ": " << j->getPontuacao() << " vitórias\n";
    }
    std::cout << "═════════════════════════\n\n";
}

void MotorJogo::reset() {
    tabuleiro.reset();
    jogadorAtual = 0;
    jogoAtivo = false;
}