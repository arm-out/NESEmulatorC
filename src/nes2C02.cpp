#include "../include/NESEmulator/nes2C02.h"

nes2C02::nes2C02() {
    // NES Color Palette
    palScreen[0x00] = olc::Pixel(84, 84, 84);
	palScreen[0x01] = olc::Pixel(0, 30, 116);
	palScreen[0x02] = olc::Pixel(8, 16, 144);
	palScreen[0x03] = olc::Pixel(48, 0, 136);
	palScreen[0x04] = olc::Pixel(68, 0, 100);
	palScreen[0x05] = olc::Pixel(92, 0, 48);
	palScreen[0x06] = olc::Pixel(84, 4, 0);
	palScreen[0x07] = olc::Pixel(60, 24, 0);
	palScreen[0x08] = olc::Pixel(32, 42, 0);
	palScreen[0x09] = olc::Pixel(8, 58, 0);
	palScreen[0x0A] = olc::Pixel(0, 64, 0);
	palScreen[0x0B] = olc::Pixel(0, 60, 0);
	palScreen[0x0C] = olc::Pixel(0, 50, 60);
	palScreen[0x0D] = olc::Pixel(0, 0, 0);
	palScreen[0x0E] = olc::Pixel(0, 0, 0);
	palScreen[0x0F] = olc::Pixel(0, 0, 0);

	palScreen[0x10] = olc::Pixel(152, 150, 152);
	palScreen[0x11] = olc::Pixel(8, 76, 196);
	palScreen[0x12] = olc::Pixel(48, 50, 236);
	palScreen[0x13] = olc::Pixel(92, 30, 228);
	palScreen[0x14] = olc::Pixel(136, 20, 176);
	palScreen[0x15] = olc::Pixel(160, 20, 100);
	palScreen[0x16] = olc::Pixel(152, 34, 32);
	palScreen[0x17] = olc::Pixel(120, 60, 0);
	palScreen[0x18] = olc::Pixel(84, 90, 0);
	palScreen[0x19] = olc::Pixel(40, 114, 0);
	palScreen[0x1A] = olc::Pixel(8, 124, 0);
	palScreen[0x1B] = olc::Pixel(0, 118, 40);
	palScreen[0x1C] = olc::Pixel(0, 102, 120);
	palScreen[0x1D] = olc::Pixel(0, 0, 0);
	palScreen[0x1E] = olc::Pixel(0, 0, 0);
	palScreen[0x1F] = olc::Pixel(0, 0, 0);

	palScreen[0x20] = olc::Pixel(236, 238, 236);
	palScreen[0x21] = olc::Pixel(76, 154, 236);
	palScreen[0x22] = olc::Pixel(120, 124, 236);
	palScreen[0x23] = olc::Pixel(176, 98, 236);
	palScreen[0x24] = olc::Pixel(228, 84, 236);
	palScreen[0x25] = olc::Pixel(236, 88, 180);
	palScreen[0x26] = olc::Pixel(236, 106, 100);
	palScreen[0x27] = olc::Pixel(212, 136, 32);
	palScreen[0x28] = olc::Pixel(160, 170, 0);
	palScreen[0x29] = olc::Pixel(116, 196, 0);
	palScreen[0x2A] = olc::Pixel(76, 208, 32);
	palScreen[0x2B] = olc::Pixel(56, 204, 108);
	palScreen[0x2C] = olc::Pixel(56, 180, 204);
	palScreen[0x2D] = olc::Pixel(60, 60, 60);
	palScreen[0x2E] = olc::Pixel(0, 0, 0);
	palScreen[0x2F] = olc::Pixel(0, 0, 0);

	palScreen[0x30] = olc::Pixel(236, 238, 236);
	palScreen[0x31] = olc::Pixel(168, 204, 236);
	palScreen[0x32] = olc::Pixel(188, 188, 236);
	palScreen[0x33] = olc::Pixel(212, 178, 236);
	palScreen[0x34] = olc::Pixel(236, 174, 236);
	palScreen[0x35] = olc::Pixel(236, 174, 212);
	palScreen[0x36] = olc::Pixel(236, 180, 176);
	palScreen[0x37] = olc::Pixel(228, 196, 144);
	palScreen[0x38] = olc::Pixel(204, 210, 120);
	palScreen[0x39] = olc::Pixel(180, 222, 120);
	palScreen[0x3A] = olc::Pixel(168, 226, 144);
	palScreen[0x3B] = olc::Pixel(152, 226, 180);
	palScreen[0x3C] = olc::Pixel(160, 214, 228);
	palScreen[0x3D] = olc::Pixel(160, 162, 160);
	palScreen[0x3E] = olc::Pixel(0, 0, 0);
	palScreen[0x3F] = olc::Pixel(0, 0, 0);
}

nes2C02::~nes2C02() {

}

olc::Sprite& nes2C02::GetScreen() {
	return sprScreen;
}

olc::Sprite & nes2C02::GetNameTable(uint8_t i) {
	return sprNameTable[i];
}

// Get the pattern table from CHROM
olc::Sprite & nes2C02::GetPatternTable(uint8_t i, uint8_t palette) {
	// Loop throught the 16x16 tiles
	for (uint16_t tileY = 0; tileY < 16; tileY++) {
		for (uint16_t tileX = 0; tileX < 16; tileX++) {
			// Tile Offset in Pattern Table
			uint16_t offset = (tileY * 256) + (tileX * 16);

			// Loop through 8x8 pixels
			for (uint16_t row = 0; row < 8; row ++) {
				// Read the MSB and LSB planes
				uint8_t lsb_tile = ppuRead(i * 0x1000 + offset + row + 0x0000);
				uint8_t msb_tile = ppuRead(i * 0x1000 + offset + row + 0x0008);

				for (uint16_t col = 0; col < 8; col++) {
					uint8_t pixel = (lsb_tile & 0x01) + (msb_tile & 0x01);

					// Shift for next col
					lsb_tile >>= 1;
					msb_tile >>= 1;

					sprPatternTable[i].SetPixel(tileX * 8 + (7-col), tileY * 8 + row, GetColorFromPaletteRam(palette, pixel));
				}
			}
		}
	}

	return sprPatternTable[i];
}

olc::Pixel& nes2C02::GetColorFromPaletteRam(uint8_t palette, uint8_t pixel) {
	return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

uint8_t nes2C02::cpuRead(uint16_t addr, bool readOnly) {
    uint8_t data = 0x00;

	if (readOnly) {
		// PPU registers
		switch (addr) {
			case 0x0000: // Control
				data = control.reg;
				break;
			case 0x0001: // Mask
				data = mask.reg;
				break;
			case 0x0002: // Status
				data = status.reg;
				break;
			case 0x0003: // OAM Address
				break;
			case 0x0004: // OAM Data
				break;
			case 0x0005: // Scroll
				break;
			case 0x0006: // PPU Address
				break;
			case 0x0007: // PPU Data
				break;
		}
	}
	else {
		switch (addr) {
			case 0x0000: // Control
				break;
			case 0x0001: // Mask
				break;
			case 0x0002: // Status
				data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
				status.vertical_blank = 0;
				address_latch = 0;
				break;
			case 0x0003: // OAM Address
				break;
			case 0x0004: // OAM Data
				data = OAMpointer[oam_addr];
				break;
			case 0x0005: // Scroll
				break;
			case 0x0006: // PPU Address
				break;
			case 0x0007: // PPU Data
				data = ppu_data_buffer;		// Read delayed by one cycle
				ppu_data_buffer = ppuRead(vram_addr.reg);

				// Not delayed when reading palette
				if (vram_addr.reg >= 0x3F00)
					data = ppu_data_buffer;
				
				// Increment nametable address according to control register
				vram_addr.reg += (control.increment_mode ? 32 : 1);
				break;
		}
	}

    return data;
}

void nes2C02::cpuWrite(uint16_t addr, uint8_t data) {
    // PPU registers
    switch (addr) {
        case 0x0000: // Control
			control.reg = data;
			tram_addr.nametable_x = control.nametable_x;
			tram_addr.nametable_y = control.nametable_y;
            break;
        case 0x0001: // Mask
			mask.reg = data;
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
			oam_addr = data;
            break;
        case 0x0004: // OAM Data
			OAMpointer[oam_addr] = data;
            break;
        case 0x0005: // Scroll
			if (address_latch == 0) {
				fine_x = data & 0x07;
				tram_addr.coarse_x = data >> 3;
				address_latch = 1;
			}
			else {
				tram_addr.fine_y = data & 0x07;
				tram_addr.coarse_y = data >> 3;
				address_latch = 0;
			}
            break;
        case 0x0006: // PPU Address
			// Read hi byte
			if (address_latch == 0) {
				tram_addr.reg = (uint16_t)((data & 0x3F) << 8) | (tram_addr.reg & 0x00FF);
				address_latch = 1;
			}
			// Read lo byte
			else {
				tram_addr.reg = (tram_addr.reg & 0xFF00) | data;
				vram_addr = tram_addr;
				address_latch = 0;
			}
            break;
        case 0x0007: // PPU Data
			ppuWrite(vram_addr.reg, data);
			vram_addr.reg += (control.increment_mode ? 32 : 1);
            break;
    }
}

uint8_t nes2C02::ppuRead(uint16_t addr, bool rdonly) {
    uint8_t data = 0x00;
	addr &= 0x3FFF;

	if (cart->ppuRead(addr, data)) {

	}
	// Pattern Table Range
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		data = patternTable[(addr && 0x1000) >> 12][addr & 0x0FFF];
	}
	// Nametable Range
	else if (addr >= 0x2000 && addr <= 0x3EFF) {
		addr &= 0x0FFF;

		// Vertical Mirroring
		if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = nameTable[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = nameTable[1][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = nameTable[0][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = nameTable[1][addr & 0x03FF];
		}
		// Horizontal Mirroring
		else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
			// Horizontal
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = nameTable[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = nameTable[0][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = nameTable[1][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = nameTable[1][addr & 0x03FF];
		}
	}
	// Palette Table Range
	else if (addr >= 0x3F00 && addr <= 0x3FFF) {
		addr &= 0x001F;

		// Mirroring
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;

		data = paletteTable[addr] & (mask.grayscale ? 0x30 : 0x3F);
	}

	return data;
}

void nes2C02::ppuWrite(uint16_t addr, uint8_t data) {
	addr &= 0x3FFF;

	if (cart->ppuWrite(addr, data)) {

	}
	// Pattern Table Range
	else if (addr >= 0x0000 && addr <= 0x1FFF) {
		patternTable[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
	}
	// Nametable Range
	else if (addr >= 0x2000 && addr <= 0x3EFF) {
		addr &= 0x0FFF;
		// Vertical Mirroring
		if (cart->mirror == Cartridge::MIRROR::VERTICAL) {
			if (addr >= 0x0000 && addr <= 0x03FF)
				nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				nameTable[1][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				nameTable[1][addr & 0x03FF] = data;
		}
		// Horizontal Mirroring
		else if (cart->mirror == Cartridge::MIRROR::HORIZONTAL) {
			if (addr >= 0x0000 && addr <= 0x03FF)
				nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				nameTable[0][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				nameTable[1][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				nameTable[1][addr & 0x03FF] = data;
		}
	}
	// Palette Table Range
	else if (addr >= 0x3F00 && addr <= 0x3FFF) {
		addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		paletteTable[addr] = data;
	}
}

void nes2C02::connectCartridge(const std::shared_ptr<Cartridge>& cartridge) {
	this->cart = cartridge;
}

// Reset PPU
void nes2C02::reset() {
	fine_x = 0x00;
	address_latch = 0x00;
	ppu_data_buffer = 0x00;
	scanline = 0;
	cycle = 0;
	bg_next_tile_id = 0x00;
	bg_next_tile_attrib = 0x00;
	bg_next_tile_lsb = 0x00;
	bg_next_tile_msb = 0x00;
	bg_shifter_pattern_lo = 0x0000;
	bg_shifter_pattern_hi = 0x0000;
	bg_shifter_attrib_lo = 0x0000;
	bg_shifter_attrib_hi = 0x0000;
	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
	vram_addr.reg = 0x0000;
	tram_addr.reg = 0x0000;
}

void nes2C02::clock() {

	// Increment Background Tile Horizontally
	auto incrementScrollX = [&]() {
		if (mask.render_background || mask.render_sprites) {
			// Wrap nametable
			if (vram_addr.coarse_x == 31) {
				vram_addr.coarse_x = 0;
				vram_addr.nametable_x = ~vram_addr.nametable_x;
			}
			else {
				vram_addr.coarse_x++;
			}
		}
	};

	// Increment Background Tile Vertically
	auto incrementScrollY = [&]() {
		if (mask.render_background || mask.render_sprites) {
			// Increment fine y if possible
			if (vram_addr.fine_y < 7)
				vram_addr.fine_y++;
			else {
				vram_addr.fine_y = 0;

				// Swap Nametables
				if (vram_addr.coarse_y == 29) {
					vram_addr.coarse_y = 0;
					vram_addr.nametable_y = ~vram_addr.nametable_y;
				}
				// Wrap Around
				else if (vram_addr.coarse_y == 31) {
					vram_addr.coarse_y = 0;
				}
				else {
					vram_addr.coarse_y++;
				}
			}
		}
	};

	// Transfer Horizontal Nametable Info
	auto transferAddressX = [&]() {
		if (mask.render_background || mask.render_sprites) {
			vram_addr.nametable_x = tram_addr.nametable_x;
			vram_addr.coarse_x = tram_addr.coarse_x;
		}
	};

	// Transfer Vertical Nametable Info
	auto transferAddressY = [&]() {
		if (mask.render_background || mask.render_sprites) {
			vram_addr.fine_y = tram_addr.fine_y;
			vram_addr.nametable_y = tram_addr.nametable_y;
			vram_addr.coarse_y = tram_addr.coarse_y;
		}
	};

	// Load the Background Shift Registers
	auto loadBackgroundShifters = [&]() {
		bg_shifter_pattern_lo = (bg_shifter_pattern_lo & 0xFF00) | bg_next_tile_lsb;
		bg_shifter_pattern_hi = (bg_shifter_pattern_hi & 0xFF00) | bg_next_tile_msb;

		// Shift Attribute bits
		bg_shifter_attrib_lo  = (bg_shifter_attrib_lo & 0xFF00) | ((bg_next_tile_attrib & 0b01) ? 0xFF : 0x00);
		bg_shifter_attrib_hi  = (bg_shifter_attrib_hi & 0xFF00) | ((bg_next_tile_attrib & 0b10) ? 0xFF : 0x00);
	};

	// Update Shift Registers
	auto updateShifters = [&]() {
		// Background Shifters
		if (mask.render_background) {
			// Shift background tile pattern
			bg_shifter_pattern_lo <<= 1;
			bg_shifter_pattern_hi <<= 1;

			// Shift palette attributes
			bg_shifter_attrib_lo <<= 1;
			bg_shifter_attrib_hi <<= 1;
		}

		// Sprite Shifters
		if (mask.render_sprites && cycle >= 1 && cycle < 258) {
			for (int  i = 0; i < sprite_count; i++) {
				// Decrement x value until cycle reaches sprite position
				if (spriteScanline[i].x > 0) {
					spriteScanline[i].x--;
				}
				else {
					sprite_shifter_pattern_lo[i] <<= 1;
					sprite_shifter_pattern_hi[i] <<= 1;
				}
			}
		}
	};

	// Visible Scanlines
	if (scanline >= -1 && scanline < 240) {
		if (scanline == 0 && cycle == 0)  {
			cycle = 1;
		}
		
		// New Frame Start
		if (scanline == -1 && cycle == 1) {
			status.vertical_blank = 0;
			status.sprite_overflow = 0;
			status.sprite_zero_hit = 0;

			// Clear Shifters
			for (int i = 0; i < 8; i++) {
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}
		}

		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338)) {
			updateShifters();
			
			switch ((cycle - 1) % 8) {
				case 0:
					loadBackgroundShifters();
					// Load next bg tile
					bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
					break;
				
				case 2:
					// Read next tile attribute
					bg_next_tile_attrib = ppuRead(0x23C0 | (vram_addr.nametable_y << 11) 
					    | (vram_addr.nametable_x << 10) 
					    | ((vram_addr.coarse_y >> 2) << 3) 
					    | (vram_addr.coarse_x >> 2));
					
					if (vram_addr.coarse_y & 0x02)
						bg_next_tile_attrib >>= 4;
					if (vram_addr.coarse_x & 0x02)
						bg_next_tile_attrib >>= 2;
					bg_next_tile_attrib &= 0x03;

					break;

				case 4:
					bg_next_tile_lsb = ppuRead((control.pattern_background << 12) 
					    + ((uint16_t)bg_next_tile_id << 4) 
					    + (vram_addr.fine_y) + 0);

					break;

				case 6:
					bg_next_tile_msb = ppuRead((control.pattern_background << 12)
					    + ((uint16_t)bg_next_tile_id << 4)
					    + (vram_addr.fine_y) + 8);

					break;

				case 7:
					// Increment to next tile
					incrementScrollX();
					break;
			}
		}

		// End of Visible Scanline
		if (cycle == 256) {
			incrementScrollY();
		}

		if (cycle == 257) {
			loadBackgroundShifters();
			transferAddressX();
		}

		if (cycle == 338 || cycle == 340) {
			bg_next_tile_id = ppuRead(0x2000 | (vram_addr.reg & 0x0FFF));
		}

		// End of Vertical Blank Period
		if (scanline == -1 && cycle >= 280 && cycle < 305) {
			transferAddressY();
		}

		// Sprite Rendering
		if (cycle == 257 && scanline >= 0) {
			// Clear sprite memory
			std::memset(spriteScanline, 0xFF, 8 * sizeof(objectAttributeEntry));

			sprite_count = 0;

			for (int i = 0; i < 8; i++) {
				sprite_shifter_pattern_lo[i] = 0;
				sprite_shifter_pattern_hi[i] = 0;
			}

			uint8_t numOAMEntry = 0;
			spriteZeroHitPossible = false;

			while (numOAMEntry < 64 && sprite_count < 9) {
				// Vertical Distance to scanline
				int16_t dist = ((int16_t) scanline - (int16_t)OAM[numOAMEntry].y);

				if (dist >= 0 && dist < (control.sprite_size ? 16 : 8)) {
					if (sprite_count < 8) {
						if (numOAMEntry == 0) {
							spriteZeroHitPossible = true;
						}

						memcpy(&spriteScanline[sprite_count], &OAM[numOAMEntry], sizeof(objectAttributeEntry));
						sprite_count++;
					}
				}

				numOAMEntry++;
			}

			// Set sprite overflow flag
			status.sprite_overflow = (sprite_count > 8);
		}

		// Prepare Sprite Shifters
		if (cycle == 340) {
			for (uint8_t i = 0; i < sprite_count; i++) {
				uint8_t sprite_pattern_bits_lo, sprite_pattern_bits_hi;
				uint16_t sprite_pattern_addr_lo, sprite_pattern_addr_hi;

				// 8x8 Sprites
				if (!control.sprite_size) {
					// Sprite is not flipped vertically  
					if (!(spriteScanline[i].attribute & 0x80)) {  
						sprite_pattern_addr_lo = 
							(control.pattern_sprite << 12  ) 
							| (spriteScanline[i].id   << 4   )
							| (scanline - spriteScanline[i].y);
													
					}
					// Sprite is flipped vertically, i.e. upside down
					else {
						sprite_pattern_addr_lo = 
							(control.pattern_sprite << 12  ) 
							| (spriteScanline[i].id   << 4   ) 
							| (7 - (scanline - spriteScanline[i].y));
					}
				}
				// 8x16 Sprites
				else {
					// Sprite is not flipped vertically
					if (!(spriteScanline[i].attribute & 0x80)) {
						// Tile top half
						if (scanline - spriteScanline[i].y < 8) {
							sprite_pattern_addr_lo = 
								((spriteScanline[i].id & 0x01)      << 12) 
								| ((spriteScanline[i].id & 0xFE)      << 4 )  
								| ((scanline - spriteScanline[i].y) & 0x07 ); 
						}
						// Tile bottom half
						else {
							sprite_pattern_addr_lo = 
								( (spriteScanline[i].id & 0x01)      << 12) 
								| (((spriteScanline[i].id & 0xFE) + 1) << 4 )  
								| ((scanline - spriteScanline[i].y) & 0x07  ); 
						}
					}
					// Sprite flipped vertically
					else {
						// Tile top half
						if (scanline - spriteScanline[i].y < 8) {
							sprite_pattern_addr_lo = 
								( (spriteScanline[i].id & 0x01)      << 12) 
								| (((spriteScanline[i].id & 0xFE) + 1) << 4 )   
								| (7 - (scanline - spriteScanline[i].y) & 0x07); 
						}
						// Tile bottom half
						else {
							sprite_pattern_addr_lo = 
								((spriteScanline[i].id & 0x01)       << 12) 
								| ((spriteScanline[i].id & 0xFE)       << 4 )   
								| (7 - (scanline - spriteScanline[i].y) & 0x07);
						}
					}
				}

				sprite_pattern_addr_hi = sprite_pattern_addr_lo + 8;
				sprite_pattern_bits_lo = ppuRead(sprite_pattern_addr_lo);
				sprite_pattern_bits_hi = ppuRead(sprite_pattern_addr_hi);

				// Flip sprite if it is flipped horizontally
				if (spriteScanline[i].attribute & 0x40) {
					auto flipbyte = [](uint8_t b) {
						b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
						b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
						b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
						return b;
					};

					// Flip patterns
					sprite_pattern_bits_lo = flipbyte(sprite_pattern_bits_lo);
					sprite_pattern_bits_hi = flipbyte(sprite_pattern_bits_hi);
				}

				// Load Patterns
				sprite_shifter_pattern_lo[i] = sprite_pattern_bits_lo;
				sprite_shifter_pattern_hi[i] = sprite_pattern_bits_hi;
			}
		}
	}

	

	// Post Render Scanline
	if (scanline == 240) {

	}

	if (scanline >= 241 && scanline < 261) {
		// End of Frame
		if (scanline == 241 && cycle == 1) {
			status.vertical_blank = 1;
			if (control.enable_nmi)
				nmi = true;
		}
	}

	// BG Pixel Information
	uint8_t bg_pixel = 0x00;
	uint8_t bg_palette = 0x00;

	if (mask.render_background) {
		// BG pixel mux
		uint16_t mux = 0x8000 >> fine_x;

		// Select Bitmap plane pixels
		uint8_t p0_pixel = (bg_shifter_pattern_lo & mux) > 0;
		uint8_t p1_pixel = (bg_shifter_pattern_hi & mux) > 0;

		// Pixel Index
		bg_pixel = (p1_pixel << 1) | p0_pixel;

		// Get Palette
		uint8_t bg_pal0 = (bg_shifter_attrib_lo & mux) > 0;
		uint8_t bg_pal1 = (bg_shifter_attrib_hi & mux) > 0;
		bg_palette = (bg_pal1 << 1) | bg_pal0;
	}

	// FG Pixel Information
	uint8_t fg_pixel = 0x00;
	uint8_t fg_palette= 0x00;
	uint8_t fg_priority = 0x00;

	if (mask.render_sprites) {
		spriteZeroRendered = false;

		for (uint8_t i = 0; i < sprite_count; i++) {
			if (spriteScanline[i].x == 0) {
				uint8_t fg_pixel_lo = (sprite_shifter_pattern_lo[i] & 0x80) > 0;
				uint8_t fg_pixel_hi = (sprite_shifter_pattern_hi[i] & 0x80) > 0;
				fg_pixel = (fg_pixel_hi << 1) | fg_pixel_lo;

				// Palette Info
				fg_palette = (spriteScanline[i].attribute & 0x03) + 0x04;
				fg_priority = (spriteScanline[i].attribute & 0x20) == 0;

				if(fg_pixel != 0) {
					// Sprite Zero
					if (i == 0) {
						spriteZeroRendered = true;
					}

					break;
				}
			}
		}
	}

	// Final Pixel
	uint8_t pixel = 0x00;
	uint8_t palette = 0x00;

	// BG and FG are transparent
	if (bg_pixel == 0 && fg_pixel == 0) {
		// Default to backgroundcolor
		pixel = 0x00;
		palette = 0x00;
	}
	// BG transparent FG visible
	else if (bg_pixel == 0 && fg_pixel > 0) {
		pixel = fg_pixel;
		palette = fg_palette;
	}
	// BG visible FG transparent
	else if (bg_pixel > 0 &&fg_pixel == 0) {
		pixel = bg_pixel;
		palette = bg_palette;
	}
	// Both Visible
	else if (bg_pixel > 0 && fg_pixel > 0) {
		if (fg_priority) {
			pixel = fg_pixel;
			palette = fg_palette;
		}
		else {
			pixel = bg_pixel;
			palette = bg_palette;
		}

		// Sprite Zero Collision
		if (spriteZeroHitPossible && spriteZeroRendered) {
			if (mask.render_background & mask.render_sprites) {
				if (~(mask.render_background_left | mask.render_sprites_left)) {
					if (cycle >= 9 && cycle < 258) {
						status.sprite_zero_hit = 1;
					}
				}
				else {
					if (cycle >= 1 && cycle < 258) {
						status.sprite_zero_hit = 1;
					}
				}
			}
		}
	}

	// Draw Pixel
	sprScreen.SetPixel(cycle - 1, scanline, GetColorFromPaletteRam(palette, pixel));

	cycle++;

    // Iterate through each pixel
	if (cycle >= 341) {
		cycle = 0;
		scanline++;
		if (scanline >= 261) {
			scanline = -1;
			frame_complete = true;
		}
	}
}