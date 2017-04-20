#include "UserLogin.h"
#include "utils/String.h"
#include "DataDefinition.h"
#include "GlobalData.h"
#include "GameEvent.h"
#include "utils/TimeTool.h"
#include "network/GameServer.h"
#include "gameLogic/PlayerLogic.h"

#define MAX_ACCOUNT_LEN 32
#define MAX_ROLE_NAME_LEN 18
#define MAX_CREATE_ROLE_COUNT 3

using namespace std::placeholders;

void UserLogin::processLogin(WebsocketClient* client, LoginMessage* msg)
{
	if (client->account)
	{
		client->sendCommonResult(ErrorCodeEnum::LOGIN_REPEAT);
		return;
	}
	if (!msg->account.size() || msg->account.size() > MAX_ACCOUNT_LEN ||
		!String::isPrintableString(msg->account.c_str()))
	{
		client->sendCommonResult(ErrorCodeEnum::INVALID_CHARACTERS);
		return;
	}
	UserAccount* account = GameDataCache::getAccount(msg->account);
	if (account)
	{
		if (account->clientID)
		{
			if (GameServer::getLogicServer()->kickClient(account->clientID))
			{
				Log::d("%s repeat login, kick out...", account->account.c_str());
			}
		}
		account->offlineTime = 0;
		client->account = account;
		account->clientID = client->id;
		replyRoleList(client, account);
	}
	else
	{
		// Ԥ����һ���˺ŷ�ֹ�ظ���½
		account = new UserAccount;
		account->account = msg->account;
		client->account = account;
		account->clientID = client->id;
		GameDataCache::addAccountCache(account);
		// ��ѯ�˺��µĽ�ɫ
		PlayerQuery* query = new PlayerQuery;
		query->getPlayersByAccount(msg->account, std::bind(&UserLogin::onRoleListGet, client->id, _1));
		GameServer::getDBQueue()->addQueueMsg(PtrDBRequest(query));
	}
}

void UserLogin::onRoleListGet(int64_t clientID, UserAccount* acc)
{
	UserAccount* account = GameDataCache::getAccount(acc->account);
	if (account)
	{
		account->roleList = acc->roleList;
		WebsocketClient* client = (WebsocketClient*)GameServer::getLogicServer()->getClient(clientID);
		if (client)
		{
			replyRoleList(client, account);
		}
	}
}

void UserLogin::replyRoleList(WebsocketClient* client, UserAccount* account)
{
	LoginMessage msg;
	msg.roleList = &account->roleList;
	client->sendMessage(CMD_LOGIN, msg);
}

void UserLogin::processCreateRole(WebsocketClient* client, CreateRoleMessage* msg)
{
	if (client->playerData)
	{
		client->sendCommonResult(ErrorCodeEnum::CREATE_ROLE_REPEAT);
		return;
	}
	if (client->account->roleList.size() >= MAX_CREATE_ROLE_COUNT)
	{
		client->sendCommonResult(ErrorCodeEnum::CREATE_ROLE_MAX);
		return;
	}
	if (msg->sex > SEX_FEMALE || msg->name.length() == 0 || msg->name.length() >= MAX_ROLE_NAME_LEN)
	{
		client->sendCommonResult(ErrorCodeEnum::INVALID_ARGUMENTS);
		return;
	}
	uint64_t now = TimeTool::getUnixtime<system_clock>();
	PlayerData* data = new PlayerData;
	// ������id
	data->id = GameDataCache::getNextPlayerID();
	data->isChanged = true;
	data->account = client->account->account;
	data->roleName = msg->name;
	data->sex = msg->sex;
	data->level = 1;	//��ʼ1��
	// �����������ӵ����棬Ȼ�󷵻����ݸ��ͻ���
	GameDataCache::addPlayerCache(data);
	// ����ɫ��ӵ��˺ŵĽ�ɫ�б�
	ChooseRoleInfo cri;
	cri.id = data->id;
	cri.sex = data->sex;
	cri.name = data->roleName;
	cri.level = data->level;
	client->account->roleList.push_back(cri);
	replyRoleList(client, client->account);
}

void UserLogin::processEnterGame(WebsocketClient* client, EnterGameMessage* msg)
{
	if (client->playerData)
	{
		client->sendCommonResult(ErrorCodeEnum::LOGIN_REPEAT);
		return;
	}
	for (ChooseRoleInfo& cri : client->account->roleList)
	{
		if (cri.id == msg->value)
		{
			GameDataCache::getPlayerByID(msg->value, std::bind(&UserLogin::replyUserData, client->id, _1));
			return;
		}
	}
	client->sendCommonResult(ErrorCodeEnum::PLAYER_NOT_EXIST);
	return;
}

void UserLogin::replyUserData(int64_t clientID, PlayerData* data)
{
	WebsocketClient* client = (WebsocketClient*)GameServer::getLogicServer()->getClient(clientID);
	if (!client)
	{
		return;
	}
	if (!data)
	{
		client->sendCommonResult(ErrorCodeEnum::PLAYER_NOT_EXIST);
		return;
	}
	client->playerData = data;
	data->clientID = client->id;
	data->offlineTime = 0;
	
	TimeTickMessage tickMsg;
	tickMsg.value = (double)TimeTool::getUnixtime<system_clock>();
	client->sendMessage(CMD_TIME_TICK, tickMsg);

	// ֪ͨ������ݸ��ͻ���
	PlayerLogic::notifyPlayerData(client, true);

	// �ɷ���ҵ�½�¼�
	CommonEvent evt;
	evt.type = EventType::EVENT_USER_LOGIN;
	evt.player = client->playerData;
	GameServer::dispatchEvent(evt);
}

