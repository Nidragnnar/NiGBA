#include "game_db.hpp"

namespace gba {

namespace {
using T = Backup::Type;

// Mesma ordem da tabela do mGBA (por nome do jogo). Entradas que só mudam o "idle loop" ficam de fora.
const GameOverride kOverrides[] = {
    {"AWRE", T::Flash64K, 0}, {"AWRP", T::Flash64K, 0},                         // Advance Wars
    {"AW2E", T::Flash64K, 0}, {"AW2P", T::Flash64K, 0},                         // Advance Wars 2
    {"U3IJ", T::Eeprom, hw::Rtc | hw::Solar},                                   // Boktai
    {"U3IE", T::Eeprom, hw::Rtc | hw::Solar},
    {"U3IP", T::Eeprom, hw::Rtc | hw::Solar},
    {"U32J", T::Eeprom, hw::Rtc | hw::Solar},                                   // Boktai 2
    {"U32E", T::Eeprom, hw::Rtc | hw::Solar},
    {"U32P", T::Eeprom, hw::Rtc | hw::Solar},
    {"AC8J", T::Eeprom, 0}, {"AC8E", T::Eeprom, 0}, {"AC8P", T::Eeprom, 0},     // Crash Bandicoot 2
    {"BDKJ", T::Eeprom, 0},                                                     // DigiCommunication Nyo
    {"ALGP", T::Eeprom, 0},                                                     // DBZ: The Legacy of Goku
    {"ALFJ", T::Eeprom, 0}, {"ALFE", T::Eeprom, 0}, {"ALFP", T::Eeprom, 0},     // DBZ: The Legacy of Goku II
    {"BDBE", T::Eeprom, 0}, {"BDBP", T::Eeprom, 0},                             // DBZ: Taiketsu
    {"V49J", T::Sram, hw::Rumble}, {"V49E", T::Sram, hw::Rumble}, {"V49P", T::Sram, hw::Rumble}, // Drill Dozer
    {"AFXE", T::Flash64K, 0},                                                   // Final Fantasy Tactics Advance
    {"BFTJ", T::Flash128K, 0},                                                  // F-Zero Climax
    {"2GBP", T::Sram, hw::Rumble},                                              // Goodboy Galaxy
    {"AI2E", T::None, 0}, {"AI2P", T::None, 0},                                 // Iridion II (sem save)
    {"BGWJ", T::Flash128K, 0},                                                  // Game Boy Wars Advance 1+2
    {"AGFE", T::Flash64K, 0},                                                   // Golden Sun: The Lost Age
    {"KHPJ", T::Eeprom, hw::Tilt},                                              // Koro Koro Puzzle
    {"BLJJ", T::Flash64K, hw::Rtc}, {"BLJK", T::Flash64K, hw::Rtc},             // Legendz: Yomigaeru Shiren no Shima
    {"BLVJ", T::Flash64K, hw::Rtc},                                             // Legendz: Sign of Nekuromu
    {"AREE", T::Sram, 0},                                                       // Mega Man Battle Network
    {"AZCE", T::Sram, 0},                                                       // Mega Man Zero
    {"BSME", T::Eeprom, 0},                                                     // Metal Slug Advance
    {"AXVJ", T::Flash128K, hw::Rtc}, {"AXVE", T::Flash128K, hw::Rtc},           // Pokémon Ruby
    {"AXVP", T::Flash128K, hw::Rtc}, {"AXVI", T::Flash128K, hw::Rtc},
    {"AXVS", T::Flash128K, hw::Rtc}, {"AXVD", T::Flash128K, hw::Rtc},
    {"AXVF", T::Flash128K, hw::Rtc},
    {"AXPJ", T::Flash128K, hw::Rtc}, {"AXPE", T::Flash128K, hw::Rtc},           // Pokémon Sapphire
    {"AXPP", T::Flash128K, hw::Rtc}, {"AXPI", T::Flash128K, hw::Rtc},
    {"AXPS", T::Flash128K, hw::Rtc}, {"AXPD", T::Flash128K, hw::Rtc},
    {"AXPF", T::Flash128K, hw::Rtc},
    {"BPEJ", T::Flash128K, hw::Rtc}, {"BPEE", T::Flash128K, hw::Rtc},           // Pokémon Emerald
    {"BPEP", T::Flash128K, hw::Rtc}, {"BPEI", T::Flash128K, hw::Rtc},
    {"BPES", T::Flash128K, hw::Rtc}, {"BPED", T::Flash128K, hw::Rtc},
    {"BPEF", T::Flash128K, hw::Rtc},
    {"B24E", T::Flash128K, 0}, {"B24P", T::Flash128K, 0},                       // Pokémon Mystery Dungeon
    {"BPRJ", T::Flash128K, 0}, {"BPRE", T::Flash128K, 0}, {"BPRP", T::Flash128K, 0}, // Pokémon FireRed
    {"BPRI", T::Flash128K, 0}, {"BPRS", T::Flash128K, 0}, {"BPRD", T::Flash128K, 0},
    {"BPRF", T::Flash128K, 0},
    {"BPGJ", T::Flash128K, 0}, {"BPGE", T::Flash128K, 0}, {"BPGP", T::Flash128K, 0}, // Pokémon LeafGreen
    {"BPGI", T::Flash128K, 0}, {"BPGS", T::Flash128K, 0}, {"BPGD", T::Flash128K, 0},
    {"BPGF", T::Flash128K, 0},
    {"BR4J", T::Flash64K, hw::Rtc},                                             // RockMan EXE 4.5
    {"AR8E", T::Eeprom, 0}, {"AROP", T::Eeprom, 0},                             // Rocky
    {"BKAJ", T::Flash128K, hw::Rtc},                                            // Sennen Kazoku
    {"U33J", T::Eeprom, hw::Rtc | hw::Solar},                                   // Shin Bokura no Taiyou (Boktai 3)
    {"ASLE", T::None, 0}, {"ASLF", T::None, 0},                                 // Stuart Little 2 (sem save)
    {"AA2J", T::Eeprom, 0}, {"AA2E", T::Eeprom, 0},                             // Super Mario Advance 2
    {"A3AJ", T::Eeprom, 0}, {"A3AE", T::Eeprom, 0}, {"A3AP", T::Eeprom, 0},     // Super Mario Advance 3
    {"AX4J", T::Flash128K, 0}, {"AX4E", T::Flash128K, 0}, {"AX4P", T::Flash128K, 0}, // Super Mario Advance 4
    {"ALUE", T::Eeprom, 0}, {"ALUP", T::Eeprom, 0},                             // Super Monkey Ball Jr.
    {"A2YE", T::None, 0},                                                       // Top Gun: Combat Zones (sem save)
    {"BUHJ", T::Eeprom, 0},                                                     // Ueki no Housoku
    {"RZWJ", T::Sram, hw::Rumble | hw::Gyro},                                   // WarioWare: Twisted!
    {"RZWE", T::Sram, hw::Rumble | hw::Gyro},
    {"RZWP", T::Sram, hw::Rumble | hw::Gyro},
    {"KYGJ", T::Eeprom, hw::Tilt}, {"KYGE", T::Eeprom, hw::Tilt}, {"KYGP", T::Eeprom, hw::Tilt}, // Yoshi Topsy-Turvy
    {"TCHK", T::Eeprom, 0},                                                     // cartucho de testes da Nintendo
};

// Série Classic NES (Famicom Mini): todos os códigos começam com F e usam EEPROM.
const GameOverride kClassicNes{"F???", T::Eeprom, 0};
} // namespace

const GameOverride* findGameOverride(const std::string& gameCode) {
    if (gameCode.size() != 4) return nullptr;
    for (const GameOverride& o : kOverrides) {
        if (gameCode == o.code) return &o;
    }
    if (gameCode[0] == 'F') return &kClassicNes;
    return nullptr;
}

std::string romGameCode(const std::vector<uint8_t>& rom) {
    if (rom.size() < 0xB0) return {};
    return std::string(reinterpret_cast<const char*>(&rom[0xAC]), 4);
}

std::string unsupportedHardware(uint8_t hardware) {
    std::string names;
    const auto add = [&](uint8_t bit, const char* name) {
        if (!(hardware & bit)) return;
        if (!names.empty()) names += ", ";
        names += name;
    };
    add(hw::Tilt, "sensor de inclinação");
    add(hw::Gyro, "giroscópio");
    add(hw::Rumble, "vibração");
    return names;
}

} // namespace gba
