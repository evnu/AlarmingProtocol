#ifndef _RTDS_MESSAGES_H_
#define _RTDS_MESSAGES_H_

#ifdef __cplusplus
extern "C" {
#endif



/* DATA TYPES FOR MESSAGES */
#ifndef RTDS_broadcast_DATA_DEFINED
typedef struct RTDS_broadcast_data
{
	int	param1;
} RTDS_broadcast_data;
#define RTDS_broadcast_DATA_DEFINED
#endif

#ifndef RTDS_submitTopology_DATA_DEFINED
typedef struct RTDS_submitTopology_data
{
	NetworkTopologyPtr	param1;
} RTDS_submitTopology_data;
#define RTDS_submitTopology_DATA_DEFINED
#endif

#ifndef RTDS_nodeHello_DATA_DEFINED
typedef struct RTDS_nodeHello_data
{
	RTDS_QueueId	param1;
} RTDS_nodeHello_data;
#define RTDS_nodeHello_DATA_DEFINED
#endif

#ifndef RTDS_hello_DATA_DEFINED
typedef struct RTDS_hello_data
{
	BlockMemberPtr	param1;
} RTDS_hello_data;
#define RTDS_hello_DATA_DEFINED
#endif

#ifndef RTDS_alert_DATA_DEFINED
typedef struct RTDS_alert_data
{
	int	param1;
} RTDS_alert_data;
#define RTDS_alert_DATA_DEFINED
#endif


/* MACRO FOR DECLARATIONS FOR MESSAGE SEND/RECEIVE */

#ifndef RTDS_MSG_DATA_DECL
#define RTDS_MSG_DATA_DECL unsigned char * RTDS_msgData;
#endif  /* RTDS_MSG_DATA_DECL defined */


/* MACRO FOR RECEPTION OF MESSAGE broadcast */

#ifndef RTDS_MSG_RECEIVE_broadcast
#define RTDS_MSG_RECEIVE_broadcast(RTDS_PARAM1) \
	{ \
	if ( RTDS_currentContext->currentMessage->pData != NULL ) \
		{ \
		RTDS_PARAM1 = ((RTDS_broadcast_data*)(RTDS_currentContext->currentMessage->pData))->param1; \
		} \
	else \
		{ \
		RTDS_MSG_INPUT_ERROR; \
		} \
	}
#endif /* RTDS_MSG_RECEIVE_broadcast defined */


/* MACRO FOR SENDING MESSAGE broadcast TO A PROCESS NAME */

#ifndef RTDS_MSG_SEND_broadcast_TO_NAME
#define RTDS_MSG_SEND_broadcast_TO_NAME(RECEIVER, RECEIVER_NUMBER, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_broadcast_data)); \
	((RTDS_broadcast_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_NAME(broadcast, sizeof(RTDS_broadcast_data), RTDS_msgData, RECEIVER, RECEIVER_NUMBER); \
	}
#endif /* RTDS_MSG_SEND_broadcast_TO_NAME defined */


/* MACRO FOR SENDING MESSAGE broadcast TO A PROCESS ID */

#ifndef RTDS_MSG_SEND_broadcast_TO_ID
#define RTDS_MSG_SEND_broadcast_TO_ID(RECEIVER, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_broadcast_data)); \
	((RTDS_broadcast_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_ID(broadcast, sizeof(RTDS_broadcast_data), RTDS_msgData, RECEIVER); \
	}
#endif /* RTDS_MSG_SEND_broadcast_TO_ID defined */


/* MACROS FOR SENDING MESSAGE broadcast TO ENVIRONMENT */

#ifndef RTDS_MSG_SEND_broadcast_TO_ENV
#define RTDS_MSG_SEND_broadcast_TO_ENV(RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_broadcast_data)); \
	((RTDS_broadcast_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_ENV(broadcast, sizeof(RTDS_broadcast_data), RTDS_msgData); \
	}
#endif /* RTDS_MSG_SEND_broadcast_TO_ENV defined */


#ifndef RTDS_MSG_SEND_broadcast_TO_ENV_W_MACRO
#define RTDS_MSG_SEND_broadcast_TO_ENV_W_MACRO(MACRO_NAME, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_broadcast_data)); \
	((RTDS_broadcast_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	MACRO_NAME(broadcast, sizeof(RTDS_broadcast_data), RTDS_msgData); \
	}
#endif /* RTDS_MSG_SEND_broadcast_TO_ENV defined */


/* MACRO FOR RECEPTION OF MESSAGE submitTopology */

#ifndef RTDS_MSG_RECEIVE_submitTopology
#define RTDS_MSG_RECEIVE_submitTopology(RTDS_PARAM1) \
	{ \
	if ( RTDS_currentContext->currentMessage->pData != NULL ) \
		{ \
		RTDS_PARAM1 = ((RTDS_submitTopology_data*)(RTDS_currentContext->currentMessage->pData))->param1; \
		} \
	else \
		{ \
		RTDS_MSG_INPUT_ERROR; \
		} \
	}
#endif /* RTDS_MSG_RECEIVE_submitTopology defined */


/* MACRO FOR SENDING MESSAGE submitTopology TO A PROCESS NAME */

#ifndef RTDS_MSG_SEND_submitTopology_TO_NAME
#define RTDS_MSG_SEND_submitTopology_TO_NAME(RECEIVER, RECEIVER_NUMBER, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_submitTopology_data)); \
	((RTDS_submitTopology_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_NAME(submitTopology, sizeof(RTDS_submitTopology_data), RTDS_msgData, RECEIVER, RECEIVER_NUMBER); \
	}
#endif /* RTDS_MSG_SEND_submitTopology_TO_NAME defined */


/* MACRO FOR SENDING MESSAGE submitTopology TO A PROCESS ID */

#ifndef RTDS_MSG_SEND_submitTopology_TO_ID
#define RTDS_MSG_SEND_submitTopology_TO_ID(RECEIVER, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_submitTopology_data)); \
	((RTDS_submitTopology_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_ID(submitTopology, sizeof(RTDS_submitTopology_data), RTDS_msgData, RECEIVER); \
	}
#endif /* RTDS_MSG_SEND_submitTopology_TO_ID defined */


/* MACROS FOR SENDING MESSAGE submitTopology TO ENVIRONMENT */

#ifndef RTDS_MSG_SEND_submitTopology_TO_ENV
#define RTDS_MSG_SEND_submitTopology_TO_ENV(RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_submitTopology_data)); \
	((RTDS_submitTopology_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_ENV(submitTopology, sizeof(RTDS_submitTopology_data), RTDS_msgData); \
	}
#endif /* RTDS_MSG_SEND_submitTopology_TO_ENV defined */


#ifndef RTDS_MSG_SEND_submitTopology_TO_ENV_W_MACRO
#define RTDS_MSG_SEND_submitTopology_TO_ENV_W_MACRO(MACRO_NAME, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_submitTopology_data)); \
	((RTDS_submitTopology_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	MACRO_NAME(submitTopology, sizeof(RTDS_submitTopology_data), RTDS_msgData); \
	}
#endif /* RTDS_MSG_SEND_submitTopology_TO_ENV defined */


/* MACRO FOR RECEPTION OF MESSAGE alert */

#ifndef RTDS_MSG_RECEIVE_alert
#define RTDS_MSG_RECEIVE_alert(RTDS_PARAM1) \
	{ \
	if ( RTDS_currentContext->currentMessage->pData != NULL ) \
		{ \
		RTDS_PARAM1 = ((RTDS_alert_data*)(RTDS_currentContext->currentMessage->pData))->param1; \
		} \
	else \
		{ \
		RTDS_MSG_INPUT_ERROR; \
		} \
	}
#endif /* RTDS_MSG_RECEIVE_alert defined */


/* MACRO FOR SENDING MESSAGE alert TO A PROCESS NAME */

#ifndef RTDS_MSG_SEND_alert_TO_NAME
#define RTDS_MSG_SEND_alert_TO_NAME(RECEIVER, RECEIVER_NUMBER, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_alert_data)); \
	((RTDS_alert_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_NAME(alert, sizeof(RTDS_alert_data), RTDS_msgData, RECEIVER, RECEIVER_NUMBER); \
	}
#endif /* RTDS_MSG_SEND_alert_TO_NAME defined */


/* MACRO FOR SENDING MESSAGE alert TO A PROCESS ID */

#ifndef RTDS_MSG_SEND_alert_TO_ID
#define RTDS_MSG_SEND_alert_TO_ID(RECEIVER, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_alert_data)); \
	((RTDS_alert_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_ID(alert, sizeof(RTDS_alert_data), RTDS_msgData, RECEIVER); \
	}
#endif /* RTDS_MSG_SEND_alert_TO_ID defined */


/* MACROS FOR SENDING MESSAGE alert TO ENVIRONMENT */

#ifndef RTDS_MSG_SEND_alert_TO_ENV
#define RTDS_MSG_SEND_alert_TO_ENV(RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_alert_data)); \
	((RTDS_alert_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_ENV(alert, sizeof(RTDS_alert_data), RTDS_msgData); \
	}
#endif /* RTDS_MSG_SEND_alert_TO_ENV defined */


#ifndef RTDS_MSG_SEND_alert_TO_ENV_W_MACRO
#define RTDS_MSG_SEND_alert_TO_ENV_W_MACRO(MACRO_NAME, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_alert_data)); \
	((RTDS_alert_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	MACRO_NAME(alert, sizeof(RTDS_alert_data), RTDS_msgData); \
	}
#endif /* RTDS_MSG_SEND_alert_TO_ENV defined */


/* MACRO FOR RECEPTION OF MESSAGE nodeHello */

#ifndef RTDS_MSG_RECEIVE_nodeHello
#define RTDS_MSG_RECEIVE_nodeHello(RTDS_PARAM1) \
	{ \
	if ( RTDS_currentContext->currentMessage->pData != NULL ) \
		{ \
		RTDS_PARAM1 = ((RTDS_nodeHello_data*)(RTDS_currentContext->currentMessage->pData))->param1; \
		} \
	else \
		{ \
		RTDS_MSG_INPUT_ERROR; \
		} \
	}
#endif /* RTDS_MSG_RECEIVE_nodeHello defined */


/* MACRO FOR SENDING MESSAGE nodeHello TO A PROCESS NAME */

#ifndef RTDS_MSG_SEND_nodeHello_TO_NAME
#define RTDS_MSG_SEND_nodeHello_TO_NAME(RECEIVER, RECEIVER_NUMBER, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_nodeHello_data)); \
	((RTDS_nodeHello_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_NAME(nodeHello, sizeof(RTDS_nodeHello_data), RTDS_msgData, RECEIVER, RECEIVER_NUMBER); \
	}
#endif /* RTDS_MSG_SEND_nodeHello_TO_NAME defined */


/* MACRO FOR SENDING MESSAGE nodeHello TO A PROCESS ID */

#ifndef RTDS_MSG_SEND_nodeHello_TO_ID
#define RTDS_MSG_SEND_nodeHello_TO_ID(RECEIVER, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_nodeHello_data)); \
	((RTDS_nodeHello_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_ID(nodeHello, sizeof(RTDS_nodeHello_data), RTDS_msgData, RECEIVER); \
	}
#endif /* RTDS_MSG_SEND_nodeHello_TO_ID defined */


/* MACROS FOR SENDING MESSAGE nodeHello TO ENVIRONMENT */

#ifndef RTDS_MSG_SEND_nodeHello_TO_ENV
#define RTDS_MSG_SEND_nodeHello_TO_ENV(RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_nodeHello_data)); \
	((RTDS_nodeHello_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_ENV(nodeHello, sizeof(RTDS_nodeHello_data), RTDS_msgData); \
	}
#endif /* RTDS_MSG_SEND_nodeHello_TO_ENV defined */


#ifndef RTDS_MSG_SEND_nodeHello_TO_ENV_W_MACRO
#define RTDS_MSG_SEND_nodeHello_TO_ENV_W_MACRO(MACRO_NAME, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_nodeHello_data)); \
	((RTDS_nodeHello_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	MACRO_NAME(nodeHello, sizeof(RTDS_nodeHello_data), RTDS_msgData); \
	}
#endif /* RTDS_MSG_SEND_nodeHello_TO_ENV defined */


/* MACRO FOR RECEPTION OF MESSAGE hello */

#ifndef RTDS_MSG_RECEIVE_hello
#define RTDS_MSG_RECEIVE_hello(RTDS_PARAM1) \
	{ \
	if ( RTDS_currentContext->currentMessage->pData != NULL ) \
		{ \
		RTDS_PARAM1 = ((RTDS_hello_data*)(RTDS_currentContext->currentMessage->pData))->param1; \
		} \
	else \
		{ \
		RTDS_MSG_INPUT_ERROR; \
		} \
	}
#endif /* RTDS_MSG_RECEIVE_hello defined */


/* MACRO FOR SENDING MESSAGE hello TO A PROCESS NAME */

#ifndef RTDS_MSG_SEND_hello_TO_NAME
#define RTDS_MSG_SEND_hello_TO_NAME(RECEIVER, RECEIVER_NUMBER, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_hello_data)); \
	((RTDS_hello_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_NAME(hello, sizeof(RTDS_hello_data), RTDS_msgData, RECEIVER, RECEIVER_NUMBER); \
	}
#endif /* RTDS_MSG_SEND_hello_TO_NAME defined */


/* MACRO FOR SENDING MESSAGE hello TO A PROCESS ID */

#ifndef RTDS_MSG_SEND_hello_TO_ID
#define RTDS_MSG_SEND_hello_TO_ID(RECEIVER, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_hello_data)); \
	((RTDS_hello_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_ID(hello, sizeof(RTDS_hello_data), RTDS_msgData, RECEIVER); \
	}
#endif /* RTDS_MSG_SEND_hello_TO_ID defined */


/* MACROS FOR SENDING MESSAGE hello TO ENVIRONMENT */

#ifndef RTDS_MSG_SEND_hello_TO_ENV
#define RTDS_MSG_SEND_hello_TO_ENV(RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_hello_data)); \
	((RTDS_hello_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	RTDS_MSG_QUEUE_SEND_TO_ENV(hello, sizeof(RTDS_hello_data), RTDS_msgData); \
	}
#endif /* RTDS_MSG_SEND_hello_TO_ENV defined */


#ifndef RTDS_MSG_SEND_hello_TO_ENV_W_MACRO
#define RTDS_MSG_SEND_hello_TO_ENV_W_MACRO(MACRO_NAME, RTDS_PARAM1) \
	{ \
	RTDS_msgData = (unsigned char *)RTDS_MALLOC(sizeof(RTDS_hello_data)); \
	((RTDS_hello_data*)RTDS_msgData)->param1 = RTDS_PARAM1; \
	MACRO_NAME(hello, sizeof(RTDS_hello_data), RTDS_msgData); \
	}
#endif /* RTDS_MSG_SEND_hello_TO_ENV defined */


#ifdef __cplusplus
}
#endif

#endif /* defined(_RTDS_MESSAGES_H_) */

