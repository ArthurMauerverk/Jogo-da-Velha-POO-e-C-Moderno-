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
    std::cout << "\n=== INICIAR NOVO JOGO ===\n";
    std::cout << "Escolha o modo de jogo:\n";
    std::cout << "1. Humano vs Humano\n";
    std::cout << "2. Humano vs Computador\n";
    std::cout << "3. Computador vs Computador\n";
    std::cout << "Opção: ";
    std::cin >> modo;
    
    if (modo < 1 || modo > 3) {
        std::cout << "Modo inválido!\n";
        return;
    }
    
    // Se for modo com computador, perguntar o nível
    int nivel = 2; // nível padrão
    if (modo == 2 || modo == 3) {
        std::cout << "\nEscolha o nível do Computador:\n";
        std::cout << "1. Nível 1 - Aleatório (Fácil)\n";
        std::cout << "2. Nível 2 - Tático (Médio)\n";
        std::cout << "3. Nível 3 - Bloqueio (Difícil)\n";
        std::cout << "Opção: ";
        std::cin >> nivel;
        
        if (nivel < 1 || nivel > 3) {
            std::cout << "Nível inválido! Usando nível 2 (Médio).\n";
            nivel = 2;
        }
    }
    
    // Iniciar partida com o modo e nível escolhidos
    motor.iniciarPartida(modo, true, nivel);
    jogoEmAndamento = true;
    
    std::cout << "\n✅ Jogo iniciado!\n";
    continuarJogo(); // Já entra direto no jogo
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
    std::cout << "3. Aleatório\n";
    std::cout << "Opção: ";
    std::cin >> escolha;
    
    if (escolha == 1) {
        motor.setJogadorAtual(0);
        std::cout << "Jogador 1 começa!\n";
    } else if (escolha == 2) {
        motor.setJogadorAtual(1);
        std::cout << "Jogador 2 começa!\n";
    } else if (escolha == 3) {
        int aleatorio = rand() % 2;
        motor.setJogadorAtual(aleatorio);
        std::cout << "Sorteio: Jogador " << (aleatorio + 1) << " começa!\n";
    }
}

void Menu::escolherModoJogo() {
    if (!jogoEmAndamento) {
        std::cout << "Inicie um jogo primeiro!\n";
        return;
    }
    
    std::cout << "\nFunção disponível ao iniciar novo jogo.\n";
}

void Menu::suspenderJogo() {
    if (!jogoEmAndamento) {
        std::cout << "Não há jogo para suspender!\n";
        return;
    }
    
    if (persistencia.salvarJogo(motor)) {
        std::cout << "✅ Jogo salvo com sucesso!\n";
        jogoEmAndamento = false;
    } else {
        std::cout << "❌ Erro ao salvar o jogo!\n";
    }
}

void Menu::continuarJogo() {
    if (!jogoEmAndamento) {
        // Tentar carregar jogo salvo
        if (persistencia.carregarJogo(motor)) {
            std::cout << "✅ Jogo carregado com sucesso!\n";
            jogoEmAndamento = true;
        } else {
            std::cout << "Nenhum jogo salvo encontrado!\n";
            return;
        }
    }
    
    std::cout << "\n=== PARTIDA EM ANDAMENTO ===\n";
    while (motor.isJogoAtivo()) {
        motor.executarTurno();
    }
    
    motor.exibirResultado();
    persistencia.salvarPontuacao(motor.getJogadores());
    jogoEmAndamento = false;
}