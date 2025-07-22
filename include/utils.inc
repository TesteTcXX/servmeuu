#if defined _utils_included
	#endinput
#endif
#define _utils_included

forward UnprotectPlayer(playerid);
public UnprotectPlayer(playerid)
{
    if(IsPlayerConnected(playerid))
    {
        SetPlayerHealth(playerid, 100.0);
        SendClientMessage(playerid, COR_VERMELHA, "INFO: Sua protecao de spawn acabou.");
    }
    return 1;
}

forward PutPlayerInVehicle_Fix(playerid, vehicleid);
public PutPlayerInVehicle_Fix(playerid, vehicleid)
{
    if(IsPlayerConnected(playerid) && IsValidVehicle(vehicleid))
    {
        PutPlayerInVehicle(playerid, vehicleid, 0);
    }
    return 1;
}