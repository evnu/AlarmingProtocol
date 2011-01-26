/*
 * NetworkTopology.cpp
 *
 *  Created on: 09.04.2009
 *      Author: ingmar
 */

#include "NetworkTopology.h"

NetworkTopology::NetworkTopology() {
}

NetworkTopology::~NetworkTopology() {
}

int NetworkTopology::getMyId() {
	return myID;
}

void NetworkTopology::setMyId(int id) {
	this->myID = id;
}


void NetworkTopology::setMyReceiverReference(RTDS_QueueId reference) {
  this->myReceiverReference = reference;
}

RTDS_QueueId NetworkTopology::getMyReceiverReference() {
  return myReceiverReference;
}

void NetworkTopology::setConnectivity(int id, double connectivity) {
	connectivityMap[id] = connectivity;
}

double NetworkTopology::getConnectivity(int id) {
	return connectivityMap[id];
}

double NetworkTopology::getConnectivity(RTDS_QueueId reference) {
	int id = nodeIds[reference];
	return connectivityMap[id];
}

void NetworkTopology::setIdReferenceMapping(int id, RTDS_QueueId reference) {
	transportReceiverReferences[id] = reference;
	nodeIds[reference] = id;
}

RTDS_QueueId NetworkTopology::getReference(int id) {
  return transportReceiverReferences[id];
}

std::list<RTDS_QueueId> NetworkTopology::getAllConnectedReferences() {
  std::list<RTDS_QueueId> connectedReferences;
  std::map<int,RTDS_QueueId>::iterator it;
  for (it = transportReceiverReferences.begin(); it != transportReceiverReferences.end(); it++) {
    connectedReferences.push_back((*it).second);
  }
  return connectedReferences;
}
