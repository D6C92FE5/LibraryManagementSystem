#include "UserManager.h"
#include "booker.h"
#include <stdlib.h>
#include <fstream>
#include <conio.h>
#include <iomanip>

using namespace UserManager;
using namespace Booker;

void printLine(char * content){
	cout << content << endl;
}

void print(char * content){
	cout << content ;
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

//��ӡ���б�
void printBookList(Datastore::Book** list){
	int i = 0;
	cout << setw(30) << "����" << setw(20) << "����" << setw(30) << "������" << setw(15) 
		<< "ISBN" << setw(10) << "�ܼ�" << setw(10) << "�ɽ�" << endl;
	while(list[i] != NULL){
		cout << setw(30) << list[i]->Name << setw(20) << list[i]->Author << setw(30) << list[i]->Publisher << setw(15) 
			<< list[i]->Isbn << setw(10) << list[i]->Total << setw(10) << list[i]->Remain << endl;
		i++;
	}
}

// �ͷ��������
template <typename T>
void DestroyArray(T** array) {
    auto temp = array;
    while (*temp != NULL) {
        delete *temp;
        temp++;
    }
    delete array;
}

//��ȡһ���ַ�������
string getInputString(){
	string input;
	getline(cin, input);
	return input;
}

//��֤�ַ���ÿһλ����������û���ȵ�0
bool allNumric(const char * str){
	if('0' == str[0]){
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
	if(13 == str.size()){
		if(allNumric(str.c_str())){
			return true;
		}
	}else if(allNumric(str.c_str()) || (allNumric(str.substr(0,str.size() - 2).c_str()) && 'X' == str.at(str.size() - 1))){
		return true;
	}
	return false;
}

//��ʾ����ȡ��������
string getInputBookName(){
	print("������������");
	return getInputString();
}

//��ʾ����ȡ�û�������
string getInputUserName(){
	print("�û�����");
	return getInputString();
}

//��ʾ����ȡ����ʾ���������루ע����C�Դ������п���ֲ�ԣ���20λ�Զ���ֹ���أ�
string getInputPassword(){
	print("���룺");
	char buf[30];
	int pos = 0;
	buf[pos] = getch();
	while(buf[pos] != 13 && pos < 20){
		pos++;
		buf[pos] = getch();
	}
	if(0 == pos){
		return "";
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
	string userName;
	string bookName;
	string pwd;
	Datastore::Book**  bookList;
	printVisitorMenu();
	choice = getInputPosNum(2);
	switch (choice)
	{
	case 1:
		bookName = getInputBookName();
		bookList = AnythingIsbnFindBook(bookName);
		printBookList(bookList);
		DestroyArray(bookList);
	case 2:
		printVisitorMenu();
		userName = getInputUserName();
		pwd = getInputPassword();
		if(!Login(userName, pwd)){
			printLine("�û���/�������");
		}else{
			printLine("��ӭ������");

		}
	default:
		break;
	}
}

int main(){
    Datastore::Init();
	visitorMenu();
	int a ;
	cin >> a;

	return 0;
}

//����ʾ��ͼ��Ĵ���
int __main ()
{
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