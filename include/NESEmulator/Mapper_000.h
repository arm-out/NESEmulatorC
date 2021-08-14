#ifndef _MAPPER000_
#define _MAPPER000_

#include "Mapper.h"

class Mapper_000 : public Mapper {

    public:
        Mapper_000(uint8_t prgBank, uint8_t chrBank);
        ~Mapper_000();

        bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
        bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr, uint8_t data = 0) override;
        bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
        bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
        void reset() override;
};

#endif // _MAPPER000_