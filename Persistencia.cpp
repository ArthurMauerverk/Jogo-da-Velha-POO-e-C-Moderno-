#include "Persistencia.h"
#include "JogadorHumano.h"
#include "JogadorComputador.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
Persistencia::Persistencia(const std::string& tabuleiro, 
                           const std::string& ranking,
                           const std::string& tipoJogadores)
    : arquivoTabuleiro(tabuleiro), 
      arquivoRanking(ranking),
      arquivoTipoJogadores(tipoJogadores) {}

bool Persistencia::salvarJogo(const MotorJogo& motor) {
    // ===== SALVAR TABULEIRO =====
    std::ofstream arqTabuleiro(arquivoTabuleiro);
    if (!arqTabuleiro.is_open()) return false;
    
    auto posicoes = motor.getTabuleiro().getPosicoes();
    for (int i = 0; i < 9; i++) {
        arqTabuleiro << posicoes[i];
        if (i < 8) arqTabuleiro << " ";
    }
    arqTabuleiro << std::endl;
    arqTabuleiro << motor.getJogadorAtual() << std::endl;
    arqTabuleiro << motor.isJogoAtivo() << std::endl;
    arqTabuleiro << motor.getModoJogo() << std::endl;
    arqTabuleiro.close();
    
    // ===== SALVAR TIPO DOS JOGADORES =====
    std::ofstream arqTipo(arquivoTipoJogadores);
    if (!arqTipo.is_open()) return false;
    
    arqTipo << motor.getJogadores().size() << std::endl;
    
    for (const auto& jogador : motor.getJogadores()) {
        arqTipo << jogador->getTipo() << std::endl;        // Humano ou Computador
        arqTipo << jogador->getSimbolo() << std::endl;     // X ou O
        arqTipo << jogador->getNome() << std::endl;        // Nome do jogador
        arqTipo << jogador->getPontuacao() << std::endl;   // Pontuação
        
        // Se for Computador, salva o nível
        if (jogador->getTipo() == "Computador") {
            // Por enquanto salvamos nível 2 como padrão
            arqTipo << "2" << std::endl;
        }
    }
    arqTipo.close();
    
    return true;
}

bool Persistencia::carregarJogo(MotorJogo& motor) {
    // ===== CARREGAR TABULEIRO =====
    std::ifstream arqTabuleiro(arquivoTabuleiro);
    if (!arqTabuleiro.is_open()) return false;
    
    std::string linha;
    
    // Carregar posições do tabuleiro
    std::getline(arqTabuleiro, linha);
    std::array<char, 9> posicoes;
    std::istringstream iss(linha);
    for (int i = 0; i < 9; i++) {
        iss >> posicoes[i];
    }
    
    // Criar tabuleiro com as posições carregadas
    Tabuleiro tab;
    tab.setPosicoes(posicoes);
    
    // Carregar informações do jogo
    int jogadorAtual;
    bool jogoAtivo;
    int modoJogo;
    
    arqTabuleiro >> jogadorAtual;
    arqTabuleiro >> jogoAtivo;
    arqTabuleiro >> modoJogo;
    arqTabuleiro.close();
    
    // ===== CARREGAR TIPO DOS JOGADORES =====
    std::ifstream arqTipo(arquivoTipoJogadores);
    if (!arqTipo.is_open()) return false;
    
    int numJogadores;
    arqTipo >> numJogadores;
    arqTipo.ignore(); // Ignorar quebra de linha
    
    std::vector<std::unique_ptr<Jogador>> jogadores;
    
    for (int i = 0; i < numJogadores; i++) {
        std::string tipo;
        char simbolo;
        std::string nome;
        int pontuacao;
        
        // Ler tipo
        std::getline(arqTipo, tipo);
        
        // Ler símbolo
        arqTipo >> simbolo;
        arqTipo.ignore();
        
        // Ler nome
        std::getline(arqTipo, nome);
        
        // Ler pontuação
        arqTipo >> pontuacao;
        arqTipo.ignore();
        
        // Recriar o jogador baseado no tipo
        if (tipo == "Humano") {
            auto jogador = std::make_unique<JogadorHumano>(simbolo, nome);
            jogador->setPontuacao(pontuacao);
            jogadores.push_back(std::move(jogador));
        } 
        else if (tipo == "Computador") {
            int nivel;
            arqTipo >> nivel;
            arqTipo.ignore();
            
            auto jogador = std::make_unique<JogadorComputador>(simbolo, nome, nivel);
            jogador->setPontuacao(pontuacao);
            jogadores.push_back(std::move(jogador));
        }
    }
    
    arqTipo.close();
    
    // Atualizar motor com os dados carregados
    motor.setTabuleiro(tab);
    motor.setJogadores(std::move(jogadores));
    motor.setJogadorAtual(jogadorAtual);
    motor.setJogoAtivo(jogoAtivo);
    motor.setModoJogo(modoJogo);
    
    return true;
}

void Persistencia::salvarPontuacao(const std::vector<std::unique_ptr<Jogador>>& jogadores) {
    // Estrutura simples para armazenar nome e pontuação
    struct JogadorRanking {
        std::string nome;
        int pontuacao;
    };
    
    std::vector<JogadorRanking> ranking;
    
    // Ler ranking existente do arquivo ranking.txt
    std::ifstream arqRankingIn(arquivoRanking);
    if (arqRankingIn.is_open()) {
        std::string linha;
        while (std::getline(arqRankingIn, linha)) {
            size_t pos = linha.find(';');
            if (pos != std::string::npos) {
                JogadorRanking j;
                j.nome = linha.substr(0, pos);
                j.pontuacao = std::stoi(linha.substr(pos + 1));
                ranking.push_back(j);
            }
        }
        arqRankingIn.close();
    }
    
    // Atualizar com as pontuações dos jogadores atuais
    for (const auto& jogador : jogadores) {
        bool encontrado = false;
        for (auto& j : ranking) {
            if (j.nome == jogador->getNome()) {
                j.pontuacao += jogador->getPontuacao();
                encontrado = true;
                break;
            }
        }
        if (!encontrado) {
            JogadorRanking j;
            j.nome = jogador->getNome();
            j.pontuacao = jogador->getPontuacao();
            ranking.push_back(j);
        }
    }
    
    //metodo sort para ordenar ranking
    std::sort(ranking.begin(), ranking.end(), 
    [](const JogadorRanking& a, const JogadorRanking& b) {
        return a.pontuacao > b.pontuacao;  // > para ordem decrescente
        }
    );    
    
    // Salvar ranking atualizado no arquivo ranking.txt
    std::ofstream arqRankingOut(arquivoRanking);
    if (arqRankingOut.is_open()) {
        for (const auto& j : ranking) {
            arqRankingOut << j.nome << ";" << j.pontuacao << std::endl;
        }
        arqRankingOut.close();
    }
}

void Persistencia::exibirTop10() {
    std::ifstream arqRanking(arquivoRanking);
    
    std::cout << "\n🏆 TOP 10 JOGADORES 🏆\n";
    std::cout << "========================\n";
    
    if (!arqRanking.is_open()) {
        std::cout << "Nenhum jogador no ranking ainda.\n";
    } else {
        std::string linha;
        int contador = 0;
        
        while (std::getline(arqRanking, linha) && contador < 10) {
            size_t pos = linha.find(';');
            if (pos != std::string::npos) {
                std::string nome = linha.substr(0, pos);
                std::string pontuacao = linha.substr(pos + 1);
                contador++;
                std::cout << contador << ". " << nome << " - " << pontuacao << " pontos\n";
            }
        }
        
        if (contador == 0) {
            std::cout << "Nenhum jogador no ranking ainda.\n";
        }
        
        arqRanking.close();
    }
    
    std::cout << "========================\n\n";
}