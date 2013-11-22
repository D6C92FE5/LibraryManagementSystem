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
* 22 �޸��û���Ϣ�˵�
* 33 �޸���Ŀ��Ϣ�˵� 
*  
* 0 �˳�ϵͳ
*/

void printLine(char * content){
	cout << content << endl;
}

void printLine(int content){
	cout << content << endl;
}

void printLine(){
	cout << endl;
}

void print(char * content){
	cout << content ;
}

void print(int content){
	cout << content ;
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
	printLine("4.�鿴���ļ�¼");
	printLine("5.ע��");
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
		return;
	}
	printLine("----------------------");
	while(list[i] != NULL){
		print("������");
		printLine(list[i]->Name);
		print("���ߣ�");
		printLine(list[i]->Author);
		print("�����磺");
		printLine(list[i]->Publisher);
		print("ISBN��");
		printLine(list[i]->Isbn);
		print("�ܼƣ�");
		print(list[i]->Total);
		print("�ɽ裺");
		print(list[i]->Remain);
		printLine();
		printLine("----------------------");
		i++;
	}
}

//��ӡ���ļ�¼�б�
void printRecordList(Datastore::Record** list){
	int i = 0;
	if(list[0] == NULL){
		printLine("���޼�¼��");
		return;
	}
	printLine("-------���ļ�¼-------");
	while(list[i] != NULL){
		Datastore::Book* book;
		book = Booker::IndexFindBook(list[i]->BookIndex);
		print("������");
		printLine(book->Name);
		print("ISBN��");
		printLine(book->Isbn);
		print("�������ڣ�");
		print(ctime(&list[i]->Datetime));
		print("�Ƿ����裺");
		if(!list[i]->IsRenew){
			printLine("��");
		}else{
			printLine("��");
		}
		printLine("----------------------");
		i++;
		delete book;
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
string getInputString(unsigned int maxLength = 0){
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

//��֤�ַ���ÿһλ����������û���ȵ�0�ĳ�����ֻ��һ��0
bool allNumric(const char * str){
	if('0' == str[0]){
		if('\0' == str[1]){
			return true;
		}
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
int getInputPosNum(int maxNum = 0){
	int num = -1;
	string input;
	while(num < 0 || cin.bad()){
		if(cin.fail()){
			printLine("�޷��ָ���������󣬳����˳���������������");
			exit(0);
		}
		while(num < 0 || cin.bad()){
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
	return maxNum;
}

//�ظ�����һ���Ϸ�ISBN���룬ֱ��������ȷ
string getInputIsbn(){
	string input = "";
	input = getInputString(LEN_BOOK_ISBN);
	while(!isbnCheck(input)){
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
	int n = 4;
	bool isSure = true;
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
	if(!f(userName, pwd)){
		printLine();
		printLine("�û���/�������");
		if(strcmp(IUser->Type, "�û�") == 0){
			menuTag = 2;
		}else{
			menuTag = 1;
		}
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
	Logout();
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
		login(UserManager::Login);
		break;
	case 3:
		menuTag = 0;
		break;
	default:
		break;
	}
}

//�޸��û���Ϣ�˵�
void userInfoChangeMenu(){
	int choice = 0;
	string newInfo, newInfo2;
	bool isAdmin = IUser != NULL && strcmp(IUser->Type, "����Ա") == 0;
	bool isUser = IUser != NULL && strcmp(IUser->Type, "�û�") == 0;
	string userName;
	string userInfoItems[3] = {"", "����", "Info"};
	void (*changFunc[3])(string) = {NULL, UserManager::UpdataOnesPassword, UserManager::UpdataOnesInfo};
	void (*changFunc2[3])(string, string) = {NULL, UserManager::UpdataUserPassword, UserManager::UpdataUserInfo};
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
				newInfo = getInputString(LEN_USER_PASSWORD);
				printLine("�ٴ����������룺");
				newInfo2 = getInputString(LEN_USER_PASSWORD);
				if(strcmp(newInfo.c_str(), newInfo2.c_str()) != 0){
					printLine("�������벻һ�£��޸�ʧ�ܣ�");
					if(isAdmin){
						menuTag = 3;
					}else {
						menuTag = 2;
					}
					return;
				}
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
	case 3:
		if(isAdmin){
			menuTag = 3;
		}else {
			menuTag = 2;
		}
		break;
	default:
		break;
	}
}

//�鿴�û����ļ�¼
void borrowRecord(){
	Datastore::Record** record;
	record = Booker::AccountFindRecord(IUser->Name);
	if(record == NULL){
		printLine("���޼�¼��");
		return;
	}else{
		printRecordList(record);
	}
}

//��ͨ�û��˵�
void normalMenu(){
	int choice = 0;
	printUserMenu();
	choice = getInputPosNum(5);
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
		borrowRecord();
		break;
	case 5:
		menuTag = 1;
		break;
	default:
		backToMainMenu();
		break;
	}
}

//�޸�ͼ����Ϣ�˵�
void bookInfoChangeMenu(){
	int choice = 0;
	string bookIsbn;
	string newInfo;
	int maxLength = 0;
	string bookInfoItems[5] = {"", "����", "����", "������", "ISBN"};
	bool (*changeFunc[5])(string,string) = {NULL, Booker::ChangeBookName, 
		Booker::ChangeBookAuthor, Booker::ChangeBookPublisher, Booker::ChangeBookIsbn};
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
		print("������Ҫ�޸ĵ�ͼ��ISBN��");
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

//ɾ��
void deleteBook(){
	string isbn;
	int num;
	print("Ҫɾ����ͼ��ISBN��");
	isbn = getInputIsbn();
	print("Ҫɾ����������");
	num = getInputPosNum();
	Booker::DeleteBook(isbn, num);
}

//ɾ���û�
void deleteUser(){
	string name;
	string name2;
	print("Ҫɾ���û����û�����");
	name = getInputIsbn();
	print("ȷ��Ҫɾ���û����û�����");
	name2 = getInputIsbn();
	if(strcmp(name.c_str(), name2.c_str()) != 0){
		printLine("�����û�����һ�£���ִ��ɾ�����������ء�");
		return;
	}else if(confirm()){
		UserManager::DeleteUser(name);
	}
}

//�������
void addBook(){
	string isbn;
	int num;
	string name;
	string publisher;
	string author;
	print("Ҫ��ӵ�ͼ��ISBN��");
	isbn = getInputIsbn();
	print("Ҫ��ӵ�������");
	num = getInputPosNum();
	Datastore::Book** book;
	book = Booker::IsbnFindBook(isbn);
	if(book[0] == NULL){
		print("Ҫ��ӵ�ͼ�����ƣ�");
		name = getInputString(LEN_BOOK_NAME);
		print("Ҫ��ӵ�ͼ�����ߣ�");
		author = getInputString(LEN_BOOK_AUTHOR);
		print("Ҫ��ӵ�ͼ������磺");
		publisher = getInputString(LEN_BOOK_PUBLISHER);
	}else{
		name = book[0]->Name;
		author = book[0]->Author;
		publisher = book[0]->Publisher;
	}
	if(Booker::AddBook(isbn,name,author,publisher,num)){
		printLine("��ӳɹ���");
		book = Booker::IsbnFindBook(isbn);
		printBookList(book);
	}
}

//ע���û�
void signUp(){
	string name;
	string pwd;
	string pwd2;
	print("�û�����");
	name = getInputString(LEN_USER_NAME);
	print("���룺");
	pwd = getInputString(LEN_USER_PASSWORD);
	print("ȷ�����룺");
	pwd2 = getInputString(LEN_USER_PASSWORD);
	if(strcmp(pwd.c_str(), pwd2.c_str()) == 0){
		UserManager::InsertUser(name, pwd);
		printLine("ע��ɹ���");
	}
}

//�����û�
void searchUser(){
	string name;
	Datastore::User *user;
	print("Ҫ�������û�����");
	name = getInputString(LEN_USER_NAME);
	user = UserManager::SelectUser(name);
	cout << "�û�����" << user->Name << " �û����ͣ�" << user->Type << " INFO��" << user->Info << endl;
}

//����
void borrowBook(bool (*b)(string, string)){
	string userName;
	string isbn;
	print("�����û���");
	userName = getInputString(LEN_USER_NAME);
	print("ͼ��ISBN��");
	isbn = getInputIsbn();
	if(b(userName, isbn)){
		printLine("���ĳɹ���");
	}else{
		printLine("����ʧ�ܣ������û����Լ�ͼ����Ϣ�Ƿ�������ġ�");
	}
}

//����
void returnBook(){
	string userName;
	string isbn;
	print("�����û���");
	userName = getInputString(LEN_USER_NAME);
	print("ͼ��ISBN��");
	isbn = getInputIsbn();
	int t = Booker::ReturnBook(userName, isbn);
	if(t == 0){
		printLine("����ɹ���δ����");
	}else if(t == -1){
		printLine("����ʧ�ܣ����������Ϣ�Ƿ���ڡ�");
	}else{
		cout << "����ɹ�������" << t << "�죡";
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
	case 2:
		deleteBook();
		break;
	case 3:
		menuTag = 33;
		break;
	case 4:
		addBook();
		break;
	case 5:
		borrowBook(Booker::BrowseBook);
		break;
	case 6:
		returnBook();
		break;
	case 7:
		borrowBook(Booker::RenewBook);
		break;
		break;
	case 8:
		searchUser();
		break;
	case 9:
		deleteUser();
		break;
	case 10:
		menuTag = 22;
		break;
	case 11:
		signUp();
		break;
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
	Datastore::Init(true);
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
		TmpBook->Total = rand() / 500 + 10;
		TmpBook->Remain = TmpBook->Total;
		Datastore::InsertOrUpdate(TmpBook);
		delete TmpBook;
	}
	return 0;
}

int _main()
{
	Datastore::Init();
	return 0;
}

int main(){
	Datastore::Init();
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
			bookInfoChangeMenu();
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

