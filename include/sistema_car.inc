/*
    M?dulo do Servidor: Comando /car
    - Cria um ve?culo e coloca o jogador diretamente dentro.
    - Otimizado para jogadores de PC.
*/

forward ProcessarComandoCar(playerid, const cmd[], const params[]);

public ProcessarComandoCar(playerid, const cmd[], const params[])
{
	if(strcmp(cmd, "car", true) == 0)
	{
		if(!strlen(params)) return SendClientMessage(playerid, COR_CINZA, "USO: /car [id do veiculo]");
		new carid = strval(params);
		if(carid < 400 || carid > 611) return SendClientMessage(playerid, COR_VERMELHA, "ERRO: ID de veiculo invalido (400-611).");

        new sendername[MAX_PLAYER_NAME];
        GetPlayerName(playerid, sendername, sizeof(sendername));

        // Usa o mesmo sistema de limite de 2 ve?culos
        new idx = PlayerInfo[playerid][pVehicleIndex];
        if(IsValidVehicle(PlayerInfo[playerid][pPlayerVehicles][idx]))
        {
            DestroyVehicle(PlayerInfo[playerid][pPlayerVehicles][idx]);
        }

		new Float:x, Float:y, Float:z, angle;
		GetPlayerPos(playerid, x, y, z);
		GetPlayerFacingAngle(playerid, angle);
		new created_vehicle = CreateVehicle(carid, x, y+3.0, z, angle, -1, -1, 60000);
		SetVehicleNumberPlate(created_vehicle, sendername);

		PlayerInfo[playerid][pPlayerVehicles][idx] = created_vehicle;
		PlayerInfo[playerid][pVehicleIndex]++;
		if(PlayerInfo[playerid][pVehicleIndex] >= MAX_PLAYER_VEHICLES)
		{
		    PlayerInfo[playerid][pVehicleIndex] = 0;
		}

        // AQUI EST? A L?GICA CORRETA: Ativa um timer para chamar a fun??o que te coloca no carro
		SetTimerEx("PutPlayerInVehicle_Fix", 750, false, "ii", playerid, created_vehicle);
		SendClientMessage(playerid, COR_AZUL, "Seu veiculo chegou!");
		return 1;
	}
	return 0;
}