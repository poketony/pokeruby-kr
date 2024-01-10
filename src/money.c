#include "global.h"
#include "event_data.h"
#include "money.h"
#include "decompress.h"
#include "menu.h"
#include "sprite.h"
#include "string_util.h"
#include "graphics.h"

#define SPRITE_TAG_MONEY (0x2722)

static EWRAM_DATA u8 sMoneySprite = 0;

static const struct OamData sMoneyOamData =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 1,
    .x = 0,
    .matrixNum = 0,
    .size = 2,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

static const union AnimCmd sMoneySpriteAnim[] =
{
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sMoneySpriteAnimTable[] =
{
    sMoneySpriteAnim,
};

static const struct SpriteTemplate sMoneySpriteTemplate =
{
    .tileTag = 10018,
    .paletteTag = 10018,
    .oam = &sMoneyOamData,
    .anims = sMoneySpriteAnimTable,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy,
};

static const struct CompressedSpriteSheet sMoneySpriteGfx[] = {gMenuMoneyGfx, 256, SPRITE_TAG_MONEY};
static const struct CompressedSpritePalette sMoneySpritePalette[] = {gMenuMoneyPal, SPRITE_TAG_MONEY};

static const u8 sText_Concurrency[] = _("원");

bool8 IsEnoughMoney(u32 budget, u32 cost)
{
    if (budget >= cost)
    {
        return TRUE;
    }

    return FALSE;
}

void AddMoney(u32 *arg0, u32 arg1)
{
    if (*arg0 > *arg0 + arg1)
    {
        *arg0 = 999999;
        return;
    }

    *arg0 += arg1;
    if (*arg0 > 999999)
    {
        *arg0 = 999999;
    }
}

void RemoveMoney(u32 *arg0, u32 arg1)
{
    if (*arg0 < arg1)
    {
        *arg0 = 0;
    }
    else
    {
        *arg0 -= arg1;
    }
}

void GetMoneyAmountText(u8 *buffer, u32 amount, u8 size)
{
    u8 width;
    u8 i;
    u8 *catPtr = buffer;

    if (amount > 999999)
        width = 7;
    else if (amount > 99999)
        width = 6;
    else if (amount > 9999)
        width = 5;
    else if (amount > 999)
        width = 4;
    else if (amount > 99)
        width = 3;
    else if (amount > 9)
        width = 2;
    else
        width = 1;

    *(buffer++) = EXT_CTRL_CODE_BEGIN;
    *(buffer++) = EXT_CTRL_CODE_MIN_LETTER_SPACING;
    *(buffer++) = 6;

    for (i = 0; i < size - width; i++)
        *(buffer++) = CHAR_SPACE;

    buffer = ConvertIntToDecimalString(buffer, amount);
    buffer = StringAppend(catPtr, sText_Concurrency);
}

void PrintMoneyAmount(u32 amount, u8 size, u8 x, u8 y)
{
    u8 buffer[16];
    u8 stringWidth;

    GetMoneyAmountText(buffer, amount, size);
    stringWidth = GetStringWidthInMenuWindow(buffer);

    if (stringWidth >= (size + 1) * 8)
    {
        Menu_PrintText(buffer, x, y);
    }
    else
    {
        int xPlusOne = x + 1;
        Menu_PrintTextPixelCoords(buffer, (xPlusOne + size) * 8 - stringWidth, y * 8, 1);
    }
}

void sub_80B7AEC(u32 arg0, u8 right, u8 top)
{
    u8 buffer[32];
    u8 *ptr;

    ptr = ConvertIntToDecimalString(buffer, arg0);
    ptr = StringAppend(buffer, sText_Concurrency);

    MenuPrint_RightAligned(buffer, right, top);
}

void UpdateMoneyWindow(u32 amount, u8 x, u8 y)
{
    PrintMoneyAmount(amount, 6, x + 6, y + 1);
}

void OpenMoneyWindow(u32 amount, u8 x, u8 y)
{
    Menu_DrawStdWindowFrame(x, y, x + 13, y + 3);
    UpdateMoneyWindow(amount, x, y);

    LoadCompressedObjectPic(sMoneySpriteGfx);
    LoadCompressedObjectPalette(sMoneySpritePalette);

    sMoneySprite = CreateSprite(&sMoneySpriteTemplate, x * 8 + 19, y * 8 + 11, 0);
}

void CloseMoneyWindow(u8 x, u8 y)
{
    DestroySpriteAndFreeResources(&gSprites[sMoneySprite]);
    FreeSpritePaletteByTag(SPRITE_TAG_MONEY);
    Menu_EraseWindowRect(x, y, x + 13, y + 3);
}

bool8 HasEnoughMoneyFor(void)
{
    return IsEnoughMoney(gSaveBlock1.money, gSpecialVar_0x8005);
}

void PayMoneyFor(void)
{
    RemoveMoney(&gSaveBlock1.money, gSpecialVar_0x8005);
}
