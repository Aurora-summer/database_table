#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdio>  // ����remove����
#include"Tree.h"
using namespace std;

class table {
public:
    vector<string> col_name;//�ֶ���(����)
    vector<string> col_type;//�ֶ���������
    string table_name;//����
    int col_len;//���Ը���
    int recount;//��¼����
    map<int, vector<Cell>> record;
    BSTree selTree;
    int attri;//��ǰ���������������������

    table();
    void Show_com();//��ӡ����
    void create_table(string op);
    void SaveFile();
    void LoadRecord(string name);//��ȡ��csv
    void Show_table(string op);//��ӡ��
    void Insert(string op);//����
    void add_rear();//β�������¼
    void alter(string op);//�޸��ֶ���
    void exitSystem(string op);
    void RecreateTree(int col);//��Ҫ�������ֶ������ؽ�������
    void Select(string op);
    void Delete(string op);
    void sort(string op);
    int Partition(int low, int high, int index);
    void Quicksort(int low, int high,int index);
    bool deleteFile(string op);//ɾ���ļ�
};
