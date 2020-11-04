/*
**  GSC-18128-1, "Core Flight Executive Version 6.7"
**
**  Copyright (c) 2006-2019 United States Government as represented by
**  the Administrator of the National Aeronautics and Space Administration.
**  All Rights Reserved.
**
**  Licensed under the Apache License, Version 2.0 (the "License");
**  you may not use this file except in compliance with the License.
**  You may obtain a copy of the License at
**
**    http://www.apache.org/licenses/LICENSE-2.0
**
**  Unless required by applicable law or agreed to in writing, software
**  distributed under the License is distributed on an "AS IS" BASIS,
**  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**  See the License for the specific language governing permissions and
**  limitations under the License.
*/

/*
**  File: cfe_es_task.h
**
**  Purpose:
**  cFE Executive Services (ES) task header file
**
**  References:
**     Flight Software Branch C Coding Standard Version 1.0a
**     cFE Flight Software Application Developers Guide
**
**  Notes:
**
*/
/*************************************************************************/
#ifndef _cfe_es_task_
#define _cfe_es_task_

/*
** Includes
*/
#include "cfe.h"
#include "cfe_es.h"
#include "cfe_es_apps.h"
#include "cfe_es_events.h"
#include "cfe_es_msg.h"
#include "cfe_es_perf.h"

/*************************************************************************/

/*
** ES File descriptions
*/
#define CFE_ES_SYS_LOG_DESC    "ES system log data file"
#define CFE_ES_TASK_LOG_DESC   "ES Task Info file"
#define CFE_ES_APP_LOG_DESC    "ES Application Info file"
#define CFE_ES_ER_LOG_DESC     "ES ERlog data file"
#define CFE_ES_PERF_LOG_DESC   "ES Performance data file"

/*
 * Limit for the total number of entries that may be
 * produced by a "query all" type command.
 */
#define CFE_ES_QUERY_ALL_MAX_ENTRIES    (CFE_PLATFORM_ES_MAX_APPLICATIONS + CFE_PLATFORM_ES_MAX_LIBRARIES)

/*************************************************************************/
/*
** Type definitions
*/

/*
 * Background log dump state structure
 *
 * This structure is stored in global memory and keeps the state
 * of the log dump from one iteration to the next.
 *
 * NOTE: This is used for log structures which are expected to be small
 * enough so such that it is not necessary to throttle the file write or
 * spread it over time.
 *
 * Therefore, the only thing necessary to be stored is whether there
 * is a pending write request, and the data file name.
 *
 * Larger log files, such as the Perf log, must implement a state machine
 * with a dedicated state data structure.
 */
typedef struct
{
    volatile bool   IsPending;
    char            DataFileName[OS_MAX_PATH_LEN];
} CFE_ES_BackgroundLogDumpGlobal_t;



/*
** Type definition (ES task global data)
*/
typedef struct
{
  /*
  ** ES Task command interface counters
  */
  uint8                 CommandCounter;
  uint8                 CommandErrorCounter;

  /*
  ** ES Task housekeeping telemetry packet
  */
  CFE_ES_HousekeepingTlm_t     HkPacket;

  /*
  ** Single application telemetry packet
  */
  CFE_ES_OneAppTlm_t    OneAppPacket;

  /*
  ** Single application telemetry packet
  */
  CFE_ES_MemStatsTlm_t MemStatsPacket;

  /*
  ** ES Task operational data (not reported in housekeeping)
  */
  CFE_MSG_Message_t *MsgPtr;
  CFE_SB_PipeId_t    CmdPipe;

  /*
  ** ES Task initialization data (not reported in housekeeping)
  */
  char                  PipeName[OS_MAX_API_NAME];
  uint16                PipeDepth;

  uint8                 LimitHK;
  uint8                 LimitCmd;

  CFE_ES_BackgroundLogDumpGlobal_t  BackgroundERLogDumpState;

  /*
   * Persistent state data associated with performance log data file writes
   */
  CFE_ES_PerfDumpGlobal_t    BackgroundPerfDumpState;

  /*
   * Persistent state data associated with background app table scans
   */
  CFE_ES_AppTableScanState_t BackgroundAppScanState;

} CFE_ES_TaskData_t;

/*
** Executive Services (ES) task global data.
*/
extern CFE_ES_TaskData_t CFE_ES_TaskData;



/*************************************************************************/

/*
** ES Task function prototypes
*/
void  CFE_ES_TaskMain(void);
int32 CFE_ES_TaskInit(void);
void  CFE_ES_TaskPipe(CFE_MSG_Message_t *MsgPtr);


/*
 * Functions related to the ES background helper task for low-priority tasks
 */
int32 CFE_ES_BackgroundInit(void);
void  CFE_ES_BackgroundTask(void);
void  CFE_ES_BackgroundWakeup(void);
void  CFE_ES_BackgroundCleanup(void);

/*
** ES Task message dispatch functions
*/
int32 CFE_ES_HousekeepingCmd(const CFE_SB_CmdHdr_t *data);
int32 CFE_ES_NoopCmd(const CFE_ES_Noop_t *Cmd);
int32 CFE_ES_ResetCountersCmd(const CFE_ES_ResetCounters_t *data);
int32 CFE_ES_RestartCmd(const CFE_ES_Restart_t *data);
int32 CFE_ES_StartAppCmd(const CFE_ES_StartApp_t *data);
int32 CFE_ES_StopAppCmd(const CFE_ES_StopApp_t *data);
int32 CFE_ES_RestartAppCmd(const CFE_ES_RestartApp_t *data);
int32 CFE_ES_ReloadAppCmd(const CFE_ES_ReloadApp_t *data);
int32 CFE_ES_QueryOneCmd(const CFE_ES_QueryOne_t *data);
int32 CFE_ES_QueryAllCmd(const CFE_ES_QueryAll_t *data);
int32 CFE_ES_QueryAllTasksCmd(const CFE_ES_QueryAllTasks_t *data);
int32 CFE_ES_ClearSyslogCmd(const CFE_ES_ClearSyslog_t *data);
int32 CFE_ES_OverWriteSyslogCmd(const CFE_ES_OverWriteSyslog_t *data);
int32 CFE_ES_WriteSyslogCmd(const CFE_ES_WriteSyslog_t *data);
int32 CFE_ES_ClearERLogCmd(const CFE_ES_ClearERLog_t *data);
int32 CFE_ES_WriteERLogCmd(const CFE_ES_WriteERLog_t *data);
int32 CFE_ES_ResetPRCountCmd(const CFE_ES_ResetPRCount_t *data);
int32 CFE_ES_SetMaxPRCountCmd(const CFE_ES_SetMaxPRCount_t *data);
int32 CFE_ES_DeleteCDSCmd(const CFE_ES_DeleteCDS_t *data);
int32 CFE_ES_StartPerfDataCmd(const CFE_ES_StartPerfData_t *data);
int32 CFE_ES_StopPerfDataCmd(const CFE_ES_StopPerfData_t *data);
int32 CFE_ES_SetPerfFilterMaskCmd(const CFE_ES_SetPerfFilterMask_t *data);
int32 CFE_ES_SetPerfTriggerMaskCmd(const CFE_ES_SetPerfTriggerMask_t *data);
int32 CFE_ES_SendMemPoolStatsCmd(const CFE_ES_SendMemPoolStats_t *data);
int32 CFE_ES_DumpCDSRegistryCmd(const CFE_ES_DumpCDSRegistry_t *data);

/*
** Message Handler Helper Functions
*/
bool CFE_ES_ValidateHandle(CFE_ES_MemHandle_t  Handle);
bool CFE_ES_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, CFE_MSG_Size_t ExpectedLength);
void CFE_ES_FileWriteByteCntErr(const char *Filename,uint32 Requested,uint32 Actual);

/*************************************************************************/

#endif /* _cfe_es_task_ */

/************************/
/*  End of File Comment */
/************************/

