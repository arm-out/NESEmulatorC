#ifndef _MAPPER_
#define _MAPPER_

#include <cstdint>

class Mapper {

    public:
        Mapper(uint8_t prgBank, uint8_t chrBank);
        ~Mapper();

        // Map CPU Bus Address
        virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
        virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) = 0;

        // Map PPU Bus Address
        virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
	    virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

        virtual void reset() = 0;

    protected:
        uint8_t numPRGBanks = 0;
        uint8_t numCHRBanks = 0;
};

#endif // _MAPPER_