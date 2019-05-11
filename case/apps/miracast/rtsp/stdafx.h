/**
 * @file: stdafx.h
 * @brief Part of the RDS library.
 *
 * INTEL CONFIDENTIAL
 * Copyright 2010 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to 
 * the source code ("Material") are owned by Intel Corporation or its 
 * suppliers or licensors.  Title to the Material remains with Intel 
 * Corporation or its suppliers and licensors.  The Material contains trade 
 * secrets and proprietary and confidential information of Intel or its 
 * suppliers and licensors.  The Material is protected by worldwide copyright 
 * and trade secret laws and treaty provisions. No part of the Material may 
 * be used, copied, reproduced, modified, published, uploaded, posted, 
 * transmitted, distributed, or disclosed in any way without Intel's prior 
 * express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual 
 * property right is granted to or conferred upon you by disclosure or 
 * delivery of the Materials,  either expressly, by implication, inducement, 
 * estoppel or otherwise.  Any license under such intellectual property 
 * rights must be express and approved by Intel in writing.
 */

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

typedef int                 BOOL;
#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#include "rdsrtsp.h"
#include "rdsparser.h"

#ifdef _POSIX_SOURCE
#define strtok_s	strtok_r
#endif
