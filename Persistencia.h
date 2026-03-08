#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H
//Vai defirnir a persistencia dos dados
#include <string>
#include <vector>
#include <memory>
#include "MotorJogo.h"

class Persistencia {
private:
    std::string arquivoTabuleiro;
    std::string arquivoRanking;
    std::string arquivoTipoJogadores;

public:
    Persistencia(const std::string& tabuleiro = "tabuleiro.txt", 
                 const std::string& ranking = "ranking.txt",
                 const std::string& tipoJogadores = "tipo_jogadores.txt");
    
    // Métodos para salvar/carregar jogo
    bool salvarJogo(const MotorJogo& motor);
    bool carregarJogo(MotorJogo& motor);
    
    // Métodos para ranking
    void salvarPontuacao(const std::vector<std::unique_ptr<Jogador>>& jogadores);
    void exibirTop10();
};

#endif