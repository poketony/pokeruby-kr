S_CuttableTree:: @ 81B0DCC
	lockall
	goto_if_unset FLAG_BADGE01_GET, CannotUseCut
	checkpartymove MOVE_CUT
	compare VAR_RESULT, 6
	goto_if_eq CannotUseCut
	setfieldeffectargument 0, VAR_RESULT
	bufferpartymonnick 0, VAR_RESULT
	buffermovename 1, MOVE_CUT
	msgbox UseCutPromptText, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq Cut_ChoseNo
	msgbox UsedCutRockSmashText, MSGBOX_DEFAULT
	closemessage
	dofieldeffect FLDEFF_USE_CUT_ON_TREE
	waitstate
	goto DoTreeCutMovement
	end

DoCutFieldEffectScript:: @ 81B0E16
	lockall
	dofieldeffect FLDEFF_USE_CUT_ON_TREE
	waitstate
	goto DoTreeCutMovement
	end

DoTreeCutMovement: @ 81B0E21
	applymovement VAR_LAST_TALKED, TreeCutMovement @ tree cut animation
	waitmovement 0
	removeobject VAR_LAST_TALKED @ tree disappears
	releaseall
	end

TreeCutMovement: @ 81B0E30
	cut_tree
	step_end

CannotUseCut: @ 81B0E32
	msgbox CannotUseCutText, MSGBOX_SIGN
	releaseall
	end

Cut_ChoseNo: @ 81B0E3C
	closemessage
	releaseall
	end

UseCutPromptText: @ 81B0E3F
	.string "...이 나무는 왠지\n"
	.string "자를 수 있을 것 같다!\p"
	.string "풀베기로 자르겠습니까?$"

UsedCutRockSmashText: @ 81B0E82
	.string "{STR_VAR_1}{K_EUNNEUN}\n"
	.string "{STR_VAR_2}{K_EULREUL} 썼다!$"

CannotUseCutText: @ 81B0E8E
	.string "이 나무는 왠지\n"
	.string "자를 수 있을 것 같다!$"

S_BreakableRock:: @ 81B0EB7
	lockall
	goto_if_unset FLAG_BADGE03_GET, CannotUseRockSmash
	checkpartymove MOVE_ROCK_SMASH
	compare VAR_RESULT, 6
	goto_if_eq CannotUseRockSmash
	setfieldeffectargument 0, VAR_RESULT
	bufferpartymonnick 0, VAR_RESULT
	buffermovename 1, MOVE_ROCK_SMASH
	msgbox UseRockSmashPromptText, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq RockSmash_ChoseNo
	msgbox UsedCutRockSmashText, MSGBOX_DEFAULT
	closemessage
	dofieldeffect FLDEFF_USE_ROCK_SMASH
	waitstate
	goto DoRockSmashMovement
	end

S_UseRockSmash:: @ 81B0F01
	lockall
	dofieldeffect FLDEFF_USE_ROCK_SMASH
	waitstate
	goto DoRockSmashMovement
	end

DoRockSmashMovement: @ 81B0F0C
	applymovement VAR_LAST_TALKED, RockSmashMovement
	waitmovement 0
	removeobject VAR_LAST_TALKED
	specialvar VAR_RESULT, TryUpdateRusturfTunnelState
	compare VAR_RESULT, 1
	goto_if_eq DoRockSmashMovement_Done
	special ScrSpecial_RockSmashWildEncounter
	compare VAR_RESULT, 0
	goto_if_eq DoRockSmashMovement_Done
	waitstate
	releaseall
	end

DoRockSmashMovement_Done: @ 81B0F3A
	releaseall
	end

RockSmashMovement: @ 81B0F3C
	rock_smash_break
	step_end

CannotUseRockSmash: @ 81B0F3E
	msgbox CannotUseRockSmashText, MSGBOX_SIGN
	releaseall
	end

RockSmash_ChoseNo: @ 81B0F48
	closemessage
	releaseall
	end

UseRockSmashPromptText: @ 81B0F4B
	.string "포켓몬 기술로\n"
	.string "깨뜨릴 수 있을 것 같다!\l"
	.string "...바위깨기를 쓰겠습니까?$"

CannotUseRockSmashText: @ 81B0F90
	.string "단단해 보이는 바위지만...\n"
	.string "포켓몬 기술로 깰 수 있을지도?$"

S_PushableBoulder:: @ 81B0FCB
	lockall
	goto_if_unset FLAG_BADGE04_GET, CannotUseStrength
	goto_if_set FLAG_SYS_USE_STRENGTH, AlreadyUsedStrength
	checkpartymove 70
	compare VAR_RESULT, 6
	goto_if_eq CannotUseStrength
	setfieldeffectargument 0, VAR_RESULT
	msgbox UseStrengthPromptText, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq Strength_ChoseNo
	closemessage
	dofieldeffect FLDEFF_USE_STRENGTH
	waitstate
	goto UsedStrength
	end

S_UseStrength:: @ 81B100E
	lockall
	dofieldeffect FLDEFF_USE_STRENGTH
	waitstate
	goto UsedStrength
	end

UsedStrength: @ 81B1019
	setflag FLAG_SYS_USE_STRENGTH
	msgbox UsedStrengthText, MSGBOX_SIGN
	releaseall
	end

CannotUseStrength: @ 81B1026
	msgbox CannotUseStrengthText, MSGBOX_SIGN
	releaseall
	end

AlreadyUsedStrength: @ 81B1030
	msgbox AlreadyUsedStrengthText, MSGBOX_SIGN
	releaseall
	end

Strength_ChoseNo: @ 81B103A
	closemessage
	releaseall
	end

UseStrengthPromptText: @ 81B103D
	.string "큰 바위이지만...\n"
	.string "포켓몬 기술로 밀 수 있을지도?\p"
	.string "괴력을 쓰겠습니까?$"

UsedStrengthText: @ 81B109D
	.string "{STR_VAR_1}{K_EUNNEUN}\n"
	.string "괴력을 발휘했다!\p"
	.string "{STR_VAR_1}의 괴력 덕분에\n"
	.string "바위를 밀 수 있게 되었다!$"

CannotUseStrengthText: @ 81B10E7
	.string "큰 바위이지만...\n"
	.string "포켓몬 기술로 밀 수 있을지도?$"

AlreadyUsedStrengthText: @ 81B1127
	.string "괴력 덕분에\n"
	.string "바위를 밀어 움직일 수 있다!$"

S_UseWaterfall:: @ 81B115A
	lockall
	checkpartymove MOVE_WATERFALL
	compare VAR_RESULT, 6
	goto_if_eq Waterfall_NoMonKnows
	bufferpartymonnick 0, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	msgbox UseWaterfallPromptText, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq Waterfall_Done
	msgbox UsedWaterfallText, MSGBOX_DEFAULT
	dofieldeffect FLDEFF_USE_WATERFALL
	goto Waterfall_Done

S_CannotUseWaterfall:: @ 81B1194
	lockall
Waterfall_NoMonKnows: @ 81B1195
	msgbox CannotUseWaterfallText, MSGBOX_DEFAULT
Waterfall_Done:  @ 81B119D
	releaseall
	end

CannotUseWaterfallText: @ 81B119F
	.string "큰 소리를 내며\n"
	.string "물이 흘러 떨어진다!$"

UseWaterfallPromptText: @ 81B11D4
	.string "큰 폭포다!\n"
	.string "...폭포오르기를 쓰겠습니까?$"

UsedWaterfallText: @ 81B120D
	.string "{STR_VAR_1}{K_EUNNEUN} 폭포오르기를 썼다!$"

UseDiveScript:: @ 81B1220
	lockall
	checkpartymove MOVE_DIVE
	compare VAR_RESULT, 6
	goto_if_eq CannotUseDive
	bufferpartymonnick 0, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	setfieldeffectargument 1, 1
	msgbox UseDivePromptText, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq Dive_Done
	msgbox UsedDiveText, MSGBOX_DEFAULT
	dofieldeffect FLDEFF_USE_DIVE
	goto Dive_Done
	lockall

CannotUseDive: @ 81B125F
	msgbox CannotUseDiveText, MSGBOX_DEFAULT

Dive_Done: @ 81B1267
	releaseall
	end

S_UseDiveUnderwater:: @ 81B1269
	lockall
	checkpartymove MOVE_DIVE
	compare VAR_RESULT, 6
	goto_if_eq UnderwaterCannotUseDive
	bufferpartymonnick 0, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	setfieldeffectargument 1, 1
	msgbox UnderwaterUseDivePromptText, MSGBOX_YESNO
	compare VAR_RESULT, NO
	goto_if_eq UnderwaterDive_Done
	msgbox UsedDiveText, MSGBOX_DEFAULT
	dofieldeffect FLDEFF_USE_DIVE
	goto UnderwaterDive_Done

UnderwaterCannotUseDive: @ 81B12A7
	lockall
	msgbox UnderwaterCannotUseDiveText, MSGBOX_DEFAULT
	goto UnderwaterDive_Done

@ not used
SurfacingBlocked: @ 81B12B5
	lockall
	msgbox SurfacingBlockedText, MSGBOX_DEFAULT

UnderwaterDive_Done: @ 81B12BE
	releaseall
	end

CannotUseDiveText: @ 81B12C0
	.string "깊은 바다다!\n"
	.string "포켓몬 기술로 잠수할 수 있을지도?$"

UseDivePromptText: @ 81B1303
	.string "깊은 바다다!\n"
	.string "다이빙을 쓰겠습니까?$"

UsedDiveText: @ 81B1335
	.string "{STR_VAR_1}{K_EUNNEUN} 다이빙을 썼다!$"

UnderwaterCannotUseDiveText: @ 81B1343
	.string "위에서 빛이 들어오고 있다!\n"
	.string "포켓몬 기술로 올라갈 수 있을지도?$"

UnderwaterUseDivePromptText: @ 81B138E
	.string "위에서 빛이 들어오고 있다!\n"
	.string "다이빙을 쓰겠습니까?$"

SurfacingBlockedText: @ 81B13CE
	.string "위에 무언가가 있습니다!\n"
	.string "여기서는 다이빙을 쓸 수 없습니다$"
	@ うえに なにか あります!ｎ
	@ ここではダイビングを つかえません

SweetScentNothingHereScript:: @ 81B140C
	msgbox SweetScentNothingHereText, MSGBOX_SIGN
	end

SweetScentNothingHereText: @ 81B1415
	.string "여기에는\n"
	.string "아무것도 없는 것 같다...$"
	@ ここにはｎ
	@ なにも いないようだ……
