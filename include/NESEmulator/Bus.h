#ifndef _BUS_
#define _BUS_

#include <cstdint>
#include <array>

#include "nes6502.h"
#include "nes2C02.h"
#include "Cartridge.h"

class Bus {

    public:
        Bus();
        ~Bus();

        // Devices on the bus
        nes6502 cpu;                        // CPU
        nes2C02 ppu;                        // PPU
        std::shared_ptr<Cartridge> cart;    // Cartridge
        uint8_t cpuRam[2048];               // 2K Ram
        uint8_t controller[2];              // Controllers

        // Read and Write onto the bus
        void cpuWrite(uint16_t addr, uint8_t data);
        uint8_t cpuRead(uint16_t addr, bool readOnly = false);

        // System Interface
        void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);
        void reset();       // Reset system
        void clock();       // System tick

    private:
        uint32_t systemClockCounter = 0;    // Total number of clocks passed
        uint8_t controller_state[2];        // Controller State Cache

        // Direct Memory Access
        uint8_t dma_page = 0x00;
        uint8_t dma_addr = 0x00;
        uint8_t dma_data = 0x00;

        // DMA Flags
        bool dma_latch = true;
        bool dma_transfer = false;

};

#endif // _BUS_