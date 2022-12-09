#if defined _Plugin_Protect_included
 #endinput
#endif
#define _Plugin_Protect_included

#define REQUIRE_EXTENSIONS
#define REQUIRE_PLUGIN
#include <SteamWorks>

/* <----> Add the servers IPs here, which can run the scripts. <----> */
static char cServers[][] = {
		"0.0.0.0:27400",
		"0.0.0.0:27500"
	};

/* <----> Do not edit below this line <----> */
stock void PluginProtect_IsValidServer()
{
	if (!SteamWorks_IsLoaded() || !SteamWorks_IsConnected())
	{
		PluginProtect_SelfDestroy("SteamWorks is required to run to use this plugin.");
		return;
	}

	static char cIP[32];

	if (cIP[0] == '\0')
	{
		int iIP[4];
		if (!SteamWorks_GetPublicIP(iIP))
		{
			PluginProtect_SelfDestroy("Failed to get the public IP address through SteamWorks.");
			return;
		}

		FormatEx(cIP, sizeof(cIP), "%d.%d.%d.%d:%d", iIP[0], iIP[1], iIP[2], iIP[3], GetConVarInt(FindConVar("hostport")));
	}

	int iServerNum = sizeof(cServers);
	for (int iServer = 0; iServer < iServerNum; iServer++)
	{
		if (strcmp(cIP, cServers[iServer], false) == 0)
		{
			return;
		}
	}

	PluginProtect_SelfDestroy("This server is not allowed to run this plugin.");
	return;
}

stock void PluginProtect_SelfDestroy(const char[] sMessage)
{
	LogError(sMessage);

	char cFileName[64];
	GetPluginFilename(INVALID_HANDLE, cFileName, sizeof(cFileName));
	ServerCommand("sm plugins unload %s", cFileName);
}

public void OnPluginStart()
{
	PluginProtect_IsValidServer();
	_REAL_OnPluginStart();
}

#define OnPluginStart _REAL_OnPluginStart
