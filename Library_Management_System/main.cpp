#include "UserManager.h"
#include <stdlib.h>
#include <fstream>
#include <conio.h>

void printLine(char * content){
	cout << content << endl;
}

void printLine(){
	cout << endl;
}

//��ӡ�οͲ˵�
void printVisitorMenu(){
	printLine("---------�˵�---------");
	printLine("1.������Ŀ");
	printLine("2.��½");
	printLine("0.�˳�ϵͳ");
	printLine("----------------------");
	printLine();
}

//��ӡ��ͨ�û��˵�
void printUserMenu(){
	printLine("---------�˵�---------");
	printLine("1.������Ŀ");
	printLine("2.�޸ĸ�����Ϣ");
	printLine("3.Ȩ������������Ա��");
	printLine("0.ע��");
	printLine("----------------------");
	printLine();
}

//��ӡ����Ա�˵�
void printAdminMenu(){
	printLine("-------ͼ�����-------");
	printLine("1.������Ŀ");
	printLine("2.ɾ����Ŀ");
	printLine("3.�޸���Ŀ��Ϣ");
	printLine("4.�������");
	printLine("5.ͼ�����");
	printLine("6.ͼ��黹");
	printLine("7.ͼ������");
	printLine("-------�û�����-------");
	printLine("8.�����û�");
	printLine("9.�Ƴ��û�");
	printLine("10.�����û�����");
	printLine("11.ע�����û�");
	printLine("0.ע��");
	printLine("----------------------");
	printLine();
}

//��ӡ�޸ĸ�����Ϣ�˵�
void printInfoChangeMenu(){
	printLine("-----�޸ĸ�����Ϣ-----");
	printLine("1.�޸�����");
	printLine("2.�޸ļ��");
	printLine("0.����");
	printLine("----------------------");
}

//��ӡ�����������ݾ���
void printWrongTypeWarning(){
	printLine("���ݲ��Ϸ������������룡");
}

//��ȡһ���ַ�������
string getInputString(){
	string input;
	getline(cin, input);
	return input;
}

//��֤�ַ���ÿһλ����������û���ȵ�0
bool allNumric(const char * str){
	if(str[0] == '0'){
		return false;
	}
	int i = 0;
	while(str[i]!='\0'){
		if(str[i] < '0' || str[i] >'9'){
			return false;
		}
		i++;
	}
	return true;
}

//ISBN�Ϸ��Լ��(����ⳤ���Լ��Ƿ��ɴ����ֺ�ĩβ���ܵ�X���)
bool isbnCheck(string str){
	if((str.size() != 13 && str.size() != 10)){
		return false;
	}
	if(str.size() == 13){
		if(allNumric(str.c_str())){
			return true;
		}
	}else if(allNumric(str.c_str()) || (allNumric(str.substr(0,str.size() - 2).c_str()) && str.at(str.size() - 1) == 'X')){
		return true;
	}
	return false;
}


//��ʾ����ȡ��������
string getInputBookName(){
	printLine("������������");
	return getInputString();
}

//��ʾ����ȡ�û�������
string getInputUserName(){
	cout << "�û�����";
	return getInputString();
}

//��ʾ����ȡ�������루ע����C�Դ������п���ֲ�ԣ�
string getInputPassword(){
	cout << "���룺";
	char buf[30];
	int pos = 0;
	buf[pos] = getch();
	while(buf[pos] != '\r' && pos < 20){
		pos++;
		buf[pos] = getch();
	}
	buf[pos] = '\0';
	return buf;
}

//�ظ�����һ�����������룬ֱ��������ȷ������ָ�����ޣ�Ĭ��û��
int getInputPosNum(int maxNum = 0){
	int num = -1;
	string input;
	while(num < 0 || cin <= 0){
		input = getInputString();
		if(!allNumric(input.c_str())){
			printWrongTypeWarning();
		}else{
			num	= atoi(input.c_str());
			if(num < 0 || (maxNum && num > maxNum)){
				printWrongTypeWarning();
			}
		}
	};
	return num;
}

//�ظ�����һ���Ϸ�ISBN���룬ֱ��������ȷ
string getInputIsbn(){
	string input = "";
	input = getInputString();
	while(isbnCheck(input)){
		printWrongTypeWarning();
		input = getInputString();
	}
	return input;
}

void visitorMenu(){
	int choice = 0;
	printVisitorMenu();
	choice = getInputPosNum(2);
	switch (choice)
	{
	case 1:
	default:
		break;
	}
}

int main(){
    Datastore::Init();
	printAdminMenu();
	printUserMenu();
	printVisitorMenu();
	string aa;
	int a ;
	cin >> a;

	return 0;
}

//����ʾ��ͼ��Ĵ���
int __main ()
{
	Datastore::Init();
	ifstream infile("��Ŀ��Ϣ.txt");
	string temp;
	while (getline(infile, temp))
	{
		auto TmpBook = Datastore::Create<Datastore::Book>();
		strcpy(TmpBook->Name, temp.c_str());
		getline(infile, temp);
		strcpy(TmpBook->Author, temp.c_str());
		getline(infile, temp);
		strcpy(TmpBook->Publisher, temp.c_str());
		getline(infile, temp);
		strcpy(TmpBook->Isbn, temp.c_str());
		TmpBook->Total = rand() / 500;
		TmpBook->Remain = TmpBook->Total;
		Datastore::InsertOrUpdate(TmpBook);
		delete TmpBook;
	}
	return 0;
}