/* Project: c:\src\rtds\alarmingprotocol\rtds-network-vorlage\network.rdp */
/* Diagram: C:\SRC\RTDS\AlarmingProtocol\rtds-network-vorlage\network.rdd */

#ifdef __cplusplus
extern "C" {
#endif

#include "RTDS_MACRO.h"

/* DEFINES FOR STATES */
#define waitForHello 1
#define initialised 2
#define waitForInit 3
#define waitForTopology 4
#define initializationPhase 5
#define RTDS_Idle 6

/* DEFINES FOR SIGNALS AND TIMERS */
#define broadcast 1
#define submitTopology 2
#define alert 3
#define nodeHello 4
#define hello 5

/* DEFINES FOR SEMAPHORES */

/* DEFINES FOR PROCESSES */
#define RTDS_process_Creator 1
#define RTDS_process_Admin 2
#define RTDS_process_RTDS_Env 3
#define RTDS_process_pTransportReceiver 4
#define RTDS_process_pTransportSender 5
#define RTDS_process_pApp 6

/* DEFINES FOR CONTROLPART PROCESSES */
#define RTDS_process_RTDS_Control_Messages 7
#define RTDS_process_RTDS_Control_NetworkTopology 8
#define RTDS_process_RTDS_Control_BlockMember 9

/* DEFINES FOR TESTCASE PROCESSES */

/* PROCESS PROTOTYPES */
RTDS_TASK_ENTRY_POINT_PROTO(Creator);
RTDS_TASK_ENTRY_POINT_PROTO(Admin);
RTDS_TASK_ENTRY_POINT_PROTO(RTDS_Env);
RTDS_TASK_ENTRY_POINT_PROTO(pTransportReceiver);
RTDS_TASK_ENTRY_POINT_PROTO(pTransportSender);
RTDS_TASK_ENTRY_POINT_PROTO(pApp);

/* TTCN control component PROTOTYPES */
RTDS_TASK_ENTRY_POINT_PROTO(RTDS_Control_Messages);
RTDS_TASK_ENTRY_POINT_PROTO(RTDS_Control_NetworkTopology);
RTDS_TASK_ENTRY_POINT_PROTO(RTDS_Control_BlockMember);

/* TTCN MTC component PROTOTYPES */

#ifdef __cplusplus
}
#endif

