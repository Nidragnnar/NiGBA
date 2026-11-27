#include "tilt.hpp"

#include <algorithm>
#include <cmath>

namespace gba {

namespace {
// Inclinação de -1 a 1 -> leitura do sensor. Inclinar para a direita (ou para baixo) diminui o valor,
// como no mGBA.
uint16_t toSample(float v) {
    const float clamped = std::clamp(v, -1.0f, 1.0f);
    return static_cast<uint16_t>(TiltSensor::kCenter - static_cast<int>(std::lround(clamped * TiltSensor::kRange)));
}
} // namespace

void TiltSensor::write(uint32_t addr, uint8_t value) {
    switch (addr & 0xFFFFFF) {
    case 0x8000: armed_ = value == 0x55; break;
    case 0x8100:
        if (armed_ && value == 0xAA) {
            sampleX_ = toSample(x_);
            sampleY_ = toSample(y_);
        }
        armed_ = false;
        break;
    default: break;
    }
}

uint8_t TiltSensor::read(uint32_t addr) const {
    switch (addr & 0xFFFFFF) {
    case 0x8200: return static_cast<uint8_t>(sampleX_);
    case 0x8300: return static_cast<uint8_t>(((sampleX_ >> 8) & 0xF) | 0x80); // bit 7: medida pronta
    case 0x8400: return static_cast<uint8_t>(sampleY_);
    case 0x8500: return static_cast<uint8_t>((sampleY_ >> 8) & 0xF);
    default: return 0xFF;
    }
}

void TiltSensor::serialize(StateArchive& ar) {
    ar.value(armed_);
    ar.value(sampleX_);
    ar.value(sampleY_);
}

} // namespace gba
