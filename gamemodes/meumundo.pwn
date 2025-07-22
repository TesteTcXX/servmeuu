/*
    GAMEMODE DEFINITIVO: SOBRADINHO by @and1k99 (v26.0 - 100% Est?vel, Sem sscanf)
    - Removida completamente a depend?ncia do plugin sscanf para m?xima estabilidade.
    - Corrigidos todos os erros de compila??o e de execu??o.
    - Adicionada a fun??o main() que estava em falta.
*/

#define SAMP_COMPAT

#include <a_samp>
#include <streamer>
#include <a_mysql>

// --- CONFIGURACOES GERAIS E BANCO DE DADOS ---
#define MYSQL_HOST "127.0.0.1"
#define MYSQL_USER "root"
#define MYSQL_PASS "94009216a"
#define MYSQL_DB   "bddrift"

new MySQL:g_mysql_connection;

#define MAX_PLAYER_VEHICLES 2

// --- CORES ---
#define COR_AZUL        0x33CCFFFF
#define COR_VERDE       0x00FF00FF
#define COR_VERMELHA    0xFF0000FF
#define COR_AMARELA     0xFFFF00AA
#define COR_CINZA       0xCCCCCCAA

// --- DIALOGS ---
#define DIALOG_ARMAS    1
#define DIALOG_LOCAIS   2
#define DIALOG_REGISTER 3
#define DIALOG_LOGIN    4
#define DIALOG_WELCOME  5

// --- ESTRUTURAS DE DADOS E VARI?VEIS GLOBAIS ---
enum pInfo
{
    pLoggedIn, pID, pAdmin, pMoney, pScore,
    pDueloCom, pDueloStatus, pLastTpTime, pLastFogosTime,
    pPlayerVehicles[MAX_PLAYER_VEHICLES], pVehicleIndex,
    pAccountExists
}
new PlayerInfo[MAX_PLAYERS][pInfo];

new Float:pOldPosX[MAX_PLAYERS];
new Float:pOldPosY[MAX_PLAYERS];
new Float:pOldPosZ[MAX_PLAYERS];
new pOldInterior[MAX_PLAYERS];

// --- DECLARA??ES (FORWARDS) ---
forward CheckarConta(playerid);
forward VerificarLogin(playerid);

// --- INCLUS?O DOS M?Dulos ---
#include <database>
#include <player>
#include <commands>
#include <utils>
#include <sistema_duelo>
#include <sistema_car>

main()
{
    print("\n---------------------------------------------------");
    print("      Gamemode 'Sobradinho' est? a iniciar...      ");
    print("---------------------------------------------------\n");
}

public OnGameModeInit()
{
    SetGameModeText("Sobradinho Modular");
    ShowPlayerMarkers(1);
    DisableInteriorEnterExits();
    EnableStuntBonusForAll(0);

    g_mysql_connection = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DB);
    if(mysql_errno(g_mysql_connection) != 0) {
        print("!!! ERRO CR?TICO: Falha ao conectar no banco de dados MySQL.");
    } else {
        print("--- SUCESSO: Conectado ao banco de dados MySQL.");
        mysql_tquery(g_mysql_connection, "SET NAMES 'utf8mb4'", "", "");
    }

    AddPlayerClass(0, 1332.1, -2037.0, 13.0, 0.0, 0, 0, 0, 0, 0, 0);
    print("\n> Gamemode 'Sobradinho v26.0' (Est?vel) Carregado.\n");
    return 1;
}

public OnGameModeExit() {
    mysql_close(g_mysql_connection);
    return 1;
}

public OnPlayerConnect(playerid)
{
    new nome[MAX_PLAYER_NAME];
    GetPlayerName(playerid, nome, sizeof(nome));

    new query[128];
    format(query, sizeof(query), "SELECT * FROM usuarios WHERE nome = '%s' LIMIT 1", nome);
    mysql_tquery(g_mysql_connection, query, "CheckarConta", "i", playerid);
    return 1;
}

public CheckarConta(playerid)
{
    if(cache_num_rows())
    {
        PlayerInfo[playerid][pAccountExists] = 1;
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_INPUT, "Login", "Digite sua senha:", "Login", "Sair");
    }
    else
    {
        PlayerInfo[playerid][pAccountExists] = 0;
        ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_INPUT, "Registrar", "Digite sua nova senha:", "Registrar", "Sair");
    }
}

public VerificarLogin(playerid)
{
    if(cache_num_rows())
    {
        PlayerInfo[playerid][pLoggedIn] = 1;
        SendClientMessage(playerid, COR_VERDE, "Login realizado com sucesso!");
    }
    else
    {
        SendClientMessage(playerid, COR_VERMELHA, "Senha incorreta.");
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_INPUT, "Login", "Senha incorreta. Tente novamente:", "Login", "Sair");
    }
}

public OnPlayerDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    new nome[MAX_PLAYER_NAME];
    GetPlayerName(playerid, nome, sizeof(nome));

    if(dialogid == DIALOG_REGISTER)
    {
        if(!response) return Kick(playerid);
        if(strlen(inputtext) < 3) {
            SendClientMessage(playerid, COR_VERMELHA, "Senha muito curta.");
            return ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_INPUT, "Registrar", "Digite sua nova senha:", "Registrar", "Sair");
        }

        new query[256];
        format(query, sizeof(query), "INSERT INTO usuarios (nome, senha) VALUES ('%s', SHA2('%s', 256))", nome, inputtext);
        mysql_tquery(g_mysql_connection, query, "", "");
        PlayerInfo[playerid][pLoggedIn] = 1;
        SendClientMessage(playerid, COR_VERDE, "Conta registrada com sucesso!");
        return 1;
    }

    if(dialogid == DIALOG_LOGIN)
    {
        if(!response) return Kick(playerid);

        new query[256];
        format(query, sizeof(query), "SELECT * FROM usuarios WHERE nome = '%s' AND senha = SHA2('%s', 256) LIMIT 1", nome, inputtext);
        mysql_tquery(g_mysql_connection, query, "VerificarLogin", "i", playerid);
        return 1;
    }

    return 1;
}
