// BOOKER_H
#pragma once
#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include "datastore.h"

using namespace std;

namespace Booker{
///内部使用
	extern string Temp, AnotherTemp;
	extern int BookIndex, UserIndex;
	//Isbn搜索图书
	bool SearchBookCondition(const Datastore::Book* book);
	
	//模糊搜索图书
	bool AnotherSearchBookCondition(const Datastore::Book* book);

	//编辑距离,辅助搜索
	int DistanceBetweenThem(string a, string b);

	//Accout搜索记录
	bool SearchRecordCondition(const Datastore::Record* record);

	//Account&Isbn搜索记录
	bool AnotherSearchRecordCondition(const Datastore::Record* record);

	//Account&Isbn查找记录
	Datastore::Record** IsbnFindRecord(string Account, string Isbn);
	
///外部InterFace
	//增加图书
	bool AddBook(string Isbn, string Name, string Author, string Publisher, int Num);
	
	//删除图书
	bool DeleteBook(string Isbn, int Num);
	
	//更改图书信息
	bool ChangeBookIsbn(string Isbn, string NewIsbn);

	bool ChangeBookName(string Isbn, string Name);

	bool ChangeBookAuthor(string Isbn, string Author);

	bool ChangeBookPublisher(string Isbn, string Publisher);
	
	//Isbn查找图书
	Datastore::Book** IsbnFindBook(string Isbn);

	//Index查找图书
	Datastore::Book* IndexFindBook(int Index);
	
	//Anything查找图书
	Datastore::Book** AnythingFindBook(string Anything);

	//借书
	bool BrowseBook(string Account, string Isbn);
	
	//1正常还书,0超期还书,-1还书失败
	int ReturnBook(string Account, string Isbn);

	//续借
	//超时不能续借，用户名或书ISBN错误不能续借，已续借不能续借，已还不能续借
	bool RenewBook(string Account, string Isbn);

	//Account查找记录
	Datastore::Record** AccountFindRecord(string Account);
}
//BOOK_H
