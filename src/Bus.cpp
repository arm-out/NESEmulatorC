#include "../include/NESEmulator/Bus.h"

Bus::Bus() {
	// Connect CPU to communication bus
	cpu.ConnectBus(this);
}

Bus::~Bus() {

}

// CPU write onto the bus
void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    // Cartridge override
    if (cart->cpuWrite(addr, data)) {

    }
    // System ram address range
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        cpuRam[addr & 0x07FF] = data;   // 2K range is mirrored
    }
    // PPU address range
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        ppu.cpuWrite(addr & 0x0007, data);  // 8 primary registers are repeated
    }
    // DAM
    else if (addr == 0x4014) {
        dma_page = data;
		dma_addr = 0x00;
		dma_transfer = true;
    }
    // Controller Range
    else if (addr >= 0x4016 && addr <= 0x4017) {
        controller_state[addr & 0x0001] = controller[addr & 0x0001];
    }
}

// CPU read onto the bus
uint8_t Bus::cpuRead(uint16_t addr, bool readOnly) {
    uint8_t data = 0x00;

    // Cartridge address range
	if (cart->cpuRead(addr, data)) {

    }
    // System ram address range
    else if (addr >= 0x0000 && addr <= 0x1FFF) {
        data = cpuRam[addr & 0x07FF];   // 2K range is mirrored
    }
    // PPU address range
    else if (addr >= 0x2000 && addr <= 0x3FFF) {
        data = ppu.cpuRead(addr & 0x0007, readOnly);    // 8 primary registers are repeated
    }
    // Controller Address Range
    else if (addr >= 0x4016 && addr <= 0x4017) {
        data = (controller_state[addr & 0x0001] & 0x80) > 0;
		controller_state[addr & 0x0001] <<= 1;
    }

	return data;
}

// Connect cartridge to bus
void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge) {
    this->cart = cartridge;
    ppu.connectCartridge(cartridge);
}

void Bus::reset() {
    cart->reset();
    cpu.reset();
    ppu.reset();
    systemClockCounter = 0;
    dma_page = 0x00;
	dma_addr = 0x00;
	dma_data = 0x00;
	dma_latch = true;
	dma_transfer = false;
}

void Bus::clock() {
    // PPU clock runs the fastest
    ppu.clock();

    // CPU clock runs 3 times slower than PPU
    if (systemClockCounter % 3 == 0) {
        // Is DMATransfer Occuring
        if (dma_transfer) {
            if (dma_latch) {
                if (systemClockCounter % 2 == 1) {
                    dma_latch = false;
                }
            }
            else {
                // Read cycle
                if (systemClockCounter % 2 == 0) {
                    dma_data = cpuRead(dma_page << 8 | dma_addr);
                }
                // Write cycle
                else {
                    ppu.OAMpointer[dma_addr] = dma_data;
                    dma_addr++;

                    // DMA Termination
                    if (dma_addr == 0x00) {
                        dma_transfer = false;
                        dma_latch = true;
                    }
                }
            }
        }
        else {
            cpu.clock();
        }
    }

    // PPU NMI Signal
    if (ppu.nmi) {
        ppu.nmi = false;
        cpu.nmi();
    }

    systemClockCounter++;
}
