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
#include "Admin.h"
#include "RTDS_messages.h"

#define RTDS_PROCESS_NUMBER RTDS_process_Admin
#define RTDS_PROCESS_NAME Admin

/*
** PROCESS Admin:
** --------------
*/

RTDS_TASK_ENTRY_POINT(Admin)
  {
  short RTDS_transitionExecuted;
  int RTDS_savedSdlState = 0;

  int nodeCount = 3;
  double connectivity[3][3] = {
  0.0 , 1.0 , 1.0 ,
  1.0 , 0.0 , 1.0 ,
  1.0 , 1.0 , 0.0 };
  RTDS_QueueId nodeTE, receiver;
  NetworkTopologyPtr topology;
  NetworkTopology nt;
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
  int initialCounter = 0;
  RTDS_SDL_STATE_SET(initializationPhase);

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
        /* Transitions for state initializationPhase */
        case initializationPhase:
          switch(RTDS_currentContext->currentMessage->messageNumber)
            {
            /* Transition for state initializationPhase - message nodeHello */
            case nodeHello:
              RTDS_MSG_RECEIVE_nodeHello(          nodeTE);
              nt.setIdReferenceMapping(initialCounter, nodeTE);
              initialCounter++;
              if ( !(initialCounter < nodeCount) )
                {
                initialCounter = 0;
                submitLoop:
                if ( !(initialCounter < nodeCount) )
                  {
                  RTDS_PROCESS_KILL;
                  break;
                  }
                else if ( initialCounter < nodeCount )
                  {
                  receiver = nt.getReference(initialCounter);
                  topology = new NetworkTopology();
                  topology->setMyId(initialCounter);
                  topology->setMyReceiverReference(receiver);
                  for (int i = 0; i < nodeCount; i++) {
                  if (connectivity[initialCounter][i] != 0.0) {
                  topology->setConnectivity(i,connectivity[initialCounter][i]);
                  topology->setIdReferenceMapping(i, nt.getReference(i));
                  }
                }
              RTDS_MSG_SEND_submitTopology_TO_ID(receiver,               topology);
              initialCounter++;
              goto submitLoop;
              }
            }
          else if ( initialCounter < nodeCount )
            {
            RTDS_SDL_STATE_SET(initializationPhase);
            break;
            }
        default:
          RTDS_transitionExecuted = 0;
          break;
        } /* End of switch on message */
      break;
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
