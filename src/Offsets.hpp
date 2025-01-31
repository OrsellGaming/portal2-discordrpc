#pragma once

namespace Offsets {

// CCvar
extern int RegisterConCommand;
extern int UnregisterConCommand;
extern int FindCommandBase;
extern int m_pConCommandList;

// CEngineClient
extern int ClientCmd;
extern int GetMaxClients;
extern int GetGameDirectory;
extern int IsInCommentaryMode;

// ConVar
extern int InternalSetValue;
extern int InternalSetFloatValue;
extern int InternalSetIntValue;

// CClientState
extern int SetSignonState;
extern int Disconnect;

// CDemoPlayer
extern int IsPlayingBack;

// CEngineTool
extern int GetCurrentMap;

// Others
extern int GetClientStateFunction;
extern int cl;
extern int demoplayer;
extern int AutoCompletionFunc;
extern int HostState_OnClientConnected;
extern int hoststate;
extern int CL_IsRecordingMovie;
extern int m_szLevelName;
}
