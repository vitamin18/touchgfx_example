// 4.16.1 0xe8088cd5
// Generated by imageconverter. Please, do not edit!

#include <BitmapDatabase.hpp>
#include <touchgfx/Bitmap.hpp>

extern const unsigned char image_menu[]; // BITMAP_MENU_ID = 0, Size: 60x32 pixels

const touchgfx::Bitmap::BitmapData bitmap_database[] =
{
    { image_menu, 0, 60, 32, 14, 10, 2, (uint8_t)(touchgfx::Bitmap::ARGB8888) >> 3, 12, (uint8_t)(touchgfx::Bitmap::ARGB8888) & 0x7 }
};

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return bitmap_database;
}

uint16_t getInstanceSize()
{
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
}
}
