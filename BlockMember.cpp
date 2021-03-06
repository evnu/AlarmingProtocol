/*
 * BlockMember.cpp
 *
 *  Created on: 09.04.2009
 *      Author: ingmar
 */

#include "BlockMember.h"

BlockMember::BlockMember() {
}

BlockMember::~BlockMember() {
}

void BlockMember::addBlockMember(std::string processName, RTDS_QueueId reference) {
	blockMember[processName] = reference;
}

const RTDS_QueueId& BlockMember::operator[] (std::string processName) {
	return blockMember[processName];
}

RTDS_QueueId BlockMember::getBlockMember(std::string processName) {
  return blockMember[processName];
}
