#include "UserManager.h"
#include "booker.h"
#include <stdlib.h>
#include <fstream>
#include <conio.h>
#include <iomanip>

using namespace UserManager;
using namespace Booker;

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
 * 22 修改个人信息菜单
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
	printLine("0.退出系统");
	printLine("----------------------");
	printLine();
}

//打印普通用户菜单
void printUserMenu(){
	printLine("---------菜单---------");
	printLine("1.检索书目");
	printLine("2.修改个人信息");
	printLine("3.权限提升（管理员）");
	printLine("0.注销");
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
	printLine("10.重置用户密码");
	printLine("11.注册新用户");
	printLine("0.注销");
	printLine("----------------------");
	printLine();
}

//打印修改个人信息菜单
void printUserInfoChangeMenu(){
	printLine("-----修改个人信息-----");
	printLine("1.修改密码");
	printLine("2.修改简介");
	printLine("0.返回");
	printLine("----------------------");
}

//打印修改书目信息菜单
void printBookInfoChangeMenu(){
	printLine("-----修改图书信息-----");
	printLine("1.修改书名");
	printLine("2.修改作者");
	printLine("3.修改出版社");
	printLine("4.修改ISBN");
	printLine("0.返回");
	printLine("----------------------");
}

//打印错误输入数据警告
void printWrongTypeWarning(){
	printLine("数据不合法，请重新输入！");
}

//打印书列表
void printBookList(Datastore::Book** list){
	int i = 0;
	if(list[0] == NULL){
		printLine("找不到相关图书！");
	}
	cout << setw(30) << "书名" << setw(20) << "作者" << setw(30) << "出版社" << setw(15) 
		<< "ISBN" << setw(10) << "总计" << setw(10) << "可借" << endl;
	while(list[i] != NULL){
		cout << setw(30) << list[i]->Name << setw(20) << list[i]->Author << setw(30) << list[i]->Publisher << setw(15) 
			<< list[i]->Isbn << setw(10) << list[i]->Total << setw(10) << list[i]->Remain << endl;
		i++;
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

//获取一行字符串输入
string getInputString(){
	string input;
	getline(cin, input);
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

//提示并获取书名输入
string getInputBookName(){
	print("请输入书名：");
	return getInputString();
}

//提示并获取用户名输入
string getInputUserName(){
	print("用户名：");
	return getInputString();
}

//提示并获取不显示的密码输入（注：非C自带不具有可移植性，满20位自动截止返回）
string getInputPassword(){
	print("密码：");
	char buf[30];
	int pos = 0;
	buf[pos] = getch();
	while(buf[pos] != '\r' && pos < 20){
		pos++;
		buf[pos] = getch();
	}
	if(0 == pos){
		return "";
	}
	if(pos == 20 && buf[pos] != '\r'){
		pos++;
	}
	buf[pos] = '\0';
	return buf;
}

//重复请求一个正整数输入，直到输入正确，参数指定上限，默认没有
int getInputPosNum(int maxNum = 0){
	int num = -1;
	string input;
	while(num < 0 || cin.bad() <= 0){
		input = getInputString();
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

//重复请求一个合法ISBN输入，直到输入正确
string getInputIsbn(){
	string input = "";
	input = getInputString();
	while(isbnCheck(input)){
		printWrongTypeWarning();
		input = getInputString();
	}
	return input;
}

//游客菜单
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
		bookList = AnythingFindBook(bookName);
		printBookList(bookList);
		DestroyArray(bookList);

		break;
	case 2:
		printVisitorMenu();
		userName = getInputUserName();
		pwd = getInputPassword();
		cout << pwd << endl;//                                                                    TODO！！！
		if(!Login(userName, pwd)){
			printLine("用户名/密码错误！");
		}else{
			printLine("欢迎回来！");
		}
	default:
		break;
	}
}

//普通用户菜单
void normalMenu(){
	int choice = 0;
	string userName;
	string bookName;
	string pwd;
	Datastore::Book**  bookList;
	printUserMenu();
	choice = getInputPosNum(11);
	switch (choice)
	{
	case 1:break;
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
	case 0:break;
	default:
		break;
	}
}

//管理员菜单
void adminMenu(){
	int choice = 0;
	string userName;
	string bookName;
	string pwd;
	Datastore::Book**  bookList;
	printUserMenu();
	choice = getInputPosNum(3);
	switch (choice)
	{
	case 1:break;
	case 2:break;
	case 3:break;
	case 0:break;
	default:
		break;
	}
}

//修改图书信息菜单
void bookInfoChangeMenu(){

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
		case 1:visitorMenu();break;
		case 2:normalMenu();break;
		case 3:adminMenu();break;
		case 22:break;
		case 33:break;
		case 0:break;
		default:
			printLine("However, you get here boy, it's not fun, you must leave!");
			break;
		}
	}
	printLine("Have a nice day!Bye bye!");
	return 0;
}

