
#include "UserManager.h"
#include <stdlib.h>
#include <fstream>

void printLine(char * content){
	cout << content << endl;
}
void printLine(){
	cout << endl;
}

void printVisitorMenu(){
	printLine("---------�˵�---------");
	printLine("1.������Ŀ");
	printLine("2.��½");
	printLine("0.�˳�ϵͳ");
	printLine("----------------------");
	printLine();
}

void printUserMenu(){
	printLine("---------�˵�---------");
	printLine("1.������Ŀ");
	printLine("2.�޸ĸ�����Ϣ");
	printLine("3.Ȩ������������Ա��");
	printLine("0.ע��");
	printLine("----------------------");
	printLine();
}

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

void printInfoChangeMenu(){
	printLine("-----�޸ĸ�����Ϣ-----");
	printLine("1.�޸�����");
	printLine("2.�޸ļ��");
	printLine("0.����");
	printLine("----------------------");
}

void printWrongTypeWarning(){
	printLine("���ݲ��Ϸ������������룡");
}

//��֤�ַ���ÿһλ����������û���ȵ�0
bool allNumric(const char * str){
	if(str[0] == '0'){
		return false;
	}
	int i = 1;
	while(str[i]!='\0'){
		if(str[i] < '0' || str[i] >'9'){
			return false;
		}
		i++;
	}
	return true;
}

//�ظ�����һ���������룬ֱ��������ȷ������ָ�����ޣ�Ĭ��û��
int getInputPosNum(int maxNum = 0){
	int num = -1;
	string input;
	while(num < 0 || cin <= 0){
		getline(cin, input);
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



int main(){
    Datastore::Init();
	printAdminMenu();
	printUserMenu();
	printVisitorMenu();
	int a;
	a = getInputPosNum(11);
	a = getInputPosNum();
	return 0;
	printf("sf ");
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