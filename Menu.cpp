#include "Menu.h"
#include <iostream>
#include <limits>

Menu::Menu() : jogoEmAndamento(false) {}

void Menu::exibirCabecalho() {
    std::cout << "\n╔════════════════════════════╗\n";
    std::cout << "║      JOGO DA VELHA         ║\n";
    std::cout << "╚════════════════════════════╝\n\n";
}

void Menu::exibirOpcoes() {
    std::cout << "1. Iniciar Novo Jogo\n";
    if (jogoEmAndamento) {
        std::cout << "2. Continuar Jogo Atual\n";
    }
    std::cout << "3. Escolher Quem Inicia\n";
    std::cout << "4. Escolher Modo de Jogo\n";
    std::cout << "5. Suspender Jogo\n";
    std::cout << "6. Exibir Top 10\n";
    std::cout << "7. Sair\n";
    std::cout << "\nEscolha uma opção: ";
}

void Menu::executar() {
    int opcao;
    
    do {
        exibirCabecalho();
        exibirOpcoes();
        std::cin >> opcao;
        
        // Tratar entrada inválida
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Opção inválida! Tente novamente.\n";
            continue;
        }
        
        processarOpcao(opcao);
        
    } while (opcao != 7);
}

void Menu::processarOpcao(int opcao) {
    switch(opcao) {
        case 1:
            iniciarNovoJogo();
            break;
        case 2:
            if (jogoEmAndamento) {
                continuarJogo();
            } else {
                std::cout << "Não há jogo em andamento!\n";
            }
            break;
        case 3:
            escolherQuemInicia();
            break;
        case 4:
            escolherModoJogo();
            break;
        case 5:
            suspenderJogo();
            break;
        case 6:
            persistencia.exibirTop10();
            break;
        case 7:
            std::cout << "Obrigado por jogar! Até mais!\n";
            break;
        default:
            std::cout << "Opção inválida!\n";
    }
}

void Menu::iniciarNovoJogo() {
    int modo;
    std::cout << "\nEscolha o modo de jogo:\n";
    std::cout << "1. Humano vs Humano\n";
    std::cout << "2. Humano vs Computador\n";
    std::cout << "3. Computador vs Computador\n";
    std::cout << "Opção: ";
    std::cin >> modo;
    
    if (modo >= 1 && modo <= 3) {
        motor.iniciarPartida(modo);
        jogoEmAndamento = true;
        continuarJogo();
    } else {
        std::cout << "Modo inválido!\n";
    }
}

void Menu::escolherQuemInicia() {
    if (!jogoEmAndamento) {
        std::cout << "Inicie um jogo primeiro!\n";
        return;
    }
    
    int escolha;
    std::cout << "\nQuem deve iniciar?\n";
    std::cout << "1. Jogador 1\n";
    std::cout << "2. Jogador 2\n";
    std::cout << "Opção: ";
    std::cin >> escolha;
    
    if (escolha == 1 || escolha == 2) {
        motor.setJogadorAtual(escolha - 1);
        std::cout << "Turno alterado!\n";
    }
}

void Menu::escolherModoJogo() {
    int modo;
    std::cout << "\nEscolha o novo modo de jogo:\n";
    std::cout << "1. Humano vs Humano\n";
    std::cout << "2. Humano vs Computador\n";
    std::cout << "3. Computador vs Computador\n";
    std::cout << "Opção: ";
    std::cin >> modo;
    
    if (modo >= 1 && modo <= 3) {
        motor.iniciarPartida(modo);
        jogoEmAndamento = true;
        std::cout << "Modo alterado! Novo jogo iniciado.\n";
    } else {
        std::cout << "Modo inválido!\n";
    }
}

void Menu::suspenderJogo() {
    if (!jogoEmAndamento) {
        std::cout << "Não há jogo para suspender!\n";
        return;
    }
    
    if (persistencia.salvarJogo(motor)) {
        std::cout << "Jogo salvo com sucesso!\n";
        jogoEmAndamento = false;
    } else {
        std::cout << "Erro ao salvar o jogo!\n";
    }
}

void Menu::continuarJogo() {
    if (!jogoEmAndamento) {
        // Tentar carregar jogo salvo
        if (persistencia.carregarJogo(motor)) {
            std::cout << "Jogo carregado com sucesso!\n";
            jogoEmAndamento = true;
        } else {
            std::cout << "Nenhum jogo salvo encontrado!\n";
            return;
        }
    }
    
    // Loop principal do jogo
    while (motor.isJogoAtivo()) {
        motor.executarTurno();
    }
    
    motor.exibirResultado();
    persistencia.salvarPontuacao(motor.getJogadores());
    jogoEmAndamento = false;
}