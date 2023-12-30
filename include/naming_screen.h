#ifndef GUARD_NAMINGSCREEN_H
#define GUARD_NAMINGSCREEN_H

#include "main.h"

#define KBEVENT_NONE 0
#define KBEVENT_PRESSED_A 5
#define KBEVENT_PRESSED_B 6
#define KBEVENT_PRESSED_SELECT 8
#define KBEVENT_PRESSED_START 9

enum
{
    NAMING_SCREEN_TEMPLATE_PLAYER_NAME,
    NAMING_SCREEN_TEMPLATE_BOX_NAME,
    NAMING_SCREEN_TEMPLATE_MON_NAME,
};

struct NamingScreenTemplate
{
    u8 copyExistingString;
    u8 maxChars;
    u8 iconFunction;
    u8 addGenderIcon;
    u8 initialPage;
    u8 filler[3];
    const u8 *title;
};

struct NamingScreenData
{
    u8 state;
    u8 koreanState;
    u8 templateNum;
    u16 nameLeftOffset;
    u16 bg1vOffset;
    u16 bg2vOffset;
    u16 bg1Priority;
    u16 bg2Priority;
    u8 bgToReveal;
    u8 bgToHide;
    u8 currentPage;
    u8 cursorSpriteId;
    u8 pageIndicatorSpriteId;
    u8 textBuffer[40];
    u8 backupBuffer[40];
    u8 filler21[0x13];
    const struct NamingScreenTemplate *template;
    u8 *destBuffer;
    u16 keyRepeatStartDelayCopy;
    u16 speciesOrPlayerGender;
    u16 monGender;
    u32 monPersonality;
    MainCallback returnCallback;
};

enum
{
    PAGE_KOREAN,
    PAGE_UPPER,
    PAGE_LOWER,
};

enum
{
    MAIN_STATE_BEGIN_FADE_IN,
    MAIN_STATE_WAIT_FADE_IN,
    MAIN_STATE_HANDLE_INPUT,
    MAIN_STATE_MOVE_TO_OK_BUTTON,
    MAIN_STATE_START_PAGE_SWAP,
    MAIN_STATE_WAIT_PAGE_SWAP,
    MAIN_STATE_6,
    MAIN_STATE_UPDATE_SENT_TO_PC_MESSAGE,
    MAIN_STATE_BEGIN_FADE_OUT,
};

enum
{
    INPUT_STATE_DISABLED,
    INPUT_STATE_ENABLED,
};

enum
{
    STATE_NONE,
    STATE_JAUM,
    STATE_MOUM_MERGEABLE,
    STATE_MOUM,
    STATE_JAUM_2_MERGEABLE,
    STATE_JAUM_2,
    STATE_MERGED_JAUM,
};

void DoNamingScreen(u8, u8*, u16, u16, u32, MainCallback);

#endif
