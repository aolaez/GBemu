#pragma once

#include <common.h> 

// Cartridge Header is an internal information area, for the GameBoy it's located at addresses 0x0100-014F
// It contains several values



typedef struct{
    u8 entry[4]; // 0x0100-0103 : Entry Point, after displaying Nintendo logo, GB jumps to 0x0100, which then jumps to main program in cartridge, 4 bytes
    u8 logo[0x30]; // 0x0104-0133 : Nintendo Logo, always same as below, if it doesn't match the GB won't allow game to run, later models only check top hald
    /* First half of bytes display top half logo
    CE ED 66 66 CC 0D 00 0B 03 73 00 83 00 0C 00 0D
    00 08 11 1F 88 89 00 0E DC CC 6E E6 DD DD D9 99
    BB BB 67 63 6E 0E EC CC DD DC 99 9F BB B9 33 3E
    */

    char title[16]; // Contain game title in upper case ASCII, if title < 16 chars, pad with $00s
    u16 new_lic_code; // 2 bytes for ASCII code indicating game publisher, key online
    u8 sgb_flag; // Specifies if game supports SGB functions, ignore SGB functions if != 0x03
    u8 type; // Indicates hardware present on cartridge
    u8 rom_size; // How much ROM is present on cartridge
    u8 ram_size; // How much RAM is present on cartridge, if cart type doesn't say RAM, set to 0
    u8 dest_code; // Is game meant to be sold in JP, 00 or 01 only
    u8 lic_code; // Used in pre-SGB carts to specify publisher, set to $33 means look at new_lic_code
    u8 version; // Specify game version number, usually $00
    u8 checksum; // Contains 8-bit checksum computed from header bytes $0134-014C, if this byte doesn't match result, cartridge won't run
    /*
    Checksum = checksum - rom[address] - 1; for loop through addresses above
    */
    u16 global_checksum; // Contain 16-bit big-endian checksum computed as the sum of all bytes of cartridge ROM except these two
} rom_header;

bool cart_load(char *cart);

u8 cart_read(u16 address);

void cart_write(u16 address, u8 value);
