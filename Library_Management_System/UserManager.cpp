#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
using namespace std;
#include "UserManager.h"
#include "datastore.h"
string Type;
int a;
Datastore::User * IUser;
//ά��
namespace UserManager
{
	//����
	bool Login(string Name, string Password)
	{
		IUser = new Datastore::User;
		char name[LEN_USER_NAME], password[LEN_USER_PASSWORD];
		memset(password, 0, sizeof(password));
		memset(name, 0, sizeof(name));
		for (int i = 0; Name[i] != 0; i++){ name[i] = Name[i]; }
		for (int i = 0; Password[i] != 0; i++){ password[i] = Password[i]; }
		strcpy(IUser->Name, name);
		auto user = Datastore::Select<Datastore::User>([Name](const Datastore::User* user) {
			return user->Name == Name;
		});
		if (user == NULL) return false;
		bool flag = strcmp(user->Password, password) == 0;
		if (flag){
			Type = user->Type;
			IUser->Index = user->Index;
			IUser->IsDeleted = user->IsDeleted;
			strcpy(IUser->Info, user->Info);
			strcpy(IUser->Password, user->Password);
			strcpy(IUser->Type, user->Type);
			delete user;
			user = NULL;
			return true;
		}
		else strcpy(IUser->Name, "");
		delete user;
		user = NULL;
		return false;
	}

	//�ǳ�
	void Logout()
	{
		delete IUser;
		IUser = NULL;
		Type.clear();
	}

	//�û�����
	bool UpLevel(string Name, string Password)
	{
		UserManager::Logout();
		return UserManager::Login(Name, Password);
	}
	//һ���û�
	//�û��ɹ������¿���
	//�û��޸�����
	bool UpdataOnesPassword(string Password)
	{
		if (Type.size() == 0)return false;
		int length = Password.size();
		char password[LEN_USER_PASSWORD];
		memset(password, 0, sizeof(password));
		memset(IUser->Password, 0, sizeof(Password));
		for (int i = 0; i < length; i++)
		{
			password[i] = IUser->Password[i];
			IUser->Password[i] = Password[i];
		}
		Datastore::InsertOrUpdate(IUser);
		auto user = Datastore::Select<Datastore::User>([](const Datastore::User* user) {
			return strcmp(user->Name, IUser->Name) == 0;
		});
		if (user->Password == Password)return true;
		else
		{
			strcpy(IUser->Password, password);
			return false;
		}
	}
	//�û��޸�INFO
	bool UpdataOnesInfo(string Info)
	{
		if (Type.size() == 0)return false;
		int length = Info.size();
		char info[LEN_USER_INFO];
		memset(info, 0, sizeof(info));
		memset(IUser->Info, 0, sizeof(Info));
		for (int i = 0; i < length; i++)
		{
			info[i] = IUser->Info[i];
			IUser->Info[i] = Info[i];
		}
		Datastore::InsertOrUpdate(IUser);
		auto user = Datastore::Select<Datastore::User>([](const Datastore::User* user) {
			return strcmp(user->Name, IUser->Name) == 0;
		});
		if (user->Info == Info)return true;
		else
		{
			strcpy(IUser->Info, info);
			return false;
		}
	}



	//����Ա
	//���в����ڹ���Աδ�����������޷�����
	//��������NULL

	//�������û�
	bool InsertUser(string Name, string Password)
	{
		if (Type != "����Ա")return false;
		auto TemporaryUser = Datastore::Select<Datastore::User>([Name](const Datastore::User* user){
			return user->Name==Name;
		});
		if (TemporaryUser != NULL)
		{
			delete TemporaryUser;
			TemporaryUser = NULL;
			return false;
		}
		else
		{
			auto user = Datastore::Create<Datastore::User>();
			char name[LEN_USER_NAME];
			char password[LEN_USER_PASSWORD];
			char type[LEN_USER_TYPE] = "�û�";
			char info[LEN_USER_INFO];
			memset(name, 0, sizeof(name));
			memset(password, 0, sizeof(password));
			memset(info, 0, sizeof(info));
			for (int i = 0; Name[i] != 0; i++){ name[i] = Name[i]; }
			for (int i = 0; Password[i] != 0; i++){ password[i] = Password[i]; }
			strcpy(user->Name, name);
			strcpy(user->Password, password);
			strcpy(user->Info, info);
			strcpy(user->Type, type);
			user->IsDeleted = false;
			Datastore::InsertOrUpdate(user);
			delete user;
			user = NULL;
			delete TemporaryUser;
			TemporaryUser = NULL;
			TemporaryUser = Datastore::Select<Datastore::User>([Name](const Datastore::User* user){
				return user->Name == Name;
			});
			if (TemporaryUser != NULL)
			{ 
				delete TemporaryUser;
				TemporaryUser = NULL;
				return true;
			}
			else
			{
				delete TemporaryUser;
				TemporaryUser = NULL;
				return false; 
			}
		}
	}
	//ѡ���û�
	Datastore::User * SelectUser(string Name)
	{
		auto user = Datastore::Select<Datastore::User>([Name](const Datastore::User* user) {
			return user->Name == Name;
		});
		return user;
	}
	//ɾ���û�
	bool DeleteUser(string Name)
	{
		if (Type != "����Ա")return false;
		auto user = Datastore::Select<Datastore::User>([Name](const Datastore::User* user) {
			return user->Name == Name;
		});
		if (user == NULL)return false;
		Datastore::Delete<Datastore::User>(user->Index);
		delete user;
		user = NULL;
		user = Datastore::Select<Datastore::User>([Name](const Datastore::User* user) {
			return user->Name == Name;
		});
		if (user == NULL)
		{
			delete user;
			user = NULL;
			return true;
		}
		else
		{
			delete user;
			user = NULL;
			return false;
		}
	}
	//�����û�����
	bool UpdataUserPassword(string Name, string Password)
	{
		if (Type != "����Ա")return false;
		if (Password == "")return false;
		else 
		{
			auto user = Datastore::Select<Datastore::User>([Name](const Datastore::User* user) {
				return user->Name == Name;
			});
			if (user == NULL)return false;
			memset(user->Password, 0, sizeof(Password));
			strcpy(user->Password, &Password[0]);
			Datastore::InsertOrUpdate(user);
			delete user;
			user = NULL;
		}
		auto user = Datastore::Select<Datastore::User>([Name](const Datastore::User* user) {
			return user->Name == Name;
		});
		if (user->Password == Password)
		{
			delete user;
			user = NULL;
			return true;
		}
		else
		{
			delete user;
			user = NULL;
			return false;
		}
	}
	//�����û�INFO
	bool UpdataUserInfo(string Name, string Info)
	{
		if (Type != "����Ա")return false;
		if (Info == "")return false;
		else 
		{
			auto user = Datastore::Select<Datastore::User>([Name](const Datastore::User* user) {
				return user->Name == Name;
			});
			if (user == NULL)return false;
			memset(user->Info, 0, sizeof(Info));
			strcpy(user->Info, &Info[0]);
			Datastore::InsertOrUpdate(user);
			delete user;
			user = NULL;
		}
		auto user = Datastore::Select<Datastore::User>([Name](const Datastore::User* user) {
			return user->Name == Name;
		});
		if (user->Password == Info)
		{
			delete user;
			user = NULL;
			return true;
		}
		else
		{
			delete user;
			user = NULL;
			return false;
		}
	}
}