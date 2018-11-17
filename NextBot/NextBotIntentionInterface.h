#ifndef NEXTBOT_NEXTBOTINTENTIONINTERFACE_H
#define NEXTBOT_NEXTBOTINTENTIONINTERFACE_H


#include "NextBotComponentInterface.h"
#include "NextBotContextualQueryInterface.h"


class IIntention : public INextBotComponent, public IContextualQuery
{
public:
	IIntention() {}
	virtual ~IIntention() {}
};


#endif
