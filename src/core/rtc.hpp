#pragma once
// Relógio do cartucho (RTC Seiko S-3511), ligado à porta GPIO da ROM (GBATEK, "GBA Cart Real-Time Clock").
// Usado pelos Pokémon Ruby/Sapphire/Emerald (e hacks deles), Boktai e outros.
//
// Porta GPIO, nos endereços da ROM:
//   080000C4  dados: bit 0 = SCK (relógio), bit 1 = SIO (dado), bit 2 = CS (seleção)
//   080000C6  direção de cada pino: 1 = o GBA escreve, 0 = o GBA lê
//   080000C8  bit 0 = 1: a CPU passa a enxergar a porta nesses endereços (senão vê a ROM normal)
//
// Protocolo: CS sobe, chega um byte de comando (bit menos significativo primeiro: bits 0-3 = 0110,
// bits 4-6 = comando, bit 7 = leitura), depois os bytes de dados, um bit por pulso de SCK.
//
// Sensor solar do Boktai (GBATEK, "GBA Cart Solar Sensor"), na mesma porta, com CS baixo:
//   bit 0 = relógio, bit 1 = reinício (zera o contador), bit 3 = saída do sensor.
// Depois do reinício, o jogo dá pulsos até o bit 3 subir: quanto mais luz, menos pulsos.
//
// Giroscópio do WarioWare: Twisted! (GBATEK, "GBA Cart Rotation Sensor"; mesmo comportamento do mGBA):
//   bit 0 = mede agora, bit 1 = relógio, bit 2 = dado (16 bits, o mais alto primeiro, um por queda do relógio).
//   Parado, a leitura fica perto de 700h; girar muda para mais ou para menos.
// Vibração (Drill Dozer, WarioWare: Twisted!): bit 3 ligado = motor vibrando.

#include <array>
#include <cstdint>
#include <ctime>
#include <functional>

#include "state.hpp"

namespace gba {

class Rtc {
public:
    Rtc();

    // Escrita da CPU num dos registradores da porta (deslocamento 0xC4, 0xC6 ou 0xC8).
    void write(uint32_t offset, uint16_t value);
    // O que a CPU lê em 0xC4/0xC6/0xC8 quando a porta está legível.
    uint16_t read(uint32_t offset) const;
    bool readable() const { return readable_; }

    // Hora usada pelo relógio (padrão: a do computador). Os testes usam uma hora fixa.
    void setClock(std::function<std::time_t()> clock) { clock_ = std::move(clock); }

    void serialize(StateArchive& ar);

    // Sensor solar: só existe nos cartuchos do Boktai (ver game_db).
    static constexpr int kMaxSunLevel = 10;
    void setSolarSensor(bool present) { solar_ = present; }
    bool hasSolarSensor() const { return solar_; }
    void setSunLevel(int level) { sunLevel_ = level < 0 ? 0 : (level > kMaxSunLevel ? kMaxSunLevel : level); }
    int sunLevel() const { return sunLevel_; }
    void serializeSolar(StateArchive& ar); // versão 7 do formato (no final)

    // O relógio responde em qualquer jogo, menos nos cartuchos cuja porta é de outro aparelho (ver game_db).
    void setClockPresent(bool present) { clockOn_ = present; }
    bool clockPresent() const { return clockOn_; }

    // Giroscópio: rotação pedida por quem joga, de -1 a 1 (0 = parado).
    static constexpr int kGyroCenter = 0x700;
    static constexpr int kGyroRange = 0x300; // quanto a rotação máxima afasta do centro (estimado)
    void setGyroSensor(bool present) { gyro_ = present; }
    bool hasGyroSensor() const { return gyro_; }
    void setRotation(float rotation) { rotation_ = rotation; }
    void serializeGyro(StateArchive& ar); // versão 8 do formato (no final)

    // Vibração: o jogo liga o motor pelo pino 3.
    void setRumbleMotor(bool present) { rumble_ = present; }
    bool hasRumbleMotor() const { return rumble_; }
    bool rumbling() const { return rumble_ && (pins_ & 8); }
    // O motor ligou em algum momento desde a última chamada? (jogos ligam e desligam várias vezes por quadro)
    bool takeRumble() { const bool on = rumbleSeen_ || rumbling(); rumbleSeen_ = false; return on; }

private:
    void updatePins();     // o jogo mudou os pinos: avança o protocolo
    void updateSolar();    // idem, para o sensor solar
    void updateGyro();     // idem, para o giroscópio
    void processByte();    // terminou de chegar um byte do jogo
    uint8_t outputBit() const;
    void latchTime();      // copia a hora atual para os 7 bytes de data/hora (BCD)

    std::function<std::time_t()> clock_;

    uint8_t pins_ = 0;       // estado dos 4 pinos
    uint8_t direction_ = 0;  // 1 = saída do GBA
    bool readable_ = false;

    int transferStep_ = 0;   // 0 = parado, 1 = SCK alto, 2 = transferindo
    uint8_t bits_ = 0;       // byte sendo montado
    int bitsRead_ = 0;
    uint8_t command_ = 0;
    int bytesRemaining_ = 0;
    bool commandActive_ = false;
    uint8_t control_ = 0x40; // bit 6 = modo 24 horas; bit 7 (falha de energia) desligado
    std::array<uint8_t, 7> time_{}; // ano, mês, dia, dia da semana, hora, minuto, segundo (BCD)

    bool solar_ = false;
    int sunLevel_ = 0;       // escolhido por quem joga (0 = escuro)
    int lightCounter_ = 0;   // pulsos desde o último reinício
    bool lightEdge_ = false; // o relógio estava baixo: o próximo alto conta um pulso
    int lightSample_ = 0xFF; // pulsos necessários para a saída subir (medido no reinício)

    bool clockOn_ = true;
    bool gyro_ = false;
    float rotation_ = 0;
    uint16_t gyroSample_ = 0; // bits ainda não enviados (o mais alto sai primeiro)
    bool gyroEdge_ = false;   // o relógio do giroscópio estava alto
    bool rumble_ = false;
    bool rumbleSeen_ = false; // o motor ligou desde o último takeRumble()
};

} // namespace gba
