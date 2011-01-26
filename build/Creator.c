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
#include "Creator.h"
#include "RTDS_messages.h"

#define RTDS_PROCESS_NUMBER RTDS_process_Creator
#define RTDS_PROCESS_NAME Creator

/*
** PROCESS Creator:
** ----------------
*/

RTDS_TASK_ENTRY_POINT(Creator)
  {
  short RTDS_transitionExecuted;
  int RTDS_savedSdlState = 0;

  BlockMemberPtr blockMember = new BlockMember();
  RTDS_QueueId app, transport_sender, transport_receiver;
  RTDS_MSG_DATA_DECL



  /* ***************************************************************** */
  /* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Process_begin.c begins */
  /* PragmaDev RTDS win32 integration                                  */

  int RTDS_sdlStatePrev = 0;
  RTDS_TimerState * RTDS_timer;
  RTDS_TimerState * RTDS_prevTimer;
  RTDS_SdlInstanceId * RTDS_senderId = NULL;

  RTDS_currentContext->readSaveQueue = NULL;
  RTDS_currentContext->writeSaveQueue = NULL;

  /* Synchronize processes at startup time */
  RTDS_START_SYNCHRO_WAIT;

  /* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Process_begin.c ends */
  /* *************************************************************** */




  /* Initial transition */
  RTDS_PROCESS_CREATE("app", RTDS_process_pApp, pApp, RTDS_DEFAULT_PROCESS_PRIORITY);
  app = OFFSPRING;
  blockMember->addBlockMember("app",app);
  RTDS_PROCESS_CREATE("transport_sender", RTDS_process_pTransportSender, pTransportSender, RTDS_DEFAULT_PROCESS_PRIORITY);
  transport_sender = OFFSPRING;
  blockMember->addBlockMember("transport_sender",OFFSPRING);
  RTDS_PROCESS_CREATE("transport_receiver", RTDS_process_pTransportReceiver, pTransportReceiver, RTDS_DEFAULT_PROCESS_PRIORITY);
  transport_receiver = OFFSPRING;
  blockMember->addBlockMember("transport_receiver",OFFSPRING);
  RTDS_MSG_SEND_hello_TO_ID(app,   blockMember);
  RTDS_MSG_SEND_hello_TO_ID(transport_sender,   blockMember);
  RTDS_MSG_SEND_hello_TO_ID(transport_receiver,   blockMember);
  RTDS_MSG_SEND_nodeHello_TO_NAME("Admin", RTDS_process_Admin,   transport_receiver);
  RTDS_PROCESS_KILL;

  /* ****************************************************************** */
  /* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Proc_loopStart.c begins */
  /* PragmaDev RTDS win32 integration                                   */

  /* Event loop */
  for ( ; ; )
    {

    /* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Proc_loopStart.c ends */
    /* *************************************************************** */

    /* *************************************************************** */
    /* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Proc_middle.c begins */
    /* PragmaDev RTDS win32 integration                                */

    /* Check saved messages */
    if ( RTDS_currentContext->readSaveQueue != NULL )
      {
      RTDS_currentContext->currentMessage = RTDS_currentContext->readSaveQueue;
      RTDS_currentContext->readSaveQueue = RTDS_currentContext->readSaveQueue->next;
      }
    /* If no saved messages left, get message from process's message queue */
    else
      {
      RTDS_currentContext->currentMessage = ( RTDS_MessageHeader * )RTDS_MALLOC( sizeof( RTDS_MessageHeader ) );
      RTDS_MSG_QUEUE_READ( RTDS_currentContext->currentMessage );
      RTDS_currentContext->currentMessage->next = NULL;
      }

    /* Discard cancelled timers */
    if ( RTDS_currentContext->currentMessage->timerUniqueId != 0 )
      {
      RTDS_prevTimer = NULL;
      for ( RTDS_timer = RTDS_currentContext->timerList ;RTDS_timer != NULL; RTDS_timer = RTDS_timer->next )
        {
        /* If timer found and cancelled */
        if ( RTDS_timer->timerUniqueId == RTDS_currentContext->currentMessage->timerUniqueId )
          {
          if ( RTDS_timer->state == RTDS_TIMER_CANCELLED )
            {
            /* Discard current message */
            RTDS_FREE( RTDS_currentContext->currentMessage );
            RTDS_currentContext->currentMessage = NULL;
            /* Remove it from list of timers */
            if ( RTDS_prevTimer == NULL )
              {
              RTDS_currentContext->timerList = RTDS_currentContext->timerList->next;
              }
            else
              {
              RTDS_prevTimer->next = RTDS_timer->next;
              }
            CloseHandle(RTDS_timer->watchDogId);
            RTDS_FREE( RTDS_timer );
            }
          break;
          }
        RTDS_prevTimer = RTDS_timer;
        }
      /* If the timer could not be found in the list. Consider it as cancelled and discard current message */
      if ( RTDS_timer == NULL )
        {
        RTDS_FREE( RTDS_currentContext->currentMessage );
        RTDS_currentContext->currentMessage = NULL;
        }
      }

    /* Let's process the message ! */
    if ( RTDS_currentContext->currentMessage != NULL )
      {
      RTDS_SIMULATOR_TRACE( RTDS_messageReceived, RTDS_currentContext->currentMessage, SELF, RTDS_currentContext );
      RTDS_senderId = RTDS_currentContext->currentMessage->sender;
      RTDS_sdlStatePrev = RTDS_currentContext->sdlState;
      /* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Proc_middle.c ends */
      /* ************************************************************* */

      /* Double switch state / signal */
      RTDS_transitionExecuted = 1;
      switch(RTDS_currentContext->sdlState)
        {
        default:
          RTDS_transitionExecuted = 0;
          break;
        } /* End of switch(RTDS_currentContext->sdlState) */
      /* ************************************************************ */
      /* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Proc_end.c begins */
      /* PragmaDev RTDS win32 integration                             */
      RTDS_TransitionCleanUp( RTDS_currentContext, RTDS_sdlStatePrev );
      } /* End of if ( RTDS_currentContext->currentMessage != NULL ) to execute transition */
    } /* End of for ( ; ; ) */

  /* $(RTDS_HOME)/share/ccg/windows/bricks/RTDS_Proc_end.c ends */
  /* ********************************************************** */
  }
