###############################################################################
#  
# Copyright (c) 2017, Broadband Forum
# Copyright (c) 2017, MaxLinear, Inc. and its affiliates
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

# When calling this Makefile, the following environment variables must be set:
#
#   CC ------------> Path to the compiler
#   CCFLAGS -------> Extra flags to use while compiling
#   LDGLAGS -------> Extra flags to use while linking
#
#   PLATFORM      -> Possible values: "linux"
#   OUTPUT_FOLDER -> Absolute path to the folder where binaries will be built
#
#   COMMON_LIB ----> Absolute path to the "common library" *.a file
#   COMMON_INC ----> Absolute path to the folder containing common (to all
#                    sub-projects) header files
#
#   FACTORY_LIB   -> Absolute path to the "factory library" *.a file 
#   FACTORY_INC   -> Absolute path to the folder containing the "factory
#                    library" header files
#
#   AL_EXE --------> Absolute path to the final AL entity binary
#
#   MKDIR ---------> Tool to create a directory
#

EXE                        := $(AL_EXE)

AL_DIRECTORY               := al

SRC_INDEPENDENT            := $(wildcard src_independent/*.c)
SRC_INDEPENDENT            += $(wildcard src_independent/extensions/*/*.c)
SRC_PLATFORM               := $(wildcard src_$(PLATFORM)/*.c)

OBJ_INDEPENDENT            := $(patsubst %.c,$(OUTPUT_FOLDER)/tmp/$(AL_DIRECTORY)/%.o,$(SRC_INDEPENDENT))
OBJ_PLATFORM               := $(patsubst %.c,$(OUTPUT_FOLDER)/tmp/$(AL_DIRECTORY)/%.o,$(SRC_PLATFORM))

INTERNAL_INC_INDEPENDENT   := src_independent internal_interfaces 
INTERNAL_INC_INDEPENDENT   += $(sort $(dir $(wildcard interfaces/extensions/*/))) $(sort $(dir $(wildcard  src_independent/extensions/*/)))
INTERNAL_INC_PLATFORM      := src_$(PLATFORM) internal_interfaces

EXTERNAL_INC               := $(COMMON_INC) $(FACTORY_INC)

HDR_INDEPENDENT            := $(shell find $(INTERNAL_INC_INDEPENDENT) $(EXTERNAL_INC) -name *.h)
HDR_PLATFORM               := $(shell find $(INTERNAL_INC_PLATFORM)    $(EXTERNAL_INC) -name *.h)


################################################################################
# Targets
################################################################################

.PHONY: all
all: $(EXE)


$(EXE):  $(OBJ_INDEPENDENT) $(OBJ_PLATFORM) $(FACTORY_LIB) $(COMMON_LIB) 
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_INDEPENDENT) : $(OUTPUT_FOLDER)/tmp/$(AL_DIRECTORY)/%.o : %.c $(HDR_INDEPENDENT)
	$(foreach directory, $(sort $(dir $(wildcard $(SRC_INDEPENDENT)))), $(MKDIR) $(OUTPUT_FOLDER)/tmp/$(AL_DIRECTORY)/$(directory);)
	$(CC) $(CCFLAGS) -c $(addprefix -I,$(INTERNAL_INC_INDEPENDENT) $(EXTERNAL_INC)) $< -o $@

$(OBJ_PLATFORM)    : $(OUTPUT_FOLDER)/tmp/$(AL_DIRECTORY)/%.o : %.c $(HDR_PLATFORM)
	$(foreach directory, $(sort $(dir $(wildcard $(SRC_PLATFORM)))), $(MKDIR) $(OUTPUT_FOLDER)/tmp/$(AL_DIRECTORY)/$(directory);)
	$(CC) $(CCFLAGS) -c $(addprefix -I,$(INTERNAL_INC_PLATFORM) $(EXTERNAL_INC)) $< -o $@


.PHONY: clean
clean:
	rm -rf $(EXE)
	rm -rf $(OUTPUT_FOLDER)/tmp/$(AL_DIRECTORY)
