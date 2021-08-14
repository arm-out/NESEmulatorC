#ifndef _CARTRIDGE_
#define _CARTRIDGE_

#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include <memory>

#include "Mapper_000.h"

class Cartridge {

    public:
        Cartridge(const std::string& sFileName);
        ~Cartridge();

        bool ImageValid();

        // Nametable Mirroring
        enum MIRROR {
            HORIZONTAL,
            VERTICAL,
            ONESCREEN_LO,
            ONESCREEN_HI,
        } mirror = HORIZONTAL;

        // Main bus communication
        bool cpuRead(uint16_t addr, uint8_t &data);
        bool cpuWrite(uint16_t addr, uint8_t data);

        // PPU bus communications
        bool ppuRead(uint16_t addr, uint8_t &data);
        bool ppuWrite(uint16_t addr, uint8_t data);

        void reset();

    private:
        bool isImgValid = false;

        /// Mapper Variables
        uint8_t mapperID = 0;
        uint8_t PRGBanks = 0;
        uint8_t CHRBanks = 0;

        std::shared_ptr<Mapper> mapper;

        // On Cartridge Memory
        std::vector<uint8_t> PRGMemory;
	    std::vector<uint8_t> CHRMemory;

};

#endif // _CARTRIDGE_