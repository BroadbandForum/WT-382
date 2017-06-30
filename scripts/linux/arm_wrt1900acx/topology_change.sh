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
# Description: This script is in charge of monitoring network topology changes
#              and inform the 1905 stack that a new device has been connected 
#              or a device has been disconnected.
#              
#              This script is automatically started by the 'start_linksys.sh' 
#              script.
#              
#              In this example, the script only monitors GHN and WIFI 
#              interfaces. If something occurs in these two interfaces,
#              a "touch" will be done to the /tmp/topology_change file to 
#              inform the 1905 stack that the topology must be refreshed.
#               
###############################################################################

GHN_INTERFACE_MAC=$1
GHN_INTERFACE=eth0
WIFI_INTERFACE=wlan1

wifi_device_list_old=""
ghn_device_list_old=""
while true;
do
  echo "Refreshing device list...."
  wifi_device_list=`iw dev $WIFI_INTERFACE station dump | grep Station | cut -f2 -d' '`
  ghn_device_list=`./configlayer -i $GHN_INTERFACE -m $GHN_INTERFACE_MAC -o GETLEGACY -p DIDMNG.GENERAL.MACS -w paterna`
  if [ "$wifi_device_list" != "$wifi_device_list_old" ] || [ "$ghn_device_list" != "$ghn_device_list_old" ] 
  then
      echo "Topology has changed"
      touch /tmp/topology_change 
      echo "Old list :"
      echo $wifi_device_list_old
      echo $ghn_device_list_old
      echo "New list :"
      echo $wifi_device_list
      echo $ghn_device_list
      wifi_device_list_old=$wifi_device_list
      ghn_device_list_old=$ghn_device_list
  fi
  sleep 5
done

