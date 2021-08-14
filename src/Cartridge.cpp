#include "../include/NESEmulator/Cartridge.h"

Cartridge::Cartridge(const std::string& sFileName) {
    // INES File Format Header
    struct HEADER {
        char name[4];
		uint8_t prg_rom_chunks;
		uint8_t chr_rom_chunks;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t prg_ram_size;
		uint8_t tv_system1;
		uint8_t tv_system2;
		char unused[5];
    } header;

    isImgValid = false;

    // READ INES File
    std::ifstream ifs;
	ifs.open(sFileName, std::ifstream::binary);
	if (ifs.is_open()) {
		// Read header
		ifs.read((char*)&header, sizeof(HEADER));

		// Skip trainer
		if (header.mapper1 & 0x04)
			ifs.seekg(512, std::ios_base::cur);

		// Mapper ID
		mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

		// INES File Type
		uint8_t fileType = 1;

        // TODO
		if (fileType == 0) {

		}

		if (fileType == 1)
		{
			PRGBanks = header.prg_rom_chunks;
			PRGMemory.resize(PRGBanks * 16384);
			ifs.read((char*)PRGMemory.data(), PRGMemory.size());

			CHRBanks = header.chr_rom_chunks;
			CHRMemory.resize(CHRBanks * 8192);
			ifs.read((char*)CHRMemory.data(), CHRMemory.size());
		}
        
        // TODO
		if (fileType == 2) {

		}

		// Load Mapper
		switch (mapperID) {
            case 0:
                mapper = std::make_shared<Mapper_000>(PRGBanks, CHRBanks);
                break;
		}

		isImgValid = true;
		ifs.close();
	}
}

Cartridge::~Cartridge() {

}

bool Cartridge::ImageValid() {
    return isImgValid;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &data) {
    uint32_t mapped_addr = 0;

    if (mapper->cpuMapRead(addr, mapped_addr)) {
        data = PRGMemory[mapped_addr];
		return true;
    }
    else
        return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;

	if (mapper->cpuMapWrite(addr, mapped_addr)) {
		PRGMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t & data) {
	uint32_t mapped_addr = 0;

	if (mapper->ppuMapRead(addr, mapped_addr))
	{
		data = CHRMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
	uint32_t mapped_addr = 0;
	if (mapper->ppuMapRead(addr, mapped_addr))
	{
		CHRMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

void Cartridge::reset() {
	if(mapper!= nullptr)
		mapper->reset();
}