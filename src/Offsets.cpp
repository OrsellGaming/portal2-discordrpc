#include "Offsets.hpp"

namespace Offsets {

// CCvar
int RegisterConCommand;
int UnregisterConCommand;
int FindCommandBase;
int m_pConCommandList;

// CEngineClient
int ClientCmd;
int GetMaxClients;
int ServerCmdKeyValues;

// ConVar
int InternalSetValue;
int InternalSetFloatValue;
int InternalSetIntValue;

// CGameEventManager
int AddListener;
int RemoveListener;
int FireEventClientSide;
int FireEventIntern;
int ConPrintEvent;

// CClientState
int SetSignonState;
int Disconnect;

// CDemoPlayer
int IsPlayingBack;

// Others
int GetClientStateFunction;
int cl;
int demoplayer;
int AutoCompletionFunc;
int HostState_OnClientConnected;
int hoststate;
int CL_IsRecordingMovie;
}