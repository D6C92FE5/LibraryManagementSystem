#include "UserManager.h"
#include "booker.h"
#include <stdlib.h>
#include <fstream>
#include<iostream>
#include <conio.h>
#include <iomanip>

using namespace UserManager;
using namespace Booker;
using namespace std;

/*
*����¼��һ����Ҫ����Ĳ˵�����־λ
* ���˵�����ͳһ��main()��
* ���ڷ�ֹ�˵��������������ջ���
*/
int menuTag = 1;
/*
* 1 �οͲ˵�
* 2 ��ͨ�û��˵�
* 3 ����Ա�˵�
* 22 �û��޸ĸ�����Ϣ�˵�
* 33 �޸���Ŀ��Ϣ�˵� 
*  
* 0 �˳�ϵͳ
*/

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
	printLine("3.�˳�ϵͳ");
	printLine("----------------------");
	printLine();
}

//��ӡ�û��˵�
void printUserMenu(){
	printLine("---------�˵�---------");
	printLine("1.������Ŀ");
	printLine("2.�޸ĸ�����Ϣ");
	printLine("3.Ȩ������������Ա��");
	printLine("4.ע��");
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
	printLine("10.�޸��û���Ϣ");
	printLine("11.ע�����û�");
	printLine("12.ע��");
	printLine("----------------------");
	printLine();
}

//��ӡ�޸��û���Ϣ�˵�
void printUserInfoChangeMenu(){
	printLine("-----�޸��û���Ϣ-----");
	printLine("1.�޸�����");
	printLine("2.�޸ļ��");
	printLine("3.����");
	printLine("----------------------");
}

//��ӡ�޸���Ŀ��Ϣ�˵�
void printBookInfoChangeMenu(){
	printLine("-----�޸�ͼ����Ϣ-----");
	printLine("1.�޸�����");
	printLine("2.�޸�����");
	printLine("3.�޸ĳ�����");
	printLine("4.�޸�ISBN");
	printLine("5.����");
	printLine("----------------------");
}

//��ӡ�����������ݾ���
void printWrongTypeWarning(){
	printLine("���ݷǷ������������룡");
}

//��ӡ���б�
void printBookList(Datastore::Book** list){
	int i = 0;
	if(list[0] == NULL){
		printLine("�Ҳ������ͼ�飡");
	}
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
	delete [] array;
}

//��ȡһ���ַ������룬����ָ����󳤶ȣ�Ĭ�ϲ���
string getInputString(int maxLength = 0){
	string input;
	if(0 == maxLength){
		getline(cin, input);
	}else {
		getline(cin, input);
		while(input.size() > maxLength){
			printLine("���ݹ�����");
			getline(cin, input);
		}
	}
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

//��ʾ����ȡ����ʾ���������루ע����C�Դ������п���ֲ�ԣ���һ��λ���Զ���ֹ���أ�
string getInputPassword(){
	print("���룺");
	char buf[LEN_USER_PASSWORD + 1];
	int pos = 0;
	buf[pos] = getch();
	while(buf[pos] != '\r' && pos < LEN_USER_PASSWORD){
		pos++;
		buf[pos] = getch();
	}
	if(0 == pos){
		return "";
	}
	if(pos == LEN_USER_PASSWORD && buf[pos] != '\r'){
		pos++;
	}
	buf[pos] = '\0';
	return buf;
}

//�ظ�����һ�����������룬ֱ��������ȷ������ָ�����ޣ�Ĭ��û��
long long int getInputPosNum(int maxNum = 0){
	long long int num = -1;
	string input;
	while(num < 0 || cin.bad() > 0){
		if(cin.fail() > 0){
			printLine("�����˳���������������");
			exit(0);
		}
		while(num < 0 || cin.bad() > 0){
			input = getInputString(15);
			if(!allNumric(input.c_str())){
				printWrongTypeWarning();
			}else{
				num	= atoi(input.c_str());
				if(num < 0 || (maxNum && num > maxNum)){
					printWrongTypeWarning();
				}
			}
		}
		return num;
	}
}

//�ظ�����һ���Ϸ�ISBN���룬ֱ��������ȷ
string getInputIsbn(){
	string input = "";
	input = getInputString(LEN_BOOK_ISBN);
	while(isbnCheck(input)){
		printWrongTypeWarning();
		input = getInputString(LEN_BOOK_ISBN);
	}
	return input;
}

//����ͼ��
void searchBooks(){
	string bookName;
	Datastore::Book**  bookList;
	print("������������");
	bookName = getInputString(LEN_BOOK_NAME);
	bookList = AnythingFindBook(bookName);
	printBookList(bookList);
	DestroyArray(bookList);
}

//���������ȷ��
bool confirm(){
	int n = 3;
	bool isSure;
	while(n-- && isSure){
		printLine("�ò��������棬ȷ����");
		printLine("1.ȷ��");
		printLine("2.�ҵ���ˣ�����");
		isSure = getInputPosNum(2) == 1 ? true : false;
	}
	return isSure;
}

//��¼
void login(bool (*f)(string, string)){
	string userName;
	string pwd;
	print("�û�����");
	userName = getInputString(LEN_USER_NAME);
	pwd = getInputPassword();
	printLine();
	cout << "������û���������ֱ��ǣ�" << userName << "|" << pwd << endl;
	if(!f(userName, pwd)){
		printLine();
		printLine("�û���/�������");
		menuTag = 1;
	}else{
		printLine();
		printLine("��ӭ������");
		if(strcmp(IUser->Type, "�û�") == 0){
			menuTag = 2;
		}else if(strcmp(IUser->Type, "����Ա") == 0){
			menuTag = 3;
		}else {
			menuTag = 1;
		}
	}
}

//����δ֪�Ĳ˵�ѡ�����Ĭ�Ϸ������˵������鲻Ҫ���������õ���������������
void backToMainMenu(){
	printLine("�����ѡ������������棡");
	printLine();
	menuTag = 1;
}

//�οͲ˵�
void visitorMenu(){
	int choice = 0;
	printVisitorMenu();
	choice = getInputPosNum(3);
	switch (choice)
	{
	case 1:
		searchBooks();
		break;
	case 2:
		login(Login);
		break;
	case 3:
		menuTag = 0;
		break;
	default:
		break;
	}
}

//�û��޸ĸ�����Ϣ�˵�
void userInfoChangeMenu(){
	int choice = 0;
	string newInfo;
	bool isAdmin = IUser != NULL && strcmp(IUser->Type, "����Ա") == 0;
	bool isUser = IUser != NULL && strcmp(IUser->Type, "�û�") == 0;
	string userName;
	string userInfoItems[3] = {NULL, "����", "Info"};
	void (*changFunc[3])(string) = {NULL, UpdataOnesPassword, UpdataOnesInfo};
	void (*changFunc2[3])(string, string) = {NULL, UpdataUserPassword, UpdataUserInfo};
	printUserInfoChangeMenu();
	choice = getInputPosNum(3);
	switch (choice)
	{
	case 1:
	case 2:
		if(isAdmin){
			userName = getInputString(LEN_USER_NAME);
		}else if(isUser){
			userName = IUser->Name;
		}else {
			menuTag = 1;
			break;
		}
		cout << "��������" << userInfoItems[choice] << "��";
		if(isUser){
			if(choice == 1){
				newInfo = getInputPassword();
			}else {
				newInfo = getInputString(LEN_USER_INFO);
			}
			changFunc[choice](newInfo);
		}else if(isAdmin){
			if(choice == 1){
				newInfo = getInputPassword();
			}else {
				newInfo = getInputString(LEN_USER_INFO);
			}
			changFunc2[choice](userName, newInfo);
		}
		
		break;
	default:
		break;
	}
}

//��ͨ�û��˵�
void normalMenu(){
	int choice = 0;
	printUserMenu();
	choice = getInputPosNum(4);
	switch (choice)
	{
	case 1:
		searchBooks();
		break;
	case 2:
		menuTag = 22;
		break;
	case 3:
		login(UpLevel);
		break;
	case 4:
		menuTag = 1;
		break;
	default:
		break;
	}
}

//�޸�ͼ����Ϣ�˵�
void bookInfoChangeMenu(){
	int choice = 0;
	string bookIsbn;
	string newInfo;
	int maxLength = 0;
	string bookInfoItems[5] = {NULL, "����", "����", "������", "ISBN"};
	bool (*changeFunc[5])(string,string) = {NULL, ChangeBookName, ChangeBookAuthor, ChangeBookPublisher, ChangeBookIsbn};
	printBookInfoChangeMenu();
	choice = getInputPosNum(5);
	switch (choice)
	{
	case 1:
		if(0 == maxLength){
			maxLength = LEN_BOOK_NAME;
		}
	case 2:
		if(0 == maxLength){
			maxLength = LEN_BOOK_AUTHOR;
		}
	case 3:
		if(0 == maxLength){
			maxLength = LEN_BOOK_PUBLISHER;
		}
	case 4:
		print("������Ҫ�޸ĵ�ͼ��isbn��");
		bookIsbn = getInputIsbn();
		cout << "�������µ�" << bookInfoItems[choice] << ":";
		if(0 == maxLength){
			newInfo = getInputIsbn();
		}
		newInfo = getInputString(maxLength);
		changeFunc[choice](bookIsbn, newInfo);
		menuTag = 33;
		break;
	case 5:
		menuTag = 3;
	default:
		break;
	}
}

//����Ա�˵�
void adminMenu(){
	int choice = 0;
	printAdminMenu();
	choice = getInputPosNum(12);
	switch (choice)
	{
	case 1:
		searchBooks();
		break;
	case 2:break;
	case 3:break;
	case 4:break;
	case 5:break;
	case 6:break;
	case 7:break;
	case 8:break;
	case 9:break;
	case 10:break;
	case 11:break;
	case 12:
		menuTag = 1;
		break;
	default:
		break;
	}
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

int main(){
	Datastore::Init();
	string s = "" + '\0';
	int a = s.size();
	cout << a;
	visitorMenu();
	while(menuTag != 0){
		switch (menuTag)
		{
		case 1:
			Logout();
			visitorMenu();
			break;
		case 2:
			if(IUser != NULL && strcmp(IUser->Type, "�û�") == 0){
				normalMenu();
			}else{
				menuTag = 1;
			}
			break;
		case 3:
			if(IUser != NULL && strcmp(IUser->Type, "����Ա") == 0){
				adminMenu();
			}else{
				menuTag = 1;
			}
			break;
		case 22:
			userInfoChangeMenu();
			break;
		case 33:
			break;
		case 0:
			break;
		default:
			printLine("However, you get here boy, it's not fun, you must leave!");
			break;
		}
	}
	printLine("Have a nice day!Bye bye!");
	return 0;
}

