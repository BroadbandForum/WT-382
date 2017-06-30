#/bin/sh
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

##############################################################################
#
# Description: This script is an example of how to start the 1905 stack in a 
#              Linksys 1900 AC device 
#
#              The following binaries/scripts must be in the same directory :
#                   - start_linksys.sh
#                   - topology_change.sh
#                   - al_entity
#                   - configlayer
#                    
#              This script will :
#                   - configure the WIFI radio 1 with default configuration
#                   - configure the ebtables to drop 1905 multicast MACs
#                   - start the topology_change.sh script to monitor topology
#                     changes and inform 1905 stack
#                   - start the 1905 stack with the default configuration
#
#              This script must be run with the following command :
#                   - ./start_linksys.sh
#
#                
###############################################################################

AL_MAC=00:50:43:22:22:22
GHN_INTERFACE_MAC=00139D00114C
GHN_INTERFACE=eth0
DEFAULT_DOMAIN_NAME=Demo1905_2
DEFAULT_WIFI_SSID=Marvell1905_2

PATH=$PATH:.

#Leave secure mode
./configlayer -i $GHN_INTERFACE -m $GHN_INTERFACE_MAC -o SETLEGACY -p PAIRING.GENERAL.LEAVE_SECURE_DOMAIN=yes -w paterna

#Default WIFI configuration
uci set wireless.@wifi-iface[1].ssid=$DEFAULT_WIFI_SSID
uci set wireless.@wifi-iface[1].encryption='psk2'
uci set wireless.@wifi-iface[1].key='12345678'
wifi
sleep 5

#Default GHN configuration
./configlayer -i $GHN_INTERFACE -m $GHN_INTERFACE_MAC -o SETLEGACY -p NODE.GENERAL.DOMAIN_NAME=$DEFAULT_DOMAIN_NAME -w paterna

#Avoid duplicate 1905 multicast messages because of bridge
ebtables -A FORWARD  -d 01:80:c2:00:00:13 -j DROP 

#Kill previous topology_change process if exsit
process_id=`ps | grep topology_change | grep exe | awk '{print $1}'`
if [ $? -eq "0" ]; then
   kill -9 $process_id
fi

#Monitor topology changes
./topology_change.sh $GHN_INTERFACE_MAC > /dev/null &

#Start 1905 entity
echo ./al_entity -m $AL_MAC -i eth0:ghnspirit:$GHN_INTERFACE_MAC:paterna,eth1,wlan1 -v
./al_entity -m $AL_MAC -i eth0:ghnspirit:$GHN_INTERFACE_MAC:paterna,eth1,wlan1 -v

