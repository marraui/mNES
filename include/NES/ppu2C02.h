#ifndef PPU2C02_H
#define PPU2C02_H
#include <cstdint>
#include <functional>

class Ppu2C02 {
  public:
    struct COLOR {
      uint8_t r;
      uint8_t g;
      uint8_t b;
      uint8_t a = 0xFF;
    };
  private:
    uint8_t nameTable[2 * 1024];
    uint8_t palette[32];
    COLOR paletteColors[16 * 4];
    COLOR screen[256 * 240];
    COLOR patternTableScreen[2][256 * 240];
    COLOR nameTableScreen[2][256 * 240];
    unsigned int cycle;
    unsigned int scanline;
  public:
    Ppu2C02(
      std::function<void(uint16_t addr, uint8_t data)> write,
      std::function<uint8_t(uint16_t addr, bool isReadOnly)> read
    );
    ~Ppu2C02();

    void clock();

    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);
    uint8_t ppuRead(uint16_t addr, bool readOnly = false);
    void ppuWrite(uint16_t addr, uint8_t data);

    std::function<void(uint16_t addr, uint8_t data)> writeCart;
    std::function<uint8_t(uint16_t addr, bool isReadOnly)> readCart;

    COLOR* getScreen();
    COLOR* getPaletteScreen();
    COLOR* getSpriteScreen();
    COLOR* getPatternTableScreen();
    COLOR* getNameTableScreen();

    bool isFrameComplete();

};

#endif // PPU2C02_H