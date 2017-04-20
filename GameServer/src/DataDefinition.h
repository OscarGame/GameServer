#ifndef __GAME_DATA_H__
#define __GAME_DATA_H__

#include <map>
#include <vector>
#include <list>
#include <memory.h>
#include "utils/Timer.h"

using namespace ws::utils;

#define getFieldSize(fromField,toField) {\
	(intptr_t)&this->toField-(intptr_t)&this->fromField+sizeof(toField)\
}

#define zeroInit(fromField,toField) {\
	void* ptr(&(this->fromField));\
	size_t size((intptr_t)(&(this->toField))-(intptr_t)ptr+sizeof(toField));\
	memset(ptr, 0, size);\
}

enum ErrorCodeEnum
{
	SUCCESS,							//�ɹ�
	MUST_LOGIN_FIRST,					//�����ȵ�¼
	ACCOUNT_NOT_EXIST,					//�˺Ų�����
	PLAYER_NOT_EXIST,					//��Ҳ�����
	LOGIN_REPEAT,						//�ظ���½
	INVALID_CHARACTERS,					//��Ч���ַ�
	CREATE_ROLE_REPEAT,					//�ظ�������ɫ
	CREATE_ROLE_MAX,					//�Ѵ���󴴽���
	INVALID_ARGUMENTS,					//��Ч�Ĳ���
};

enum SexEnum
{
	SEX_MALE,
	SEX_FEMALE
};

// ѡ���ɫ��Ϣ
struct ChooseRoleInfo
{
	ChooseRoleInfo(){ zeroInit(id, level) }
	uint64_t						id;
	uint8_t							sex;
	uint16_t						level;
	std::string						name;
};

//�������
struct PlayerData
{
	PlayerData(){ zeroInit(clientID, level) }
	int64_t									clientID;		//�ͻ���id
	uint64_t								offlineTime;	//����ʱ��
	bool									isChanged;

	uint64_t								id;						//id
	uint8_t									sex;					//�Ա�
	uint16_t								level;					//�ȼ�
	std::string								account;				//ƽ̨UID
	std::string								roleName;				//��ɫ��
};

//�û��˺�
struct UserAccount
{
	UserAccount() :clientID(0), offlineTime(0){ }
	int64_t							clientID;
	uint64_t						offlineTime;	//����ʱ��
	std::string						account;
	std::vector<ChooseRoleInfo>		roleList;
};

#endif
