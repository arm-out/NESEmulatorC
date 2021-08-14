#include "../include/NESEmulator/Mapper_000.h"

Mapper_000::Mapper_000(uint8_t prgBank, uint8_t chrBank) :
    Mapper(prgBank, chrBank) {}

Mapper_000::~Mapper_000() {

}

void Mapper_000::reset() {

}

bool Mapper_000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr) {
    // Addressable range
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        // Upper limit for 2 PRGROM is 0x3FFF
        // Upper limit for 1 PRGROM is 0x7FFF (mirrored)
        mapped_addr = addr & (numPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper_000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data) {
    // Addressable range
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        // Upper limit for 2 PRGROM is 0x3FFF
        // Upper limit for 1 PRGROM is 0x7FFF (mirrored)
        mapped_addr = addr & (numPRGBanks > 1 ? 0x7FFF : 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper_000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr) {
    // No mapping required
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        mapped_addr = addr;
        return true;
    }

    return false;
}

bool Mapper_000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
    // No mapping required
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        if (numCHRBanks == 0) {
			mapped_addr = addr;
			return true;
		}
    }

    return false;
}
