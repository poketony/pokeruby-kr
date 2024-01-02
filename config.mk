### Build Configuration ###

# Default variables
GAME_VERSION  ?= RUBY
GAME_REVISION ?= 0
GAME_LANGUAGE ?= ENGLISH
DEBUG         ?= 0
MODERN        ?= 0
DEBUG_FIX     ?= 0

# For gbafix
MAKER_CODE  := 01

# Version
ifeq ($(GAME_VERSION), RUBY)
  BUILD_NAME := ruby
  TITLE      := POKEMON RUBY
  GAME_CODE  := AXV
else
ifeq ($(GAME_VERSION), SAPPHIRE)
  BUILD_NAME := sapphire
  TITLE      := POKEMON SAPP
  GAME_CODE  := AXP
else
  $(error unknown version $(GAME_VERSION))
endif
endif

# Language
ifeq ($(GAME_LANGUAGE), ENGLISH)
  BUILD_NAME := $(BUILD_NAME)
  GAME_CODE  := $(GAME_CODE)E
else
  $(error unknown language $(GAME_LANGUAGE))
endif

# Revision
ifeq ($(GAME_REVISION), 0)
  BUILD_NAME := $(BUILD_NAME)
else
  $(error unknown revision $(GAME_REVISION))
endif

# Debug fixes (always nonmatching)
ifeq ($(DEBUG_FIX), 1)
  DEBUG = 1
endif

# Debug
ifeq ($(DEBUG), 1)
  BUILD_NAME := $(BUILD_NAME)_debug
else
ifeq ($(DEBUG_FIX), 1)
  BUILD_NAME := $(BUILD_NAME)_fixed
endif
endif

# Modern GCC
ifeq ($(MODERN), 0)
  BUILD_NAME := $(BUILD_NAME)
else
  BUILD_NAME := $(BUILD_NAME)_modern
endif
