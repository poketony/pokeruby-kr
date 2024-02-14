#include "global.h"
#include "naming_screen.h"
#include "data2.h"
#include "graphics.h"
#include "field_effect.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "korean.h"
#include "main.h"
#include "menu.h"
#include "palette.h"
#include "pokemon_icon.h"
#include "constants/songs.h"
#include "sound.h"
#include "sprite.h"
#include "string_util.h"
#include "strings2.h"
#include "task.h"
#include "text.h"
#include "trig.h"
#include "util.h"
#include "ewram.h"

#define COLUMN_COUNT 10

const u8 gSpriteImage_83CE094[] = INCBIN_U8("graphics/naming_screen/pc_icon/0.4bpp");
const u8 gSpriteImage_83CE154[] = INCBIN_U8("graphics/naming_screen/pc_icon/1.4bpp");

struct NamingScreenData *const namingScreenDataPtr = &namingScreenData;

extern const struct SubspriteTable gSubspriteTables_83CE558[];
extern const struct SubspriteTable gSubspriteTables_83CE560[];
extern const struct SubspriteTable gSubspriteTables_83CE578[];
extern const struct SubspriteTable gSubspriteTables_83CE580[];
extern const struct SpriteTemplate gSpriteTemplate_83CE5C8;
extern const struct SpriteTemplate gSpriteTemplate_83CE5E0;
extern const struct SpriteTemplate gSpriteTemplate_83CE5F8;
extern const struct SpriteTemplate gSpriteTemplate_83CE610;
extern const struct SpriteTemplate gSpriteTemplate_83CE628;
extern const struct SpriteTemplate gSpriteTemplate_83CE640;
extern const struct SpriteTemplate gSpriteTemplate_83CE658;
extern const struct SpriteTemplate gSpriteTemplate_83CE670;
extern const struct SpriteTemplate gSpriteTemplate_83CE688;
extern const struct SpriteSheet gUnknown_083CE6A0[];
extern const struct SpritePalette gUnknown_083CE708[];
extern const u8 gNamingScreenMenu_Gfx[];
extern const u16 gUnknown_083CE748[];
extern const u16 gUnknown_083CEBF8[];
extern const u16 gUnknown_083CF0A8[];
extern const u16 gUnknown_08E86258[];

static void C2_NamingScreen(void);
static void sub_80B5AA0(void);
static void StoreNamingScreenParameters(u8, u8 *, u16, u16, u32, MainCallback);
static void NamingScreen_TurnOffScreen(void);
static void NamingScreen_Init(void);
static void NamingScreen_ClearVram(void);
static void NamingScreen_ClearOam(void);
static void NamingScreen_SetUpVideoRegs(void);
static void NamingScreen_SetUpWindow(void);
static void NamingScreen_ResetObjects(void);
static void sub_80B5DFC(void);
static void sub_80B5E20(void);
static void sub_80B5E3C(void);
static void NamingScreen_InitDisplayMode(void);
static void Task_DoNothing(u8);
static void sub_80B7558(void);
static void sub_80B753C(void);
static void sub_80B7680(void);
static void sub_80B75C4(void);
static void sub_80B7794(void);
static void sub_80B78A8(void);
static void DrawTextEntry(void);
static void CursorInit(void);
static void CreatePageSwapButtonSprites(void);
static void CreateBackOkSprites(void);
static void CreateTextEntrySprites(void);
static void CreateInputTargetIcon(void);
static void InputInit(void);
static void sub_80B6438(void);
static void sub_80B5E50(void);
static void Task_NamingScreenMain(u8);
static u8 GetInputEvent(void);
static void SetInputState(u8);
static void SetCursorFlashing(bool8);
static bool8 HandleKeyboardEvent(void);
static bool8 IsCursorAnimFinished(void);
static void MoveCursorToOKButton(void);
static void StartPageSwapButtonAnim(void);
static void StartPageSwapAnim(void);
static void SetCursorInvisibility(bool8);
static void TryStartButtonFlash(u8, u8, u8);
static bool8 IsPageSwapAnimNotInProgress(void);
static void sub_80B7614(void);
static void GetCursorPos(s16 *, s16 *);
static void SetCursorPos(s16, s16);
static void sub_80B77F8(void);
static void SaveInputText(void);
static void DisplaySentToPCMessage(void);
static u8 GetKeyRoleAtCursorPos(void);
static u8 SwapKeyboardPage(void);
static void DeleteTextCharacter(void);
static bool8 AddTextCharacter(void);
static void SquishCursor(void);
static void Task_HandlePageSwapAnim(u8);
static void SetPageSwapButtonGfx(u8, struct Sprite *, struct Sprite *);
static u8 GetTextEntryPosition(void);
static u8 GetCharAtKeyboardPos(s16, s16);
static void BufferCharacter(u8);
static void sub_80B7568(void);
static void sub_80B75B0(void);
static void sub_80B7698(u16 *, const u16 *);
static void sub_80B76E0();
static void nullsub_20(u8, u8);
static void PrintKeyboardCharacters(u8);

void DoNamingScreen(u8 templateNum, u8 *destBuffer, u16 speciesOrPlayerGender, u16 monGender, u32 monPersonality, MainCallback returnCallback)
{
    StoreNamingScreenParameters(templateNum, destBuffer, speciesOrPlayerGender, monGender, monPersonality, returnCallback);
    SetMainCallback2(C2_NamingScreen);
}

static void C2_NamingScreen(void)
{
    switch (gMain.state)
    {
    case 0:
        NamingScreen_TurnOffScreen();
        NamingScreen_Init();
        gMain.state++;
        break;
    case 1:
        NamingScreen_ClearVram();
        gMain.state++;
        break;
    case 2:
        NamingScreen_ClearOam();
        gMain.state++;
        break;
    case 3:
        NamingScreen_SetUpVideoRegs();
        gMain.state++;
        break;
    case 4:
        NamingScreen_SetUpWindow();
        gMain.state++;
        break;
    case 5:
        NamingScreen_ResetObjects();
        gMain.state++;
        break;
    case 6:
        sub_80B5DFC();
        gMain.state++;
        break;
    case 7:
        sub_80B5E20();
        sub_80B5E3C();
        NamingScreen_InitDisplayMode();
        SetMainCallback2(sub_80B5AA0);
        break;
    }
}

static u8 *NamingScreen_StringCopyMultibyte(u8 *dest, const u8 *src)
{
    while (*src != EOS)
    {
        if (IsKoreanGlyph(*src))
        {
            *dest++ = *src++;
            *dest++ = *src++;
        }
        else
        {
            *dest++ = 0;
            *dest++ = *src++;
        }
    }

    *dest = EOS;
    return dest;
}

static u8 *NamingScreen_StringCopy(u8 *dest, const u8 *src)
{
    while (*src != EOS)
    {
        if (*src == 0)
        {
            *(dest++) = *(++src);
            src++;
        }
        else
        {
            *dest++ = *src++;
            *dest++ = *src++;
        }
    }

    *dest = EOS;
    return dest;
}

static void sub_80B5AA0(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_NamingScreen(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
    REG_BG1VOFS = namingScreenDataPtr->bg1vOffset;
    REG_BG2VOFS = namingScreenDataPtr->bg2vOffset;
    REG_BG1CNT &= 0xFFFC;
    REG_BG1CNT |= namingScreenDataPtr->bg1Priority;
    REG_BG2CNT &= 0xFFFC;
    REG_BG2CNT |= namingScreenDataPtr->bg2Priority;
}

static void StoreNamingScreenParameters(u8 templateNum, u8 *destBuffer, u16 speciesOrPlayerGender, u16 monGender, u32 monPersonality, MainCallback returnCallback)
{
    struct Task *task;

    //Create a task that does nothing, and use it as a temporary space to store parameters
    task = &gTasks[CreateTask(Task_DoNothing, 0xFF)];
    task->data[0] = templateNum;
    task->data[1] = speciesOrPlayerGender;
    task->data[2] = monGender;
    task->data[3] = monPersonality >> 16;
    task->data[4] = monPersonality;
    StoreWordInTwoHalfwords(&task->data[5], (u32)destBuffer);
    StoreWordInTwoHalfwords(&task->data[7], (u32)returnCallback);
}

static void GetNamingScreenParameters(void)
{
    u8 taskId;
    struct Task *task;

    taskId = FindTaskIdByFunc(Task_DoNothing);
    task = &gTasks[taskId];
    namingScreenDataPtr->templateNum = task->data[0];
    namingScreenDataPtr->speciesOrPlayerGender = task->data[1];
    namingScreenDataPtr->monGender = task->data[2];
    namingScreenDataPtr->monPersonality = (task->data[3] << 16) | (u16)task->data[4];
    LoadWordFromTwoHalfwords(&task->data[5], (u32 *)&namingScreenDataPtr->destBuffer);
    LoadWordFromTwoHalfwords(&task->data[7], (u32 *)&namingScreenDataPtr->returnCallback);
    DestroyTask(taskId);
}

static void Task_DoNothing(u8 taskId)
{
}

static void NamingScreen_TurnOffScreen(void)
{
    SetVBlankCallback(NULL);
    SetHBlankCallback(NULL);
    REG_DISPCNT = 0;
}

static void NamingScreen_InitDisplayMode(void)
{
    u16 savedIme;

    SetVBlankCallback(VBlankCB_NamingScreen);
    savedIme = REG_IME;
    REG_IME = 0;
    REG_IE |= INTR_FLAG_VBLANK;
    REG_IME = savedIme;
    REG_DISPSTAT |= DISPSTAT_VBLANK_INTR;
    REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
}

static void NamingScreen_ClearVram(void)
{
    DmaFill16Large(3, 0, (void *)VRAM, 0x10000, 0x1000);
}

static void NamingScreen_ClearOam(void)
{
    DmaClear16(3, (void *)OAM, 0x400);
}

static void NamingScreen_SetUpVideoRegs(void)
{
    REG_BG0CNT = 0;
    REG_BG1CNT = 0;
    REG_BG2CNT = 0;
    REG_BG3CNT = 0;
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(31) | BGCNT_16COLOR | BGCNT_TXT256x256;
    REG_BG1CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(28) | BGCNT_16COLOR | BGCNT_TXT256x256;
    REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(29) | BGCNT_16COLOR | BGCNT_TXT256x256;
    REG_BG3CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(0) | BGCNT_SCREENBASE(30) | BGCNT_16COLOR | BGCNT_TXT256x256;
    REG_BLDCNT = 0x640;
    REG_BLDALPHA = 0x80C;
}

static const struct NamingScreenTemplate *const sNamingScreenTemplates[];  //forward declaration

static void NamingScreen_Init(void)
{
    GetNamingScreenParameters();
    namingScreenDataPtr->state = 0;
    namingScreenDataPtr->koreanState = 0;
    namingScreenDataPtr->bg1vOffset = 0;
    namingScreenDataPtr->bg2vOffset = 0;
    namingScreenDataPtr->bg1Priority = BGCNT_PRIORITY(1);
    namingScreenDataPtr->bg2Priority = BGCNT_PRIORITY(2);
    namingScreenDataPtr->bgToReveal = 0;
    namingScreenDataPtr->bgToHide = 1;
    namingScreenDataPtr->template = sNamingScreenTemplates[namingScreenDataPtr->templateNum];
    namingScreenDataPtr->currentPage = namingScreenDataPtr->template->initialPage;
    namingScreenDataPtr->nameLeftOffset = 14 - namingScreenDataPtr->template->maxChars / 2;
    namingScreenDataPtr->keyRepeatStartDelayCopy = gKeyRepeatStartDelay;
    memset(namingScreenDataPtr->textBuffer, 0xFF, sizeof(namingScreenDataPtr->textBuffer));
    if (namingScreenDataPtr->template->copyExistingString)
        NamingScreen_StringCopyMultibyte(namingScreenDataPtr->textBuffer, namingScreenDataPtr->destBuffer);
    NamingScreen_StringCopyMultibyte(namingScreenDataPtr->backupBuffer, namingScreenDataPtr->destBuffer);
    gKeyRepeatStartDelay = 16;
}

static void NamingScreen_SetUpWindow(void)
{
    Text_LoadWindowTemplate(&gWindowTemplate_81E6E88);
    InitMenuWindow(&gWindowTemplate_81E6E88);
}

static void NamingScreen_ResetObjects(void)
{
    ResetPaletteFade();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetTasks();
}

static void sub_80B5DFC(void)
{
    sub_80B7558();
    sub_80B753C();
    sub_80B7680();
    sub_80B75C4();
    sub_80B7794();
    sub_80B78A8();
    DrawTextEntry();
}

static void sub_80B5E20(void)
{
    CursorInit();
    CreatePageSwapButtonSprites();
    CreateBackOkSprites();
    CreateTextEntrySprites();
    CreateInputTargetIcon();
}

static void sub_80B5E3C(void)
{
    InputInit();
    sub_80B6438();
    sub_80B5E50();
}

//--------------------------------------------------
// Naming screen main
//--------------------------------------------------

static bool8 MainState_BeginFadeIn(struct Task *);
static bool8 MainState_WaitFadeIn(struct Task *);
static bool8 MainState_HandleInput(struct Task *);
static bool8 MainState_MoveToOKButton(struct Task *);
static bool8 MainState_StartPageSwap(struct Task *);
static bool8 MainState_WaitPageSwap(struct Task *);
static bool8 MainState_6(struct Task *);
static bool8 MainState_UpdateSentToPCMessage(struct Task *);
static bool8 MainState_BeginFadeInOut(struct Task *);
static bool8 MainState_WaitFadeOutAndExit(struct Task *);

static bool8 (*const sMainStateFuncs[])(struct Task *) =
{
    MainState_BeginFadeIn,
    MainState_WaitFadeIn,
    MainState_HandleInput,
    MainState_MoveToOKButton,
    MainState_StartPageSwap,
    MainState_WaitPageSwap,
    MainState_6,
    MainState_UpdateSentToPCMessage,
    MainState_BeginFadeInOut,
    MainState_WaitFadeOutAndExit,
};

static void sub_80B5E50(void)
{
    u8 taskId;

    taskId = CreateTask(Task_NamingScreenMain, 2);
    Task_NamingScreenMain(taskId);
}

static void Task_NamingScreenMain(u8 taskId)
{
    while (sMainStateFuncs[namingScreenDataPtr->state](&gTasks[taskId]) != 0);
}

static bool8 MainState_BeginFadeIn(struct Task *task)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB(0, 0, 0));
    namingScreenDataPtr->state++;
    return 0;
}

static bool8 MainState_WaitFadeIn(struct Task *task)
{
    if (!gPaletteFade.active)
    {
        SetInputState(INPUT_STATE_ENABLED);
        SetCursorFlashing(1);
        namingScreenDataPtr->state++;
    }
    return FALSE;
}

static bool8 MainState_HandleInput(struct Task *task)
{
    return HandleKeyboardEvent();
}

static bool8 MainState_MoveToOKButton(struct Task *task)
{
    if (IsCursorAnimFinished())
    {
        SetInputState(INPUT_STATE_ENABLED);
        MoveCursorToOKButton();
        namingScreenDataPtr->state = MAIN_STATE_HANDLE_INPUT;
    }
    return FALSE;
}

static bool8 MainState_StartPageSwap(struct Task *task)
{
    SetInputState(INPUT_STATE_DISABLED);
    StartPageSwapButtonAnim();
    StartPageSwapAnim();
    SetCursorInvisibility(1);
    TryStartButtonFlash(0, 0, 1);
    PlaySE(SE_WIN_OPEN);
    namingScreenDataPtr->state = MAIN_STATE_WAIT_PAGE_SWAP;
    return FALSE;
}

static bool8 MainState_WaitPageSwap(struct Task *task)
{
    if (IsPageSwapAnimNotInProgress())
    {
        namingScreenDataPtr->state = MAIN_STATE_HANDLE_INPUT;
        namingScreenDataPtr->currentPage++;
        namingScreenDataPtr->currentPage %= 3;

        sub_80B7614();
        sub_80B77F8();
        SetInputState(INPUT_STATE_ENABLED);
        SetCursorInvisibility(0);
    }
    return FALSE;
}

static bool8 MainState_6(struct Task *task)
{
    SaveInputText();
    SetInputState(INPUT_STATE_DISABLED);
    SetCursorFlashing(0);
    TryStartButtonFlash(3, 0, 1);
    gKeyRepeatStartDelay = namingScreenDataPtr->keyRepeatStartDelayCopy;
    if (namingScreenDataPtr->templateNum == NAMING_SCREEN_TEMPLATE_MON_NAME
     && CalculatePlayerPartyCount() >= 6)
    {
        DisplaySentToPCMessage();
        namingScreenDataPtr->state = MAIN_STATE_UPDATE_SENT_TO_PC_MESSAGE;
        return FALSE;
    }
    else
    {
        namingScreenDataPtr->state = MAIN_STATE_BEGIN_FADE_OUT;
        return TRUE;  //Exit the naming screen
    }
}

static bool8 MainState_UpdateSentToPCMessage(struct Task *task)
{
    if (Menu_UpdateWindowText())
        namingScreenDataPtr->state++;
    return FALSE;
}

static bool8 MainState_BeginFadeInOut(struct Task *task)
{
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB(0, 0, 0));
    namingScreenDataPtr->state++;
    return FALSE;
}

static bool8 MainState_WaitFadeOutAndExit(struct Task *task)
{
    if (!gPaletteFade.active)
        SetMainCallback2(namingScreenDataPtr->returnCallback);
    return FALSE;
}

//--------------------------------------------------
// Keyboard handling
//--------------------------------------------------

enum
{
    KEY_ROLE_CHAR,
    KEY_ROLE_PAGE,
    KEY_ROLE_BACKSPACE,
    KEY_ROLE_OK,
};


static bool8 KeyboardKeyHandler_Character(u8);
static bool8 KeyboardKeyHandler_Page(u8);
static bool8 KeyboardKeyHandler_Backspace(u8);
static bool8 KeyboardKeyHandler_OK(u8);

static bool8 (*const sKeyboardKeyHandlers[])(u8) =
{
    KeyboardKeyHandler_Character,
    KeyboardKeyHandler_Page,
    KeyboardKeyHandler_Backspace,
    KeyboardKeyHandler_OK,
};

static bool8 HandleKeyboardEvent(void)
{
    u8 event = GetInputEvent();
    u8 keyRole = GetKeyRoleAtCursorPos();

    if (event == KBEVENT_PRESSED_SELECT)
    {
        return SwapKeyboardPage();
    }
    else if (event == KBEVENT_PRESSED_B)
    {
        DeleteTextCharacter();
        return FALSE;
    }
    else if (event == 7)
    {
        MoveCursorToOKButton();
        return FALSE;
    }

    return sKeyboardKeyHandlers[keyRole](event);
}

static bool8 KeyboardKeyHandler_Character(u8 event)
{
    TryStartButtonFlash(3, 0, 0);
    if (event == KBEVENT_PRESSED_A)
    {
        u8 textFull = AddTextCharacter();

        SquishCursor();
        if (textFull)
        {
            SetInputState(INPUT_STATE_DISABLED);
            namingScreenDataPtr->state = MAIN_STATE_MOVE_TO_OK_BUTTON;
        }
    }
    return FALSE;
}

static bool8 KeyboardKeyHandler_Page(u8 event)
{
    TryStartButtonFlash(0, 1, 0);
    if (event == KBEVENT_PRESSED_A)
        return SwapKeyboardPage();
    else
        return FALSE;
}

static bool8 KeyboardKeyHandler_Backspace(u8 event)
{
    TryStartButtonFlash(1, 1, 0);
    if (event == KBEVENT_PRESSED_A)
        DeleteTextCharacter();
    return FALSE;
}

static bool8 KeyboardKeyHandler_OK(u8 event)
{
    TryStartButtonFlash(2, 1, 0);
    if (event == KBEVENT_PRESSED_A)
    {
        PlaySE(SE_SELECT);
        namingScreenDataPtr->state = MAIN_STATE_6;
        return TRUE;
    }
    else
        return FALSE;
}

static bool8 SwapKeyboardPage(void)
{
    namingScreenDataPtr->state = MAIN_STATE_START_PAGE_SWAP;
    return TRUE;
}

//--------------------------------------------------
// Input handling
//--------------------------------------------------

enum
{
    FNKEY_CASE,
    FNKEY_BACK,
    FNKEY_OK,
};

#define tState data[0]
#define tKeyboardEvent data[1]
#define tKbFunctionKey data[2]

static void InputState_Disabled(struct Task *);
static void InputState_Enabled(struct Task *);

static void (*const sInputStateFuncs[])(struct Task *) =
{
    InputState_Disabled,
    InputState_Enabled,
};

static void Task_HandleInput(u8);
static void HandleDpadMovement(struct Task *);

static void InputInit(void)
{
    CreateTask(Task_HandleInput, 1);
}

static u8 GetInputEvent(void)
{
    u8 taskId = FindTaskIdByFunc(Task_HandleInput);

    return gTasks[taskId].tKeyboardEvent;
}

static void SetInputState(u8 state)
{
    u8 taskId = FindTaskIdByFunc(Task_HandleInput);

    gTasks[taskId].tState = state;
}

static void Task_HandleInput(u8 taskId)
{
    sInputStateFuncs[gTasks[taskId].tState](&gTasks[taskId]);
}

static void InputState_Disabled(struct Task *task)
{
    task->tKeyboardEvent = 0;
}

static void InputState_Enabled(struct Task *task)
{
    task->tKeyboardEvent = 0;
    if (gMain.newKeys & A_BUTTON)
    {
        task->tKeyboardEvent = KBEVENT_PRESSED_A;
        return;
    }
    if (gMain.newKeys & B_BUTTON)
    {
        task->tKeyboardEvent = KBEVENT_PRESSED_B;
        return;
    }
    if (gMain.newKeys & SELECT_BUTTON)
    {
        task->tKeyboardEvent = KBEVENT_PRESSED_SELECT;
        return;
    }
    if (gMain.newKeys & START_BUTTON)
    {
        task->tKeyboardEvent = KBEVENT_PRESSED_START;
        MoveCursorToOKButton();
        return;
    }
    HandleDpadMovement(task);
}

static void HandleDpadMovement(struct Task *task)
{
    const s16 sDpadDeltaX[] =
    {
        0,   //none
        0,   //up
        0,   //down
        -1,  //left
        1    //right
    };

    const s16 sDpadDeltaY[] =
    {
        0,   //none
        -1,  //up
        1,   //down
        0,   //left
        0    //right
    };

    const s16 sKeyRowToButtonRow[] = {0, 1, 1, 2};
    const s16 sButtonRowToKeyRow[] = {0, 0, 3, 0};

    s16 cursorX;
    s16 cursorY;
    u16 dpadDir;
    s16 prevCursorX;

    GetCursorPos(&cursorX, &cursorY);
    dpadDir = 0;
    if (gMain.newAndRepeatedKeys & DPAD_UP)
        dpadDir = 1;
    if (gMain.newAndRepeatedKeys & DPAD_DOWN)
        dpadDir = 2;
    if (gMain.newAndRepeatedKeys & DPAD_LEFT)
        dpadDir = 3;
    if (gMain.newAndRepeatedKeys & DPAD_RIGHT)
        dpadDir = 4;

    // Get new cursor position
    prevCursorX = cursorX;
    cursorX += sDpadDeltaX[dpadDir];
    cursorY += sDpadDeltaY[dpadDir];

    // Wrap cursor position in the X direction
    if (cursorX < 0)
        cursorX = COLUMN_COUNT;
    if (cursorX > COLUMN_COUNT)
        cursorX = 0;

    // Handle cursor movement in X direction
    if (sDpadDeltaX[dpadDir] != 0)
    {
        if (cursorX == COLUMN_COUNT)
        {
            // We are now on the last column
            task->tKbFunctionKey = cursorY;
            cursorY = sKeyRowToButtonRow[cursorY];
        }
        else if (prevCursorX == COLUMN_COUNT)
        {
            if (cursorY == 1)
                cursorY = task->tKbFunctionKey;
            else
                cursorY = sButtonRowToKeyRow[cursorY];
        }
    }

    if (cursorX == COLUMN_COUNT)
    {
        // There are only 3 keys on the last column, unlike the others,
        // so wrap Y accordingly
        if (cursorY < 0)
            cursorY = 2;
        if (cursorY > 2)
            cursorY = 0;

        if (cursorY == 0)
            task->tKbFunctionKey = FNKEY_BACK;
        else if (cursorY == 2)
            task->tKbFunctionKey = FNKEY_OK;
    }
    else
    {
        if (cursorY < 0)
            cursorY = 3;
        if (cursorY > 3)
            cursorY = 0;
    }
    SetCursorPos(cursorX, cursorY);
}

#undef tState
#undef tKeyboardEvent
#undef tKbFunctionKey

//--------------------------------------------------
//
//--------------------------------------------------

static void Task_80B64D4(u8);
static u16 sub_80B654C(u8);
static void sub_80B65AC(u8);
static void sub_80B65D4(struct Task *, u8, u8);

static void sub_80B6438(void)
{
    u8 taskId;

    taskId = CreateTask(Task_80B64D4, 3);
    gTasks[taskId].data[0] = 3;
}

static void TryStartButtonFlash(u8 a, u8 b, u8 c)
{
    struct Task *task = &gTasks[FindTaskIdByFunc(Task_80B64D4)];

    if (a == task->data[0] && c == 0)
    {
        task->data[1] = b;
        task->data[2] = 1;
        return;
    }
    if (a == 3 && task->data[1] == 0 && c == 0)
        return;
    if (task->data[0] != 3)
        sub_80B65AC(task->data[0]);
    sub_80B65D4(task, a, b);
}

static void Task_80B64D4(u8 taskId)
{
    struct Task *task = &gTasks[taskId];

    if (task->data[0] == 3 || task->data[2] == 0)
        return;
    MultiplyInvertedPaletteRGBComponents(sub_80B654C(task->data[0]), task->data[3], task->data[3], task->data[3]);
    if (task->data[5] != 0)
    {
        task->data[5]--;
        if (task->data[5] != 0)
            return;
    }
    task->data[5] = 2;
    task->data[3] += task->data[4];
    if (task->data[3] == 16)
        task->data[4] = -task->data[4];
    else if (task->data[3] == 0)
    {
        task->data[2] = task->data[1];
        task->data[4] = -task->data[4];
    }
}

static u16 sub_80B654C(u8 a)
{
    const u16 arr[] =
    {
        IndexOfSpritePaletteTag(4) * 16 + 0x10E,
        IndexOfSpritePaletteTag(6) * 16 + 0x10C,
        IndexOfSpritePaletteTag(6) * 16 + 0x10E,
    };

    return arr[a];
}

static void sub_80B65AC(u8 a)
{
    u16 index = sub_80B654C(a);

    gPlttBufferFaded[index] = gPlttBufferUnfaded[index];
}

static void sub_80B65D4(struct Task *task, u8 b, u8 c)
{
    task->data[0] = b;
    task->data[1] = c;
    task->data[2] = 1;
    task->data[3] = 15;
    task->data[4] = 1;
    task->data[5] = 0;
}

//--------------------------------------------------
// Page Swap
//--------------------------------------------------

#define tState data[0]
#define tFrameCount data[1]

static bool8 PageSwapAnimState_Init(struct Task *);
static bool8 PageSwapAnimState_1(struct Task *);
static bool8 PageSwapAnimState_2(struct Task *);
static bool8 PageSwapAnimState_Done(struct Task *);

static bool8 (*const sPageSwapAnimStateFuncs[])(struct Task *) =
{
    PageSwapAnimState_Init,
    PageSwapAnimState_1,
    PageSwapAnimState_2,
    PageSwapAnimState_Done,
};

static void StartPageSwapAnim(void)
{
    u8 taskId;

    taskId = CreateTask(Task_HandlePageSwapAnim, 0);
    Task_HandlePageSwapAnim(taskId);
}

static bool8 IsPageSwapAnimNotInProgress(void)
{
    if (FindTaskIdByFunc(Task_HandlePageSwapAnim) == 0xFF)
        return TRUE;
    else
        return FALSE;
}

static void Task_HandlePageSwapAnim(u8 taskId)
{
    while (sPageSwapAnimStateFuncs[gTasks[taskId].tState](&gTasks[taskId]) != 0)
        ;
}

static bool8 PageSwapAnimState_Init(struct Task *task)
{
    namingScreenDataPtr->bg1vOffset = 0;
    namingScreenDataPtr->bg2vOffset = 0;
    task->tState++;
    return 0;
}

static bool8 PageSwapAnimState_1(struct Task *task)
{
    u16 *const arr[] =
    {
        &namingScreenDataPtr->bg2vOffset,
        &namingScreenDataPtr->bg1vOffset
    };

    task->tFrameCount += 4;
    *arr[namingScreenDataPtr->bgToReveal] = Sin(task->tFrameCount, 40);
    *arr[namingScreenDataPtr->bgToHide] = Sin((task->tFrameCount + 128) & 0xFF, 40);
    if (task->tFrameCount >= 64)
    {
        u8 temp = namingScreenDataPtr->bg1Priority;
        namingScreenDataPtr->bg1Priority = namingScreenDataPtr->bg2Priority;
        namingScreenDataPtr->bg2Priority = temp;
        task->tState++;
    }
    return 0;
}

static bool8 PageSwapAnimState_2(struct Task *task)
{
    u16 *const arr[] = {&namingScreenDataPtr->bg2vOffset, &namingScreenDataPtr->bg1vOffset};

    task->tFrameCount += 4;
    *arr[namingScreenDataPtr->bgToReveal] = Sin(task->tFrameCount, 40);
    *arr[namingScreenDataPtr->bgToHide] = Sin((task->tFrameCount + 128) & 0xFF, 40);
    if (task->tFrameCount >= 128)
    {
        u8 temp = namingScreenDataPtr->bgToReveal;

        namingScreenDataPtr->bgToReveal = namingScreenDataPtr->bgToHide;
        namingScreenDataPtr->bgToHide = temp;
        task->tState++;
    }
    return 0;
}

static bool8 PageSwapAnimState_Done(struct Task *task)
{
    DestroyTask(FindTaskIdByFunc(Task_HandlePageSwapAnim));
    return 0;
}

#undef tState
#undef tFrameCount

//--------------------------------------------------
// Cursor
//--------------------------------------------------

static void CursorInit(void)
{
    namingScreenDataPtr->cursorSpriteId = CreateSprite(&gSpriteTemplate_83CE640, 0, 0, 0);
    gSprites[namingScreenDataPtr->cursorSpriteId].oam.priority = 1;
    gSprites[namingScreenDataPtr->cursorSpriteId].oam.objMode = 1;
    gSprites[namingScreenDataPtr->cursorSpriteId].data[6] = 1;
    gSprites[namingScreenDataPtr->cursorSpriteId].data[6] = 2;
    SetCursorPos(0, 0);
}

static const u8 sKeyboardSymbolPositions[][COLUMN_COUNT] = {
    0, 16, 32, 48, 64, 80, 96, 112, 128, 144,
    0, 16, 32, 48, 64, 80, 96, 112, 128, 144,
    0, 16, 32, 48, 64, 80, 96, 112, 128, 144
};

static u8 CursorColToKeyboardCol(s16 x)
{
    return sKeyboardSymbolPositions[namingScreenDataPtr->currentPage][x];
}

static void SetCursorPos(s16 x, s16 y)
{
    struct Sprite *cursorSprite = &gSprites[namingScreenDataPtr->cursorSpriteId];

    cursorSprite->x = CursorColToKeyboardCol(x) + 27;
    cursorSprite->y = y * 16 + 80;
    cursorSprite->data[2] = cursorSprite->data[0];
    cursorSprite->data[3] = cursorSprite->data[1];
    cursorSprite->data[0] = x;
    cursorSprite->data[1] = y;
}

static void GetCursorPos(s16 *x, s16 *y)
{
    struct Sprite *cursorSprite = &gSprites[namingScreenDataPtr->cursorSpriteId];

    *x = cursorSprite->data[0];
    *y = cursorSprite->data[1];
}

static void MoveCursorToOKButton(void)
{
    SetCursorPos(COLUMN_COUNT, 2);
}

static void SetCursorInvisibility(bool8 invisible)
{
    gSprites[namingScreenDataPtr->cursorSpriteId].data[4] &= -256;
    gSprites[namingScreenDataPtr->cursorSpriteId].data[4] |= invisible;
    StartSpriteAnim(&gSprites[namingScreenDataPtr->cursorSpriteId], 0);
}

static void SetCursorFlashing(bool8 flashing)
{
    gSprites[namingScreenDataPtr->cursorSpriteId].data[4] &= 0xFF;
    gSprites[namingScreenDataPtr->cursorSpriteId].data[4] |= flashing << 8;
}

static void SquishCursor(void)
{
    StartSpriteAnim(&gSprites[namingScreenDataPtr->cursorSpriteId], 1);
}

static bool8 IsCursorAnimFinished(void)
{
    return gSprites[namingScreenDataPtr->cursorSpriteId].animEnded;
}

static u8 GetKeyRoleAtCursorPos(void)
{
    const u8 keyRoles[] = {KEY_ROLE_PAGE, KEY_ROLE_BACKSPACE, KEY_ROLE_OK};
    s16 cursorX;
    s16 cursorY;

    GetCursorPos(&cursorX, &cursorY);
    if (cursorX < COLUMN_COUNT)
        return KEY_ROLE_CHAR;
    else
        return keyRoles[cursorY];
}

void sub_80B6998(struct Sprite *sprite)
{
    if (sprite->animEnded)
        StartSpriteAnim(sprite, 0);
    sprite->invisible = (sprite->data[4] & 0xFF);
    if (sprite->data[0] == COLUMN_COUNT)
        sprite->invisible = TRUE;
    if (sprite->invisible || (sprite->data[4] & 0xFF00) == 0
     || sprite->data[0] != sprite->data[2] || sprite->data[1] != sprite->data[3])
    {
        sprite->data[5] = 0;
        sprite->data[6] = 1;
        sprite->data[7] = 2;
    }
    sprite->data[7]--;
    if (sprite->data[7] == 0)
    {
        sprite->data[5] += sprite->data[6];
        if (sprite->data[5] == 16 || sprite->data[5] == 0)
            sprite->data[6] = -sprite->data[6];
        sprite->data[7] = 2;
    }
    if ((sprite->data[4] & 0xFF00) != 0)
    {
        s8 gb = sprite->data[5];
        s8 r = sprite->data[5] >> 1;
        u16 index = IndexOfSpritePaletteTag(5) * 16 + 0x0101;

        MultiplyInvertedPaletteRGBComponents(index, r, gb, gb);
    }
}

static void CreatePageSwapButtonSprites(void)
{
    u8 spriteId1;
    u8 spriteId2;
    u8 spriteId3;

    spriteId1 = CreateSprite(&gSpriteTemplate_83CE5C8, 0xCC, 0x50, 0);
    namingScreenDataPtr->pageIndicatorSpriteId = spriteId1;
    SetSubspriteTables(&gSprites[spriteId1], gSubspriteTables_83CE558);

    spriteId2 = CreateSprite(&gSpriteTemplate_83CE5F8, 0xCC, 0x4C, 1);
    gSprites[spriteId1].data[6] = spriteId2;
    SetSubspriteTables(&gSprites[spriteId2], gSubspriteTables_83CE560);

    spriteId3 = CreateSprite(&gSpriteTemplate_83CE5E0, 0xCC, 0x4B, 2);
    gSprites[spriteId3].oam.priority = 1;
    gSprites[spriteId1].data[7] = spriteId3;
}

static void StartPageSwapButtonAnim(void)
{
    struct Sprite *sprite = &gSprites[namingScreenDataPtr->pageIndicatorSpriteId];

    sprite->data[0] = 2;
    sprite->data[1] = namingScreenDataPtr->currentPage;
}

static u8 PageSwapSprite_Init(struct Sprite *);
static u8 PageSwapSprite_Idle(struct Sprite *);
static u8 PageSwapSprite_SlideOff(struct Sprite *);
static u8 PageSwapSprite_SlideOn(struct Sprite *);

static u8 (*const sPageSwapSpriteFuncs[])(struct Sprite *) =
{
    PageSwapSprite_Init,
    PageSwapSprite_Idle,
    PageSwapSprite_SlideOff,
    PageSwapSprite_SlideOn,
};

void SpriteCB_PageSwap(struct Sprite *sprite)
{
    while (sPageSwapSpriteFuncs[sprite->data[0]](sprite) != 0);
}

static u8 PageSwapSprite_Init(struct Sprite *sprite)
{
    struct Sprite *sprite1 = &gSprites[sprite->data[6]];
    struct Sprite *sprite2 = &gSprites[sprite->data[7]];

    SetPageSwapButtonGfx(namingScreenDataPtr->currentPage, sprite1, sprite2);
    sprite->data[0]++;
    return 0;
}

static u8 PageSwapSprite_Idle(struct Sprite *sprite)
{
    return 0;
}

static u8 PageSwapSprite_SlideOff(struct Sprite *sprite)
{
    struct Sprite *r4 = &gSprites[sprite->data[6]];
    struct Sprite *r5 = &gSprites[sprite->data[7]];

    r4->y2++;
    if (r4->y2 > 7)
    {
        sprite->data[0]++;
        r4->y2 = -4;
        r4->invisible = TRUE;
        SetPageSwapButtonGfx(((u8)sprite->data[1] + 1) % 3, r4, r5);
    }
    return 0;
}

static u8 PageSwapSprite_SlideOn(struct Sprite *sprite)
{
    struct Sprite *r2 = &gSprites[sprite->data[6]];

    r2->invisible = FALSE;
    r2->y2++;
    if (r2->y2 >= 0)
    {
        r2->y2 = 0;
        sprite->data[0] = 1;
    }
    return 0;
}

static void SetPageSwapButtonGfx(u8 a, struct Sprite *b, struct Sprite *c)
{
    const u16 sPageSwapPalTags[] = {1, 3, 2};
    const u16 sPageSwapGfxTags[] = {4, 6, 5};

    c->oam.paletteNum = IndexOfSpritePaletteTag(sPageSwapPalTags[a]);
    b->sheetTileStart = GetSpriteTileStartByTag(sPageSwapGfxTags[a]);
    b->subspriteTableNum = a;
}

static void CreateBackOkSprites(void)
{
    u8 spriteId;

    spriteId = CreateSprite(&gSpriteTemplate_83CE610, 0xCC, 0x6C, 0);
    SetSubspriteTables(&gSprites[spriteId], gSubspriteTables_83CE578);

    spriteId = CreateSprite(&gSpriteTemplate_83CE628, 0xCC, 0x84, 0);
    SetSubspriteTables(&gSprites[spriteId], gSubspriteTables_83CE578);
}

static void CreateTextEntrySprites(void)
{
    u8 spriteId;
    s16 left;
    u8 i;

    left = (namingScreenDataPtr->nameLeftOffset - 1) * 8 + 4;
    spriteId = CreateSprite(&gSpriteTemplate_83CE658, left, 0x28, 0);
    gSprites[spriteId].oam.priority = 3;
    left = namingScreenDataPtr->nameLeftOffset * 8 + 4;
    for (i = 0; i < namingScreenDataPtr->template->maxChars * 2; i += 2, left += 8)
    {
        spriteId = CreateSprite(&gSpriteTemplate_83CE670, left, 0x2C, 0);
        gSprites[spriteId].oam.priority = 3;
        gSprites[spriteId].data[0] = i;
    }
}

void sub_80B6D9C(struct Sprite *sprite)
{
    const s16 arr[] = {0, -4, -2, -1};

    if (sprite->data[0] == 0 || --sprite->data[0] == 0)
    {
        sprite->data[0] = 8;
        sprite->data[1] = (sprite->data[1] + 1) & 3;
    }
    sprite->x2 = arr[sprite->data[1]];
}

void sub_80B6DE8(struct Sprite *sprite)
{
    const s16 arr[] = {2, 3, 2, 1};
    u8 var;

    var = GetTextEntryPosition();
    if (var != (u8)sprite->data[0])
    {
        sprite->y2 = 0;
        sprite->data[1] = 0;
        sprite->data[2] = 0;
    }
    else
    {
        sprite->y2 = arr[sprite->data[1]];
        sprite->data[2]++;
        if (sprite->data[2] > 8)
        {
            sprite->data[1] = (sprite->data[1] + 1) & 3;
            sprite->data[2] = 0;
        }
    }
}

//

static void NamingScreen_NoIcon(void);
static void NamingScreen_CreatePlayerIcon(void);
static void NamingScreen_CreatePCIcon(void);
static void NamingScreen_CreateMonIcon(void);

static void (*const sIconFunctions[])(void) =
{
    NamingScreen_NoIcon,
    NamingScreen_CreatePlayerIcon,
    NamingScreen_CreatePCIcon,
    NamingScreen_CreateMonIcon,
};

static void CreateInputTargetIcon(void)
{
    sIconFunctions[namingScreenDataPtr->template->iconFunction]();
}

static void NamingScreen_NoIcon(void)
{
    // No-op
}

static void NamingScreen_CreatePlayerIcon(void)
{
    u8 rivalGfxId;
    u8 spriteId;

    rivalGfxId = GetRivalAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_NORMAL, namingScreenDataPtr->speciesOrPlayerGender);
    spriteId = AddPseudoObjectEvent(rivalGfxId, SpriteCallbackDummy, 0x38, 0x18, 0);
    gSprites[spriteId].oam.priority = 3;
    StartSpriteAnim(&gSprites[spriteId], 4);
}

static void NamingScreen_CreatePCIcon(void)
{
    u8 spriteId;

    spriteId = CreateSprite(&gSpriteTemplate_83CE688, 0x34, 0x18, 0);
    SetSubspriteTables(&gSprites[spriteId], gSubspriteTables_83CE580);
    gSprites[spriteId].oam.priority = 3;
}

static void NamingScreen_CreateMonIcon(void)
{
    u8 spriteId;

    sub_809D51C();
    spriteId = CreateMonIcon(namingScreenDataPtr->speciesOrPlayerGender, SpriteCallbackDummy, 0x34, 0x18, 0, namingScreenDataPtr->monPersonality);
    gSprites[spriteId].oam.priority = 3;
}

static u8 GetTextEntryPosition(void)
{
    u8 i;

    for (i = 0; i < namingScreenDataPtr->template->maxChars; i++)
    {
        if (namingScreenDataPtr->textBuffer[i * 2] == EOS)
            return i * 2;
    }
    return namingScreenDataPtr->template->maxChars * 2;
}

static u8 GetPreviousTextCaretPosition(void)
{
    s8 i;

    for (i = namingScreenDataPtr->template->maxChars - 1; i > 0; i--)
    {
        if (namingScreenDataPtr->textBuffer[i * 2] != EOS)
            return i * 2;
    }
    return 0;
}

static void DeleteTextCharacter(void)
{
    u8 index;
    u8 keyRole;
    u8 ch;
    u16 korean;

    // 마지막으로 입력된 인덱스, 문자
    index = GetPreviousTextCaretPosition();
    korean = (namingScreenDataPtr->textBuffer[index] << 8) | namingScreenDataPtr->textBuffer[index + 1];

    switch (namingScreenDataPtr->koreanState)
    {
    // 0 - STATE_NONE : 초기상태
    // 1 - STATE_JAUM : 자음입력상태
    // 한 글자 제거
    case STATE_NONE:
    case STATE_JAUM:
        namingScreenDataPtr->koreanState = STATE_NONE;
        namingScreenDataPtr->textBuffer[index] = EOS;
        namingScreenDataPtr->textBuffer[index + 1] = EOS;
        break;

    // 2, 3 - STATE_MOUM_MERGEABLE, STATE_MOUM : 자음+모음 입력상태
    // 모음 제거 및 분리
    case STATE_MOUM:
    case STATE_MOUM_MERGEABLE:
        ch = GetJungByHangul(korean);
        if (ch == 0x1d || ch == 0x1e || ch == 0x1f 
            || ch == 0x22 || ch == 0x23 || ch == 0x24 
            || ch == 0x27)
        {
            namingScreenDataPtr->koreanState = STATE_MOUM_MERGEABLE;

            korean = AssembleHangul(
                GetCho(GetChoByHangul(korean)),
                SplitJung(GetJung(GetJungByHangul(korean)), 0),
                0);

            namingScreenDataPtr->textBuffer[index] = (korean & 0xFF00) >> 8;
            namingScreenDataPtr->textBuffer[index + 1] = korean & 0x00FF;
        }
        else
        {
            namingScreenDataPtr->koreanState = STATE_JAUM;
            namingScreenDataPtr->textBuffer[index] = 0x41;
            namingScreenDataPtr->textBuffer[index + 1] = GetChoByHangul(korean);
        }
        break;

    // 4, 5 - STATE_JAUM_2_MERGEABLE, STATE_JAUM_2 : 자음+모음+자음 입력상태
    // 자음 제거
    case STATE_JAUM_2_MERGEABLE:
    case STATE_JAUM_2:
        ch = GetJungByHangul(korean);
        if (ch == 0x1d || ch == 0x1e || ch == 0x1f 
            || ch == 0x22 || ch == 0x23 || ch == 0x24 
            || ch == 0x27)
            namingScreenDataPtr->koreanState = STATE_MOUM;
        else
            namingScreenDataPtr->koreanState = STATE_MOUM_MERGEABLE;

        korean = AssembleHangul(
            GetCho(GetChoByHangul(korean)),
            GetJung(GetJungByHangul(korean)),
            0);

        namingScreenDataPtr->textBuffer[index] = (korean & 0xFF00) >> 8;
        namingScreenDataPtr->textBuffer[index + 1] = korean & 0x00FF;
        break;

    // 6 - STATE_MERGED_JAUM : 이중자음 입력상태
    // 이중 자음 분리
    case STATE_MERGED_JAUM:
        namingScreenDataPtr->koreanState = STATE_JAUM_2_MERGEABLE;

        korean = AssembleHangul(
            GetCho(GetChoByHangul(korean)),
            GetJung(GetJungByHangul(korean)),
            SplitJong(GetJongIndexByHangul(korean), 0));

        namingScreenDataPtr->textBuffer[index] = (korean & 0xFF00) >> 8;
        namingScreenDataPtr->textBuffer[index + 1] = korean & 0x00FF;
        break;
    }

    DrawTextEntry();

    keyRole = GetKeyRoleAtCursorPos();
    if (keyRole == 0 || keyRole == 2)
        TryStartButtonFlash(1, 0, 1);

    PlaySE(SE_BALL);
}

static bool8 AddTextCharacter(void)
{
    s16 x;
    s16 y;

    GetCursorPos(&x, &y);
    BufferCharacter(GetCharAtKeyboardPos(x, y));
    DrawTextEntry();
    PlaySE(SE_SELECT);

    return FALSE;
}

static void BufferCharacter(u8 ch)
{
    u8 index = GetTextEntryPosition();
    u8 state = namingScreenDataPtr->koreanState;
    u16 koreanChar, prevChar;

    // 이전에 입력한 글자 불러오기
    if (index >= 2)
    {
        prevChar = namingScreenDataPtr->textBuffer[index - 2] << 8 |
                   namingScreenDataPtr->textBuffer[index - 1];
    }

    if (ch >= 0xa1 || ch == 0x00)
    {
        if (index == namingScreenDataPtr->template->maxChars * 2)
            return;

        namingScreenDataPtr->textBuffer[index++] = 0;
        namingScreenDataPtr->textBuffer[index] = ch;
        state = STATE_NONE;
    }
    else
    {
        switch (state)
        {
        // 초기 상태(모든 문자 입력가능)
        case STATE_NONE:
        {
            if (index == namingScreenDataPtr->template->maxChars * 2)
                return;

            namingScreenDataPtr->textBuffer[index++] = 0x41;
            namingScreenDataPtr->textBuffer[index] = ch;

            if (IsJaum(ch))
                state = STATE_JAUM;
            break;
        }
        // 자음 입력 상태
        case STATE_JAUM:
        {
            if (IsMoum(ch))
            {
                koreanChar = AssembleHangul(
                    GetCho(prevChar & 0xff),
                    GetJung(ch),
                    0);

                if (koreanChar == 0xffff)
                {
                    if (index == namingScreenDataPtr->template->maxChars * 2)
                        return;

                    state = STATE_NONE;
                    namingScreenDataPtr->textBuffer[index++] = 0x41;
                    namingScreenDataPtr->textBuffer[index] = ch;
                }
                else
                {
                    index -= 2;
                    namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                    namingScreenDataPtr->textBuffer[index] = koreanChar & 0xff;

                    if (ch == 0x1c || ch == 0x21 || ch == 0x26)
                        state = STATE_MOUM_MERGEABLE;
                    else
                        state = STATE_MOUM;
                }
            }
            else
            {
                if (index == namingScreenDataPtr->template->maxChars * 2)
                    return;

                namingScreenDataPtr->textBuffer[index++] = 0x41;
                namingScreenDataPtr->textBuffer[index] = ch;
            }
            break;
        }
        // 모음 입력 상태(모음 조합가능)
        case STATE_MOUM_MERGEABLE:
        {
            if (IsMoum(ch))
            {
                if (!JoinMoum(GetJungByHangul(prevChar), ch))
                {
                    state = STATE_NONE;

                    if (index == namingScreenDataPtr->template->maxChars * 2)
                        return;

                    namingScreenDataPtr->textBuffer[index++] = 0x41;
                    namingScreenDataPtr->textBuffer[index] = ch;
                }
                else
                {
                    koreanChar = AssembleHangul(
                        GetCho(GetChoByHangul(prevChar)), 
                        GetJung(JoinMoum(GetJungByHangul(prevChar), ch)), 
                        0);

                    if (koreanChar == 0xffff)
                    {
                        if (index == namingScreenDataPtr->template->maxChars * 2)
                            return;

                        state = STATE_NONE;
                        namingScreenDataPtr->textBuffer[index++] = 0x41;
                        namingScreenDataPtr->textBuffer[index] = ch;
                    }
                    else
                    {
                        index -= 2;
                        namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                        namingScreenDataPtr->textBuffer[index] = koreanChar & 0xff;
                        state = STATE_MOUM;
                    }
                }
            }
            else
            {
                koreanChar = AssembleHangul(
                    GetCho(GetChoByHangul(prevChar)), 
                    GetJung(GetJungByHangul(prevChar)), 
                    GetJong(ch));

                if (koreanChar == 0xffff || GetJong(ch) == 0xff)
                {
                    if (index == namingScreenDataPtr->template->maxChars * 2)
                        return;

                    state = STATE_JAUM;
                    namingScreenDataPtr->textBuffer[index++] = 0x41;
                    namingScreenDataPtr->textBuffer[index] = ch;
                }
                else
                {
                    index -= 2;
                    namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                    namingScreenDataPtr->textBuffer[index] = koreanChar & 0xff;
                    state = STATE_JAUM_2;
                }
            }
            break;
        }
        // 모음 입력 상태
        case STATE_MOUM:
        {
            if (IsMoum(ch))
            {
                if (index == namingScreenDataPtr->template->maxChars * 2)
                    return;

                state = STATE_NONE;
                namingScreenDataPtr->textBuffer[index++] = 0x41;
                namingScreenDataPtr->textBuffer[index] = ch;
            }
            else
            {
                koreanChar = AssembleHangul(
                    GetCho(GetChoByHangul(prevChar)), 
                    GetJung(GetJungByHangul(prevChar)), 
                    GetJong(ch));

                if (koreanChar == 0xffff || GetJong(ch) == 0x00 || GetJong(ch) == 0xff)
                {
                    if (index == namingScreenDataPtr->template->maxChars * 2)
                        return;

                    state = STATE_JAUM;
                    namingScreenDataPtr->textBuffer[index++] = 0x41;
                    namingScreenDataPtr->textBuffer[index] = ch;
                }
                else
                {
                    index -= 2;
                    namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                    namingScreenDataPtr->textBuffer[index] = koreanChar & 0xff;

                    if (ch == 0x01 || ch == 0x03 || ch == 0x06 || ch == 0x08)
                        state = STATE_JAUM_2_MERGEABLE;
                    else
                        state = STATE_JAUM_2;
                }
            }
            break;
        }
        // 4 - STATE_JAUM_2_MERGEABLE : 자음 입력 상태(자음 조합가능)
        case STATE_JAUM_2_MERGEABLE:
        {
            if (IsJaum(ch))
            {
                if (!JoinJaum(GetJongByHangul(prevChar), ch))
                {
                    if (index == namingScreenDataPtr->template->maxChars * 2)
                        return;

                    state = STATE_JAUM;
                    namingScreenDataPtr->textBuffer[index++] = 0x41;
                    namingScreenDataPtr->textBuffer[index] = ch;
                }
                else
                {
                    koreanChar = AssembleHangul(
                        GetCho(GetChoByHangul(prevChar)), 
                        GetJung(GetJungByHangul(prevChar)), 
                        JoinJaum(GetJongByHangul(prevChar), ch));

                    if (koreanChar == 0xffff)
                    {
                        state = STATE_JAUM;

                        if (index == namingScreenDataPtr->template->maxChars * 2)
                            return;

                        namingScreenDataPtr->textBuffer[index++] = 0x41;
                        namingScreenDataPtr->textBuffer[index] = ch;
                    }
                    else
                    {
                        index -= 2;
                        namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                        namingScreenDataPtr->textBuffer[index] = koreanChar & 0xff;
                        state = STATE_MERGED_JAUM;
                    }
                }
            }
            else
            {
                if (index == namingScreenDataPtr->template->maxChars * 2)
                    return;

                koreanChar = AssembleHangul(
                    GetCho(GetChoByHangul(prevChar)), 
                    GetJung(GetJungByHangul(prevChar)), 
                    0);

                index -= 2;
                namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                namingScreenDataPtr->textBuffer[index++] = koreanChar & 0xff;

                koreanChar = AssembleHangul(
                    GetCho(GetJongByHangul(prevChar)), 
                    GetJung(ch), 
                    0);

                if (koreanChar == 0xffff)
                {
                    state = STATE_NONE;
                    namingScreenDataPtr->textBuffer[index++] = 0x41;
                    namingScreenDataPtr->textBuffer[index] = ch;
                }
                else
                {
                    namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                    namingScreenDataPtr->textBuffer[index] = koreanChar & 0xff;

                    if (ch == 0x1c || ch == 0x21 || ch == 0x26)
                        state = STATE_MOUM_MERGEABLE;
                    else
                        state = STATE_MOUM;
                }
            }
            break;
        }        
        // 5 - STATE_JAUM_2 : 자음 입력 상태(모음 조합 가능)
        case STATE_JAUM_2:
        {
            if (IsJaum(ch))
            {
                if (index == namingScreenDataPtr->template->maxChars * 2)
                    return;

                state = STATE_JAUM;
                namingScreenDataPtr->textBuffer[index++] = 0x41;
                namingScreenDataPtr->textBuffer[index] = ch;
            }
            else
            {
                if (index == namingScreenDataPtr->template->maxChars * 2)
                    return;

                koreanChar = AssembleHangul(
                    GetCho(GetChoByHangul(prevChar)), 
                    GetJung(GetJungByHangul(prevChar)), 
                    0);

                index -= 2;
                namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                namingScreenDataPtr->textBuffer[index++] = koreanChar & 0xff;

                koreanChar = AssembleHangul(
                    GetCho(GetJongByHangul(prevChar)), 
                    GetJung(ch), 
                    0);

                if (koreanChar == 0xffff)
                {
                    state = STATE_NONE;
                    namingScreenDataPtr->textBuffer[index++] = 0x41;
                    namingScreenDataPtr->textBuffer[index] = ch;
                }
                else
                {
                    namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                    namingScreenDataPtr->textBuffer[index] = koreanChar & 0xff;

                    if (ch == 0x1c || ch == 0x21 || ch == 0x26)
                        state = STATE_MOUM_MERGEABLE;
                    else
                        state = STATE_MOUM;
                }
            }
            break;
        }
        // 6 - STATE_MERGED_JAUM : 이중 자음 입력상태
        case STATE_MERGED_JAUM:
        {
            if (IsJaum(ch))
            {
                if (index == namingScreenDataPtr->template->maxChars * 2)
                    return;

                state = STATE_JAUM;
                namingScreenDataPtr->textBuffer[index++] = 0x41;
                namingScreenDataPtr->textBuffer[index] = ch;
            }
            else
            {
                if (index == namingScreenDataPtr->template->maxChars * 2)
                    return;

                // 이전 글자에서 종성만 제거
                koreanChar = AssembleHangul(
                    GetCho(GetChoByHangul(prevChar)), 
                    GetJung(GetJungByHangul(prevChar)), 
                    SplitJong(GetJongIndexByHangul(prevChar), 0));

                index -= 2;
                namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                namingScreenDataPtr->textBuffer[index++] = koreanChar & 0xff;
                
                // 새 글자
                koreanChar = AssembleHangul(
                    GetCho(ConvertJongToCho(SplitJong(GetJongIndexByHangul(prevChar), 1))), 
                    GetJung(ch), 
                    0);

                if (koreanChar == 0xffff)
                {
                    state = STATE_NONE;
                    namingScreenDataPtr->textBuffer[index++] = 0x41;
                    namingScreenDataPtr->textBuffer[index] = ch;
                }
                else
                {
                    namingScreenDataPtr->textBuffer[index++] = (koreanChar & 0xff00) >> 8;
                    namingScreenDataPtr->textBuffer[index] = koreanChar & 0xff;

                    // 모음 조합가능 여부에 따른 분기
                    if (ch == 0x1c || ch == 0x21 || ch == 0x26)
                        state = STATE_MOUM_MERGEABLE;
                    else
                        state = STATE_MOUM;
                }
            }
            break;
        }
        }
    }

    namingScreenDataPtr->koreanState = state;
}

static void SaveInputText(void)
{
    u8 i;

    NamingScreen_StringCopy(namingScreenDataPtr->destBuffer, namingScreenDataPtr->textBuffer);

    i = StringLength(namingScreenDataPtr->destBuffer);
    if (i == 0)
    {
        NamingScreen_StringCopy(namingScreenDataPtr->destBuffer, namingScreenDataPtr->backupBuffer);
    }
}

static void DisplaySentToPCMessage(void)
{
    StringCopy(gStringVar1, namingScreenDataPtr->destBuffer);
    StringExpandPlaceholders(gStringVar4, gOtherText_SentToPC);
    BasicInitMenuWindow(&gWindowTemplate_81E6E88);
    Menu_DisplayDialogueFrame();
    MenuPrintMessageDefaultCoords(gStringVar4);
}

static void sub_80B753C(void)
{
    LoadSpriteSheets(gUnknown_083CE6A0);
    LoadSpritePalettes(gUnknown_083CE708);
}

static void sub_80B7558(void)
{
    sub_80B7568();
    sub_80B75B0();
}

static void sub_80B7568(void)
{
    DmaCopy16Defvars(3, gNamingScreenMenu_Gfx, (void *)(VRAM + gMenuMessageBoxContentTileOffset * 32), 0x800);
    DmaCopy16Defvars(3, gNamingScreenMenu_Gfx, (void *)(VRAM + 0x8000 + gMenuMessageBoxContentTileOffset * 32), 0x800);
}

static void sub_80B75B0(void)
{
    LoadPalette(gNamingScreenPalettes, 0, 0x80);
}

static void sub_80B7650(u16 *);
static void sub_80B7660(u16 *);
static void sub_80B7670(u16 *);

static void (*const gUnknown_083CE2F0[][2])(u16 *) =
{
    {sub_80B7660, sub_80B7650},
    {sub_80B7650, sub_80B7670},
    {sub_80B7670, sub_80B7660},
};

static void sub_80B75C4(void)
{
    u16 *const arr[] =
    {
        (u16 *)(VRAM + 0xE000),
        (u16 *)(VRAM + 0xE800),
    };

    gUnknown_083CE2F0[namingScreenDataPtr->currentPage][0](arr[namingScreenDataPtr->bgToReveal]);
    gUnknown_083CE2F0[namingScreenDataPtr->currentPage][1](arr[namingScreenDataPtr->bgToHide]);
}

static void sub_80B7614(void)
{
    u16 *const arr[] =
    {
        (u16 *)(VRAM + 0xE000),
        (u16 *)(VRAM + 0xE800),
    };

    gUnknown_083CE2F0[namingScreenDataPtr->currentPage][1](arr[namingScreenDataPtr->bgToHide]);
}

static void sub_80B7650(u16 *vramBuffer)
{
    sub_80B7698(vramBuffer, gUnknown_083CE748);
}

static void sub_80B7660(u16 *vramBuffer)
{
    sub_80B7698(vramBuffer, gUnknown_083CEBF8);
}

static void sub_80B7670(u16 *vramBuffer)
{
    sub_80B7698(vramBuffer, gUnknown_083CF0A8);
}

static void sub_80B7680(void)
{
    sub_80B76E0(VRAM + 0xF000, gUnknown_08E86258);
}

static void sub_80B7698(u16 *vramBuffer, const u16 *src)
{
    s16 i;
    s16 j;

    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 30; j++, src++)
        {
            vramBuffer[i * 32 + j] = *src + gMenuMessageBoxContentTileOffset;
        }
    }
}

static void sub_80B76E0(u16 *vramBuffer, const u16 *src)
{
    s16 i;
    s16 j;

    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 30; j++, src++)
        {
            vramBuffer[i * 32 + j] = *src + gMenuMessageBoxContentTileOffset;
        }
        src += 2;
    }
}

static void sub_80B772C(void)
{
    nullsub_20(namingScreenDataPtr->currentPage, namingScreenDataPtr->bgToReveal);
}

static void sub_80B7740(void)
{
    nullsub_20((namingScreenDataPtr->currentPage + 1) % 3, namingScreenDataPtr->bgToHide);
}

static void nullsub_20(u8 a, u8 b)
{
}

static void sub_80B7838(void);
static void sub_80B7844(void);
static void sub_80B7850(void);

static void (*const gUnknown_083CE310[][2])(void) =
{
    sub_80B7844,
    sub_80B7838,
    sub_80B7838,
    sub_80B7850,
    sub_80B7850,
    sub_80B7844,
};

static const struct WindowTemplate *const gUnknown_083CE328[][2][2] =
{
    {
        {&gWindowTemplate_81E6EDC, &gWindowTemplate_81E6EF8},
        {&gWindowTemplate_81E6EA4, &gWindowTemplate_81E6EC0},
    },
    {
        {&gWindowTemplate_81E6EA4, &gWindowTemplate_81E6EC0},
        {&gWindowTemplate_81E6F14, &gWindowTemplate_81E6F30},
    },
    {
        {&gWindowTemplate_81E6F14, &gWindowTemplate_81E6F30},
        {&gWindowTemplate_81E6EDC, &gWindowTemplate_81E6EF8},
    },
};

static void nullsub_61(void);
static void sub_80B78F8(void);

static void (*const gUnknown_083CE358[])(void) =
{
    nullsub_61,
    nullsub_61,
    sub_80B78F8,
    sub_80B78F8,
};

static void nullsub_62(void);
static void sub_80B7924(void);

static void (*const gUnknown_083CE368[])(void) =
{
    nullsub_62,
    sub_80B7924,
};

static const u8 sKeyboardCharacters[][4][20];
static const u8 sKeyboardCharactersTexts[][4][56];

static u8 GetCharAtKeyboardPos(s16 a, s16 b)
{
    return sKeyboardCharacters[namingScreenDataPtr->currentPage][b][a];
}

static void sub_80B7794(void)
{
    BasicInitMenuWindow(gUnknown_083CE328[namingScreenDataPtr->currentPage][0][namingScreenDataPtr->bgToReveal]);
    gUnknown_083CE310[namingScreenDataPtr->currentPage][0]();
    BasicInitMenuWindow(gUnknown_083CE328[namingScreenDataPtr->currentPage][1][namingScreenDataPtr->bgToHide]);
    gUnknown_083CE310[namingScreenDataPtr->currentPage][1]();
    sub_80B772C();
    sub_80B7740();
}

static void sub_80B77F8(void)
{
    BasicInitMenuWindow(gUnknown_083CE328[namingScreenDataPtr->currentPage][1][namingScreenDataPtr->bgToHide]);
    gUnknown_083CE310[namingScreenDataPtr->currentPage][1]();
    sub_80B7740();
}

static void sub_80B7838(void)
{
    PrintKeyboardCharacters(1);
}

static void sub_80B7844(void)
{
    PrintKeyboardCharacters(0);
}

static void sub_80B7850(void)
{
    PrintKeyboardCharacters(2);
}

static void PrintKeyboardCharacters(u8 page)  //print letters on page
{
    s16 xPos;
    s16 yPos;

    yPos = 9;
    for (xPos = 0; xPos < 4; xPos++)
    {
        Menu_PrintText(sKeyboardCharactersTexts[page][xPos], 3, yPos);
        yPos += 2;
    }
}

static void sub_80B78A8(void)
{
    BasicInitMenuWindow(&gWindowTemplate_81E6F4C);
    gUnknown_083CE358[namingScreenDataPtr->templateNum]();
    gUnknown_083CE368[namingScreenDataPtr->template->addGenderIcon]();
    Menu_PrintText(namingScreenDataPtr->template->title, 9, 2);
}

static void nullsub_61(void)
{
}

static void sub_80B78F8(void)
{
    StringCopy(gStringVar1, gSpeciesNames[(s16)namingScreenDataPtr->speciesOrPlayerGender]);
}

static void nullsub_62(void)
{
}

static void sub_80B7924(void)
{
    u8 genderSymbol[2] = _("♂");

    if ((s16)namingScreenDataPtr->monGender != MON_GENDERLESS)
    {
        if ((s16)namingScreenDataPtr->monGender == MON_FEMALE)
            genderSymbol[0] = 0xB6;  //female symbol
        Menu_PrintText(genderSymbol, 0x14, 4);
    }
}

static void DrawTextEntry(void)
{
    u8 i;
    u8 first, second;
    u8 *string = gStringVar1;

    string[0] = 0xFC;
    string[1] = 0x14;
    string[2] = 8;
    string[3] = 0xFC;
    string[4] = 0x11;
    string[5] = 1;
    string += 6;

    for (i = 0; i < sizeof(namingScreenDataPtr->textBuffer); i += 2)
    {
        first = namingScreenDataPtr->textBuffer[i];
        second = namingScreenDataPtr->textBuffer[i + 1];

        // 한글을 제외한 문자 처리
        if (first == 0)
        {
            *string = second;
            string++;
            continue;
        }

        *string++ = first;
        *string++ = second;
    }

    BasicInitMenuWindow(&gWindowTemplate_81E6F4C);

    // 폰트폭이 8px가 넘는 경우에 이름 한 글자 지우기 시, 잔상이 남을 수 있기 때문에 이름란의 타일을 비웁니다.
    Menu_BlankWindowRect(
        namingScreenDataPtr->nameLeftOffset,
        4,
        namingScreenDataPtr->nameLeftOffset + namingScreenDataPtr->template->maxChars,
        (4 + 2) - 1
    );

    Menu_PrintText(gStringVar1, namingScreenDataPtr->nameLeftOffset, 4);
}

//--------------------------------------------------
// Forward-declared variables
//--------------------------------------------------

static const struct NamingScreenTemplate playerNamingScreenTemplate =
{
    .copyExistingString = 0,
    .maxChars = 3,
    .iconFunction = 1,
    .addGenderIcon = 0,
    .initialPage = 0,
    .title = OtherText_YourName,
};

static const struct NamingScreenTemplate pcBoxNamingTemplate =
{
    .copyExistingString = 0,
    .maxChars = 8,
    .iconFunction = 2,
    .addGenderIcon = 0,
    .initialPage = 0,
    .title = OtherText_BoxName,
};

static const struct NamingScreenTemplate monNamingScreenTemplate =
{
    .copyExistingString = 0,
    .maxChars = 5,
    .iconFunction = 3,
    .addGenderIcon = 1,
    .initialPage = 0,
    .title = OtherText_PokeName,
};

static const struct NamingScreenTemplate *const sNamingScreenTemplates[] =
{
    &playerNamingScreenTemplate,
    &pcBoxNamingTemplate,
    &monNamingScreenTemplate,
    &monNamingScreenTemplate,
};

static const u8 sKeyboardCharacters[][4][20] =
{
    [PAGE_KOREAN] =
    {
        /* ㅃㅉㄸㄲㅆ!?-ㅒㅖ */
        { 0x09, 0x0E, 0x05, 0x02, 0x0B, 0xAB, 0xAC, 0xAE, 0x17, 0x1B },
        /* ㅂㅈㄷㄱㅅㅛㅕㅑㅐㅔ */
        { 0x08, 0x0D, 0x04, 0x01, 0x0A, 0x20, 0x1A, 0x16, 0x15, 0x19 },
        /* ㅁㄴㅇㄹㅎㅗㅓㅏㅣ */
        { 0x07, 0x03, 0x0C, 0x06, 0x13, 0x1C, 0x18, 0x14, 0x28, 0xBA },
        /* ㅋㅌㅊㅍ ㅠㅜㅡ,. */
        { 0x10, 0x11, 0x0F, 0x12, 0x00, 0x25, 0x21, 0x26, 0xB8, 0xAD },
    },
    [PAGE_LOWER] =
    {
        _("1234567890"),
        _("abcdefghij"),
        _("klmnopqrst"),
        _("uvwxyz♂♀-·"),
    },
    [PAGE_UPPER] =
    {
        _("1234567890"),
        _("ABCDEFGHIJ"),
        _("KLMNOPQRST"),
        _("UVWXYZ‘'“”"),
    }
};

static const u8 sKeyboardCharactersTexts[][4][56] =
{
    [PAGE_KOREAN] =
    {
        _("ㅃ{CLEAR_TO 16}ㅉ{CLEAR_TO 32}ㄸ{CLEAR_TO 48}ㄲ{CLEAR_TO 64}ㅆ{CLEAR_TO 80}!{CLEAR_TO 96}?{CLEAR_TO 112}-{CLEAR_TO 128}ㅒ{CLEAR_TO 144}ㅖ"),
        _("ㅂ{CLEAR_TO 16}ㅈ{CLEAR_TO 32}ㄷ{CLEAR_TO 48}ㄱ{CLEAR_TO 64}ㅅ{CLEAR_TO 80}ㅛ{CLEAR_TO 96}ㅕ{CLEAR_TO 112}ㅑ{CLEAR_TO 128}ㅐ{CLEAR_TO 144}ㅔ"),
        _("ㅁ{CLEAR_TO 16}ㄴ{CLEAR_TO 32}ㅇ{CLEAR_TO 48}ㄹ{CLEAR_TO 64}ㅎ{CLEAR_TO 80}ㅗ{CLEAR_TO 96}ㅓ{CLEAR_TO 112}ㅏ{CLEAR_TO 128}ㅣ{CLEAR_TO 144}/"),
        _("ㅋ{CLEAR_TO 16}ㅌ{CLEAR_TO 32}ㅊ{CLEAR_TO 48}ㅍ{CLEAR_TO 64}{CLEAR_TO 80}ㅠ{CLEAR_TO 96}ㅜ{CLEAR_TO 112}ㅡ{CLEAR_TO 128},{CLEAR_TO 144}."),
    },
    [PAGE_UPPER] =
    {
        _("1{CLEAR_TO 16}2{CLEAR_TO 32}3{CLEAR_TO 48}4{CLEAR_TO 64}5{CLEAR_TO 80}6{CLEAR_TO 96}7{CLEAR_TO 112}8{CLEAR_TO 128}9{CLEAR_TO 144}0"),
        _("A{CLEAR_TO 16}B{CLEAR_TO 32}C{CLEAR_TO 48}D{CLEAR_TO 64}E{CLEAR_TO 80}F{CLEAR_TO 96}G{CLEAR_TO 112}H{CLEAR_TO 128}I{CLEAR_TO 144}J"),
        _("K{CLEAR_TO 16}L{CLEAR_TO 32}M{CLEAR_TO 48}N{CLEAR_TO 64}O{CLEAR_TO 80}P{CLEAR_TO 96}Q{CLEAR_TO 112}R{CLEAR_TO 128}S{CLEAR_TO 144}T"),
        _("U{CLEAR_TO 16}V{CLEAR_TO 32}W{CLEAR_TO 48}X{CLEAR_TO 64}Y{CLEAR_TO 80}Z{CLEAR_TO 96}‘{CLEAR_TO 112}'{CLEAR_TO 128}“{CLEAR_TO 144}”"),
    },
    [PAGE_LOWER] =
    {
        _("1{CLEAR_TO 16}2{CLEAR_TO 32}3{CLEAR_TO 48}4{CLEAR_TO 64}5{CLEAR_TO 80}6{CLEAR_TO 96}7{CLEAR_TO 112}8{CLEAR_TO 128}9{CLEAR_TO 144}0"),
        _("a{CLEAR_TO 16}b{CLEAR_TO 32}c{CLEAR_TO 48}d{CLEAR_TO 64}e{CLEAR_TO 80}f{CLEAR_TO 96}g{CLEAR_TO 112}h{CLEAR_TO 128}i{CLEAR_TO 144}j"),
        _("k{CLEAR_TO 16}l{CLEAR_TO 32}m{CLEAR_TO 48}n{CLEAR_TO 64}o{CLEAR_TO 80}p{CLEAR_TO 96}q{CLEAR_TO 112}r{CLEAR_TO 128}s{CLEAR_TO 144}t"),
        _("u{CLEAR_TO 16}v{CLEAR_TO 32}w{CLEAR_TO 48}x{CLEAR_TO 64}y{CLEAR_TO 80}z{CLEAR_TO 96}♂{CLEAR_TO 112}♀{CLEAR_TO 128}-{CLEAR_TO 144}·"),
    }
};

const struct OamData gOamData_83CE498 =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = 0,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

const struct OamData gOamData_83CE4A0 =
{
    .y = 0,
    .affineMode = 0,
    .objMode = 0,
    .mosaic = 0,
    .bpp = 0,
    .shape = 0,
    .x = 0,
    .matrixNum = 0,
    .size = 1,
    .tileNum = 0,
    .priority = 0,
    .paletteNum = 0,
    .affineParam = 0,
};

const struct OamData gOamData_83CE4A8 =
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


const struct Subsprite gSubspriteTable_83CE4B0[] = {
    { -20, -16, ST_OAM_H_RECTANGLE, 1,   0, 1 },
    {  12, -16, ST_OAM_SQUARE, 0,   4, 1 },
    { -20,  -8, ST_OAM_H_RECTANGLE, 1,   5, 1 },
    {  12,  -8, ST_OAM_SQUARE, 0,   9, 1 },
    { -20,   0, ST_OAM_H_RECTANGLE, 1,  10, 1 },
    {  12,   0, ST_OAM_SQUARE, 0,  14, 1 },
    { -20,   8, ST_OAM_H_RECTANGLE, 1,  15, 1 },
    {  12,   8, ST_OAM_SQUARE, 0,  19, 1 }
};

const struct Subsprite gSubspriteTable_83CE4F0[] = {
    { -12,  -4, ST_OAM_SQUARE, 0,   0, 1 },
    {   4,  -4, ST_OAM_SQUARE, 0,   1, 1 }
};

const struct Subsprite gSubspriteTable_83CE500[] = {
    { -12,  -4, ST_OAM_H_RECTANGLE, 0,   0, 1 },
    {   4,  -4, ST_OAM_SQUARE, 0,   2, 1 }
};

const struct Subsprite gSubspriteTable_83CE510[] = {
    { -20, -12, ST_OAM_H_RECTANGLE, 1,   0, 1 },
    {  12, -12, ST_OAM_SQUARE, 0,   4, 1 },
    { -20,  -4, ST_OAM_H_RECTANGLE, 1,   5, 1 },
    {  12,  -4, ST_OAM_SQUARE, 0,   9, 1 },
    { -20,   4, ST_OAM_H_RECTANGLE, 1,  10, 1 },
    {  12,   4, ST_OAM_SQUARE, 0,  14, 1 }
};

const struct Subsprite gSubspriteTable_83CE540[] = {
    {  -8, -12, ST_OAM_H_RECTANGLE, 0,   0, 3 },
    {  -8,  -4, ST_OAM_H_RECTANGLE, 0,   2, 3 },
    {  -8,   4, ST_OAM_H_RECTANGLE, 0,   4, 3 }
};

const struct SubspriteTable gSubspriteTables_83CE558[] = {
    { 8, gSubspriteTable_83CE4B0 }
};

const struct SubspriteTable gSubspriteTables_83CE560[] = {
    { 2, gSubspriteTable_83CE500 },
    { 2, gSubspriteTable_83CE500 },
    { 2, gSubspriteTable_83CE500 }
};

const struct SubspriteTable gSubspriteTables_83CE578[] = {
    { 6, gSubspriteTable_83CE510 }
};

const struct SubspriteTable gSubspriteTables_83CE580[] = {
    { 3, gSubspriteTable_83CE540 }
};

const struct SpriteFrameImage gSpriteImageTable_83CE588[] = {
    { gSpriteImage_83CE094, 0xc0 },
    { gSpriteImage_83CE154, 0xc0 }
};

const union AnimCmd gSpriteAnim_83CE598[] = {
    ANIMCMD_FRAME(0, 1),
    ANIMCMD_JUMP(0)
};

const union AnimCmd gSpriteAnim_83CE5A0[] = {
    ANIMCMD_FRAME(4, 8),
    ANIMCMD_FRAME(8, 8),
    ANIMCMD_END
};

const union AnimCmd gSpriteAnim_83CE5AC[] = {
    ANIMCMD_FRAME(0, 2),
    ANIMCMD_FRAME(1, 2),
    ANIMCMD_JUMP(0)
};

const union AnimCmd *const gSpriteAnimTable_83CE5B8[] = {
    gSpriteAnim_83CE598
};

const union AnimCmd *const gSpriteAnimTable_83CE5BC[] = {
    gSpriteAnim_83CE598,
    gSpriteAnim_83CE5A0
};

const union AnimCmd *const gSpriteAnimTable_83CE5C4[] = {
    gSpriteAnim_83CE5AC
};

const struct SpriteTemplate gSpriteTemplate_83CE5C8 = {
    2, 4, &gOamData_83CE498, gSpriteAnimTable_83CE5B8, NULL, gDummySpriteAffineAnimTable, SpriteCB_PageSwap
};

const struct SpriteTemplate gSpriteTemplate_83CE5E0 = {
    3, 1, &gOamData_83CE4A8, gSpriteAnimTable_83CE5B8, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

const struct SpriteTemplate gSpriteTemplate_83CE5F8 = {
    4, 4, &gOamData_83CE498, gSpriteAnimTable_83CE5B8, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

const struct SpriteTemplate gSpriteTemplate_83CE610 = {
    0, 6, &gOamData_83CE498, gSpriteAnimTable_83CE5B8, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

const struct SpriteTemplate gSpriteTemplate_83CE628 = {
    1, 6, &gOamData_83CE498, gSpriteAnimTable_83CE5B8, NULL, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

const struct SpriteTemplate gSpriteTemplate_83CE640 = {
    7, 5, &gOamData_83CE4A0, gSpriteAnimTable_83CE5BC, NULL, gDummySpriteAffineAnimTable, sub_80B6998
};

const struct SpriteTemplate gSpriteTemplate_83CE658 = {
    10, 3, &gOamData_83CE498, gSpriteAnimTable_83CE5B8, NULL, gDummySpriteAffineAnimTable, sub_80B6D9C
};

const struct SpriteTemplate gSpriteTemplate_83CE670 = {
    11, 3, &gOamData_83CE498, gSpriteAnimTable_83CE5B8, NULL, gDummySpriteAffineAnimTable, sub_80B6DE8
};

const struct SpriteTemplate gSpriteTemplate_83CE688 = {
    0xFFFF, 0, &gOamData_83CE498, gSpriteAnimTable_83CE5C4, gSpriteImageTable_83CE588, gDummySpriteAffineAnimTable, SpriteCallbackDummy
};

const struct SpriteSheet gUnknown_083CE6A0[] = {
    { gNamingScreenBackButtonTiles, 0x1E0, 0 },
    { gNamingScreenOKButtonTiles, 0x1E0, 1 },
    { gNamingScreenChangeKeyboardBoxTiles, 0x280, 2 },
    { gNamingScreenChangeKeyboardButtonTiles, 0x100, 3 },
    { gNamingScreenLowerTextTiles, 0x60, 4 },
    { gNamingScreenUpperTextTiles, 0x60, 5 },
    { gNamingScreenOthersTextTiles, 0x60, 6 },
    { gNamingScreenCursorTiles, 0x80, 7 },
    { gNamingScreenActiveCursorSmallTiles, 0x80, 8 },
    { gNamingScreenActiveCursorBigTiles, 0x80, 9 },
    { gNamingScreenRightPointingTriangleTiles, 0x20, 10 },
    { gNamingScreenUnderscoreTiles, 0x20, 11 },
    {}
};

const struct SpritePalette gUnknown_083CE708[] = {
    {gNamingScreenPalettes[0], 0},
    {gNamingScreenPalettes[1], 1},
    {gNamingScreenPalettes[2], 2},
    {gNamingScreenPalettes[3], 3},
    {gNamingScreenPalettes[4], 4},
    {gNamingScreenPalettes[5], 5},
    {gNamingScreenPalettes[4], 6},
    {}
};

const u16 gUnknown_083CE748[] = INCBIN_U16("graphics/naming_screen/lower_keyboard_map.bin");

const u16 gUnknown_083CEBF8[] = INCBIN_U16("graphics/naming_screen/upper_keyboard_map.bin");

const u16 gUnknown_083CF0A8[] = INCBIN_U16("graphics/naming_screen/others_keyboard_map.bin");

