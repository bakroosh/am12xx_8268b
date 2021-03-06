#!/bin/bash
#-------------------------------------------------------------------------------
#-                                                                            --
#-       This software is confidential and proprietary and may be used        --
#-        only as expressly authorized by a licensing agreement from          --
#-                                                                            --
#-                            Hantro Products Oy.                             --
#-                                                                            --
#-                   (C) COPYRIGHT 2006 HANTRO PRODUCTS OY                    --
#-                            ALL RIGHTS RESERVED                             --
#-                                                                            --
#-                 The entire notice above must be reproduced                 --
#-                  on all copies and should not be removed.                  --
#-                                                                            --
#-------------------------------------------------------------------------------
#-
#--  Description : Load device driver
#--
#--------------------------------------------------------------------------------
#--
#--  Version control information, please leave untouched.
#--
#--  $RCSfile: driver_load.sh,v $
#--  $Revision: 1.1 $
#--  $Date: 2006/12/20 15:17:10 $
#--
#--------------------------------------------------------------------------------

module="/lib/modules/2.6.27.29/am7x_graph"
device="/dev/graph"
mode="666"

echo

#insert module
insmod $module.ko $* || exit 1

echo "module $module inserted"

#remove old nod
rm -f $device

#read the major asigned at loading time
#major=`cat /proc/devices | grep $module | cut -c1-3`
major=30

echo "$module major = $major"

#create dev node
mknod $device c $major 0

echo "node $device created"

#give all 'rw' access
chmod $mode $device

echo "set node access to $mode"

#the end
echo
