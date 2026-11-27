#pragma once
// Temas da interface. "Pastel": rosa e lavanda, texto ameixa (o original do NiGBA).
// "Nidra": roxo e dourado, a paleta do NidraIA (Z:\NidraIA\public\style.css), a mesma do tema Nidra do NidraX.

#include <imgui.h>

enum class ThemeId { Pastel = 0, Nidra = 1 };

struct Theme {
    bool dark;
    bool dragons;                    // o enfeite é o dragão do NidraIA em vez do coração
    // Janelas e controles
    ImVec4 text, textSoft, windowBg, childBg, border, frameBg, light, strong, deep;
    ImVec4 title, titleActive, menuBar, separator, modalDim, check, grab, grabActive;
    // Destaques
    ImVec4 accent;                   // títulos e valores em destaque
    ImVec4 dim;                      // texto apagado no depurador
    ImVec4 warning, error;
    ImVec4 primary, primaryHover, primaryActive, primaryText; // botão principal da tela inicial
    ImVec4 waiting;                  // botão esperando uma tecla
    // Desenhos (tela inicial, pílulas e avisos)
    ImU32 screenBorder;              // atrás da tela do jogo
    ImU32 gradient[4];               // fundo da tela inicial (cantos: sup. esq., sup. dir., inf. dir., inf. esq.)
    ImU32 hearts[3];                 // enfeites flutuando (alfa próprio)
    ImU32 pill, pillAlt, pillText;   // "Pausado" e "Voltando no tempo"
    ImU32 heart;                     // enfeite das pílulas e dos avisos
    ImU32 toast, toastText;          // sem alfa: o aviso some aos poucos
};

const Theme& theme();
void selectTheme(ThemeId id);
const char* themeName(ThemeId id);
