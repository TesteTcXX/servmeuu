/*
    M?dulo do Servidor: Sistema de Duelo 1x1 (v1.2 - Sem sscanf)
*/

// --- DECLARACOES (FORWARDS) ---
forward IniciarDuelo(playerid, targetid);
forward FinalizarDuelo(vencedor, perdedor, motivo[]);
forward DueloCountdown(playerid, targetid);
forward DueloCountdown2(playerid, targetid);
forward DueloCountdown3(playerid, targetid);
forward ProcessarComandosDuelo(playerid, const cmd[], const params[]);

// --- PROCESSADOR DE COMANDOS DE DUELO ---
public ProcessarComandosDuelo(playerid, const cmd[], const params[])
{
	if(strcmp(cmd, "duelo", true) == 0)
	{
		if(!strlen(params)) return SendClientMessage(playerid, COR_CINZA, "USO: /duelo [id do jogador]");

		new targetid = strval(params);
		if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, COR_VERMELHA, "ERRO: Este jogador nao esta online.");
		if(targetid == playerid) return SendClientMessage(playerid, COR_VERMELHA, "ERRO: Voce nao pode duelar contra si mesmo.");
		if(PlayerInfo[playerid][pDueloStatus] != 0) return SendClientMessage(playerid, COR_VERMELHA, "ERRO: Voce ja esta envolvido em um duelo.");
		if(PlayerInfo[targetid][pDueloStatus] != 0) return SendClientMessage(playerid, COR_VERMELHA, "ERRO: Este jogador ja esta envolvido em um duelo.");

        new sendername[MAX_PLAYER_NAME], targetname[MAX_PLAYER_NAME], str[128];
		GetPlayerName(playerid, sendername, sizeof(sendername));
		GetPlayerName(targetid, targetname, sizeof(targetname));

		format(str, sizeof(str), "INFO: Voce desafiou %s para um duelo 1x1. Ele tem 20 segundos para aceitar.", targetname);
		SendClientMessage(playerid, COR_AMARELA, str);
		format(str, sizeof(str), "INFO: %s te desafiou para um duelo 1x1! Digite /aceitar para lutar ou /recusar.", sendername);
		SendClientMessage(targetid, COR_AMARELA, str);

		PlayerInfo[playerid][pDueloStatus] = 1;
		PlayerInfo[playerid][pDueloCom] = targetid;
		PlayerInfo[targetid][pDueloStatus] = 2;
		PlayerInfo[targetid][pDueloCom] = playerid;
		return 1;
	}
	if(strcmp(cmd, "aceitar", true) == 0)
	{
		if(PlayerInfo[playerid][pDueloStatus] != 2) return SendClientMessage(playerid, COR_VERMELHA, "ERRO: Voce nao foi desafiado para um duelo.");
		new desafiante = PlayerInfo[playerid][pDueloCom];
		if(!IsPlayerConnected(desafiante)) return SendClientMessage(playerid, COR_VERMELHA, "ERRO: O jogador que te desafiou desconectou.");

		IniciarDuelo(desafiante, playerid);
		return 1;
	}
	if(strcmp(cmd, "recusar", true) == 0)
	{
		if(PlayerInfo[playerid][pDueloStatus] != 2) return SendClientMessage(playerid, COR_VERMELHA, "ERRO: Voce nao foi desafiado para um duelo.");
		new desafiante = PlayerInfo[playerid][pDueloCom];

		new name[MAX_PLAYER_NAME], str[128];
		GetPlayerName(playerid, name, sizeof(name));
		format(str, sizeof(str), "INFO: %s recusou seu desafio de duelo.", name);
		if(IsPlayerConnected(desafiante)) SendClientMessage(desafiante, COR_AMARELA, str);
		SendClientMessage(playerid, COR_AMARELA, "INFO: Voce recusou o duelo.");

		PlayerInfo[playerid][pDueloStatus] = 0;
		PlayerInfo[playerid][pDueloCom] = -1;
		if(IsPlayerConnected(desafiante))
		{
		    PlayerInfo[desafiante][pDueloStatus] = 0;
		    PlayerInfo[desafiante][pDueloCom] = -1;
		}
		return 1;
	}
	if(strcmp(cmd, "cancelar", true) == 0)
	{
		if(PlayerInfo[playerid][pDueloStatus] != 1) return SendClientMessage(playerid, COR_VERMELHA, "ERRO: Voce nao desafiou ninguem.");
		new desafiado = PlayerInfo[playerid][pDueloCom];

		if(IsPlayerConnected(desafiado)) SendClientMessage(desafiado, COR_AMARELA, "INFO: O desafio de duelo foi cancelado.");
		SendClientMessage(playerid, COR_AMARELA, "INFO: Voce cancelou seu desafio.");

		PlayerInfo[playerid][pDueloStatus] = 0;
		PlayerInfo[playerid][pDueloCom] = -1;
		if(IsPlayerConnected(desafiado))
		{
		    PlayerInfo[desafiado][pDueloStatus] = 0;
		    PlayerInfo[desafiado][pDueloCom] = -1;
		}
		return 1;
	}
	return 0;
}

public IniciarDuelo(playerid, targetid)
{
    GetPlayerPos(playerid, pOldPosX[playerid], pOldPosY[playerid], pOldPosZ[playerid]);
    pOldInterior[playerid] = GetPlayerInterior(playerid);
    GetPlayerPos(targetid, pOldPosX[targetid], pOldPosY[targetid], pOldPosZ[targetid]);
    pOldInterior[targetid] = GetPlayerInterior(targetid);

    SetPlayerPos(playerid, 1493.5, -1770.8, 18.7);
    SetPlayerFacingAngle(playerid, 0.0);
    SetPlayerInterior(playerid, 0);
    SetPlayerPos(targetid, 1512.5, -1770.8, 18.7);
    SetPlayerFacingAngle(targetid, 180.0);
    SetPlayerInterior(targetid, 0);

    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 100.0);
    ResetPlayerWeapons(playerid);
    GivePlayerWeapon(playerid, 24, 200);
    GivePlayerWeapon(playerid, 31, 500);
    SetPlayerHealth(targetid, 100.0);
    SetPlayerArmour(targetid, 100.0);
    ResetPlayerWeapons(targetid);
    GivePlayerWeapon(targetid, 24, 200);
    GivePlayerWeapon(targetid, 31, 500);

    PlayerInfo[playerid][pDueloStatus] = 3;
    PlayerInfo[targetid][pDueloStatus] = 3;
    TogglePlayerControllable(playerid, 0);
    TogglePlayerControllable(targetid, 0);

    GameTextForAll("~r~3", 1000, 3);
	SetTimerEx("DueloCountdown", 1000, false, "ii", playerid, targetid);

	new pName[MAX_PLAYER_NAME], tName[MAX_PLAYER_NAME], str[128];
	GetPlayerName(playerid, pName, sizeof(pName));
	GetPlayerName(targetid, tName, sizeof(tName));
	format(str, sizeof(str), "DUELO: %s vs %s comecou!", pName, tName);
	SendClientMessageToAll(COR_VERMELHA, str);
	return 1;
}

public FinalizarDuelo(vencedor, perdedor, motivo[])
{
    if(IsPlayerConnected(vencedor))
    {
        TogglePlayerControllable(vencedor, 1);
        SetPlayerPos(vencedor, pOldPosX[vencedor], pOldPosY[vencedor], pOldPosZ[vencedor]);
        SetPlayerInterior(vencedor, pOldInterior[vencedor]);
        PlayerInfo[vencedor][pDueloStatus] = 0;
        PlayerInfo[vencedor][pDueloCom] = -1;
    }
    if(IsPlayerConnected(perdedor))
    {
        SpawnPlayer(perdedor);
        PlayerInfo[perdedor][pDueloStatus] = 0;
        PlayerInfo[perdedor][pDueloCom] = -1;
    }

    new vName[MAX_PLAYER_NAME], pName[MAX_PLAYER_NAME], str[128];
    GetPlayerName(vencedor, vName, sizeof(vName));
    GetPlayerName(perdedor, pName, sizeof(pName));
    format(str, sizeof(str), "DUELO: %s venceu o duelo contra %s (%s)!", vName, pName, motivo);
    SendClientMessageToAll(COR_VERDE, str);
    return 1;
}

public DueloCountdown(playerid, targetid)
{
	GameTextForAll("~r~2", 1000, 3);
	SetTimerEx("DueloCountdown2", 1000, false, "ii", playerid, targetid);
	return 1;
}

public DueloCountdown2(playerid, targetid)
{
	GameTextForAll("~r~1", 1000, 3);
	SetTimerEx("DueloCountdown3", 1000, false, "ii", playerid, targetid);
	return 1;
}

public DueloCountdown3(playerid, targetid)
{
	GameTextForAll("~g~LUTE!", 2000, 3);
	if(IsPlayerConnected(playerid)) TogglePlayerControllable(playerid, 1);
	if(IsPlayerConnected(targetid)) TogglePlayerControllable(targetid, 1);
	return 1;
}