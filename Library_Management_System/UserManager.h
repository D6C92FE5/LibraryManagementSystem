#pragma once
#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;
#include "datastore.h"
extern string Type;
extern Datastore::User * IUser;
namespace UserManager{
	//����
	bool Login(string Name, string Password);
	//�ǳ�
	void Logout();
	//�û�����
	bool UpLevel(string Name, string Password);
	//�û��޸�����
	bool UpdataOnesPassword(string Password);
	//�û��޸�INFO
	bool UpdataOnesInfo(string Info);
	//����Ա����
	//�������û�
	bool InsertUser(string Name, string Password);
	//ѡ���û�
	Datastore::User * SelectUser(string Name);
	//ɾ���û�
	bool DeleteUser(string Name);
	//�����û�����
	bool UpdataUserPassword(string Name, string Password);
	//�����û�INFO
	bool UpdataUserInfo(string Name, string Info = "");
}