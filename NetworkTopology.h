/*
 * NetworkTopology.h
 *
 *  Created on: 09.04.2009
 *      Author: ingmar
 */

#ifndef NETWORKTOPOLOGY_H_
#define NETWORKTOPOLOGY_H_

#include <map>
#include <list>
#include "RTDS_gen.h"

class NetworkTopology {
private:
	int myID;
	RTDS_QueueId myReceiverReference;
	std::map<int, double> connectivityMap;
	std::map<int, RTDS_QueueId> transportReceiverReferences;

	std::map<RTDS_QueueId, int> nodeIds;
public:

	NetworkTopology();
	virtual ~NetworkTopology();

	void setMyId(int id);
	int getMyId();
	
	void setMyReceiverReference(RTDS_QueueId reference);
	RTDS_QueueId getMyReceiverReference();

	double getConnectivity(int id);
	double getConnectivity(RTDS_QueueId teRef);

	void setIdReferenceMapping(int id, RTDS_QueueId reference);
	void setConnectivity(int id, double connectivity);
	
	std::list<RTDS_QueueId> getAllConnectedReferences();
	RTDS_QueueId getReference(int id);
};

typedef NetworkTopology* NetworkTopologyPtr;

#endif /* NETWORKTOPOLOGY_H_ */
