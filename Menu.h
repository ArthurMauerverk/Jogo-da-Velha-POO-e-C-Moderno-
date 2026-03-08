#ifndef MENU_H
#define MENU_H

#include "MotorJogo.h"
#include "Persistencia.h"

class Menu {
private:
    MotorJogo motor;
    Persistencia persistencia;
    bool jogoEmAndamento;

    void exibirCabecalho();
    void exibirOpcoes();
    void processarOpcao(int opcao);
    
    // Métodos para cada opção do menu
    void iniciarNovoJogo();
    void escolherQuemInicia();
    void escolherModoJogo();
    void suspenderJogo();
    void continuarJogo();

public:
    Menu();
    void executar();  // Loop principal do menu
};

#endif