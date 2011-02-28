#ifndef _DATATYPES_H
#define _DATATYPES_H

/* Some utilities */
typedef std::list<RTDS_QueueId> RefList;
typedef RefList::iterator RefListIt;

/* we need to wrap alarming messages */
enum MessageType {
   ALERT      /* Message tells about suspected alarm */
  ,FALSEALERT /* Message tells about a false alarm */
  ,REAWAKE    /* Reawake the app layer if it was suspended after a global alarm */
};

/*
  TransportMessage
  The following struct describes the payload which the messages broadcast and unicast
  carry between two process instances of type pTransportSender/pTransportReceiver.
  Such a message carries the payload from an application layer - Therefore, it is marked
  with the type MessageType and contains either an integer cnt if it is marked as ALERT,
  or a RTDS_QueueId appLayer if marked as FALSEALERT. If such a message is marked REAWAKE,
  the contained payload isn't initialized and must be ignored.
*/
typedef struct TransportMessage {
  MessageType mark;
  
  union {
    int cnt;
    RTDS_QueueId appLayer;
  } payload;
  
  int hops;
  
} TransportMessage;


#endif
