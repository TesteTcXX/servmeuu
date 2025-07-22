#if defined _commands_included
	#endinput
#endif
#define _commands_included

public OnPlayerDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(!response && dialogid != DIALOG_WELCOME && !PlayerInfo[playerid][pLoggedIn])
    {
        Kick(playerid);
        return 1;
    }

    switch(dialogid)
    {
        case DIALOG_WELCOME:
        {
            if(response) // Bot?o da Esquerda (Entrar)
            {
                ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login", "Por favor, introduza a sua palavra-passe:", "Entrar", "Voltar");
            }
            else // Bot?o da Direita (Registar)
            {
                ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Registo", "Por favor, crie uma palavra-passe para a sua nova conta:", "Registar", "Voltar");
            }
            return 1;
        }
        case DIALOG_REGISTER:
        {
            if(!response) return ShowPlayerDialog(playerid, DIALOG_WELCOME, DIALOG_STYLE_MSGBOX, "Bem-vindo!", "Ol?! Voc? deseja entrar na sua conta ou registar uma nova?", "Entrar", "Registar");
            if(strlen(inputtext) < 4) return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, "Registo - ERRO", "{FF0000}A sua palavra-passe deve ter no m?nimo 4 caracteres.\n\n{FFFFFF}Por favor, crie uma palavra-passe:", "Registar", "Voltar");
            
            new playerName[MAX_PLAYER_NAME];
            GetPlayerName(playerid, playerName, sizeof(playerName));
            AttemptRegister(playerid, playerName, inputtext);
            return 1;
        }
        case DIALOG_LOGIN:
        {
            if(!response) return ShowPlayerDialog(playerid, DIALOG_WELCOME, DIALOG_STYLE_MSGBOX, "Bem-vindo!", "Ol?! Voc? deseja entrar na sua conta ou registar uma nova?", "Entrar", "Registar");
            if(!strlen(inputtext)) return ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, "Login - ERRO", "{FF0000}Voc? precisa de digitar uma palavra-passe.\n\n{FFFFFF}Por favor, digite a sua palavra-passe:", "Entrar", "Voltar");

            new playerName[MAX_PLAYER_NAME];
            GetPlayerName(playerid, playerName, sizeof(playerName));
            AttemptLogin(playerid, playerName, inputtext);
            return 1;
        }
    }
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if(!PlayerInfo[playerid][pLoggedIn])
	{
		return SendClientMessage(playerid, COR_VERMELHA, "Voc? precisa estar logado para usar comandos.");
	}
    
    new cmd[128], params[128];
    new space = strfind(cmdtext, " ", false);
    if(space == -1)
    {
        strmid(cmd, cmdtext, 1, strlen(cmdtext));
        params[0] = EOS;
    }
    else
    {
        strmid(cmd, cmdtext, 1, space);
        strmid(params, cmdtext, space + 1, strlen(cmdtext));
    }

	if(ProcessarComandosDuelo(playerid, cmd, params)) return 1;
	if(ProcessarComandoCar(playerid, cmd, params)) return 1;

	if(strcmp(cmd, "comandos", true) == 0)
	{
		SendClientMessage(playerid, -1, "--- COMANDOS DE JOGADOR ---");
		SendClientMessage(playerid, COR_AZUL, "/locais, /tp, /veh, /reparar, /pular, /cor, /jetpack, /flip, /nos, /armas, /mp, /vida, /colete");
		SendClientMessage(playerid, COR_AZUL, "/duelo, /aceitar, /recusar, /cancelar, /car");

		if(PlayerInfo[playerid][pAdmin] > 0)
		{
			SendClientMessage(playerid, COR_VERMELHA, "--- COMANDOS DE ADMIN ---");
			SendClientMessage(playerid, COR_VERMELHA, "/get, /goto, /kick, /ban, /allweapons, /anuncio, /god, /clima");
		}
		return 1;
	}
	return 1;
}