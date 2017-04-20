#ifndef __ACTIVATION_CODE_QUERY_H__
#define __ACTIVATION_CODE_QUERY_H__

#include "Database.h"
#include "utils/ByteArray.h"
#include "DataDefinition.h"

using namespace ws;

class PlayerQuery : public DBRequest
{
public:
	PlayerQuery() : data(nullptr), names(nullptr), count(0), playerID(0) {}
	virtual ~PlayerQuery(){}

	typedef std::function<void(UserAccount*)>	AccountCallback;
	typedef std::function<void(PlayerData*)>	PlayerDataCallback;
	typedef std::function<void(uint64_t)>		PlayerIDCallback;

	void getMaxPlayerID(PlayerIDCallback callback);
	// ͨ������˺Ų�ѯ���ID�б�
	void getPlayersByAccount(const std::string& accName, AccountCallback callback);
	// ͨ�����ID��ѯ�������
	void getPlayerByID(uint64_t id, PlayerDataCallback callback);
	// �����������
	void saveUserData(const PlayerData* player);
	// ���������Ϣ
	void savePlayerBase(const PlayerData* player);

	virtual void onRequest(Database& db);
	virtual void onFinish();

private:
	enum QueryType
	{
		GET_MAX_PLAYER_ID,
		GET_PLAYERS_BY_ACCOUNT,
		GET_PLAYER_BY_ID,
		SAVE_PLAYER_DATA,
	};
	QueryType							type;
	// player data query and updates
	void*								data;
	std::string*						names;
	uint32_t							count;
	uint64_t							playerID;
	PlayerIDCallback					playerIDCallback;
	PlayerDataCallback					playerCallback;
	AccountCallback						accountCallback;
};

#endif