#pragma once
#include <iostream>
#include<stack>
#include<queue>
#include <vector>
#include <string>
#include <map>
using namespace std;
class Cell {
public:
    string type;
    int x;
    double y;
    string z;
    vector<int> next;
    Cell() {
        type = ""; x = 0; y = 0.0; z = "";
        next.clear();
    };
    Cell(string T, int valx = 0, double valy = 0.0, string valz = "") {
        type = T;
        x = valx; y = valy; z = valz;
        if (T == "string") z = valz;
        if (T == "int") x = valx;
        if (T == "double") y = valy;
        next.clear();
    }
    //���أ�=
    bool operator != (Cell& c) {
        if ((type != c.type || x != c.x || y != c.y || z != c.z)) return true;
        else return false;
    }
    bool operator == (Cell& c) {
        if (type != c.type) return false;
        else {
            if (type == "int")
                if (x == c.x) return true;
            if (type == "double")
                if (y == c.y) return true;
            if (type == "string")
                if (z == c.z) return true;
        }
        return false;
    }
    bool operator>(Cell& c)
    {
        if (type == "string") return z > c.z;
        if (type == "int") return x > c.x;
        if (type == "double") return y > c.y;
    }
    bool operator<(Cell& c)
    {
        if (type == "string") return z < c.z;
        if (type == "int") return x < c.x;
        if (type == "double") return y < c.y;
    }
    void get_Next(string s)
    {
        int len =s.size();
        for (int k = 0; k < len; k++) next.push_back(0);
        int j = 0;
        for (int i = 1; i < len; i++) {	//iָ��ָ����Ǻ�׺ĩβ��jָ��ָ�����ǰ׺ĩβ
            while (j > 0 && s[i] !=s[j])	j = next[j - 1];	//ǰ��׺����ͬ��ȥ��jǰһλ������ǰ��׺
            if (s[i] == s[j])	j++;	//ǰ��׺��ͬ��jָ�����
            next[i] = j;	//����next����
        }
    }
    ////KMPƥ�䣬�ҵ��򷵻�trueû����false
    bool strmatch(string s)	
    {
        if (s.size() == 0)	return true;
        get_Next(s);
        int j = 0;
        for (int i = 0; i < z.size(); i++) {
            while (j > 0 && z[i] != s[j])	j = next[j - 1];
            if (z[i] == s[j])	j++;
            if (j == s.size())	return true;
        }
        return false;
    }
};
//Cell<<���������
static ostream& operator<<(ostream& cout, Cell c) {
    if (c.type == "int") cout << c.x;
    if (c.type == "double") cout << c.y;
    if (c.type == "string") cout << c.z;
    return cout;
}
//typedef vector<Cell> Row;
class TreeNode {
public:
	vector<Cell> node;
    int row;//ָ�ڱ��е�����
	TreeNode* left;
	TreeNode* right;
	TreeNode(int index,vector<Cell> ce){
        row = index;
		node = ce;
		left = NULL;
		right = NULL;
	}
};
class BSTree {
public:
	BSTree() {
		root = NULL;
	}
	TreeNode* root;//���ڵ�
	int GetDepth(TreeNode* r);//�����������
	void inorderTraversal();//�������������
	void levelOrder();//��α���
    TreeNode* searchBST(TreeNode* r, Cell key, int col); //����
	TreeNode* balanceBST(TreeNode* root);//תƽ��
	TreeNode* leftRotate(TreeNode* root);//����
	TreeNode* rightRotate(TreeNode* root);//����
	void InsertintoBST(vector<Cell> ce, int col, int index);//����
	TreeNode* deleteNode(TreeNode* r, Cell key, int col);//ɾ��
};
