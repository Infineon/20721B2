#
# Copyright 2019, Cypress Semiconductor Corporation or a subsidiary of
# Cypress Semiconductor Corporation. All Rights Reserved.
#
# This software, including source code, documentation and related
# materials ("Software"), is owned by Cypress Semiconductor Corporation
# or one of its subsidiaries ("Cypress") and is protected by and subject to
# worldwide patent protection (United States and foreign),
# United States copyright laws and international treaty provisions.
# Therefore, you may use this Software only as provided in the license
# agreement accompanying the software package from which you
# obtained this Software ("EULA").
# If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
# non-transferable license to copy, modify, and compile the Software
# source code solely for use in connection with Cypress's
# integrated circuit products. Any reproduction, modification, translation,
# compilation, or representation of this Software except as specified
# above is prohibited without the express written permission of Cypress.
#
# Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
# reserves the right to make changes to the Software without notice. Cypress
# does not assume any liability arising out of the application or use of the
# Software or any product or circuit described in the Software. Cypress does
# not authorize its products for use in any products where a malfunction or
# failure of the Cypress product may reasonably be expected to result in
# significant property damage, injury or death ("High Risk Product"). By
# including Cypress's product in a High Risk Product, the manufacturer
# of such system or application assumes all risk of such use and in doing
# so agrees to indemnify Cypress against all liability.
#

ifeq ($(WHICHFILE),true)
$(info Processing $(lastword $(MAKEFILE_LIST)))
endif

#
# Flags construction
#
CY_RECIPE_CFLAGS?=\
    $(CY_CORE_CFLAGS)\
    $(CY_TOOLCHAIN_CFLAGS)\
    -Wno-unused-variable\
    -Wno-unused-function\
    @$(CY_CORE_PATCH_CFLAGS)

CY_RECIPE_ASFLAGS?=\
    $(CY_CORE_SFLAGS)\
	$(CY_TOOLCHAIN_ASFLAGS)

CY_RECIPE_LDFLAGS?=\
	$(LDFLAGS)\
	$(CY_TOOLCHAIN_LDFLAGS)\
    $(CY_CORE_LDFLAGS)\
    -Wl,--entry=$(CY_CORE_APP_ENTRY)\
    -Wl,-z,muldefs\
    -Wl,--no-warn-mismatch\
    -Wl,--just-symbols="$(CY_CORE_PATCH)"\
    -T$(CY_CONFIG_DIR)/$(APPNAME).ld

CY_RECIPE_ARFLAGS?=$(CY_TOOLCHAIN_ARFLAGS)

#
# Defines construction
#
CY_RECIPE_DEFINES?=\
	$(addprefix -D,$(DEFINES))\
	$(CY_APP_DEFINES)\
	$(CY_APP_OTA_DEFINES)\
    $(CY_CORE_DEFINES)\
    $(CY_CORE_EXTRA_DEFINES)\
    $(CY_TOOLCHAIN_DEBUG_FLAG)\
    -DSPAR_CRT_SETUP=$(CY_CORE_APP_ENTRY)\
	$(foreach feature,$(CY_COMPONENT_LIST),-DCOMPONENT_$(subst -,_,$(feature)))

#
# Includes construction
#
CY_RECIPE_INCLUDES?=\
	$(addprefix -I,$(INCLUDES))\
    $(CY_TOOLCHAIN_INCLUDES)\
    $(addprefix -I,$(CY_SEARCH_APP_INCLUDES))

#
# Sources construction
#
CY_RECIPE_SOURCE=$(CY_SEARCH_APP_SOURCE)

#
# Libraries construction
#
CY_RECIPE_LIBS=$(LDLIBS) $(CY_SEARCH_APP_LIBS)

#
# Generate source step
#
ifeq ($(LIBNAME),)
CY_RECIPE_GENERATED_FLAG=TRUE
CY_RECIPE_GENERATED=$(CY_GENERATED_DIR)/lib_installer.c
CY_RECIPE_GENSRC=\
    bash --norc --noprofile\
    "$(CY_INTERNAL_BASELIB_PATH)/make/scripts/bt_gen_lib_installer.bash"\
    "--shell=$(CY_MODUS_SHELL_DIR)"\
    "--scripts=$(CY_INTERNAL_BASELIB_PATH)/make/scripts"\
    "--out=$(CY_RECIPE_GENERATED)"\
    $(addprefix $(CY_INTERNAL_BASELIB_PATH)/$(CY_CORE_PATCH_LIB_PATH)/,$(CY_APP_PATCH_LIBS))\
    $(addprefix $(CY_APP_PATCH_LIBS_PRO_PATH)/,$(CY_APP_PATCH_LIBS_PRO))\
    $(if $(VERBOSE),"--verbose"); CY_CMD_TERM=;
endif

#
# Prebuild step
#
ifeq ($(LIBNAME),)
CY_RECIPE_PREBUILD?=\
    bash --norc --noprofile\
    "$(CY_INTERNAL_BASELIB_PATH)/make/scripts/bt_pre_build.bash"\
    "--shell=$(CY_MODUS_SHELL_DIR)"\
    "--scripts=$(CY_INTERNAL_BASELIB_PATH)/make/scripts"\
    "--defs=$(CY_CORE_LD_DEFS)"\
    "--patch=$(CY_CORE_PATCH)"\
    "--ld=$(CY_CONFIG_DIR)/$(APPNAME).ld"\
    $(if $(VERBOSE),"--verbose"); CY_CMD_TERM=;
endif

#
# Postbuild step
#
ifeq ($(LIBNAME),)
CY_RECIPE_POSTBUILD?=\
    bash --norc --noprofile\
    "$(CY_INTERNAL_BASELIB_PATH)/make/scripts/bt_post_build.bash"\
    "--shell=$(CY_MODUS_SHELL_DIR)"\
    "--cross=$(CY_CROSSPATH)/arm-none-eabi-"\
    "--scripts=$(CY_INTERNAL_BASELIB_PATH)/make/scripts"\
    "--tools=$(CY_WICED_TOOLS_DIR)"\
    "--builddir=$(CY_CONFIG_DIR)"\
    "--elfname=$(APPNAME).elf"\
    "--appname=$(APPNAME)"\
    "--hdf=$(CY_CORE_HDF)"\
    "--entry=$(CY_CORE_APP_ENTRY).entry"\
    "--cgslist=$(CY_CORE_CGSLIST)"\
    "--cgsargs=$(CY_CORE_CGS_ARGS)"\
    "--btp=$(CY_CORE_BTP)"\
    "--id=$(CY_CORE_HCI_ID)"\
    "--chip=$(CHIP)$(CHIP_REV)"\
    "--target=$(TARGET)"\
    "--minidriver=$(CY_CORE_MINIDRIVER)"\
    "--clflags=$(CY_CORE_APP_CHIPLOAD_FLAGS)"\
    --extras=$(CY_APP_OTA)$(APP_STATIC_DATA)$(CY_CORE_APP_XIP_EXTRA)$(CY_CORE_DS2_EXTRA)\
    $(if $(VERBOSE),"--verbose"); CY_CMD_TERM=;
endif
