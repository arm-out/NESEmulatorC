#include "../include/NESEmulator/Mapper.h"

Mapper::Mapper(uint8_t prgBank, uint8_t chrBank) {
    numPRGBanks = prgBank;
    numCHRBanks = chrBank;

    reset();
}

Mapper::~Mapper() {
    
}

void Mapper::reset() {
    
}