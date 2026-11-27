#pragma once
// Sensor de inclinação (Yoshi Topsy-Turvy, Koro Koro Puzzle), na região do save (GBATEK, "GBA Cart Tilt
// Sensor"; mesmo comportamento do mGBA). Esses cartuchos salvam em EEPROM, então 0x0E fica livre para ele:
//   0E008000 <- 55h, depois 0E008100 <- AAh: mede a inclinação agora
//   0E008200 / 0E008300: X (bits 0-7 / bits 8-11; o bit 7 de 8300 indica medida pronta)
//   0E008400 / 0E008500: Y (bits 0-7 / bits 8-11)
// O centro (aparelho na mão, reto) é perto de 3A0h; inclinar muda cada eixo para mais ou para menos.

#include <cstdint>

#include "state.hpp"

namespace gba {

class TiltSensor {
public:
    // Inclinação pedida por quem joga, de -1 a 1 em cada eixo (0 = reto).
    void setTilt(float x, float y) { x_ = x; y_ = y; }
    float tiltX() const { return x_; }
    float tiltY() const { return y_; }

    static bool isRegister(uint32_t addr) { const uint32_t a = addr & 0xFFFFFF; return a >= 0x8000 && a <= 0x8500 && !(a & 0xFF); }
    void write(uint32_t addr, uint8_t value);
    uint8_t read(uint32_t addr) const;
    void serialize(StateArchive& ar);

    static constexpr int kCenter = 0x3A0;
    static constexpr int kRange = 0xD0; // quanto a inclinação máxima afasta do centro (estimado)

private:
    float x_ = 0, y_ = 0;
    bool armed_ = false;              // recebeu 55h: o próximo AAh mede
    uint16_t sampleX_ = 0xFFF, sampleY_ = 0xFFF; // última medida (FFFh antes da primeira)
};

} // namespace gba
