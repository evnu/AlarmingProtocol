/* PragmaDev RTDS win32 integration */

/* Include unistd when using a unix based system or cygwin */
#ifdef __CYGWIN__
#include <unistd.h>
#else
#include <stdlib.h>
#endif

#include "RTDS_MACRO.h"

#if defined( RTDS_FORMAT_TRACE )
#include "RTDS_FormatTrace.h"
#endif


#include "RTDS_gen.h"
#include "network.h"
/* ***************************************************************** */
/* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Startup_begin.c begins */
/* PragmaDev RTDS win32 integration                                  */
#ifdef RTDS_SOCKET_PORT
#include "RTDS_TCP_Client.h"
#endif

#ifdef RTDS_CMD_INTERPRETER
#include "RTDS_CmdInterpreter.h"
#endif

#ifdef RTDS_FORMAT_TRACE
#include "RTDS_FormatTrace.h"
#endif

#ifdef RTDS_SOCKET_PORT
int globalClientSocketId = 0;
#endif

#ifdef __cplusplus
extern "C" {
DWORD RTDS_Start( void * );
void RTDS_StopProgram( void );
}
#endif

/* Pointers to global list of information on processes and semaphores */
RTDS_GlobalProcessInfo * RTDS_globalProcessInfo = NULL;
RTDS_GlobalSemaphoreInfo * RTDS_globalSemaphoreInfo = NULL;

RTDS_COVERAGE_DECL;
RTDS_SYS_TIME_DECL;
RTDS_START_SYNCHRO_DECL;
RTDS_CRITICAL_SECTION_DECL;
RTDS_SOCKET_ACCESS_DECL;
RTDS_CRITICAL_TRACE_SECTION_DECL;
RTDS_DTRACE_ACKNOWLEDGE_DECL;

DWORD RTDS_Start( void * dummy )
  {
  RTDS_GlobalProcessInfo * RTDS_currentContext;

  RTDS_SYS_TIME_INIT;
  RTDS_COVERAGE_INIT;

  #ifdef RTDS_SIMULATOR
  RTDS_globalTraceEntry.event = RTDS_systemError;
  RTDS_globalTraceEntry.currentContext = NULL;
  #endif

  #if defined( RTDS_BACK_TRACE_MAX_EVENT_NUM ) && defined( RTDS_SIMULATOR )
  RTDS_TraceInit();
  #endif

  #if defined( RTDS_SIMULATOR )

  /* Set up the message unique id pool */
  RTDS_globalMessageUniqueIdPool = RTDS_MALLOC( RTDS_MESSAGE_UNIQUE_ID_POOL_SIZE );
    {
    int RTDS_i;
    unsigned char * RTDS_index;

    RTDS_index = ( unsigned char * )RTDS_globalMessageUniqueIdPool;
    for( RTDS_i = 0; RTDS_i < RTDS_MESSAGE_UNIQUE_ID_POOL_SIZE; RTDS_i++ )
      {
      *RTDS_index = 0x00;
      RTDS_index++;
      }
    }
  #endif

  /* Set up a dummy process context needed to create tasks */
  RTDS_currentContext = ( RTDS_GlobalProcessInfo * )RTDS_MALLOC( sizeof( RTDS_GlobalProcessInfo ) );
  if ( RTDS_currentContext == NULL )
    {
    RTDS_SYSTEM_ERROR( RTDS_ERROR_STARTUP_CONTEXT_MALLOC );
    }
  RTDS_currentContext->sdlProcessNumber = 0;
  RTDS_currentContext->myRtosTaskId = ( RTDS_RtosTaskId )0;
  RTDS_currentContext->sdlState = 0;
  RTDS_currentContext->next = NULL;
  RTDS_currentContext->parentSdlInstanceId = NULL;
  RTDS_currentContext->offspringSdlInstanceId = NULL;
  RTDS_currentContext->currentMessage = NULL;
  RTDS_currentContext->timerList = NULL;
  RTDS_currentContext->mySdlInstanceId = NULL;

  RTDS_SOCKET_ACCESS_INIT;
  RTDS_CRITICAL_SECTION_INIT;
  RTDS_START_SYNCHRO_INIT;
  RTDS_CRITICAL_TRACE_SECTION_INIT;

  /* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Startup_begin.c ends */
  /* *************************************************************** */


  /* SEMAPHORE CREATIONS */

  /* PROCESS CREATIONS */
  RTDS_STARTUP_PROCESS_CREATE("Creator", RTDS_process_Creator, Creator, RTDS_DEFAULT_PROCESS_PRIORITY);
  RTDS_STARTUP_PROCESS_CREATE("Creator", RTDS_process_Creator, Creator, RTDS_DEFAULT_PROCESS_PRIORITY);
  RTDS_STARTUP_PROCESS_CREATE("Creator", RTDS_process_Creator, Creator, RTDS_DEFAULT_PROCESS_PRIORITY);
  RTDS_STARTUP_PROCESS_CREATE("Admin", RTDS_process_Admin, Admin, RTDS_DEFAULT_PROCESS_PRIORITY);
  RTDS_STARTUP_PROCESS_CREATE("RTDS_Env", RTDS_process_RTDS_Env, RTDS_Env, RTDS_DEFAULT_PROCESS_PRIORITY);
  /* *************************************************************** */
  /* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Startup_end.c begins */
  /* PragmaDev RTDS win32 integration                                */

  RTDS_SIMULATOR_TRACE( RTDS_initDone, NULL, NULL, RTDS_currentContext );

  /* Let all the created process run now */
  RTDS_START_SYNCHRO_GO;

  return 0;
  }


/* **************************************************************** *
*    RTDS_StopProgram
* **************************************************************** *
*    Function for stopping program using breakpoint and
*    command interpreter Client
* **************************************************************** */
void RTDS_StopProgram( void )
  {
  }

/* **************************************************************** *
*    main
* **************************************************************** *
*    Main entry
* **************************************************************** */
#ifndef RTDS_MAIN
int main( void )
#else
int RTDS_MAIN( void )
#endif
  {
  int socketEnable = 0;

  #ifdef RTDS_CMD_INTERPRETER
  enum RTDS_CmdType              commandId;
  RTDS_UnionCommandParam       * pCommandParam;
  RTDS_CmdSendMsg2ProcNumber   * paramSendMsg2pNumber = NULL;    /* Variable for sending SDL message*/
  RTDS_CmdSendMsg2ProcId       * paramSendMsg2pId = NULL;
  RTDS_SdlInstanceId           * receiverSdlInstanceId;
  RTDS_SdlInstanceId           * senderSdlInstanceId;
  RTDS_GlobalProcessInfo       * processInfoSender;
  RTDS_GlobalProcessInfo       * processInfoReceiver;
  RTDS_CmdForceTimer           * paramForceTimer = NULL;    /* Variable for forcing timer */
  RTDS_CmdTraceConfig          * paramTraceConf = NULL;    /* Variable for configuring trace */
  #endif


  #if RTDS_SOCKET_PORT
  char * currentData;       /* ptr on data to read from socket */
  int currentSize;          /* Data size to read from socket */
  /* win32 variables */
  DWORD dwThreadId = 1;
  HANDLE hThread;

  RTDS_SOCKET_ID_TYPE clientSocketId=0;
  RTDS_RtosTaskId commandInterThreadId;

  clientSocketId = RTDS_InitSocket( GET_HOSTNAME_STRING( RTDS_SOCKET_IP_ADDRESS ), RTDS_SOCKET_PORT );
  if( !(clientSocketId == RTDS_ERROR || clientSocketId == 0 ) )
    {
    socketEnable = 1;
    /* global variable for sending data */
    globalClientSocketId = clientSocketId;
    }
  #endif


  #if defined( RTDS_SIMULATOR ) && defined( RTDS_CMD_INTERPRETER ) && defined( RTDS_SOCKET_PORT )
  /* Init acknowledge semaphore */
  RTDS_DTRACE_ACKNOWLEDGE_INIT;

  /* RTDS_Start is started in a thread becauseit handles the socket communication with the SDL-RT debugger */
  hThread = CreateThread(
  NULL,                                                                       /* default security attributes */
  0,                                                                          /* use default stack size */
  ( RTDS_THREAD_START_FUNC )RTDS_Start,                                       /* thread function */
  ( HANDLE )NULL,                                                             /* argument to thread function */
  0,                                                                          /* use default creation flags */
  &dwThreadId );                                                              /* returns the thread identifier */

  /* Check the return value and set priority */
  if ( hThread == NULL )
    {
    RTDS_SYSTEM_ERROR( RTDS_ERROR_CREATE_THREAD );
    }
  else
    {
    if( SetThreadPriority( hThread, RTDS_DEFAULT_TCP_CLIENT_PRIORITY ) == 0 )
      {
      RTDS_SYSTEM_ERROR( RTDS_ERROR_SET_THREAD_PRIORITY );
      }
    CloseHandle( hThread );         /* Close handle after creating thread */
    }

  commandInterThreadId = dwThreadId;


  if ( socketEnable == 1 )
    {
    while( RTDS_ReadSocket( clientSocketId, &currentData, &currentSize, RTDS_CMD_DELIMITERCHAR, RTDS_ESCAPECHAR )!= -1 )
      {
      if ( RTDS_InterpretCommand( currentData, &commandId, &pCommandParam ) != RTDS_ERROR )
        {
        /* Process command */
        switch( commandId )
          {
          case ACK_CMDID:
          RTDS_DTRACE_ACKNOWLEDGE_RECEIVED;
          break;

          case STOP_CMDID:
          RTDS_StopProgram();
          break;

          case MSG2PROCID:
          /* pCommandParam is a valid RTDS_CmdSendMsg2ProcId struct */
          paramSendMsg2pId = ( RTDS_CmdSendMsg2ProcId * )pCommandParam;
          /* Check Receiver exists */
          processInfoReceiver = RTDS_GetProcessInfoFromSdlInstanceId( paramSendMsg2pId->receiver );
          /* Check Sender exists */
          processInfoSender = RTDS_GetProcessInfoFromSdlInstanceId( paramSendMsg2pId->sender );

          /* If receiver or sender are not known */
          if (processInfoReceiver == NULL)
            {
            break;
            }
          if (processInfoSender == NULL)
          senderSdlInstanceId = NULL;
          else
          senderSdlInstanceId = processInfoSender->mySdlInstanceId;

          receiverSdlInstanceId = processInfoReceiver->mySdlInstanceId;

          RTDS_SimulatorMsgQueueSend
          (
          paramSendMsg2pId->messageNumber,
          paramSendMsg2pId->dataLength,
          paramSendMsg2pId->pData,
          receiverSdlInstanceId,
          senderSdlInstanceId,
          processInfoSender
          );
          break;

          case MSG2PROCNUM:
          /* pCommandParam is a valid RTDS_CmdSendMsg2ProcNumber struct */
          paramSendMsg2pNumber = ( RTDS_CmdSendMsg2ProcNumber * )pCommandParam;
          /* Get Sender QueueId*/
          processInfoSender = RTDS_GetProcessInfoFromProcessNum( paramSendMsg2pNumber->sender );

          RTDS_SimulatorMsgQueueSend
          (
          paramSendMsg2pNumber->messageNumber,
          paramSendMsg2pNumber->dataLength,
          paramSendMsg2pNumber->pData,
          RTDS_GetProcessQueueId( paramSendMsg2pNumber->receiver ),
          RTDS_GetProcessQueueId( paramSendMsg2pNumber->sender ),
          processInfoSender
          );
          break;

          case TIMER_CMDID:
          /* pCommandParam is a valid RTDS_CmdForceTimer struct */
          paramForceTimer = ( RTDS_CmdForceTimer * )pCommandParam;
          RTDS_ForceTimer( paramForceTimer->pId, paramForceTimer->timerNum );
          break;

          case SYSCLOCK_CMDID:
          RTDS_SetSystemTime( ( long )pCommandParam );
          break;

          case TRACE_CMDID:
          #ifdef RTDS_FORMAT_TRACE
          paramTraceConf = ( RTDS_CmdTraceConfig * )pCommandParam;
          RTDS_ConfigureTrace( paramTraceConf );
          #endif
          break;

          default:
          break;
          }
        }
      RTDS_FREE( currentData );
      }
    }
  #else
  /* Call Start function (that will run SDL Task and run program) */
  RTDS_Start( NULL );
  #endif

  #ifndef RTDS_MAIN
  /* To avoid the main thread to exit */
  while ( 1 )
    {
    Sleep( 3600 );
    }
  #endif

  return 0;
  }

/* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Startup_end.c ends */
/* ************************************************************* */

