#include "frontend/theme.hpp"

namespace {
constexpr ImVec4 rgb(unsigned hex, float a = 1.0f) {
    return ImVec4(static_cast<float>((hex >> 16) & 0xFF) / 255.0f, static_cast<float>((hex >> 8) & 0xFF) / 255.0f,
                  static_cast<float>(hex & 0xFF) / 255.0f, a);
}
constexpr ImU32 col(unsigned hex, unsigned a = 255) {
    return IM_COL32((hex >> 16) & 0xFF, (hex >> 8) & 0xFF, hex & 0xFF, a);
}

const Theme kPastel{
    false, false,
    ImVec4(0.38f, 0.24f, 0.40f, 1.0f), ImVec4(0.62f, 0.50f, 0.64f, 1.0f), ImVec4(1.00f, 0.96f, 0.98f, 1.0f),
    ImVec4(1, 1, 1, 0.5f), ImVec4(0.90f, 0.72f, 0.84f, 1.0f), ImVec4(1.0f, 0.90f, 0.95f, 1.0f),
    ImVec4(0.97f, 0.80f, 0.88f, 1.0f), ImVec4(0.93f, 0.58f, 0.76f, 1.0f), ImVec4(0.86f, 0.42f, 0.64f, 1.0f),
    ImVec4(0.85f, 0.79f, 0.96f, 1.0f), ImVec4(0.74f, 0.64f, 0.93f, 1.0f), ImVec4(0.97f, 0.80f, 0.88f, 1.0f),
    ImVec4(0.92f, 0.78f, 0.88f, 1.0f), ImVec4(0.55f, 0.35f, 0.55f, 0.35f), ImVec4(0.86f, 0.42f, 0.64f, 1.0f),
    ImVec4(0.93f, 0.58f, 0.76f, 1.0f), ImVec4(0.86f, 0.42f, 0.64f, 1.0f),
    ImVec4(0.86f, 0.42f, 0.64f, 1.0f), ImVec4(0.62f, 0.50f, 0.64f, 1.0f),
    ImVec4(0.85f, 0.45f, 0.20f, 1.0f), ImVec4(0.85f, 0.25f, 0.35f, 1.0f),
    ImVec4(0.93f, 0.58f, 0.76f, 1.0f), ImVec4(0.93f, 0.58f, 0.76f, 1.0f), ImVec4(0.86f, 0.42f, 0.64f, 1.0f), ImVec4(1, 1, 1, 1),
    ImVec4(0.74f, 0.64f, 0.93f, 1.0f),
    col(0xF3E6F7),
    {col(0xFDE4F0), col(0xECE2FC), col(0xE2ECFC), col(0xFCECF4)},
    {col(0xF7A8CD, 150), col(0xC8B0F5, 140), col(0xFAC4D6, 160)},
    col(0xFFF4FA, 235), col(0xECE4FC, 235), col(0x613D66),
    col(0xF08CBE),
    col(0xFFEEF7), col(0x613D66),
};

// Nidra: painéis roxo-escuros, roxo nos controles, ouro nos destaques.
const Theme kNidra{
    true, true,
    rgb(0xECE6F8), rgb(0xA89EC0), rgb(0x18121F),           // texto, texto suave, painel
    rgb(0x0F0B16, 0.6f), rgb(0x2E2540), rgb(0x1E1728),     // fundo interno, borda, campo
    rgb(0x2A1F40), rgb(0x7C4DDB), rgb(0x9A75E8),           // seleção, roxo, roxo 2
    rgb(0x130E1C), rgb(0x24123F), rgb(0x130E1C),           // títulos e barra de menus
    rgb(0x251D33), rgb(0x000000, 0.55f), rgb(0xE6BE5C),    // separador, escurecimento, marca
    rgb(0xE6BE5C), rgb(0xF3D891),                          // controle deslizante em ouro
    rgb(0xE6BE5C), rgb(0x766C8E),                          // destaque em ouro, texto fraco
    rgb(0xE0AE4C), rgb(0xEE8478),                          // aviso, erro
    rgb(0xE6BE5C), rgb(0xF3D891), rgb(0xB88A2C), rgb(0x1A1208), // botão de ouro
    rgb(0x7C4DDB),
    col(0x0F0B16),
    {col(0x24123F), col(0x130E1C), col(0x0F0B16), col(0x1E1728)},
    {col(0xE6BE5C, 150), col(0x9A75E8, 120), col(0xBFA3F5, 90)},
    col(0x24123F, 235), col(0x2A1F40, 235), col(0xECE6F8),
    col(0xE6BE5C),
    col(0x2E2614), col(0xF3D891),
};

const Theme* g_theme = &kNidra;
} // namespace

const Theme& theme() { return *g_theme; }
void selectTheme(ThemeId id) { g_theme = id == ThemeId::Pastel ? &kPastel : &kNidra; }
const char* themeName(ThemeId id) { return id == ThemeId::Pastel ? "Pastel (rosa e lavanda)" : "Nidra (roxo e dourado)"; }
