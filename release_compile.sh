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

# This is a simple "packaging" utility that executes Make and installs the
# output binaries in a "deployment" folder.
#
# Fell free to customize this to your own development/testing environment
#
# Usage:
#
#   ./release_compile.sh <type>
#
# ...where <type> can take any of the following values:
#
#  - "cvs"
#      The tools will be built and installed in "$RELEASE_FOLDER/<version_tag>",
#      but only if that folder did not already existed (otherwise, it won't do
#      anything)
#
#  - "VersionXXX" (ex: "VersionA7", "VersionB13", ...)
#      The tools will be built and installed in
#      "$RELEASE_FOLDER/VersionXXX/<version_tag>", wiping a previous folder with
#      the same name in case it existed.
#
# In both cases, "<version_tag>" is the value contained in the first line of
# file "version.txt" and it must have the following format:
#
#   lowercase "i" + monotonically increasing number
#
# Examples: i145, i146, i147, ....
#
# The last line of output produced by this script will always contain two words:
#   1. The HASH of the current git commit
#   2. The path to the folder where the release has been installed.
#

set -e

RELEASE_FOLDER="/external_dir/fs-rd-server/releases/Ieee1905Tools";

CURRENT_GIT_COMMIT=`git show | head -1 | cut -d" " -f2`

function help_and_quit
{
    echo "You must provide one (and only one) argument, wich must be either ";
    echo "the string 'cvs' or a string starting with 'Version' (examples: ";
    echo "'VersionA7', 'VersionB13', ...)"
    echo ""
    echo "$CURRENT_GIT_COMMIT <ERROR>"
    exit -1;
}


# Check validity of input arguments
#####################################################

echo ""

if [[ $# -ne 1 ]]; then
    help_and_quit;
fi

if [[ "$1" != "cvs" ]] && [[ "$1" != "CVS" ]] && ! [[ "$1" =~ "Version" ]]; then
    help_and_quit;
fi

version_tag=`cat version.txt | head -1`

if ! [[ "$version_tag" =~ ^i[0-9+]+$ ]]; then
    echo "Invalid version tag in file 'version.txt'";
    echo "Open that file and make sure the first line contains a valid ";
    echo "version tag, which looks like this: o+[number] (ex: 'o106')";
    echo ""
    echo "$CURRENT_GIT_COMMIT <ERROR>"
    exit -1;
fi


# Check if the requested version is already installed
#####################################################

echo "*************************************************************************"
echo "Setup..."
echo "*************************************************************************"

if [[ "$1" == "cvs" ]] || [[ "$1" == "CVS" ]]; then

    INSTALL_FOLDER=$RELEASE_FOLDER/$version_tag;

    if [[ -d $INSTALL_FOLDER ]]; then
        echo "The CVS version you are trying to generate is already installed "
        echo "in the 'releases' folder ($INSTALL_FOLDER)"
        echo "If you *really* want to regenerate it, first *manually* delete "
        echo "that folder and then re-execute this script"
        echo "Exiting..."
        echo ""
        echo "$CURRENT_GIT_COMMIT $INSTALL_FOLDER" 
        exit 0;
    fi

else

    INSTALL_FOLDER=$RELEASE_FOLDER/$1/$version_tag

    if [[ -d $INSTALL_FOLDER ]]; then
        echo "The destination folder where tools are going to be installed "
        echo "already existed ($INSTALL_FOLDER)"
        echo "We are going to wipe it out."
        echo ""
        
        rm -rf $INSTALL_FOLDER

    fi

    INSTALL_FOLDER_PARENT=$RELEASE_FOLDER/$1

    if ! [[ -d $INSTALL_FOLDER_PARENT ]]; then
        echo "Creating folder $INSTALL_FOLDER_PARENT ..."
        mkdir $INSTALL_FOLDER_PARENT;
    fi
fi

echo "Destination folder is: $INSTALL_FOLDER"


# Build Linux (X86 generic) tools
#####################################################

echo ""
echo "*************************************************************************"
echo "Building Linux (X86 generic) tools..."
echo "*************************************************************************"

PLATFORM=linux FLAVOUR=x86_generic make clean all
rm -rf linux_x86_generic
mkdir  linux_x86_generic
cp output/al_entity  linux_x86_generic
cp output/hle_entity linux_x86_generic


# Build Linux (ARM wrt1900acx) tools
#####################################################

echo ""
echo "*************************************************************************"
echo "Building Linux (ARM wrt1900acx) tools..."
echo "*************************************************************************"

PLATFORM=linux FLAVOUR=arm_wrt1900acx make clean all
rm -rf linux_arm_wrt1900acx
mkdir  linux_arm_wrt1900acx
cp output/al_entity  linux_arm_wrt1900acx
cp output/hle_entity linux_arm_wrt1900acx


# Build Linux (X86 Windows mingw cross-compilation) tools
#########################################################

echo ""
echo "*************************************************************************"
echo "Building Linux (X86 Windows mingw cross-compilation) tools..."
echo "*************************************************************************"

PLATFORM=linux FLAVOUR=x86_windows_mingw make clean all
rm -rf linux_x86_windows_mingw
mkdir  linux_x86_windows_mingw
cp output/hle_entity linux_x86_windows_mingw/hle_entity.exe


# Install 
#####################################################

echo ""
echo "*************************************************************************"
echo "Installing ..."
echo "*************************************************************************"

mkdir -p $INSTALL_FOLDER

echo "X86 generic binaries   --> $INSTALL_FOLDER/linux_x86_generic/*"
mv linux_x86_generic $INSTALL_FOLDER

echo "ARM wrt1900acx binaries --> $INSTALL_FOLDER/linux_arm_wrt1900acx/*"
mv linux_arm_wrt1900acx $INSTALL_FOLDER

echo "X86 windows mingw binaries --> $INSTALL_FOLDER/linux_x86_windows_mingw/*"
mv linux_x86_windows_mingw $INSTALL_FOLDER

make clean &>/dev/null
git archive --format=tar.gz --prefix=ieee1905/ HEAD > ieee1905_src.tgz
echo "Source code            --> $INSTALL_FOLDER/ieee1905_src.tgz"
mv ieee1905_src.tgz $INSTALL_FOLDER/

echo ""
echo $CURRENT_GIT_COMMIT $INSTALL_FOLDER

