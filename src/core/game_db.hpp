#pragma once
// Jogos que precisam de ajuste, pelo código do cartucho (cabeçalho, 0xAC): tipo de save que a assinatura
// na ROM não revela ou revela errado, e hardware extra no cartucho (sensor solar, inclinação...).
// Fonte dos dados: tabela de exceções do mGBA (src/gba/overrides.c).

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "backup.hpp"

namespace gba {

// Hardware extra do cartucho (bits combináveis).
namespace hw {
constexpr uint8_t Rtc = 1 << 0;    // relógio (o NiGBA põe em qualquer jogo, menos nos de giroscópio ou vibração sem ele)
constexpr uint8_t Solar = 1 << 1;  // sensor de luz (Boktai)
constexpr uint8_t Tilt = 1 << 2;   // sensor de inclinação (Yoshi Topsy-Turvy, Koro Koro Puzzle)
constexpr uint8_t Gyro = 1 << 3;   // giroscópio (WarioWare: Twisted!)
constexpr uint8_t Rumble = 1 << 4; // vibração (Drill Dozer, WarioWare: Twisted!)
} // namespace hw

struct GameOverride {
    const char* code;                // 4 letras; a última é a região
    std::optional<Backup::Type> save; // vazio: a detecção pela assinatura continua valendo
    uint8_t hardware = 0;
};

// Ajuste do jogo, ou nullptr se ele não precisa de nenhum.
const GameOverride* findGameOverride(const std::string& gameCode);
// Código do jogo direto da ROM (vazio se a ROM for menor que o cabeçalho).
std::string romGameCode(const std::vector<uint8_t>& rom);

} // namespace gba
