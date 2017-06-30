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
#   COMMON_INC ----> Absolute path to the folder containing common (to all
#                    sub-projects) header files
#
#   FACTORY_LIB ---> Absolute path to the "factory library" *.a file 
#
#   MKDIR ---------> Tool to create a directory
#

FACTORY_DIRECTORY := factory

LIB               := $(FACTORY_LIB)
SRC               := $(wildcard src_independent/*.c)
SRC               += $(wildcard src_independent/extensions/*/*.c)
OBJ               := $(patsubst %.c,$(OUTPUT_FOLDER)/tmp/$(FACTORY_DIRECTORY)/%.o,$(SRC))

INTERNAL_INC      := src_independent interfaces
INTERNAL_INC      += $(sort $(dir $(wildcard interfaces/extensions/*/)))
EXTERNAL_INC      := $(COMMON_INC)

HDR               := $(shell find $(INTERNAL_INC) $(EXTERNAL_INC) -name *.h)


################################################################################
# Targets
################################################################################

.PHONY: all
all: $(LIB)


$(LIB) : $(OBJ)
	$(AR) -rc $@ $^


$(OBJ) : $(OUTPUT_FOLDER)/tmp/$(FACTORY_DIRECTORY)/%.o : %.c $(HDR)
	$(foreach directory, $(sort $(dir $(wildcard $(SRC)))), $(MKDIR) $(OUTPUT_FOLDER)/tmp/$(FACTORY_DIRECTORY)/$(directory);)
	$(CC) $(CCFLAGS) -c $(addprefix -I,$(INTERNAL_INC) $(EXTERNAL_INC)) $< -o $@


.PHONY: unit_tests
unit_tests:
	$(MAKE) -C unit_tests all


.PHONY: clean
clean:
	rm -rf $(LIB)
	rm -rf $(OUTPUT_FOLDER)/tmp/$(FACTORY_DIRECTORY)
	$(MAKE) -C unit_tests clean

