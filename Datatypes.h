#ifndef _DATATYPES
#define _DATATYPES

/* Some utilities */
typedef std::list<RTDS_QueueId> RefList;
typedef RefList::iterator RefListIt;

/* we need to wrap alarming messages */
enum MessageMarker {
   ALERT      /* Message tells about suspected alarm */
  ,FALSEALERT /* Message tells about a false alarm */
};

typedef struct {
  MessageMarker mark;
  
  union payload_t {
  int cnt;
  RTDS_QueueId appLayer;
  } payload;
  
} TransportMessage;


#endif
