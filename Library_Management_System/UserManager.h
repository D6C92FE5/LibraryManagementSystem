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
	void UpdataOnesPassword(string Password);
	//�û��޸�INFO
	void UpdataOnesInfo(string Info);
	//����Ա����
	//�������û�
	void InsertUser(string Name, string Password);
	//ѡ���û�
	Datastore::User * SelectUser(string Name);
	//ɾ���û�
	void DeleteUser(string Name);
	//�����û�����
	void UpdataUserPassword(string Name, string Password = "");
	//�����û�INFO
	void UpdataUserInfo(string Name, string Info = "");
}