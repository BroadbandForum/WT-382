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
#
#   OUTPUT_FOLDER -> Absolute path to the folder where binaries will be built
#
#   COMMON_LIB ----> Absolute path to the "common library" *.a file
#   COMMON_INC ----> Absolute path to the folder containing common (to all
#                    sub-projects) header files

#   FACTORY_LIB   -> Absolute path to the "factory library" *.a file 
#   FACTORY_INC   -> Absolute path to the folder containing the "factory
#                    library" header files
#
#   MKDIR ---------> Tool to create a directory
#
UNIT_TESTS_DIRECTORY := factory/unit_tests

UNITS := 1905_tlv_forging
UNITS += 1905_tlv_parsing
UNITS += 1905_cmdu_forging
UNITS += 1905_cmdu_parsing
UNITS += 1905_alme_forging
UNITS += 1905_alme_parsing
UNITS += lldp_tlv_parsing
UNITS += lldp_payload_forging
UNITS += lldp_payload_parsing

UNITS += extensions/bbf/bbf_tlv_forging
UNITS += extensions/bbf/bbf_tlv_parsing

EXE  := $(addprefix $(OUTPUT_FOLDER)/UNITTEST_, $(UNITS))
SRC  := $(addsuffix .c, $(UNITS))
OBJ  := $(addprefix $(OUTPUT_FOLDER)/tmp/$(UNIT_TESTS_DIRECTORY)/, $(addsuffix .o, $(UNITS)))


INTERNAL_INC := . $(sort $(dir $(wildcard extensions/*/)))
EXTERNAL_INC := $(COMMON_INC) $(FACTORY_INC)

HDR := $(shell find $(INTERNAL_INC) $(EXTERNAL_INC) -name "*.h")

SHARED_SRC := $(wildcard *test_vectors.c)
SHARED_SRC += $(wildcard extensions/*/*test_vectors.c)
SHARED_OBJ := $(addprefix $(OUTPUT_FOLDER)/tmp/$(UNIT_TESTS_DIRECTORY)/,$(patsubst %.c,%.o,$(SHARED_SRC)))
SHARED_HDR := $(patsubst %.c,%.h,$(SHARED_SRC))

#LDFLAGS += -fdata-sections -ffunction-sections -Wl,--gc-sections

TESTS      := $(addprefix UNITTEST_,$(basename $(UNITS)))

################################################################################
# Targets
################################################################################

.PHONY: all
all: $(TESTS) 


.PHONY: $(TESTS)
$(TESTS) : UNITTEST_% : $(OUTPUT_FOLDER)/UNITTEST_%
	$<


$(EXE) : $(OUTPUT_FOLDER)/UNITTEST_% : $(OUTPUT_FOLDER)/tmp/$(UNIT_TESTS_DIRECTORY)/%.o $(SHARED_OBJ) $(COMMON_LIB) $(FACTORY_LIB)
	$(foreach directory, $(UNITS), $(MKDIR) $(OUTPUT_FOLDER)/UNITTEST_$(shell dirname $(directory);))
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ) : $(OUTPUT_FOLDER)/tmp/$(UNIT_TESTS_DIRECTORY)/%.o : %.c 
	$(foreach directory, $(sort $(dir $(wildcard $(SRC)))), $(MKDIR) $(OUTPUT_FOLDER)/tmp/$(UNIT_TESTS_DIRECTORY)/$(directory);)
	$(CC)  $(CCFLAGS) -c $(addprefix -I,$(INTERNAL_INC) $(EXTERNAL_INC)) $< -o $@;


$(SHARED_OBJ) : $(OUTPUT_FOLDER)/tmp/$(UNIT_TESTS_DIRECTORY)/%.o : %.c $(SHARED_HDR)
	$(foreach directory, $(sort $(dir $(wildcard $(SHARED_SRC)))), $(MKDIR) $(OUTPUT_FOLDER)/tmp/$(UNIT_TESTS_DIRECTORY)/$(directory);)
	$(CC)  $(CCFLAGS) -c $(addprefix -I,$(INTERNAL_INC) $(EXTERNAL_INC)) $< -o $@;


.PHONY: clean
clean:
	rm -f $(EXE)
	rm -rf $(OUTPUT_FOLDER)/UNITTEST_*
	rm -rf $(OUTPUT_FOLDER)/tmp/UNITTEST_*
	rm -rf $(OUTPUT_FOLDER)/tmp/$(UNIT_TESTS_DIRECTORY)
