#if defined _player_included
	#endinput
#endif
#define _player_included

public OnPlayerConnect(playerid)
{
	SendClientMessage(playerid, COR_AMARELA, ">> Bem-vindo ao Sobradinho by @and1k99!");
    for(new pInfo:i = pLoggedIn; i <= pVehicleIndex; i++) PlayerInfo[playerid][i] = 0;
	PlayerInfo[playerid][pDueloCom] = -1;
	for(new i=0; i < MAX_PLAYER_VEHICLES; i++) PlayerInfo[playerid][pPlayerVehicles][i] = INVALID_VEHICLE_ID;
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if(PlayerInfo[playerid][pLoggedIn]) SavePlayerData(playerid);
	if(PlayerInfo[playerid][pDueloStatus] >= 2) FinalizarDuelo(PlayerInfo[playerid][pDueloCom], playerid, "desconectou");
	for(new i=0; i < MAX_PLAYER_VEHICLES; i++) if(IsValidVehicle(PlayerInfo[playerid][pPlayerVehicles][i])) DestroyVehicle(PlayerInfo[playerid][pPlayerVehicles][i]);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    // Apenas mostra o di?logo de boas-vindas. Nenhuma chamada ? base de dados aqui para evitar o crash.
    ShowPlayerDialog(playerid, DIALOG_WELCOME, DIALOG_STYLE_MSGBOX, "Bem-vindo!", "Ol?! Voc? deseja entrar na sua conta ou registar uma nova?", "Entrar", "Registar");
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
    if(PlayerInfo[playerid][pLoggedIn]) SpawnPlayer(playerid);
    return 0;
}

public OnPlayerSpawn(playerid)
{
    if(!PlayerInfo[playerid][pLoggedIn]) return Kick(playerid);
    SetPlayerPos(playerid, 1332.1, -2037.0, 13.0);
    SetPlayerInterior(playerid, 0);
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, PlayerInfo[playerid][pMoney]);
    SetPlayerScore(playerid, PlayerInfo[playerid][pScore]);
	GivePlayerWeapon(playerid, 24, 200);
	SendClientMessage(playerid, COR_VERDE, ">> Bem-vindo de volta! Use /comandos para ver as op??es.");
	SetPlayerHealth(playerid, 999999.0);
	SetTimerEx("UnprotectPlayer", 5000, 0, "i", playerid);
	TogglePlayerControllable(playerid, 1);
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
    SavePlayerData(playerid);
	if(PlayerInfo[playerid][pDueloStatus] == 3) FinalizarDuelo(killerid, playerid, "matou");
    SpawnPlayer(playerid);
	return 1;
}