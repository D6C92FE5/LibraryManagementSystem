// BOOKER_H
#ifndef _BOOKER_H
#define _BOOKER_H

#include <iostream>
#include <algorithm>
#include <string.h>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "datastore.h"

using namespace std;

namespace Booker{
///�ڲ�ʹ��
	extern string Temp, AnotherTemp;
	extern int BookIndex, UserIndex;
	//Isbn����ͼ��
	bool SearchBookCondition(const Datastore::Book* book);
	
	//ģ������ͼ��
	bool AnotherSearchBookCondition(const Datastore::Book* book);

	//�༭����,��������
	int DistanceBetweenThem(string a, string b);

	//Accout������¼
	bool SearchRecordCondition(const Datastore::Record* record);

	//Account&Isbn������¼
	bool AnotherSearchRecordCondition(const Datastore::Record* record);

	//Account&Isbn���Ҽ�¼
	Datastore::Record** IsbnFindRecord(string Account, string Isbn);
	
///�ⲿInterFace
	//����ͼ��
	bool AddBook(string Isbn, string Name, string Author, string Publisher, int Num);
	
	//ɾ��ͼ��
	bool DeleteBook(string Isbn, int Num);
	
	//����ͼ����Ϣ
	bool ChangeBookIsbn(string Isbn, string NewIsbn);

	bool ChangeBookName(string Isbn, string Name);

	bool ChangeBookAuthor(string Isbn, string Author);

	bool ChangeBookPublisher(string Isbn, string Publisher);
	
	//Isbn����ͼ��
	Datastore::Book** IsbnFindBook(string Isbn);
	
	//Anything����ͼ��
	Datastore::Book** AnythingFindBook(string Anything);

	//����
	bool BrowseBook(string Account, string Isbn);
	
	//1��������,0���ڻ���,-1����ʧ��
	int ReturnBook(string Account, string Isbn);

	//����
	//��ʱ�������裬�û�������ISBN���������裬�����費�����裬�ѻ���������
	bool RenewBook(string Account, string Isbn);

	//Account���Ҽ�¼
	Datastore::Record** AccountFindRecord(string Account);
}

#endif  //BOOK_H
