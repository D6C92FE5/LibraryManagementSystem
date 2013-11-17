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
*“记录下一步将要进入的菜单”标志位
* 将菜单调度统一到main()中
* 用于防止菜单函数间调用引发栈溢出
*/
int menuTag = 1;
/*
* 1 游客菜单
* 2 普通用户菜单
* 3 管理员菜单
* 22 修改用户信息菜单
* 33 修改书目信息菜单 
*  
* 0 退出系统
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

//打印游客菜单
void printVisitorMenu(){
	printLine("---------菜单---------");
	printLine("1.检索书目");
	printLine("2.登陆");
	printLine("3.退出系统");
	printLine("----------------------");
	printLine();
}

//打印用户菜单
void printUserMenu(){
	printLine("---------菜单---------");
	printLine("1.检索书目");
	printLine("2.修改个人信息");
	printLine("3.权限提升（管理员）");
	printLine("4.查看借阅记录");
	printLine("5.注销");
	printLine("----------------------");
	printLine();
}

//打印管理员菜单
void printAdminMenu(){
	printLine("-------图书管理-------");
	printLine("1.检索书目");
	printLine("2.删除书目");
	printLine("3.修改书目信息");
	printLine("4.新书入馆");
	printLine("5.图书借阅");
	printLine("6.图书归还");
	printLine("7.图书续借");
	printLine("-------用户管理-------");
	printLine("8.检索用户");
	printLine("9.移除用户");
	printLine("10.修改用户信息");
	printLine("11.注册新用户");
	printLine("12.注销");
	printLine("----------------------");
	printLine();
}

//打印修改用户信息菜单
void printUserInfoChangeMenu(){
	printLine("-----修改用户信息-----");
	printLine("1.修改密码");
	printLine("2.修改简介");
	printLine("3.返回");
	printLine("----------------------");
}

//打印修改书目信息菜单
void printBookInfoChangeMenu(){
	printLine("-----修改图书信息-----");
	printLine("1.修改书名");
	printLine("2.修改作者");
	printLine("3.修改出版社");
	printLine("4.修改ISBN");
	printLine("5.返回");
	printLine("----------------------");
}

//打印错误输入数据警告
void printWrongTypeWarning(){
	printLine("数据非法，请重新输入！");
}

//打印书列表
void printBookList(Datastore::Book** list){
	int i = 0;
	if(list[0] == NULL){
		printLine("找不到相关图书！");
		return;
	}
	cout << setw(30) << "书名" << setw(20) << "作者" << setw(30) << "出版社" << setw(15) 
		<< "ISBN" << setw(10) << "总计" << setw(10) << "可借" << endl;
	while(list[i] != NULL){
		cout << setw(30) << list[i]->Name << setw(20) << list[i]->Author << setw(30) << list[i]->Publisher << setw(15) 
			<< list[i]->Isbn << setw(10) << list[i]->Total << setw(10) << list[i]->Remain << endl;
		i++;
	}
}

//打印借阅记录列表
void printRecordList(Datastore::Record** list){
	int i = 0;
	if(list[0] == NULL){
		printLine("找不到相关图书！");
		return;
	}

	cout << setw(30) << "书名" << setw(15) << "ISBN" << setw(10) << "借阅日期" 
		<< setw(10) << "是否续借" << endl;
	while(list[i] != NULL){
		Datastore::Book* book;
		book = Booker::IndexFindBook(list[i]->BookIndex);
		cout << setw(30) << book->Name << setw(15) << book->Isbn << setw(10) << list[i]->Datetime 
			<< setw(10) << list[i]->IsRenew << endl;
	}
}

// 释放搜索结果
template <typename T>
void DestroyArray(T** array) {
	auto temp = array;
	while (*temp != NULL) {
		delete *temp;
		temp++;
	}
	delete [] array;
}

//获取一行字符串输入，参数指定最大长度，默认不限
string getInputString(unsigned int maxLength = 0){
	string input;
	if(0 == maxLength){
		getline(cin, input);
	}else {
		getline(cin, input);
		while(input.size() > maxLength){
			printLine("数据过长！");
			getline(cin, input);
		}
	}
	return input;
}

//验证字符串每一位都是数字且没有先导0
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

//ISBN合法性检测(仅检测长度以及是否由纯数字和末尾可能的X组成)
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

//提示并获取不显示的密码输入（注：非C自带不具有可移植性，满一定位数自动截止返回）
string getInputPassword(){
	print("密码：");
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

//重复请求一个正整数输入，直到输入正确，参数指定上限，默认没有
int getInputPosNum(int maxNum = 0){
	int num = -1;
	string input;
	while(num < 0 || cin.bad()){
		if(cin.fail()){
			printLine("无法恢复的输入错误，程序将退出，请重新启动！");
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

//重复请求一个合法ISBN输入，直到输入正确
string getInputIsbn(){
	string input = "";
	input = getInputString(LEN_BOOK_ISBN);
	while(isbnCheck(input)){
		printWrongTypeWarning();
		input = getInputString(LEN_BOOK_ISBN);
	}
	return input;
}

//搜索图书
void searchBooks(){
	string bookName;
	Datastore::Book**  bookList;
	print("请输入书名：");
	bookName = getInputString(LEN_BOOK_NAME);
	bookList = AnythingFindBook(bookName);
	printBookList(bookList);
	DestroyArray(bookList);
}

//不可逆操作确认
bool confirm(){
	int n = 4;
	bool isSure = true;
	while(n-- && isSure){
		printLine("该操作不可逆，确定吗？");
		printLine("1.确定");
		printLine("2.我点错了，返回");
		isSure = getInputPosNum(2) == 1 ? true : false;
	}
	return isSure;
}

//登录
void login(bool (*f)(string, string)){
	string userName;
	string pwd;
	print("用户名：");
	userName = getInputString(LEN_USER_NAME);
	pwd = getInputPassword();
	printLine();
	if(!f(userName, pwd)){
		printLine();
		printLine("用户名/密码错误！");
		menuTag = 1;
	}else{
		printLine();
		printLine("欢迎回来！");
		if(strcmp(IUser->Type, "用户") == 0){
			menuTag = 2;
		}else if(strcmp(IUser->Type, "管理员") == 0){
			menuTag = 3;
		}else {
			menuTag = 1;
		}
	}
}

//遇到未知的菜单选择错误默认返回主菜单（建议不要允许发生调用到这个函数的情况）
void backToMainMenu(){
	printLine("错误的选项，将返回主界面！");
	printLine();
	menuTag = 1;
	Logout();
}

//游客菜单
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

//修改用户信息菜单
void userInfoChangeMenu(){
	int choice = 0;
	string newInfo;
	bool isAdmin = IUser != NULL && strcmp(IUser->Type, "管理员") == 0;
	bool isUser = IUser != NULL && strcmp(IUser->Type, "用户") == 0;
	string userName;
	string userInfoItems[3] = {"", "密码", "Info"};
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
		cout << "请输入新" << userInfoItems[choice];
		if(isUser){
			if(choice == 1){
				newInfo = getInputPassword();
			}else {
				cout << ":";
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

//查看用户借阅记录
void borrowRecord(){
	Datastore::Record** record;
	record = Booker::AccountFindRecord(IUser->Name);
	if(record == NULL){
		printLine("暂无记录！");
		return;
	}else{
		printRecordList(record);
	}
}

//普通用户菜单
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

//修改图书信息菜单
void bookInfoChangeMenu(){
	int choice = 0;
	string bookIsbn;
	string newInfo;
	int maxLength = 0;
	string bookInfoItems[5] = {NULL, "书名", "作者", "出版社", "ISBN"};
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
		print("请输入要修改的图书isbn：");
		bookIsbn = getInputIsbn();
		cout << "请输入新的" << bookInfoItems[choice] << ":";
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

//删书
void deleteBook(){
	string isbn;
	int num;
	print("要删除的图书ISBN：");
	isbn = getInputIsbn();
	print("要删除的数量：");
	num = getInputPosNum();
	Booker::DeleteBook(isbn, num);
}

void deleteUser(){
	string name;
	string name2;
	print("要删除用户的用户名：");
	name = getInputIsbn();
	print("确认要删除用户的用户名：");
	name2 = getInputIsbn();
	if(strcmp(name.c_str(), name2.c_str()) != 0){
		printLine("两次用户名不一致，不执行删除操作，返回。");
		return;
	}else if(confirm()){
		UserManager::DeleteUser(name);
	}
}

//添加新书
void addBook(){
	string isbn;
	int num;
	string name;
	string publisher;
	string author;
	print("要添加的图书ISBN：");
	isbn = getInputIsbn();
	print("要添加的数量：");
	num = getInputPosNum();
	print("要添加的图书名称（如已存在则以馆藏名为准）：");
	name = getInputString(LEN_BOOK_NAME);
	print("要添加的图书作者（如已存在则以馆藏名为准）：");
	author = getInputString(LEN_BOOK_AUTHOR);
	print("要添加的图书出版社（如已存在则以馆藏名为准）：");
	publisher = getInputString(LEN_BOOK_PUBLISHER);
	Booker::AddBook(isbn,name,author,publisher,num);
}

//注册用户
void signUp(){
	string name;
	string pwd;
	string pwd2;
	print("用户名：");
	name = getInputString(LEN_USER_NAME);
	print("密码：");
	pwd = getInputString(LEN_USER_PASSWORD);
	print("确认密码：");
	pwd2 = getInputString(LEN_USER_PASSWORD);
	if(strcmp(pwd.c_str(), pwd2.c_str()) == 0){
		UserManager::InsertUser(name, pwd);
		printLine("注册成功！");
	}
}

//搜索用户
void searchUser(){
	string name;
	Datastore::User *user;
	print("要搜索的用户名：");
	name = getInputString(LEN_USER_NAME);
	user = UserManager::SelectUser(name);
	cout << "用户名：" << user->Name << " 用户类型：" << user->Type << " INFO：" << user->Info << endl;
}

//借书
void borrowBook(bool (*b)(string, string)){
	string userName;
	string isbn;
	print("借阅用户：");
	userName = getInputString(LEN_USER_NAME);
	print("图书ISBN：");
	isbn = getInputIsbn();
	if(b(userName, isbn)){
		printLine("借阅成功！");
	}else{
		printLine("借阅失败！请检查用户名以及图书信息是否允许借阅。");
	}
}

//还书
void returnBook(){
	string userName;
	string isbn;
	print("借阅用户：");
	userName = getInputString(LEN_USER_NAME);
	print("图书ISBN：");
	isbn = getInputIsbn();
	int t = Booker::ReturnBook(userName, isbn);
	if(t == 0){
		printLine("还书成功！未超期");
	}else if(t == -1){
		printLine("还书失败！请检查借阅信息是否存在。");
	}else{
		cout << "还书成功！超期" << t << "天！";
	}
}

//管理员菜单
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

//插入示例图书的代码
int __main ()
{
	ifstream infile("书目信息.txt");
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
	visitorMenu();
	while(menuTag != 0){
		switch (menuTag)
		{
		case 1:
			Logout();
			visitorMenu();
			break;
		case 2:
			if(IUser != NULL && strcmp(IUser->Type, "用户") == 0){
				normalMenu();
			}else{
				menuTag = 1;
			}
			break;
		case 3:
			if(IUser != NULL && strcmp(IUser->Type, "管理员") == 0){
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

