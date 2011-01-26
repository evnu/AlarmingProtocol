/*
 * BlockMember.h
 *
 *  Created on: 09.04.2009
 *      Author: ingmar
 */

#ifndef BLOCKMEMBER_H_
#define BLOCKMEMBER_H_

#include <map>
#include <string>
#include "RTDS_gen.h"

class BlockMember {
public:
	BlockMember();
	virtual ~BlockMember();

	void addBlockMember(std::string processName, RTDS_QueueId reference);
	
	RTDS_QueueId getBlockMember(std::string processName);

	// it's easier to just overload an operator than using a getter method..
	const RTDS_QueueId& operator[] (std::string processName);

private:
	std::map<std::string, RTDS_QueueId> blockMember;
};

typedef BlockMember* BlockMemberPtr;

#endif /* BLOCKMEMBER_H_ */
