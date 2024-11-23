#include <iostream>
#include "table.h"
#include "Tree.h"
using namespace std;
bool is_exist(vector<table>& tables, string name);
void dele(vector<table>& tables, string name);
int main() {
	string op;
	vector<table> database;  // 用于存储所有的table对象，数据库
	bool del = false;
	while (1) {
		del = false;
		table one;
		one.Show_com();
		getline(cin, op);
		one.create_table(op);
		one.Show_table(op);
		one.alter(op);
		one.Select(op);
		one.Insert(op);
		one.Delete(op);
		one.sort(op);
		del=one.deleteFile(op);
		one.exitSystem(op);
		system("pause");
		system("cls");
		if (!is_exist(database, one.table_name)) database.push_back(one);
		if (del) dele(database, one.table_name);
	}//while
	return 0;
}
bool is_exist(vector<table> &tables,string name) {
	for (int i = 0; i < tables.size(); i++) {
		if (name == tables[i].table_name) return true;//存在
	}
	return false;
}
void dele(vector<table>& tables, string name) {
	for (int i = 0; i < tables.size(); i++) {
		if (name == tables[i].table_name) {
			for (int j = i; j < tables.size()-1; j++) tables[j] = tables[j + 1];
			break;
		}
		tables.pop_back();
	}
}