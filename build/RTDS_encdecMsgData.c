#include "RTDS_gen.h"
#include "network.h"
#include "RTDS_messages.h"
#include <string.h>
#include <stdio.h>
#ifdef softOSE
/* ctype is broken with OSE/SoftKernel => define our own macros */
#define isxdigit(C) (( (C) >= '0' && (C) <= '9') || ( (C) >= 'A' && (C) <= 'F' ) || ( (C) >= 'a' && (C) <= 'f' ))
#define isalnum(C)  (( (C) >= '0' && (C) <= '9') || ( (C) >= 'A' && (C) <= 'Z' ) || ( (C) >= 'a' && (C) <= 'z' ))
#define isspace(C)  ( (C) == ' ' || (C) == '\t' || (C) == '\n' || (C) == '\r')
#else
#include <ctype.h>
#endif

#include "RTDS_MACRO.h"

#ifndef RTDS_PARAM_CODEC_CHUNK_SIZE
#define RTDS_PARAM_CODEC_CHUNK_SIZE 1024
#endif

#define RTDS_MEM_RESET(PTR, SIZE) { unsigned char * p = (unsigned char*)(PTR); unsigned i = 0; while ( i < SIZE) p[i++] = 0; }
/*
 * FUNCTION RTDS_extendNCat:
 * -------------------------
 * Adds a given string to the buffer containing the text for a message data, extending it if needed
 * Parameters:
 * - buffer: a pointer on the buffer containing the data
 * - allocatedChunks: a pointer on the number of already allocated chunks in buffer
 * - string2Add: the string to add to the buffer
 */

static void RTDS_extendNCat(char ** buffer, long * allocatedChunks, char * string2Add)
  {
  int length;
  char * oldBuffer = *buffer;
  int i;
  
  /* If buffer is null, no memory left => over */
  if ( *buffer == NULL ) return;
  /* If buffer is not big enough */
  length = strlen(*buffer) + strlen(string2Add);
  if ( *allocatedChunks * RTDS_PARAM_CODEC_CHUNK_SIZE < length )
    {
    /* Reallocate it */
    *allocatedChunks = 1 + length / RTDS_PARAM_CODEC_CHUNK_SIZE;
    *buffer = (char*)RTDS_MALLOC(RTDS_PARAM_CODEC_CHUNK_SIZE * (*allocatedChunks) + 1);
    if ( *buffer == NULL )
      {
      RTDS_FREE(oldBuffer);
      return;
      }
    for ( i = 0; oldBuffer[i] != '\0'; i++ ) (*buffer)[i] = oldBuffer[i];
    (*buffer)[i] = '\0';
    RTDS_FREE(oldBuffer);
    }
  /* Add string */
  strcat(*buffer, string2Add);
  }

/* FUNCTION PROTOTYPES */
static void RTDS_structDataToString_RTDS_QCB(char ** buffer, long * allocatedChunks, struct RTDS_QCB * ptr, int level, int followPointers);
static char * RTDS_structStringToData_RTDS_QCB(struct RTDS_QCB * dataPointer, char * dataString);
static void RTDS_structDataToString_RTDS_submitTopology_data(char ** buffer, long * allocatedChunks, struct RTDS_submitTopology_data * ptr, int level, int followPointers);
static char * RTDS_structStringToData_RTDS_submitTopology_data(struct RTDS_submitTopology_data * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_submitTopology_data(char ** buffer, long * allocatedChunks, RTDS_submitTopology_data * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_submitTopology_data(RTDS_submitTopology_data * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_SemaphoreId(char ** buffer, long * allocatedChunks, RTDS_SemaphoreId * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_SemaphoreId(RTDS_SemaphoreId * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_SemaphoreInfo(char ** buffer, long * allocatedChunks, RTDS_SemaphoreInfo * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_SemaphoreInfo(RTDS_SemaphoreInfo * dataPointer, char * dataString);
static void RTDS_structDataToString_RTDS_SdlInstanceId(char ** buffer, long * allocatedChunks, struct RTDS_SdlInstanceId * ptr, int level, int followPointers);
static char * RTDS_structStringToData_RTDS_SdlInstanceId(struct RTDS_SdlInstanceId * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_QCB(char ** buffer, long * allocatedChunks, RTDS_QCB * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_QCB(RTDS_QCB * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_broadcast_data(char ** buffer, long * allocatedChunks, RTDS_broadcast_data * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_broadcast_data(RTDS_broadcast_data * dataPointer, char * dataString);
static void RTDS_structDataToString_RTDS_alert_data(char ** buffer, long * allocatedChunks, struct RTDS_alert_data * ptr, int level, int followPointers);
static char * RTDS_structStringToData_RTDS_alert_data(struct RTDS_alert_data * dataPointer, char * dataString);
static void RTDS_structDataToString_RTDS_SemaphoreInfo(char ** buffer, long * allocatedChunks, struct RTDS_SemaphoreInfo * ptr, int level, int followPointers);
static char * RTDS_structStringToData_RTDS_SemaphoreInfo(struct RTDS_SemaphoreInfo * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_SdlInstanceId(char ** buffer, long * allocatedChunks, RTDS_SdlInstanceId * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_SdlInstanceId(RTDS_SdlInstanceId * dataPointer, char * dataString);
static void RTDS_typeDataToString_BlockMemberPtr(char ** buffer, long * allocatedChunks, BlockMemberPtr * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_BlockMemberPtr(BlockMemberPtr * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_QueueId(char ** buffer, long * allocatedChunks, RTDS_QueueId * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_QueueId(RTDS_QueueId * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_RtosQueueId(char ** buffer, long * allocatedChunks, RTDS_RtosQueueId * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_RtosQueueId(RTDS_RtosQueueId * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_nodeHello_data(char ** buffer, long * allocatedChunks, RTDS_nodeHello_data * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_nodeHello_data(RTDS_nodeHello_data * dataPointer, char * dataString);
static void RTDS_structDataToString_RTDS_broadcast_data(char ** buffer, long * allocatedChunks, struct RTDS_broadcast_data * ptr, int level, int followPointers);
static char * RTDS_structStringToData_RTDS_broadcast_data(struct RTDS_broadcast_data * dataPointer, char * dataString);
static void RTDS_structDataToString_RTDS_MessageHeader(char ** buffer, long * allocatedChunks, struct RTDS_MessageHeader * ptr, int level, int followPointers);
static char * RTDS_structStringToData_RTDS_MessageHeader(struct RTDS_MessageHeader * dataPointer, char * dataString);
static void RTDS_typeDataToString_HANDLE(char ** buffer, long * allocatedChunks, HANDLE * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_HANDLE(HANDLE * dataPointer, char * dataString);
static void RTDS_enumDataToString_RTDS_SemaphoreType(char ** buffer, long * allocatedChunks, enum RTDS_SemaphoreType * ptr, int level, int followPointers);
static char * RTDS_enumStringToData_RTDS_SemaphoreType(enum RTDS_SemaphoreType * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_hello_data(char ** buffer, long * allocatedChunks, RTDS_hello_data * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_hello_data(RTDS_hello_data * dataPointer, char * dataString);
static void RTDS_structDataToString_RTDS_nodeHello_data(char ** buffer, long * allocatedChunks, struct RTDS_nodeHello_data * ptr, int level, int followPointers);
static char * RTDS_structStringToData_RTDS_nodeHello_data(struct RTDS_nodeHello_data * dataPointer, char * dataString);
static void RTDS_structDataToString_RTDS_hello_data(char ** buffer, long * allocatedChunks, struct RTDS_hello_data * ptr, int level, int followPointers);
static char * RTDS_structStringToData_RTDS_hello_data(struct RTDS_hello_data * dataPointer, char * dataString);
static void RTDS_typeDataToString_NetworkTopologyPtr(char ** buffer, long * allocatedChunks, NetworkTopologyPtr * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_NetworkTopologyPtr(NetworkTopologyPtr * dataPointer, char * dataString);
static void RTDS_typeDataToString_RTDS_alert_data(char ** buffer, long * allocatedChunks, RTDS_alert_data * ptr, int level, int followPointers);
static char * RTDS_typeStringToData_RTDS_alert_data(RTDS_alert_data * dataPointer, char * dataString);


/*
 * FUNCTION RTDS_typeDataToString_RTDS_nodeHello_data:
 * ---------------------------------------------------
 */

static void RTDS_typeDataToString_RTDS_nodeHello_data(char ** buffer, long * allocatedChunks, RTDS_nodeHello_data * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  RTDS_structDataToString_RTDS_nodeHello_data(buffer, allocatedChunks, &(*ptr), level, followPointers);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_nodeHello_data:
 * ---------------------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_nodeHello_data(RTDS_nodeHello_data * dataPointer, char * dataString)
  {
  dataString = RTDS_structStringToData_RTDS_nodeHello_data(dataPointer, dataString);
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_RTDS_hello_data:
 * -----------------------------------------------
 */

static void RTDS_typeDataToString_RTDS_hello_data(char ** buffer, long * allocatedChunks, RTDS_hello_data * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  RTDS_structDataToString_RTDS_hello_data(buffer, allocatedChunks, &(*ptr), level, followPointers);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_hello_data:
 * -----------------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_hello_data(RTDS_hello_data * dataPointer, char * dataString)
  {
  dataString = RTDS_structStringToData_RTDS_hello_data(dataPointer, dataString);
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_RTDS_submitTopology_data:
 * --------------------------------------------------------
 */

static void RTDS_typeDataToString_RTDS_submitTopology_data(char ** buffer, long * allocatedChunks, RTDS_submitTopology_data * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  RTDS_structDataToString_RTDS_submitTopology_data(buffer, allocatedChunks, &(*ptr), level, followPointers);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_submitTopology_data:
 * --------------------------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_submitTopology_data(RTDS_submitTopology_data * dataPointer, char * dataString)
  {
  dataString = RTDS_structStringToData_RTDS_submitTopology_data(dataPointer, dataString);
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_RTDS_alert_data:
 * -----------------------------------------------
 */

static void RTDS_typeDataToString_RTDS_alert_data(char ** buffer, long * allocatedChunks, RTDS_alert_data * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  RTDS_structDataToString_RTDS_alert_data(buffer, allocatedChunks, &(*ptr), level, followPointers);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_alert_data:
 * -----------------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_alert_data(RTDS_alert_data * dataPointer, char * dataString)
  {
  dataString = RTDS_structStringToData_RTDS_alert_data(dataPointer, dataString);
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_RTDS_broadcast_data:
 * ---------------------------------------------------
 */

static void RTDS_typeDataToString_RTDS_broadcast_data(char ** buffer, long * allocatedChunks, RTDS_broadcast_data * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  RTDS_structDataToString_RTDS_broadcast_data(buffer, allocatedChunks, &(*ptr), level, followPointers);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_broadcast_data:
 * ---------------------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_broadcast_data(RTDS_broadcast_data * dataPointer, char * dataString)
  {
  dataString = RTDS_structStringToData_RTDS_broadcast_data(dataPointer, dataString);
  return dataString;
  }



/*
 * FUNCTION RTDS_structDataToString_RTDS_alert_data:
 * -------------------------------------------------
 */

static void RTDS_structDataToString_RTDS_alert_data(char ** buffer, long * allocatedChunks, struct RTDS_alert_data * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|{param1|=");
  sprintf(RTDS_tmpString, "%d", ptr->param1);
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  RTDS_extendNCat(buffer, allocatedChunks, "|}");
  }



/*
 * FUNCTION RTDS_structStringToData_RTDS_alert_data:
 * -------------------------------------------------
 */

static char * RTDS_structStringToData_RTDS_alert_data(struct RTDS_alert_data * dataPointer, char * dataString)
  {
  int i;
  int field_count = 0;  if ( dataString[0] != '|' || dataString[1] != '{' ) return dataString;
  dataString += 2;
  for ( ; ; )
    {
    if ( dataString[0] == '|' && dataString[1] == '+' )
      {
      field_count = -1;
      dataString += 2;
      }
    if ( dataString[0] == '|' && dataString[1] == '}' )
      {
      if ( field_count != -1 && field_count != 1)
        return dataString;
      dataString += 2;
      break;
      }
    else if ( field_count == -1 )
      {
      return dataString;
      }
    if ( field_count > 0 )
      {
      if ( dataString[0] != '|' || dataString[1] != ',' ) return dataString;
      dataString += 2;
      }
    i = 0;
    while ( isspace((int)(dataString[i])) || isalnum((int)(dataString[i])) || dataString[i] == '_' ) i++;
    if ( dataString[i] != '|' || dataString[i + 1] != '=' ) return dataString;
    if ( strncmp(dataString, "param1", i) == 0 )
      {
      dataString += 8;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        *(&(dataPointer->param1)) = (int)strtol(dataString, &dataString, 10);
        }
      field_count++;
      }
    else
      {
      return dataString;
      }
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_structDataToString_RTDS_broadcast_data:
 * -----------------------------------------------------
 */

static void RTDS_structDataToString_RTDS_broadcast_data(char ** buffer, long * allocatedChunks, struct RTDS_broadcast_data * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|{param1|=");
  sprintf(RTDS_tmpString, "%d", ptr->param1);
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  RTDS_extendNCat(buffer, allocatedChunks, "|}");
  }



/*
 * FUNCTION RTDS_structStringToData_RTDS_broadcast_data:
 * -----------------------------------------------------
 */

static char * RTDS_structStringToData_RTDS_broadcast_data(struct RTDS_broadcast_data * dataPointer, char * dataString)
  {
  int i;
  int field_count = 0;  if ( dataString[0] != '|' || dataString[1] != '{' ) return dataString;
  dataString += 2;
  for ( ; ; )
    {
    if ( dataString[0] == '|' && dataString[1] == '+' )
      {
      field_count = -1;
      dataString += 2;
      }
    if ( dataString[0] == '|' && dataString[1] == '}' )
      {
      if ( field_count != -1 && field_count != 1)
        return dataString;
      dataString += 2;
      break;
      }
    else if ( field_count == -1 )
      {
      return dataString;
      }
    if ( field_count > 0 )
      {
      if ( dataString[0] != '|' || dataString[1] != ',' ) return dataString;
      dataString += 2;
      }
    i = 0;
    while ( isspace((int)(dataString[i])) || isalnum((int)(dataString[i])) || dataString[i] == '_' ) i++;
    if ( dataString[i] != '|' || dataString[i + 1] != '=' ) return dataString;
    if ( strncmp(dataString, "param1", i) == 0 )
      {
      dataString += 8;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        *(&(dataPointer->param1)) = (int)strtol(dataString, &dataString, 10);
        }
      field_count++;
      }
    else
      {
      return dataString;
      }
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_structDataToString_RTDS_hello_data:
 * -------------------------------------------------
 */

static void RTDS_structDataToString_RTDS_hello_data(char ** buffer, long * allocatedChunks, struct RTDS_hello_data * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|{param1|=");
  RTDS_typeDataToString_BlockMemberPtr(buffer, allocatedChunks, &(ptr->param1), level, followPointers);
  RTDS_extendNCat(buffer, allocatedChunks, "|}");
  }



/*
 * FUNCTION RTDS_structStringToData_RTDS_hello_data:
 * -------------------------------------------------
 */

static char * RTDS_structStringToData_RTDS_hello_data(struct RTDS_hello_data * dataPointer, char * dataString)
  {
  int i;
  int field_count = 0;  if ( dataString[0] != '|' || dataString[1] != '{' ) return dataString;
  dataString += 2;
  for ( ; ; )
    {
    if ( dataString[0] == '|' && dataString[1] == '+' )
      {
      field_count = -1;
      dataString += 2;
      }
    if ( dataString[0] == '|' && dataString[1] == '}' )
      {
      if ( field_count != -1 && field_count != 1)
        return dataString;
      dataString += 2;
      break;
      }
    else if ( field_count == -1 )
      {
      return dataString;
      }
    if ( field_count > 0 )
      {
      if ( dataString[0] != '|' || dataString[1] != ',' ) return dataString;
      dataString += 2;
      }
    i = 0;
    while ( isspace((int)(dataString[i])) || isalnum((int)(dataString[i])) || dataString[i] == '_' ) i++;
    if ( dataString[i] != '|' || dataString[i + 1] != '=' ) return dataString;
    if ( strncmp(dataString, "param1", i) == 0 )
      {
      dataString += 8;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        dataString = RTDS_typeStringToData_BlockMemberPtr(&(dataPointer->param1), dataString);
        }
      field_count++;
      }
    else
      {
      return dataString;
      }
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_structDataToString_RTDS_submitTopology_data:
 * ----------------------------------------------------------
 */

static void RTDS_structDataToString_RTDS_submitTopology_data(char ** buffer, long * allocatedChunks, struct RTDS_submitTopology_data * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|{param1|=");
  RTDS_typeDataToString_NetworkTopologyPtr(buffer, allocatedChunks, &(ptr->param1), level, followPointers);
  RTDS_extendNCat(buffer, allocatedChunks, "|}");
  }



/*
 * FUNCTION RTDS_structStringToData_RTDS_submitTopology_data:
 * ----------------------------------------------------------
 */

static char * RTDS_structStringToData_RTDS_submitTopology_data(struct RTDS_submitTopology_data * dataPointer, char * dataString)
  {
  int i;
  int field_count = 0;  if ( dataString[0] != '|' || dataString[1] != '{' ) return dataString;
  dataString += 2;
  for ( ; ; )
    {
    if ( dataString[0] == '|' && dataString[1] == '+' )
      {
      field_count = -1;
      dataString += 2;
      }
    if ( dataString[0] == '|' && dataString[1] == '}' )
      {
      if ( field_count != -1 && field_count != 1)
        return dataString;
      dataString += 2;
      break;
      }
    else if ( field_count == -1 )
      {
      return dataString;
      }
    if ( field_count > 0 )
      {
      if ( dataString[0] != '|' || dataString[1] != ',' ) return dataString;
      dataString += 2;
      }
    i = 0;
    while ( isspace((int)(dataString[i])) || isalnum((int)(dataString[i])) || dataString[i] == '_' ) i++;
    if ( dataString[i] != '|' || dataString[i + 1] != '=' ) return dataString;
    if ( strncmp(dataString, "param1", i) == 0 )
      {
      dataString += 8;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        dataString = RTDS_typeStringToData_NetworkTopologyPtr(&(dataPointer->param1), dataString);
        }
      field_count++;
      }
    else
      {
      return dataString;
      }
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_structDataToString_RTDS_nodeHello_data:
 * -----------------------------------------------------
 */

static void RTDS_structDataToString_RTDS_nodeHello_data(char ** buffer, long * allocatedChunks, struct RTDS_nodeHello_data * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|{param1|=");
  RTDS_typeDataToString_RTDS_QueueId(buffer, allocatedChunks, &(ptr->param1), level, followPointers);
  RTDS_extendNCat(buffer, allocatedChunks, "|}");
  }



/*
 * FUNCTION RTDS_structStringToData_RTDS_nodeHello_data:
 * -----------------------------------------------------
 */

static char * RTDS_structStringToData_RTDS_nodeHello_data(struct RTDS_nodeHello_data * dataPointer, char * dataString)
  {
  int i;
  int field_count = 0;  if ( dataString[0] != '|' || dataString[1] != '{' ) return dataString;
  dataString += 2;
  for ( ; ; )
    {
    if ( dataString[0] == '|' && dataString[1] == '+' )
      {
      field_count = -1;
      dataString += 2;
      }
    if ( dataString[0] == '|' && dataString[1] == '}' )
      {
      if ( field_count != -1 && field_count != 1)
        return dataString;
      dataString += 2;
      break;
      }
    else if ( field_count == -1 )
      {
      return dataString;
      }
    if ( field_count > 0 )
      {
      if ( dataString[0] != '|' || dataString[1] != ',' ) return dataString;
      dataString += 2;
      }
    i = 0;
    while ( isspace((int)(dataString[i])) || isalnum((int)(dataString[i])) || dataString[i] == '_' ) i++;
    if ( dataString[i] != '|' || dataString[i + 1] != '=' ) return dataString;
    if ( strncmp(dataString, "param1", i) == 0 )
      {
      dataString += 8;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        dataString = RTDS_typeStringToData_RTDS_QueueId(&(dataPointer->param1), dataString);
        }
      field_count++;
      }
    else
      {
      return dataString;
      }
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_RTDS_QueueId:
 * --------------------------------------------
 */

static void RTDS_typeDataToString_RTDS_QueueId(char ** buffer, long * allocatedChunks, RTDS_QueueId * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(*ptr));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_QueueId:
 * --------------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_QueueId(RTDS_QueueId * dataPointer, char * dataString)
  {
  if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
  if ( isxdigit((int)(*dataString)) )
    {
    *(dataPointer) = (RTDS_SdlInstanceId*)strtoul(dataString, &dataString, 16);
    }
  else
    {
    *(dataPointer) = (RTDS_SdlInstanceId*)RTDS_MALLOC(sizeof(RTDS_SdlInstanceId));
    if ( *(dataPointer) == NULL ) return dataString;
    }
  if ( *(dataPointer) != NULL && dataString[0] == '|' && dataString[1] == ':' )
    {
    dataString += 2;
    dataString = RTDS_typeStringToData_RTDS_SdlInstanceId(*(dataPointer), dataString);
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_NetworkTopologyPtr:
 * --------------------------------------------------
 */

static void RTDS_typeDataToString_NetworkTopologyPtr(char ** buffer, long * allocatedChunks, NetworkTopologyPtr * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(*ptr));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  }



/*
 * FUNCTION RTDS_typeStringToData_NetworkTopologyPtr:
 * --------------------------------------------------
 */

static char * RTDS_typeStringToData_NetworkTopologyPtr(NetworkTopologyPtr * dataPointer, char * dataString)
  {
  if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
  if ( isxdigit((int)(*dataString)) )
    {
    *(dataPointer) = (NetworkTopology*)strtoul(dataString, &dataString, 16);
    }
  else
    {
    *(dataPointer) = (NetworkTopology*)NULL;
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_BlockMemberPtr:
 * ----------------------------------------------
 */

static void RTDS_typeDataToString_BlockMemberPtr(char ** buffer, long * allocatedChunks, BlockMemberPtr * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(*ptr));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  }



/*
 * FUNCTION RTDS_typeStringToData_BlockMemberPtr:
 * ----------------------------------------------
 */

static char * RTDS_typeStringToData_BlockMemberPtr(BlockMemberPtr * dataPointer, char * dataString)
  {
  if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
  if ( isxdigit((int)(*dataString)) )
    {
    *(dataPointer) = (BlockMember*)strtoul(dataString, &dataString, 16);
    }
  else
    {
    *(dataPointer) = (BlockMember*)NULL;
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_RTDS_SdlInstanceId:
 * --------------------------------------------------
 */

static void RTDS_typeDataToString_RTDS_SdlInstanceId(char ** buffer, long * allocatedChunks, RTDS_SdlInstanceId * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  RTDS_structDataToString_RTDS_SdlInstanceId(buffer, allocatedChunks, &(*ptr), level, 0);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_SdlInstanceId:
 * --------------------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_SdlInstanceId(RTDS_SdlInstanceId * dataPointer, char * dataString)
  {
  dataString = RTDS_structStringToData_RTDS_SdlInstanceId(dataPointer, dataString);
  return dataString;
  }



/*
 * FUNCTION RTDS_structDataToString_RTDS_SdlInstanceId:
 * ----------------------------------------------------
 */

static void RTDS_structDataToString_RTDS_SdlInstanceId(char ** buffer, long * allocatedChunks, struct RTDS_SdlInstanceId * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|{queueId|=");
  RTDS_typeDataToString_RTDS_RtosQueueId(buffer, allocatedChunks, &(ptr->queueId), level, followPointers);
  RTDS_extendNCat(buffer, allocatedChunks, "|,instanceNumber|=");
  sprintf(RTDS_tmpString, "%d", ptr->instanceNumber);
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  RTDS_extendNCat(buffer, allocatedChunks, "|}");
  }



/*
 * FUNCTION RTDS_structStringToData_RTDS_SdlInstanceId:
 * ----------------------------------------------------
 */

static char * RTDS_structStringToData_RTDS_SdlInstanceId(struct RTDS_SdlInstanceId * dataPointer, char * dataString)
  {
  int i;
  int field_count = 0;  if ( dataString[0] != '|' || dataString[1] != '{' ) return dataString;
  dataString += 2;
  for ( ; ; )
    {
    if ( dataString[0] == '|' && dataString[1] == '+' )
      {
      field_count = -1;
      dataString += 2;
      }
    if ( dataString[0] == '|' && dataString[1] == '}' )
      {
      if ( field_count != -1 && field_count != 2)
        return dataString;
      dataString += 2;
      break;
      }
    else if ( field_count == -1 )
      {
      return dataString;
      }
    if ( field_count > 0 )
      {
      if ( dataString[0] != '|' || dataString[1] != ',' ) return dataString;
      dataString += 2;
      }
    i = 0;
    while ( isspace((int)(dataString[i])) || isalnum((int)(dataString[i])) || dataString[i] == '_' ) i++;
    if ( dataString[i] != '|' || dataString[i + 1] != '=' ) return dataString;
    if ( strncmp(dataString, "queueId", i) == 0 )
      {
      dataString += 9;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        dataString = RTDS_typeStringToData_RTDS_RtosQueueId(&(dataPointer->queueId), dataString);
        }
      field_count++;
      }
    else if ( strncmp(dataString, "instanceNumber", i) == 0 )
      {
      dataString += 16;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        *(&(dataPointer->instanceNumber)) = (int)strtol(dataString, &dataString, 10);
        }
      field_count++;
      }
    else
      {
      return dataString;
      }
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_RTDS_RtosQueueId:
 * ------------------------------------------------
 */

static void RTDS_typeDataToString_RTDS_RtosQueueId(char ** buffer, long * allocatedChunks, RTDS_RtosQueueId * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(*ptr));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_RtosQueueId:
 * ------------------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_RtosQueueId(RTDS_RtosQueueId * dataPointer, char * dataString)
  {
  if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
  if ( isxdigit((int)(*dataString)) )
    {
    *(dataPointer) = (RTDS_QCB*)strtoul(dataString, &dataString, 16);
    }
  else
    {
    *(dataPointer) = (RTDS_QCB*)RTDS_MALLOC(sizeof(RTDS_QCB));
    if ( *(dataPointer) == NULL ) return dataString;
    }
  if ( *(dataPointer) != NULL && dataString[0] == '|' && dataString[1] == ':' )
    {
    dataString += 2;
    dataString = RTDS_typeStringToData_RTDS_QCB(*(dataPointer), dataString);
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_RTDS_QCB:
 * ----------------------------------------
 */

static void RTDS_typeDataToString_RTDS_QCB(char ** buffer, long * allocatedChunks, RTDS_QCB * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  RTDS_structDataToString_RTDS_QCB(buffer, allocatedChunks, &(*ptr), level, 0);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_QCB:
 * ----------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_QCB(RTDS_QCB * dataPointer, char * dataString)
  {
  dataString = RTDS_structStringToData_RTDS_QCB(dataPointer, dataString);
  return dataString;
  }



/*
 * FUNCTION RTDS_structDataToString_RTDS_QCB:
 * ------------------------------------------
 */

static void RTDS_structDataToString_RTDS_QCB(char ** buffer, long * allocatedChunks, struct RTDS_QCB * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|{chainedListSemId|=");
  RTDS_typeDataToString_RTDS_SemaphoreId(buffer, allocatedChunks, &(ptr->chainedListSemId), level, followPointers);
  RTDS_extendNCat(buffer, allocatedChunks, "|,blockingSemId|=");
  RTDS_typeDataToString_RTDS_SemaphoreId(buffer, allocatedChunks, &(ptr->blockingSemId), level, followPointers);
  RTDS_extendNCat(buffer, allocatedChunks, "|,queue|=");
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(ptr->queue));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  if ( followPointers && ptr->queue != NULL )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|:");
    RTDS_structDataToString_RTDS_MessageHeader(buffer, allocatedChunks, ptr->queue, level, followPointers);
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|}");
  }



/*
 * FUNCTION RTDS_structStringToData_RTDS_QCB:
 * ------------------------------------------
 */

static char * RTDS_structStringToData_RTDS_QCB(struct RTDS_QCB * dataPointer, char * dataString)
  {
  int i;
  int field_count = 0;  if ( dataString[0] != '|' || dataString[1] != '{' ) return dataString;
  dataString += 2;
  for ( ; ; )
    {
    if ( dataString[0] == '|' && dataString[1] == '+' )
      {
      field_count = -1;
      dataString += 2;
      }
    if ( dataString[0] == '|' && dataString[1] == '}' )
      {
      if ( field_count != -1 && field_count != 3)
        return dataString;
      dataString += 2;
      break;
      }
    else if ( field_count == -1 )
      {
      return dataString;
      }
    if ( field_count > 0 )
      {
      if ( dataString[0] != '|' || dataString[1] != ',' ) return dataString;
      dataString += 2;
      }
    i = 0;
    while ( isspace((int)(dataString[i])) || isalnum((int)(dataString[i])) || dataString[i] == '_' ) i++;
    if ( dataString[i] != '|' || dataString[i + 1] != '=' ) return dataString;
    if ( strncmp(dataString, "chainedListSemId", i) == 0 )
      {
      dataString += 18;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        dataString = RTDS_typeStringToData_RTDS_SemaphoreId(&(dataPointer->chainedListSemId), dataString);
        }
      field_count++;
      }
    else if ( strncmp(dataString, "blockingSemId", i) == 0 )
      {
      dataString += 15;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        dataString = RTDS_typeStringToData_RTDS_SemaphoreId(&(dataPointer->blockingSemId), dataString);
        }
      field_count++;
      }
    else if ( strncmp(dataString, "queue", i) == 0 )
      {
      dataString += 7;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
        if ( isxdigit((int)(*dataString)) )
          {
          *(&(dataPointer->queue)) = (struct RTDS_MessageHeader*)strtoul(dataString, &dataString, 16);
          }
        else
          {
          *(&(dataPointer->queue)) = (struct RTDS_MessageHeader*)RTDS_MALLOC(sizeof(struct RTDS_MessageHeader));
          if ( *(&(dataPointer->queue)) == NULL ) return dataString;
          }
        if ( *(&(dataPointer->queue)) != NULL && dataString[0] == '|' && dataString[1] == ':' )
          {
          dataString += 2;
          dataString = RTDS_structStringToData_RTDS_MessageHeader(*(&(dataPointer->queue)), dataString);
          }
        }
      field_count++;
      }
    else
      {
      return dataString;
      }
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_structDataToString_RTDS_MessageHeader:
 * ----------------------------------------------------
 */

static void RTDS_structDataToString_RTDS_MessageHeader(char ** buffer, long * allocatedChunks, struct RTDS_MessageHeader * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|{messageNumber|=");
  sprintf(RTDS_tmpString, "%ld", ptr->messageNumber);
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  RTDS_extendNCat(buffer, allocatedChunks, "|,timerUniqueId|=");
  sprintf(RTDS_tmpString, "%ld", ptr->timerUniqueId);
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  RTDS_extendNCat(buffer, allocatedChunks, "|,messageUniqueId|=");
  sprintf(RTDS_tmpString, "%lu", ptr->messageUniqueId);
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  RTDS_extendNCat(buffer, allocatedChunks, "|,sender|=");
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(ptr->sender));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  if ( followPointers && ptr->sender != NULL )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|:");
    RTDS_typeDataToString_RTDS_SdlInstanceId(buffer, allocatedChunks, ptr->sender, level, followPointers);
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|,receiver|=");
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(ptr->receiver));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  if ( followPointers && ptr->receiver != NULL )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|:");
    RTDS_typeDataToString_RTDS_SdlInstanceId(buffer, allocatedChunks, ptr->receiver, level, followPointers);
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|,dataLength|=");
  sprintf(RTDS_tmpString, "%ld", ptr->dataLength);
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  RTDS_extendNCat(buffer, allocatedChunks, "|,pData|=");
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(ptr->pData));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  if ( followPointers && ptr->pData != NULL )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|:");
    sprintf(RTDS_tmpString, "%u", *(ptr->pData));
    RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|,next|=");
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(ptr->next));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  if ( followPointers && ptr->next != NULL )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|:");
    RTDS_structDataToString_RTDS_MessageHeader(buffer, allocatedChunks, ptr->next, level, followPointers);
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|}");
  }



/*
 * FUNCTION RTDS_structStringToData_RTDS_MessageHeader:
 * ----------------------------------------------------
 */

static char * RTDS_structStringToData_RTDS_MessageHeader(struct RTDS_MessageHeader * dataPointer, char * dataString)
  {
  int i;
  int field_count = 0;  if ( dataString[0] != '|' || dataString[1] != '{' ) return dataString;
  dataString += 2;
  for ( ; ; )
    {
    if ( dataString[0] == '|' && dataString[1] == '+' )
      {
      field_count = -1;
      dataString += 2;
      }
    if ( dataString[0] == '|' && dataString[1] == '}' )
      {
      if ( field_count != -1 && field_count != 8)
        return dataString;
      dataString += 2;
      break;
      }
    else if ( field_count == -1 )
      {
      return dataString;
      }
    if ( field_count > 0 )
      {
      if ( dataString[0] != '|' || dataString[1] != ',' ) return dataString;
      dataString += 2;
      }
    i = 0;
    while ( isspace((int)(dataString[i])) || isalnum((int)(dataString[i])) || dataString[i] == '_' ) i++;
    if ( dataString[i] != '|' || dataString[i + 1] != '=' ) return dataString;
    if ( strncmp(dataString, "messageNumber", i) == 0 )
      {
      dataString += 15;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        *(&(dataPointer->messageNumber)) = (long)strtol(dataString, &dataString, 10);
        }
      field_count++;
      }
    else if ( strncmp(dataString, "timerUniqueId", i) == 0 )
      {
      dataString += 15;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        *(&(dataPointer->timerUniqueId)) = (long)strtol(dataString, &dataString, 10);
        }
      field_count++;
      }
    else if ( strncmp(dataString, "messageUniqueId", i) == 0 )
      {
      dataString += 17;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        *(&(dataPointer->messageUniqueId)) = (unsigned long)strtoul(dataString, &dataString, 10);
        }
      field_count++;
      }
    else if ( strncmp(dataString, "sender", i) == 0 )
      {
      dataString += 8;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
        if ( isxdigit((int)(*dataString)) )
          {
          *(&(dataPointer->sender)) = (RTDS_SdlInstanceId*)strtoul(dataString, &dataString, 16);
          }
        else
          {
          *(&(dataPointer->sender)) = (RTDS_SdlInstanceId*)RTDS_MALLOC(sizeof(RTDS_SdlInstanceId));
          if ( *(&(dataPointer->sender)) == NULL ) return dataString;
          }
        if ( *(&(dataPointer->sender)) != NULL && dataString[0] == '|' && dataString[1] == ':' )
          {
          dataString += 2;
          dataString = RTDS_typeStringToData_RTDS_SdlInstanceId(*(&(dataPointer->sender)), dataString);
          }
        }
      field_count++;
      }
    else if ( strncmp(dataString, "receiver", i) == 0 )
      {
      dataString += 10;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
        if ( isxdigit((int)(*dataString)) )
          {
          *(&(dataPointer->receiver)) = (RTDS_SdlInstanceId*)strtoul(dataString, &dataString, 16);
          }
        else
          {
          *(&(dataPointer->receiver)) = (RTDS_SdlInstanceId*)RTDS_MALLOC(sizeof(RTDS_SdlInstanceId));
          if ( *(&(dataPointer->receiver)) == NULL ) return dataString;
          }
        if ( *(&(dataPointer->receiver)) != NULL && dataString[0] == '|' && dataString[1] == ':' )
          {
          dataString += 2;
          dataString = RTDS_typeStringToData_RTDS_SdlInstanceId(*(&(dataPointer->receiver)), dataString);
          }
        }
      field_count++;
      }
    else if ( strncmp(dataString, "dataLength", i) == 0 )
      {
      dataString += 12;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        *(&(dataPointer->dataLength)) = (long)strtol(dataString, &dataString, 10);
        }
      field_count++;
      }
    else if ( strncmp(dataString, "pData", i) == 0 )
      {
      dataString += 7;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
        if ( isxdigit((int)(*dataString)) )
          {
          *(&(dataPointer->pData)) = (unsigned char*)strtoul(dataString, &dataString, 16);
          }
        else
          {
          *(&(dataPointer->pData)) = (unsigned char*)RTDS_MALLOC(sizeof(unsigned char));
          if ( *(&(dataPointer->pData)) == NULL ) return dataString;
          }
        if ( *(&(dataPointer->pData)) != NULL && dataString[0] == '|' && dataString[1] == ':' )
          {
          dataString += 2;
          *(*(&(dataPointer->pData))) = (unsigned char)strtoul(dataString, &dataString, 16);
          }
        }
      field_count++;
      }
    else if ( strncmp(dataString, "next", i) == 0 )
      {
      dataString += 6;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
        if ( isxdigit((int)(*dataString)) )
          {
          *(&(dataPointer->next)) = (struct RTDS_MessageHeader*)strtoul(dataString, &dataString, 16);
          }
        else
          {
          *(&(dataPointer->next)) = (struct RTDS_MessageHeader*)RTDS_MALLOC(sizeof(struct RTDS_MessageHeader));
          if ( *(&(dataPointer->next)) == NULL ) return dataString;
          }
        if ( *(&(dataPointer->next)) != NULL && dataString[0] == '|' && dataString[1] == ':' )
          {
          dataString += 2;
          dataString = RTDS_structStringToData_RTDS_MessageHeader(*(&(dataPointer->next)), dataString);
          }
        }
      field_count++;
      }
    else
      {
      return dataString;
      }
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_RTDS_SemaphoreId:
 * ------------------------------------------------
 */

static void RTDS_typeDataToString_RTDS_SemaphoreId(char ** buffer, long * allocatedChunks, RTDS_SemaphoreId * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(*ptr));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_SemaphoreId:
 * ------------------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_SemaphoreId(RTDS_SemaphoreId * dataPointer, char * dataString)
  {
  if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
  if ( isxdigit((int)(*dataString)) )
    {
    *(dataPointer) = (RTDS_SemaphoreInfo*)strtoul(dataString, &dataString, 16);
    }
  else
    {
    *(dataPointer) = (RTDS_SemaphoreInfo*)RTDS_MALLOC(sizeof(RTDS_SemaphoreInfo));
    if ( *(dataPointer) == NULL ) return dataString;
    }
  if ( *(dataPointer) != NULL && dataString[0] == '|' && dataString[1] == ':' )
    {
    dataString += 2;
    dataString = RTDS_typeStringToData_RTDS_SemaphoreInfo(*(dataPointer), dataString);
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_RTDS_SemaphoreInfo:
 * --------------------------------------------------
 */

static void RTDS_typeDataToString_RTDS_SemaphoreInfo(char ** buffer, long * allocatedChunks, RTDS_SemaphoreInfo * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  RTDS_structDataToString_RTDS_SemaphoreInfo(buffer, allocatedChunks, &(*ptr), level, 0);
  }



/*
 * FUNCTION RTDS_typeStringToData_RTDS_SemaphoreInfo:
 * --------------------------------------------------
 */

static char * RTDS_typeStringToData_RTDS_SemaphoreInfo(RTDS_SemaphoreInfo * dataPointer, char * dataString)
  {
  dataString = RTDS_structStringToData_RTDS_SemaphoreInfo(dataPointer, dataString);
  return dataString;
  }



/*
 * FUNCTION RTDS_structDataToString_RTDS_SemaphoreInfo:
 * ----------------------------------------------------
 */

static void RTDS_structDataToString_RTDS_SemaphoreInfo(char ** buffer, long * allocatedChunks, struct RTDS_SemaphoreInfo * ptr, int level, int followPointers)
  {
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  RTDS_extendNCat(buffer, allocatedChunks, "|{semId|=");
  RTDS_typeDataToString_HANDLE(buffer, allocatedChunks, &(ptr->semId), level, followPointers);
  RTDS_extendNCat(buffer, allocatedChunks, "|,semType|=");
  RTDS_enumDataToString_RTDS_SemaphoreType(buffer, allocatedChunks, &(ptr->semType), level, followPointers);
  RTDS_extendNCat(buffer, allocatedChunks, "|}");
  }



/*
 * FUNCTION RTDS_structStringToData_RTDS_SemaphoreInfo:
 * ----------------------------------------------------
 */

static char * RTDS_structStringToData_RTDS_SemaphoreInfo(struct RTDS_SemaphoreInfo * dataPointer, char * dataString)
  {
  int i;
  int field_count = 0;  if ( dataString[0] != '|' || dataString[1] != '{' ) return dataString;
  dataString += 2;
  for ( ; ; )
    {
    if ( dataString[0] == '|' && dataString[1] == '+' )
      {
      field_count = -1;
      dataString += 2;
      }
    if ( dataString[0] == '|' && dataString[1] == '}' )
      {
      if ( field_count != -1 && field_count != 2)
        return dataString;
      dataString += 2;
      break;
      }
    else if ( field_count == -1 )
      {
      return dataString;
      }
    if ( field_count > 0 )
      {
      if ( dataString[0] != '|' || dataString[1] != ',' ) return dataString;
      dataString += 2;
      }
    i = 0;
    while ( isspace((int)(dataString[i])) || isalnum((int)(dataString[i])) || dataString[i] == '_' ) i++;
    if ( dataString[i] != '|' || dataString[i + 1] != '=' ) return dataString;
    if ( strncmp(dataString, "semId", i) == 0 )
      {
      dataString += 7;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        dataString = RTDS_typeStringToData_HANDLE(&(dataPointer->semId), dataString);
        }
      field_count++;
      }
    else if ( strncmp(dataString, "semType", i) == 0 )
      {
      dataString += 9;
      if ( dataString[0] == '|' && dataString[1] == '-' )
        {
        dataString += 2;
        }
      else
        {
        dataString = RTDS_enumStringToData_RTDS_SemaphoreType(&(dataPointer->semType), dataString);
        }
      field_count++;
      }
    else
      {
      return dataString;
      }
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_enumDataToString_RTDS_SemaphoreType:
 * --------------------------------------------------
 */

static void RTDS_enumDataToString_RTDS_SemaphoreType(char ** buffer, long * allocatedChunks, enum RTDS_SemaphoreType * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
#ifdef RTDS_SYMBOLIC_ENUM_ENCODING
  switch(*ptr)
    {
    case COUNTING:
      RTDS_extendNCat(buffer, allocatedChunks, "COUNTING");
      break;
    case BINARY:
      RTDS_extendNCat(buffer, allocatedChunks, "BINARY");
      break;
    case MUTEX:
      RTDS_extendNCat(buffer, allocatedChunks, "MUTEX");
      break;
    }
#else
  sprintf(RTDS_tmpString, "%d", *((int*)ptr));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
#endif
  }



/*
 * FUNCTION RTDS_enumStringToData_RTDS_SemaphoreType:
 * --------------------------------------------------
 */

static char * RTDS_enumStringToData_RTDS_SemaphoreType(enum RTDS_SemaphoreType * dataPointer, char * dataString)
  {
  if ( dataString[0] >= '0' && dataString[0] <= '9' )
    {
    *dataPointer = (enum RTDS_SemaphoreType)strtol(dataString, &dataString, 10);
    }
  else if ( strncmp(dataString, "COUNTING|", 9) == 0 )
    {
    *dataPointer = COUNTING;
    dataString += 8;
    }
  else if ( strncmp(dataString, "BINARY|", 7) == 0 )
    {
    *dataPointer = BINARY;
    dataString += 6;
    }
  else if ( strncmp(dataString, "MUTEX|", 6) == 0 )
    {
    *dataPointer = MUTEX;
    dataString += 5;
    }
  return dataString;
  }



/*
 * FUNCTION RTDS_typeDataToString_HANDLE:
 * --------------------------------------
 */

static void RTDS_typeDataToString_HANDLE(char ** buffer, long * allocatedChunks, HANDLE * ptr, int level, int followPointers)
  {
  char RTDS_tmpString[64];
  if ( level == 0 )
    {
    RTDS_extendNCat(buffer, allocatedChunks, "|-");
    return;
    }
  level--;
  sprintf(RTDS_tmpString, "0x%lX", (unsigned long)(*ptr));
  RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
  }



/*
 * FUNCTION RTDS_typeStringToData_HANDLE:
 * --------------------------------------
 */

static char * RTDS_typeStringToData_HANDLE(HANDLE * dataPointer, char * dataString)
  {
  if ( dataString[0] == '0' && dataString[1] == 'x' ) dataString += 2;
  if ( isxdigit((int)(*dataString)) )
    {
    *(dataPointer) = (void*)strtoul(dataString, &dataString, 16);
    }
  else
    {
    *(dataPointer) = (void*)NULL;
    }
  return dataString;
  }


/*
 * FUNCTION RTDS_messageDataToString:
 * ----------------------------------
 */

void RTDS_messageDataToString(char ** buffer, long messageNumber, long dataLength, void * dataPointer, int level)
  {
  long chunks = 1, i;               /* Number of allocated chunks */
  long *allocatedChunks = &chunks;  /* Pointer on it to pass to fucntions that may change it */
  char RTDS_tmpString[64];
  int followPointers = 1;           /* True to follow pointers */

  /* Allocate first chunk */
  *buffer = (char*)RTDS_MALLOC(RTDS_PARAM_CODEC_CHUNK_SIZE + 1);
  if ( *buffer == NULL ) return;
  **buffer = '\0';
  /* Convert data to string according to message number */
  switch ( messageNumber )
    {
    case broadcast:
      RTDS_typeDataToString_RTDS_broadcast_data(buffer, allocatedChunks, (RTDS_broadcast_data*)dataPointer, level, followPointers);
      break;
    case submitTopology:
      RTDS_typeDataToString_RTDS_submitTopology_data(buffer, allocatedChunks, (RTDS_submitTopology_data*)dataPointer, level, followPointers);
      break;
    case alert:
      RTDS_typeDataToString_RTDS_alert_data(buffer, allocatedChunks, (RTDS_alert_data*)dataPointer, level, followPointers);
      break;
    case nodeHello:
      RTDS_typeDataToString_RTDS_nodeHello_data(buffer, allocatedChunks, (RTDS_nodeHello_data*)dataPointer, level, followPointers);
      break;
    case hello:
      RTDS_typeDataToString_RTDS_hello_data(buffer, allocatedChunks, (RTDS_hello_data*)dataPointer, level, followPointers);
      break;
    default:
      for ( i = 0; i < dataLength; i ++ )
        {
        if ( i == 0 )
          sprintf(RTDS_tmpString, "%02X", ((unsigned char*)dataPointer)[i]);
        else
          sprintf(RTDS_tmpString, " %02X", ((unsigned char*)dataPointer)[i]);
        RTDS_extendNCat(buffer, allocatedChunks, RTDS_tmpString);
        }
      break;
    }
  }

/*
 * FUNCTION RTDS_stringToMessageData:
 * ----------------------------------
 */

char * RTDS_stringToMessageData(long messageNumber, long * dataLength, void ** dataPointer, char * dataString)
  {
  long i, j;

  /* Convert data from string according to message number */
  switch ( messageNumber )
    {
    case broadcast:
      *dataPointer = (void*)RTDS_MALLOC(sizeof(RTDS_broadcast_data));
      if ( *dataPointer == NULL ) return dataString;
      RTDS_MEM_RESET(*dataPointer, sizeof(RTDS_broadcast_data));      dataString = RTDS_typeStringToData_RTDS_broadcast_data((RTDS_broadcast_data*)(*dataPointer), dataString);
      break;
    case submitTopology:
      *dataPointer = (void*)RTDS_MALLOC(sizeof(RTDS_submitTopology_data));
      if ( *dataPointer == NULL ) return dataString;
      RTDS_MEM_RESET(*dataPointer, sizeof(RTDS_submitTopology_data));      dataString = RTDS_typeStringToData_RTDS_submitTopology_data((RTDS_submitTopology_data*)(*dataPointer), dataString);
      break;
    case alert:
      *dataPointer = (void*)RTDS_MALLOC(sizeof(RTDS_alert_data));
      if ( *dataPointer == NULL ) return dataString;
      RTDS_MEM_RESET(*dataPointer, sizeof(RTDS_alert_data));      dataString = RTDS_typeStringToData_RTDS_alert_data((RTDS_alert_data*)(*dataPointer), dataString);
      break;
    case nodeHello:
      *dataPointer = (void*)RTDS_MALLOC(sizeof(RTDS_nodeHello_data));
      if ( *dataPointer == NULL ) return dataString;
      RTDS_MEM_RESET(*dataPointer, sizeof(RTDS_nodeHello_data));      dataString = RTDS_typeStringToData_RTDS_nodeHello_data((RTDS_nodeHello_data*)(*dataPointer), dataString);
      break;
    case hello:
      *dataPointer = (void*)RTDS_MALLOC(sizeof(RTDS_hello_data));
      if ( *dataPointer == NULL ) return dataString;
      RTDS_MEM_RESET(*dataPointer, sizeof(RTDS_hello_data));      dataString = RTDS_typeStringToData_RTDS_hello_data((RTDS_hello_data*)(*dataPointer), dataString);
      break;
    default:
      /* Compute number of bytes in data */
      i = 0; *dataLength = 0;
      while ( isxdigit((int)(dataString[i])) || isspace((int)(dataString[i])) )
        {
        while ( isspace((int)(dataString[i])) ) i++;
        if ( isxdigit((int)(dataString[i])) )
          {
          i++; *dataLength += 1;
          if ( isxdigit((int)(dataString[i])) ) i++;
          }
        }
      /* Allocate space */
      *dataPointer = (void*)RTDS_MALLOC(*dataLength);
      if ( *dataPointer == NULL ) return dataString;
      /* Get actual bytes */
      i = 0;
      for ( j = 0; j < *dataLength; j++ )
        {
        char byte[3];
        while ( isspace((int)(dataString[i])) ) i++;
        byte[0] = dataString[i];
        i++;
        if ( isxdigit((int)(dataString[i])) )
          {
          byte[1] = dataString[i];
          byte[2] = '\0';
          i++;
          }
        else
          {
          byte[1] = '\0';
          }
        ((unsigned char *)*dataPointer)[j] = strtol(byte, NULL, 16);
        }
      return &(dataString[i]);
    }
    return dataString;
  }

