#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdio>  // 包含remove函数
#include"Tree.h"
using namespace std;

class table {
public:
    vector<string> col_name;//字段名(属性)
    vector<string> col_type;//字段类型名·
    string table_name;//表名
    int col_len;//属性个数
    int recount;//记录个数
    map<int, vector<Cell>> record;
    BSTree selTree;
    int attri;//当前二叉树用来构造的列索引

    table();
    void Show_com();//打印命令
    void create_table(string op);
    void SaveFile();
    void LoadRecord(string name);//读取表csv
    void Show_table(string op);//打印表
    void Insert(string op);//各种
    void add_rear();//尾部插入记录
    void alter(string op);//修改字段名
    void exitSystem(string op);
    void RecreateTree(int col);//按要搜索的字段索引重建二叉树
    void Select(string op);
    void Delete(string op);
    void sort(string op);
    int Partition(int low, int high, int index);
    void Quicksort(int low, int high,int index);
    bool deleteFile(string op);//删除文件
};
