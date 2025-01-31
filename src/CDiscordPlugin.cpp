#include "CDiscordPlugin.hpp"

#include "Modules/Console.hpp"
#include "Modules/Engine.hpp"
#include "Modules/Module.hpp"
#include "Modules/Steam.hpp"
#include "Modules/Tier1.hpp"

#include "Game.hpp"
#include "SDK.hpp"

CDiscordPlugin g_DiscordPlugin;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CDiscordPlugin, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_DiscordPlugin);

bool CDiscordPlugin::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
    console = new Console();
    if (!console->Init())
        return false;

    this->game = Game::CreateNew();
    if (this->game) {
        this->game->LoadOffsets();

        this->modules = new Modules();
        this->modules->AddModule<Tier1>(&tier1);
        this->modules->InitAll();

        if (tier1->hasLoaded) {
            this->modules->AddModule<Engine>(&engine);
            this->modules->AddModule<Steam>(&steam);
            this->modules->InitAll();

            if (engine->hasLoaded) {
                if (steam->hasLoaded) {
                    this->discord = new Discord();
                    this->StartMainThread();

                    this->plugin = new Plugin();
                    this->StartPluginThread();

                    console->PrintActive("Loaded sugar-glider plugin, Version %s (by NeKz)\n", this->Version());
                    return true;
                } else {
                    console->Warning("SGP: Failed to load steam module!\n");
                }
            } else {
                console->Warning("SGP: Failed to load engine module!\n");
            }
        } else {
            console->Warning("SGP: Failed to load tier1 module!\n");
        }
    } else {
        console->Warning("SGP: Game not supported!\n");
    }

    console->Warning("SGP: sugar-glider plugin failed to load!\n");
    return false;
}
void CDiscordPlugin::Unload()
{
    this->Cleanup();
}
const char* CDiscordPlugin::GetPluginDescription()
{
    return SGP_SIGNATURE;
}

void CDiscordPlugin::Cleanup()
{
    if (console)
        console->Print("SGP: Cya!\n");

    this->isRunning = false;
    if (this->mainThread.joinable())
        this->mainThread.detach();

    SAFE_UNLOAD(this->game);
    SAFE_UNLOAD(this->plugin);
    SAFE_UNLOAD(this->modules);
    SAFE_UNLOAD(console);
}

// We don't use any callbacks
void CDiscordPlugin::StartPluginThread()
{
    this->pluginThread = std::thread([this]() {
        GO_THE_FUCK_TO_SLEEP(1000);
        if (!this->plugin->Find()) {
            console->DevWarning("SGP: Failed to find sugar-glider plugin in the plugin list!\n");
        } else {
            this->plugin->ptr->m_bDisable = true;
        }
    });
    this->pluginThread.detach();
}
// Main loop
void CDiscordPlugin::StartMainThread()
{
    this->isRunning = true;
    this->mainThread = std::thread([this]() {
        if (this->discord->Init()) {
            this->discord->SendPresence();
            while (this->isRunning && engine->hoststate->m_currentState != HOSTSTATES::HS_SHUTDOWN) {
                this->discord->Update();
                GO_THE_FUCK_TO_SLEEP(60);
            }
        }
        SAFE_UNLOAD(this->discord);
    });
}
// Might fix potential deadlock
#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_DETACH) {
        g_DiscordPlugin.Cleanup();
    }
    return TRUE;
}
#endif

#pragma region Unused callbacks
void CDiscordPlugin::Pause()
{
}
void CDiscordPlugin::UnPause()
{
}
void CDiscordPlugin::LevelInit(char const* pMapName)
{
}
void CDiscordPlugin::ServerActivate(void* pEdictList, int edictCount, int clientMax)
{
}
void CDiscordPlugin::GameFrame(bool simulating)
{
}
void CDiscordPlugin::LevelShutdown()
{
}
void CDiscordPlugin::ClientFullyConnect(void* pEdict)
{
}
void CDiscordPlugin::ClientActive(void* pEntity)
{
}
void CDiscordPlugin::ClientDisconnect(void* pEntity)
{
}
void CDiscordPlugin::ClientPutInServer(void* pEntity, char const* playername)
{
}
void CDiscordPlugin::SetCommandClient(int index)
{
}
void CDiscordPlugin::ClientSettingsChanged(void* pEdict)
{
}
int CDiscordPlugin::ClientConnect(bool* bAllowConnect, void* pEntity, const char* pszName, const char* pszAddress, char* reject, int maxrejectlen)
{
    return 0;
}
int CDiscordPlugin::ClientCommand(void* pEntity, const void*& args)
{
    return 0;
}
int CDiscordPlugin::NetworkIDValidated(const char* pszUserName, const char* pszNetworkID)
{
    return 0;
}
void CDiscordPlugin::OnQueryCvarValueFinished(int iCookie, void* pPlayerEntity, int eStatus, const char* pCvarName, const char* pCvarValue)
{
}
void CDiscordPlugin::OnEdictAllocated(void* edict)
{
}
void CDiscordPlugin::OnEdictFreed(const void* edict)
{
}
#pragma endregion
