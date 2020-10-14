################################################################################
# \file GCC_ARM.mk
# \version 1.0
#
# \brief
# GCC ARM toolchain configuration.
#
################################################################################
# \copyright
# Copyright 2018-2019 Cypress Semiconductor Corporation
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

ifeq ($(WHICHFILE),true)
$(info Processing $(lastword $(MAKEFILE_LIST)))
endif

#
# The base path to the GCC cross compilation executables
#
ifeq ($(CY_COMPILER_PATH),)
CY_CROSSPATH=$(CY_COMPILER_DIR_BWC)/bin
CY_INCLUDEPATH=$(CY_COMPILER_DIR_BWC)
else
CY_CROSSPATH=$(CY_COMPILER_PATH)/bin
CY_INCLUDEPATH=$(CY_COMPILER_PATH)
endif

#
# Build tools
#
CC=$(CY_CROSSPATH)/arm-none-eabi-gcc
AS=$(CC)
AR=$(CY_CROSSPATH)/arm-none-eabi-ar
LD=$(CC)

#
# DEBUG/NDEBUG selection
#
ifeq ($(CONFIG),Debug)
CY_TOOLCHAIN_DEBUG_FLAG=-g3
CY_TOOLCHAIN_OPTIMIZATION=-Os
else ifeq ($(CONFIG),Release)
CY_TOOLCHAIN_OPTIMIZATION=-Os
else
CY_TOOLCHAIN_DEBUG_FLAG=-g3
CY_TOOLCHAIN_OPTIMIZATION=-Os
endif

#
# CM4-specific flags
#
CY_TOOLCHAIN_CM4_COMMON_FLAGS=\
	-mcpu=cortex-m4 \
	-mthumb\
	-mlittle-endian

CY_TOOLCHAIN_CM4_CFLAGS += \
	$(CY_TOOLCHAIN_CM4_COMMON_FLAGS)\
	-isystem '"$(CY_INCLUDEPATH)/arm-none-eabi/include"'\
	-isystem '"$(CY_INCLUDEPATH)/arm-none-eabi/lib/include"'\
	-isystem '"$(CY_INCLUDEPATH)/arm-none-eabi/lib/include-fixed"'

CY_TOOLCHAIN_CM4_SFLAGS += \
	$(CY_TOOLCHAIN_CM4_COMMON_FLAGS)\
	-isystem '"$(CY_INCLUDEPATH)/arm-none-eabi/include"'\
	-isystem '"$(CY_INCLUDEPATH)/arm-none-eabi/lib/include"'\
	-isystem '"$(CY_INCLUDEPATH)/arm-none-eabi/lib/include-fixed"'

CY_TOOLCHAIN_CM4_LDFLAGS += \
	$(CY_TOOLCHAIN_CM4_COMMON_FLAGS)\
	-Wl,-A,thumb

#
# Flags common to compile and link
#
CY_TOOLCHAIN_COMMON_FLAGS=\
	-funsigned-char\
	-ffunction-sections\
	-Wall\
	-Wno-strict-aliasing

#
# Command line flags for c-files
#
CY_TOOLCHAIN_CFLAGS=\
	-c\
    $(CY_TOOLCHAIN_DEBUG_FLAG)\
	$(CY_TOOLCHAIN_COMMON_FLAGS)\
	$(CY_TOOLCHAIN_CM4_CFLAGS)\
	$(CY_TOOLCHAIN_OPTIMIZATION)\
	-std=gnu11

# Specific to FreeRTOS_LwIP
ifeq ($(OSNS), FreeRTOS_LwIP)
CY_TOOLCHAIN_CFLAGS += -Wno-address
endif

#
# Command line flags for s-files
#
CY_TOOLCHAIN_ASFLAGS=\
	-c\
	$(CY_TOOLCHAIN_COMMON_FLAGS)\
	$(CY_TOOLCHAIN_CM4_SFLAGS)\
	-std=gnu11

#
# Command line flags for linking
#
CY_TOOLCHAIN_LDFLAGS=\
	$(CY_TOOLCHAIN_COMMON_FLAGS)\
	$(CY_TOOLCHAIN_CM4_LDFLAGS)\
	-nostartfiles\
	-Xlinker --gc-sections\
	-L$(CY_COMPILER_DIR_BWC)/lib

#
# Command line flags for archiving
#
CY_TOOLCHAIN_ARFLAGS=rvs

#
# Toolchain-specific suffixes
#
CY_TOOLCHAIN_SUFFIX_S=S
CY_TOOLCHAIN_SUFFIX_s=s
CY_TOOLCHAIN_SUFFIX_C=c
CY_TOOLCHAIN_SUFFIX_H=h
CY_TOOLCHAIN_SUFFIX_CPP=cpp
CY_TOOLCHAIN_SUFFIX_HPP=hpp
CY_TOOLCHAIN_SUFFIX_O=o
CY_TOOLCHAIN_SUFFIX_A=a
CY_TOOLCHAIN_SUFFIX_D=d
CY_TOOLCHAIN_SUFFIX_LS=ld
CY_TOOLCHAIN_SUFFIX_MAP=map
CY_TOOLCHAIN_SUFFIX_TARGET=elf
CY_TOOLCHAIN_SUFFIX_ARCHIVE=a

#
# Toolchain specific flags
#
CY_TOOLCHAIN_OUTPUT_OPTION=-o
CY_TOOLCHAIN_MAPFILE=-Wl,-Map,
CY_TOOLCHAIN_STARTGROUP=-Wl,--start-group
CY_TOOLCHAIN_ENDGROUP=-Wl,--end-group
CY_TOOLCHAIN_LSFLAGS=-T
CY_TOOLCHAIN_INCRSPFILE=@
CY_TOOLCHAIN_INCRSPFILE_ASM=@
CY_TOOLCHAIN_OBJRSPFILE=@

#
# Produce a makefile dependency rule for each input file
#
CY_TOOLCHAIN_DEPENDENCIES=-MMD -MP -MF"$(subst .$(CY_TOOLCHAIN_SUFFIX_O),.$(CY_TOOLCHAIN_SUFFIX_D),$@)" -MT"$@"

#
# Additional includes in the compilation process based on this
# toolchain
#
CY_TOOLCHAIN_INCLUDES=

#
# Additional libraries in the link process based on this toolchain
#
CY_TOOLCHAIN_DEFINES=
