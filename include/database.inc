#if defined _database_included
	#endinput
#endif
#define _database_included

// ==============================================================================
// FUN??ES DE DADOS DO JOGADOR
// ==============================================================================

forward OnAccountCheck_Register(playerid, const pass[]);
public OnAccountCheck_Register(playerid, const pass[]) {
    if(!IsPlayerConnected(playerid)) return 1;

    if(cache_get_row_count())
    {
        ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Registo - ERRO", "{FF0000}Este nome de utilizador j? est? registado.\n\n{FFFFFF}Tente fazer login ou escolha outro nome.", "Registar", "Voltar");
    }
    else
    {
        new playerName[MAX_PLAYER_NAME], escaped_name[MAX_PLAYER_NAME*2], escaped_pass[128], query[256];
        GetPlayerName(playerid, playerName, sizeof(playerName));
        mysql_escape_string(playerName, escaped_name, sizeof(escaped_name));
        mysql_escape_string(pass, escaped_pass, sizeof(escaped_pass));
        format(query, sizeof(query), "INSERT INTO `users` (`username`, `password`, `money`, `score`, `admin`) VALUES ('%s', '%s', 1000, 0, 0)", escaped_name, escaped_pass);
        mysql_tquery(g_mysql_connection, query, "OnPlayerRegister", "ss", playerName, pass);
    }
    return 1;
}

forward OnPlayerDataLoaded(playerid);
public OnPlayerDataLoaded(playerid)
{
    if(!IsPlayerConnected(playerid)) return 1;

	if(cache_get_row_count())
	{
		PlayerInfo[playerid][pLoggedIn] = true;
		cache_get_value_name_int(0, "id", PlayerInfo[playerid][pID]);
		cache_get_value_name_int(0, "admin", PlayerInfo[playerid][pAdmin]);
		cache_get_value_name_int(0, "money", PlayerInfo[playerid][pMoney]);
		cache_get_value_name_int(0, "score", PlayerInfo[playerid][pScore]);
		SendClientMessage(playerid, COR_VERDE, "Login efetuado com sucesso!");
		SpawnPlayer(playerid);
	}
	else
	{
		ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login - ERRO", "{FF0000}Palavra-passe incorreta.\n\n{FFFFFF}Por favor, introduza a sua palavra-passe novamente:", "Entrar", "Voltar");
	}
    return 1;
}

forward OnPlayerRegister(playerid, const name[], const pass[]);
public OnPlayerRegister(playerid, const name[], const pass[]) {
    if(!IsPlayerConnected(playerid)) return 1;
    SendClientMessage(playerid, COR_VERDE, "A sua conta foi registada com sucesso!");
    AttemptLogin(playerid, name, pass);
    return 1;
}

stock AttemptRegister(playerid, const name[], const password[])
{
    new escaped_name[MAX_PLAYER_NAME*2], query[128];
    mysql_escape_string(name, escaped_name, sizeof(escaped_name));
    format(query, sizeof(query), "SELECT `id` FROM `users` WHERE `username` = '%s'", escaped_name);
    mysql_tquery(g_mysql_connection, query, "OnAccountCheck_Register", "s", password);
}

stock AttemptLogin(playerid, const name[], const password[])
{
    new escaped_name[MAX_PLAYER_NAME*2], escaped_pass[128], query[256];
    mysql_escape_string(name, escaped_name, sizeof(escaped_name));
    mysql_escape_string(password, escaped_pass, sizeof(escaped_pass));
    format(query, sizeof(query), "SELECT * FROM `users` WHERE `username` = '%s' AND `password` = '%s'", escaped_name, escaped_pass);
    mysql_tquery(g_mysql_connection, query, "OnPlayerDataLoaded", "i", playerid);
}

stock SavePlayerData(playerid)
{
    if(!PlayerInfo[playerid][pLoggedIn]) return;
    PlayerInfo[playerid][pMoney] = GetPlayerMoney(playerid);
    PlayerInfo[playerid][pScore] = GetPlayerScore(playerid);
    new query[256];
    format(query, sizeof(query), "UPDATE `users` SET `money` = %d, `score` = %d WHERE `id` = %d", PlayerInfo[playerid][pMoney], PlayerInfo[playerid][pScore], PlayerInfo[playerid][pID]);
    mysql_tquery(g_mysql_connection, query, "", "");
}