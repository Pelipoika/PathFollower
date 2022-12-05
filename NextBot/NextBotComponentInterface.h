#ifndef NEXTBOT_NEXTBOTCOMPONENTINTERFACE_H
#define NEXTBOT_NEXTBOTCOMPONENTINTERFACE_H


#include "NextBotEventResponderInterface.h"


class INextBot;


class INextBotComponent : public INextBotEventResponder
{
public:
	INextBotComponent() {}
	virtual ~INextBotComponent() {}
	
	virtual void Reset()                = 0;
	virtual void Update()               = 0;
	virtual void Upkeep()               = 0;
	virtual INextBot *GetBot() const    = 0;
	virtual void *GetScriptDesc() const = 0;
	
private:
	uint8_t pad[0x14];
};


#endif
