#ifndef _INCLUDE_PLUGINBOT_PLUGINBOTCOMMON_H_
#define _INCLUDE_PLUGINBOT_PLUGINBOTCOMMON_H_


#if defined(_MSC_VER)
#include <intrin.h>
#define BREAK() __debugbreak()
#else
#define BREAK() asm volatile ( "int 3" : : : "memory" )
#endif


inline void StubWarn(const char *fmt, const char *func)
{
	if (cv_pathfollower_stubs.GetBool()) {
		DevWarning(fmt, func);
	}
}


#define STUB_LOCO()    do { StubWarn("STUB: [PluginBotLoco   %s]\n", __FUNCTION__); } while (false)
#define STUB_BODY()    do { StubWarn("STUB: [PluginBotBody   %s]\n", __FUNCTION__); } while (false)
#define STUB_INTENT()  do { StubWarn("STUB: [PluginBotIntent %s]\n", __FUNCTION__); } while (false)
#define STUB_VISION()  do { StubWarn("STUB: [PluginBotVision %s]\n", __FUNCTION__); } while (false)
#define STUB_NEXTBOT() do { StubWarn("STUB: [PluginBot       %s]\n", __FUNCTION__); } while (false)


/* using these instead of vec3_origin/vec3_angle because we can bind a non-const reference to these ones */
static Vector vecStub(0.0f, 0.0f, 0.0f);
static QAngle angStub(0.0f, 0.0f, 0.0f);


#endif // _INCLUDE_PLUGINBOT_PLUGINBOTCOMMON_H_
