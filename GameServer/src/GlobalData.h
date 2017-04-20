#ifndef __GLOBAL_DATA_H__
#define __GLOBAL_DATA_H__

#include "DataDefinition.h"
#include "database/PlayerQuery.h"

class GameDataCache
{
public:
	static void													init();
	static void													update();
	static void													cleanup();
	// global ids
	static uint64_t												getNextPlayerID(){ return ++nextPlayerID; }
	// �������˺Ż���
	static void													addAccountCache(UserAccount* account);
	// ɾ������˺Ż���
	static bool													delAccountCache(const std::string& accName);
	// ��ȡ����˺�
	static UserAccount*											getAccount(const std::string& account);
	
	typedef std::function<void(PlayerData*)>	PlayerCallback;
	// �����ҽ�ɫ����
	static void													addPlayerCache(PlayerData* data);
	// ɾ����ҽ�ɫ����
	static bool													delPlayerCache(uint64_t id);
	// ͨ��id��ȡ��ҽ�ɫ����
	static void													getPlayerByID(uint64_t id, PlayerCallback callback);
	// �����������
	static void													savePlayerData(PlayerData* data);

private:
	static uint64_t												nextPlayerID;
	static std::map<std::string, UserAccount*>					accountCache;		//�˺Ż���
	static std::map<std::string, UserAccount*>::iterator		lastAccountIter;	//�������������

	static std::map<uint64_t, PlayerData*>						playerCache;			//��ɫ����
	static std::map<uint64_t, PlayerData*>::iterator			lastPlayerIter;			//�������������
	static std::map<uint64_t, std::list<PlayerCallback>>		playerCallbacks;		//��ѯ��ɫ�ص�

	// ��ʼ�����id�Ļص�
	static void													onPlayerIDGet(uint64_t id);
	// ��ѯ��ɫ�Ļص�����
	static void onPlayerGet(uint64_t id, PlayerData* data);
};

#endif