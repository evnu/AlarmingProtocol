#ifndef _DATATYPES_H
#define _DATATYPES_H

/* Some utilities */
typedef std::list<RTDS_QueueId> RefList;
typedef RefList::iterator RefListIt;

#include <set>
typedef std::set<RTDS_QueueId> RefSet;
typedef RefSet::iterator RefSetIt;


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
struct TransportMessage {
  MessageType mark;
  
  union _payload {
    int cnt;
    RTDS_QueueId appLayer;
    
    _payload (int cnt):
      cnt(cnt) {}
    _payload (RTDS_QueueId appLayer):
      appLayer (appLayer) {}
    _payload (): cnt(0) {}
  } payload;
  
  RefList processes;
  
  TransportMessage (MessageType mark, int cnt):
    mark (mark)
    , payload (cnt)
   {}
   
   TransportMessage (MessageType mark, RTDS_QueueId id):
    mark (mark)
    , payload (id)
   {}
   
   TransportMessage ()
   {}
};


#endif
