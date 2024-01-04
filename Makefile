TOOLCHAIN := $(DEVKITARM)

# don't use dkP's base_tools anymore
# because the redefinition of $(CC) conflicts
# with when we want to use $(CC) to preprocess files
# thus, manually create the variables for the bin
# files, or use arm-none-eabi binaries on the system
# if dkP is not installed on this system

ifneq (,$(TOOLCHAIN))
ifneq ($(wildcard $(TOOLCHAIN)/bin),)
export PATH := $(TOOLCHAIN)/bin:$(PATH)
endif
endif

PREFIX := arm-none-eabi-
OBJCOPY := $(PREFIX)objcopy
AS := $(PREFIX)as

LD := $(PREFIX)ld
NM := $(PREFIX)nm
OBJDUMP := $(PREFIX)objdump

include config.mk

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

ifeq (modern,$(MAKECMDGOALS))
  MODERN := 1
endif

# use arm-none-eabi-cpp for macOS
# as macOS's default compiler is clang
# and clang's preprocessor will warn on \u
# when preprocessing asm files, expecting a unicode literal
# we can't unconditionally use arm-none-eabi-cpp
# as installations which install binutils-arm-none-eabi
# don't come with it
ifeq ($(shell uname -s),Darwin)
	CPP := $(PREFIX)cpp
else
	CPP := $(CC) -E
endif

#### Tools ####

SHELL     := /bin/bash -o pipefail
CC1       := tools/agbcc/bin/agbcc$(EXE)
CPP       := $(PREFIX)cpp
LD        := $(PREFIX)ld
OBJCOPY   := $(PREFIX)objcopy

GBAGFX    := tools/gbagfx/gbagfx$(EXE)
RSFONT    := tools/rsfont/rsfont$(EXE)
AIF2PCM   := tools/aif2pcm/aif2pcm$(EXE)
MID2AGB   := tools/mid2agb/mid2agb$(EXE)
PREPROC   := tools/preproc/preproc$(EXE)
SCANINC   := tools/scaninc/scaninc$(EXE)
RAMSCRGEN := tools/ramscrgen/ramscrgen$(EXE)
GBAFIX    := tools/gbafix/gbafix$(EXE)
MAPJSON   := tools/mapjson/mapjson$(EXE)
JSONPROC  := tools/jsonproc/jsonproc$(EXE)

PERL := perl

ASFLAGS  := -mcpu=arm7tdmi -I include --defsym $(GAME_VERSION)=1 --defsym REVISION=$(GAME_REVISION) --defsym DEBUG_FIX=$(DEBUG_FIX) --defsym $(GAME_LANGUAGE)=1 --defsym DEBUG=$(DEBUG) --defsym MODERN=$(MODERN)
CPPFLAGS := -iquote include -Werror -Wno-trigraphs -D $(GAME_VERSION) -D REVISION=$(GAME_REVISION) -D $(GAME_LANGUAGE) -D=DEBUG_FIX$(DEBUG_FIX) -D DEBUG=$(DEBUG) -D MODERN=$(MODERN)
CPPFLAGS += -I tools/agbcc/include -nostdinc -undef
CC1FLAGS := -g -mthumb-interwork -Wimplicit -Wparentheses -Wunused -Werror -O2 -fhex-asm

ifneq (,$(NONMATCHING))
CPPFLAGS += -DNONMATCHING
ASFLAGS  += --defsym NONMATCHING=1
endif

#### Files ####

ROM := poke$(BUILD_NAME).gba
MAP := $(ROM:%.gba=%.map)
ELF := $(ROM:%.gba=%.elf)
SYM := $(ROM:%.gba=%.sym)

BUILD_DIR := build/$(BUILD_NAME)

C_SOURCES    := $(wildcard src/*.c src/*/*.c src/*/*/*.c)
ASM_SOURCES  := $(wildcard src/*.s src/*/*.s asm/*.s data/*.s sound/*.s sound/*/*.s)

C_OBJECTS    := $(addprefix $(BUILD_DIR)/, $(C_SOURCES:%.c=%.o))
ASM_OBJECTS  := $(addprefix $(BUILD_DIR)/, $(ASM_SOURCES:%.s=%.o))
ALL_OBJECTS  := $(C_OBJECTS) $(ASM_OBJECTS)
OBJS_REL     := $(ALL_OBJECTS:$(BUILD_DIR)/%=%)

SUBDIRS        := $(sort $(dir $(ALL_OBJECTS)))
DATA_SRC_SUBDIR = src/data

GCC_VER = $(shell $(MODERNCC) -dumpversion)

LIBDIRS := ../../tools/agbcc/lib
LDFLAGS := $(LIBDIRS:%=-L %) -lgcc -lc

LD_SCRIPT := $(BUILD_DIR)/ld_script.ld

# Special configurations required for lib files
%src/libs/siirtc.o:       CC1FLAGS := -mthumb-interwork
%src/libs/agb_flash.o:    CC1FLAGS := -O1 -mthumb-interwork
%src/libs/agb_flash_1m.o: CC1FLAGS := -O1 -mthumb-interwork
%src/libs/agb_flash_mx.o: CC1FLAGS := -O1 -mthumb-interwork
%src/libs/m4a.o:          CC1 := tools/agbcc/bin/old_agbcc$(EXE)
%src/libs/libisagbprn.o:  CC1 := tools/agbcc/bin/old_agbcc$(EXE)
%src/libs/libisagbprn.o:  CC1FLAGS := -mthumb-interwork

#### Main Rules ####

ALL_BUILDS := ruby ruby_debug ruby_rev1 ruby_rev2 sapphire sapphire_debug sapphire_rev1 sapphire_rev2
MODERN_BUILDS := $(ALL_BUILDS:%=%_modern)

# Available targets
.PHONY: all clean mostlyclean tidy tools syms $(ALL_BUILDS)

infoshell = $(foreach line, $(shell $1 | sed "s/ /__SPACE__/g"), $(info $(subst __SPACE__, ,$(line))))

# Build tools when building the rom
# Disable dependency scanning for clean/tidy/tools
ifeq (,$(filter-out all modern syms,$(MAKECMDGOALS)))
$(call infoshell, $(MAKE) tools)
else
NODEP := 1
endif

# Disable dependency scanning when NODEP is used for quick building
ifeq ($(NODEP),)
  $(BUILD_DIR)/src/%.o:  C_FILE = $(*D)/$(*F).c
  $(BUILD_DIR)/src/%.o:  C_DEP = $(shell $(SCANINC) -I include $(C_FILE:$(BUILD_DIR)/=))
  $(BUILD_DIR)/asm/%.o:  ASM_DEP = $(shell $(SCANINC) asm/$(*F).s)
  $(BUILD_DIR)/data/%.o: ASM_DEP = $(shell $(SCANINC) data/$(*F).s)
endif

MAKEFLAGS += --no-print-directory
# Secondary expansion is required for dependency variables in object rules.
.SECONDEXPANSION:
# Clear the default suffixes
.SUFFIXES:
# Don't delete intermediate files
.SECONDARY:
# Delete files that weren't built properly
.DELETE_ON_ERROR:

# Create build subdirectories
$(shell mkdir -p $(SUBDIRS))

AUTO_GEN_TARGETS :=

all: $(ROM)

syms: $(SYM)

mostlyclean: tidy
	find sound/direct_sound_samples \( -iname '*.bin' \) -exec rm {} +
	$(RM) $(ALL_OBJECTS)
	find . \( -iname '*.1bpp' -o -iname '*.4bpp' -o -iname '*.8bpp' -o -iname '*.gbapal' -o -iname '*.lz' -o -iname '*.rl' \) -exec rm {} +
	rm -f data/layouts/layouts.inc data/layouts/layouts_table.inc
	rm -f data/maps/connections.inc data/maps/events.inc data/maps/groups.inc data/maps/headers.inc
	find data/maps \( -iname 'connections.inc' -o -iname 'events.inc' -o -iname 'header.inc' \) -exec rm {} +
	rm -f $(AUTO_GEN_TARGETS)

clean: mostlyclean
	$(MAKE) clean -C tools/gbagfx
	$(MAKE) clean -C tools/scaninc
	$(MAKE) clean -C tools/preproc
	$(MAKE) clean -C tools/bin2c
	$(MAKE) clean -C tools/rsfont
	$(MAKE) clean -C tools/aif2pcm
	$(MAKE) clean -C tools/ramscrgen
	$(MAKE) clean -C tools/gbafix
	$(MAKE) clean -C tools/mapjson
	$(MAKE) clean -C tools/jsonproc

tools:
	@$(MAKE) -C tools/gbagfx
	@$(MAKE) -C tools/scaninc
	@$(MAKE) -C tools/preproc
	@$(MAKE) -C tools/bin2c
	@$(MAKE) -C tools/rsfont
	@$(MAKE) -C tools/aif2pcm
	@$(MAKE) -C tools/ramscrgen
	@$(MAKE) -C tools/mid2agb
	@$(MAKE) -C tools/gbafix
	@$(MAKE) -C tools/mapjson
	@$(MAKE) -C tools/jsonproc

tidy:
	$(RM) $(ALL_BUILDS:%=poke%{.gba,.elf,.map})
	$(RM) $(MODERN_BUILDS:%=poke%{.gba,.elf,.map})
	$(RM) -r build

$(ROM): %.gba: %.elf
	$(OBJCOPY) -O binary --gap-fill 0xFF --pad-to 0x9000000 $< $@

%.elf: $(LD_SCRIPT) $(ALL_OBJECTS)
	@echo "cd $(BUILD_DIR) && $(LD) -T $(LD_SCRIPT:$(BUILD_DIR)/%=%) -Map ../../$(MAP) -o ../../$@ <objects> <lib>"
	@cd $(BUILD_DIR) && $(LD) -T $(LD_SCRIPT:$(BUILD_DIR)/%=%) -Map ../../$(MAP) -o ../../$@ $(OBJS_REL) $(LDFLAGS)
	$(GBAFIX) $@ -p -t"$(TITLE)" -c$(GAME_CODE) -m$(MAKER_CODE) -r$(GAME_REVISION) --silent

$(LD_SCRIPT): $(LD_SCRIPT:$(BUILD_DIR)/%.ld=%.txt) $(BUILD_DIR)/sym_common.ld $(BUILD_DIR)/sym_ewram.ld $(BUILD_DIR)/sym_bss.ld
	sed -e "s#tools/#../../tools/#g" $< >$@
$(BUILD_DIR)/sym_%.ld: sym_%.txt
	$(CPP) -P $(CPPFLAGS) $< | sed -e "s#tools/#../../tools/#g" > $@

$(C_OBJECTS): $(BUILD_DIR)/%.o: %.c $$(C_DEP)
	@echo "$(CC1) <flags> -o $@ $<"
	@$(CPP) $(CPPFLAGS) $< -o $(BUILD_DIR)/$*.i
	@$(PREPROC) $(BUILD_DIR)/$*.i charmap.txt | $(CC1) $(CC1FLAGS) -o $(BUILD_DIR)/$*.s
	@printf ".text\n\t.align\t2, 0\n" >> $(BUILD_DIR)/$*.s
	@$(AS) $(ASFLAGS) -W -o $@ $(BUILD_DIR)/$*.s

# Only .s files in data need preproc
$(BUILD_DIR)/data/%.o: data/%.s $$(ASM_DEP)
	@echo "$(AS) <flags> -o $@ $<"
	@$(PREPROC) $< charmap.txt | $(CPP) -I include | $(AS) $(ASFLAGS) -o $@

$(BUILD_DIR)/%.o: %.s $$(ASM_DEP)
	@echo "$(AS) <flags> -o $@ $<"
	@$(AS) $(ASFLAGS) $< -o $@

# "friendly" target names for convenience sake
ruby:              ; @$(MAKE) GAME_VERSION=RUBY
ruby_debug:        ; @$(MAKE) GAME_VERSION=RUBY DEBUG=1
sapphire:          ; @$(MAKE) GAME_VERSION=SAPPHIRE
sapphire_debug:    ; @$(MAKE) GAME_VERSION=SAPPHIRE DEBUG=1

modern:                   ; @$(MAKE) GAME_VERSION=RUBY MODERN=1
ruby_modern:              ; @$(MAKE) GAME_VERSION=RUBY MODERN=1
ruby_debug_modern:        ; @$(MAKE) GAME_VERSION=RUBY DEBUG=1 MODERN=1
sapphire_modern:          ; @$(MAKE) GAME_VERSION=SAPPHIRE MODERN=1
sapphire_debug_modern:    ; @$(MAKE) GAME_VERSION=SAPPHIRE DEBUG=1 MODERN=1

#### Graphics Rules ####

GFX_OPTS :=

include castform.mk
include tilesets.mk
include fonts.mk
include misc.mk
include spritesheet_rules.mk
include map_data_rules.mk
include json_data_rules.mk

%.1bpp:   %.png ; $(GBAGFX) $< $@ $(GFX_OPTS)
%.4bpp:   %.png ; $(GBAGFX) $< $@ $(GFX_OPTS)
%.8bpp:   %.png ; $(GBAGFX) $< $@ $(GFX_OPTS)
%.gbapal: %.pal ; $(GBAGFX) $< $@ $(GFX_OPTS)
%.gbapal: %.png ; $(GBAGFX) $< $@ $(GFX_OPTS)
%.lz:     %     ; $(GBAGFX) $< $@ $(GFX_OPTS)
%.rl:     %     ; $(GBAGFX) $< $@ $(GFX_OPTS)

#### Sound Rules ####

sound/direct_sound_samples/cries/cry_%.bin: sound/direct_sound_samples/cries/cry_%.aif
	$(AIF2PCM) $< $@ --compress

sound/%.bin: sound/%.aif
	$(AIF2PCM) $< $@

sound/songs/%.s: sound/songs/%.mid
	cd $(@D) && ../../$(MID2AGB) $(<F)

###################
### Symbol file ###
###################

$(SYM): $(ELF)
	$(OBJDUMP) -t $< | sort -u | grep -E "^0[2389]" | $(PERL) -p -e 's/^(\w{8}) (\w).{6} \S+\t(\w{8}) (\S+)$$/\1 \2 \3 \4/g' > $@
