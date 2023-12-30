#ifndef GUARD_POKEDEX_H
#define GUARD_POKEDEX_H

#include "sprite.h"

#define HOENN_DEX_COUNT 202
#define NATIONAL_DEX_COUNT 386

void ResetPokedex(void);
void ResetPokedexScrollPositions(void);
void CB2_InitPokedex(void);
u8 sub_809070C(u16 dexNum, u32 b, u32 c);
const u8 *GetPokemonCategory(u16);
u16 GetPokedexHeightWeight(u16 dexNum, u8 data);

enum
{
    FLAG_GET_SEEN,
    FLAG_GET_CAUGHT,
    FLAG_SET_SEEN,
    FLAG_SET_CAUGHT
};

enum
{
    NAME_1 = 1,
    NAME_2,
    NAME_3,
    NAME_4,
    NAME_5,
    NAME_6,
    NAME_7,
};

s8 GetSetPokedexFlag(u16, u8);

u16 GetNationalPokedexCount(u8);
u16 GetHoennPokedexCount(u8);
bool8 CompletedHoennPokedex(void);
bool16 CompletedNationalPokedex(void);

extern bool8 gUnusedPokedexU8;

struct PokedexEntry
{
    u8 categoryName[14];
    u16 height; //in decimeters
    u16 weight; //in hectograms
    const u8 *description;
    u16 unused;
    u16 pokemonScale;
    u16 pokemonOffset;
    u16 trainerScale;
    u16 trainerOffset;
};

extern const struct PokedexEntry gPokedexEntries[];

u16 CreateSizeScreenTrainerPic(u16 gender, s16 x, s16 y, u16 paletteNum);
u16 CreateMonSpriteFromNationalDexNumber(u16 num, s16 x, s16 y, u16 paletteNum);

void PrintFootprint(u16 num, u16 b, u16 c);

#endif // GUARD_POKEDEX_H
