#ifndef POKERUBY_MON_MARKINGS_H
#define POKERUBY_MON_MARKINGS_H

struct PokemonMarkMenu
{
    u16 baseTileTag;
    u16 basePaletteTag;
    u8 markings; // bit flags
    s8 cursorPos;
    bool8 markingsArray[4];
    u8 cursorBaseY;
    bool8 spriteSheetLoadRequired;
    struct Sprite *menuWindowSprites[2]; // upper and lower halves of menu window
    struct Sprite *menuMarkingSprites[4];
    struct Sprite *menuTextSprite;
    struct Sprite *menuCursorSprite;
    const u8 *frameTiles;
    const u16 *framePalette;
    u8 menuWindowSpriteTiles[0x1000];
    u8 filler1030[0x80];
    u8 tileLoadState;
};

void sub_80F727C(struct PokemonMarkMenu *ptr);
void sub_80F7404(void);
void sub_80F7418(u8 markings, s16 x, s16 y);
void sub_80F7470(void);
bool8 sub_80F7500(void);
struct Sprite *sub_80F7940(u16 tileTag, u16 paletteTag, const u16 *palette);
void sub_80F7A10(u8 markings, void *dest);

#endif //POKERUBY_MON_MARKINGS_H
