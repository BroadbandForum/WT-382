###############################################################################
#  
# Copyright (c) 2017, Broadband Forum
# Copyright (c) 2016, Marvell International Ltd. and its affiliates
# 
# This is draft software, is subject to change, and has not been
# approved by members of the Broadband Forum. It is made available to
# non-members for internal study purposes only. For such study
# purposes, you have the right to make copies and modifications only
# for distributing this software internally within your organization
# among those who are working on it (redistribution outside of your
# organization for other than study purposes of the original or
# modified works is not permitted). For the avoidance of doubt, no
# patent rights are conferred by this license.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# 
# Unless a different date is specified upon issuance of a draft
# software release, all member and non-member license rights under the
# draft software release will expire on the earliest to occur of (i)
# nine months from the date of issuance, (ii) the issuance of another
# version of the same software release, or (iii) the adoption of the
# draft software release as final.
# 
# ---
# 
# This version of this source file is part of the Broadband Forum
# WT-382 IEEE 1905.1/1a stack project.
# 
# Please follow the release link (given below) for further details
# of the release, e.g. license validity dates and availability of
# more recent draft or final releases.
# 
# Release name: WT-382_draft1
# Release link: https://www.broadband-forum.org/software#WT-382_draft1

################################################################################
# Configuration section. Tune the parameters below according to your needs
################################################################################

# Platform dependent CONFIGs
#
# You can either "export PLATFORM=..." (and "FLAVOUR", if needed) before
# calling this Makefile or let "PLATFORM" undefined to use the default value
# (which is "linux")

ifndef PLATFORM

PLATFORM := linux
 FLAVOUR  := x86_generic
 #FLAVOUR  := arm_wrt1900acx
 #FLAVOUR  := x86_windows_mingw

endif


ifeq ($(PLATFORM),linux)
    ifeq ($(FLAVOUR), x86_generic)
        CC        := gcc
       #CC        := clang

        CCFLAGS   := -D_FLAVOUR_X86_GENERIC_

        LDFLAGS       += -lrt -lpthread   # For threads
        LDFLAGS       += -lpcap           # For packet capture
        LDFLAGS       += -lcrypto         # For WPS crypto

        AL_SUPPORTED  := yes
        HLE_SUPPORTED := yes
    else ifeq ($(FLAVOUR), arm_wrt1900acx)
        CC        := $(WRT1900_CROSS)gcc
        AR        := $(WRT1900_CROSS)ar
        CCFLAGS   := -I $(WRT1900_SYSROOT)/include/
        LDFLAGS   := -L $(WRT1900_SYSROOT)/lib/

        CCFLAGS   += -D_FLAVOUR_ARM_WRT1900ACX_

        LDFLAGS       += -lrt -lpthread   # For threads
        LDFLAGS       += -lpcap           # For packet capture
        LDFLAGS       += -lcrypto         # For WPS crypto

        AL_SUPPORTED  := yes
        HLE_SUPPORTED := yes
    else ifeq ($(FLAVOUR), x86_windows_mingw)
        CC        := $(MINGW_CROSS)gcc
        LD        := $(MINGW_CROSS)ld
        AR        := $(MINGW_CROSS)ar
        CCFLAGS   += -I $(MINGW_SYSROOT)/include/
        LDFLAGS   := -L $(MINGW_SYSROOT)/lib/

        CCFLAGS   += -D_FLAVOUR_X86_WINDOWS_MINGW_

        LDFLAGS       += -lws2_32 # Network stuff

        AL_SUPPORTED  := no
        HLE_SUPPORTED := yes
    else
        $(error "Linux FLAVOUR unknown")
    endif

    CCFLAGS       += -g -O0 -Wall -Werror #-Wextra
    CCFLAGS       += -D_HOST_IS_LITTLE_ENDIAN_=1 -DMAX_NETWORK_SEGMENT_SIZE=1500
    CCFLAGS       += -DINT8U="unsigned char"
    CCFLAGS       += -DINT16U="unsigned short int"
    CCFLAGS       += -DINT32U="unsigned int"
    CCFLAGS       += -DINT8S="signed char"
    CCFLAGS       += -DINT16S="signed short int"
    CCFLAGS       += -DINT32S="signed int"
    CCFLAGS       += -D_GNU_SOURCE
else
    $(error "'PLATFORM' variable was not set to a valid value")
endif

# Platform independent CONFIGs

#CCFLAGS += -DDO_NOT_ACCEPT_UNAUTHENTICATED_COMMANDS
CCFLAGS += -DSEND_EMPTY_TLVS
CCFLAGS += -DFIX_BROKEN_TLVS
CCFLAGS += -DSPEED_UP_DISCOVERY
  #
  # These are special flags that change the way the implementation behaves.
  # The README file contains more information regarding them.

CCFLAGS += -D_BUILD_NUMBER_=\"$(shell cat version.txt)\"
  #
  # Version flag to identify the binaries
CCFLAGS += -DREGISTER_EXTENSION_BBF
  #
  # These are special flags to enable Protocol extensions

################################################################################
# End of configuration section. Do not touch anything from this point on
################################################################################


# Shortcuts for later
#
SRC_FOLDER    := $(shell pwd)/src
OUTPUT_FOLDER := $(shell pwd)/output

# "Common" library. Includes platform specific code that is used by both the
# AL and HLE entities (example: "PLATFORM_MALLOC()", "PLATFORM_PRINTF()", ...)
# 
COMMON_LIB    := $(OUTPUT_FOLDER)/libcommon.a
COMMON_INC    := $(SRC_FOLDER)/common/interfaces

# "Factory" library. Includes "pure" 1905 related functions to parse/build 1905
# packets. This library is also used by both AL and HLE but most (if not all) of
# it is platform independent.
#
FACTORY_LIB   := $(OUTPUT_FOLDER)/libfactory.a
FACTORY_INC   := $(SRC_FOLDER)/factory/interfaces
FACTORY_INC   += $(sort $(dir $(wildcard $(SRC_FOLDER)/factory/interfaces/extensions/*/)))

# The actual binaries that we are going to build: one for the AL and another one
# for the HLE entity.
#
AL_EXE        := $(OUTPUT_FOLDER)/al_entity
HLE_EXE       := $(OUTPUT_FOLDER)/hle_entity

# Compilation will need to create new directories during the process
#
MKDIR         := mkdir -p

# Export all these variable to child Makefiles
#
export


################################################################################
# Targets
################################################################################

.PHONY: all
all:
ifeq ($(AL_SUPPORTED),yes)
all: $(AL_EXE)
endif
ifeq ($(HLE_SUPPORTED),yes)
all: $(HLE_EXE)
endif

.PHONY: $(COMMON_LIB)
$(COMMON_LIB):
	$(MAKE) -C src/common

.PHONY: $(FACTORY_LIB)
$(FACTORY_LIB):
	$(MAKE) -C src/factory 

$(AL_EXE): $(COMMON_LIB) $(FACTORY_LIB)
	$(MAKE) -C src/al

$(HLE_EXE): $(COMMON_LIB) $(FACTORY_LIB)
	$(MAKE) -C src/hle


.PHONY: unit_tests
unit_tests: all
	$(MAKE) -C src/factory unit_tests


.PHONY: clean
clean:
	$(MAKE) -C src/common  clean
	$(MAKE) -C src/factory clean
	$(MAKE) -C src/al      clean
	$(MAKE) -C src/hle     clean


.PHONY: static-analysis
static-analysis: clean
	scan-build $(MAKE) all
	cppcheck --enable=warning,style,performance,portability --inconclusive --xml-version=2 --error-exitcode=2 --suppress=memleakOnRealloc:* src

# See https://code.broadband-forum.org/scm/software/tools.git (needs username/password) for
# the licenses/add-license.py tool
.PHONY: update-license
update-license:
	add-license.py --exclude=version.txt --separator=---
