#include "table.h"
table::table() {
    attri = -1;
    col_len = 0;
    recount = 0;
}
//打印命令
void table::Show_com() {
    cout << "-----命令-----" << endl;
    cout << "新建表：" << "create table 表名(类型1 属性1，……)" << endl;
    cout << "显示表：show table 表名" << endl;
    cout << "删除表：drop table 表名" << endl;
    cout << "查找：select from 表名 where 属性名[=值]/[like %T]" << endl;
    cout << "表尾添加数据：insert to 表名" << endl;
    cout << "删除数据：delete from 表名 where 属性=值 " << endl;
    cout << "alter table 表名 rename column 字段名 to 新字段名"<<endl;
    cout << "alter table 表名 add column 字段名 字段类型" << endl;
    cout << "alter table 表名 drop column 字段名" << endl;
    cout << "sort table name by 字段名" << endl;
    cout << "退出：exit" << endl;
    cout << "-----------" << endl;
}
void table::create_table(string op) {
    if (op.substr(0, 12) != "create table") return ;
    col_len = 0;
    string tmp = "";
    int k = 13;
    //获取表的名字
    table_name = "";
    for (k; op[k] != '\n' && op[k] != '(' && op[k] != ' ' && op[k] != '\r' && k < op.length(); ++k)
        table_name += op[k];
    op.insert(op.size() - 1, 1, '\n');
    //查一下有没有重名
    string tt = table_name + ".csv";
    ifstream ifs(tt, ios::in);//读文件
    if (ifs.is_open()) {//为空
        ifs.close();
        cout << "该文件名已存在" << endl;
        return;
    }
    //开始建表
    int p;
    for (p = 0; p < op.size(); p++)
    {
        if (op[p] == '(') {
            p++;
            break;
        }
    }
    op.insert(p, 1, '\n');//在p这个位置插入一个\n
    for (p; p < op.size(); p++) 
    {
        if (op[p] == ',')
        {
            p++;
            op.insert(p, 1, '\n');
        }
    }
    stringstream sp;
    sp << op;
    //获取表中字段名及字段类型
    string s;
    getline(sp, s); //全局函数getline(输入流对象，数据要放到的地方)
    while (getline(sp, s) && s != ")")
    {
        col_len++;//属性个数+1
        int i = 0;
        tmp = "";
        //字段类型名
        while (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))) ++i;
        for (i; s[i] != ' ' && i < s.length(); ++i)
        {
            tmp += s[i];
        }
        if (tmp.length() == 0) //什么情况下会是0？
        {
            cout << "create 语法错误！" << endl;
        }
        //判断字段类型是否正确
        if ((tmp != "int") &&  (tmp != "double") && (tmp != "string") && (tmp != "date"))
        {
            cout << "字段类型错误！" << endl;
        }

        //防止字段名和字段类型中间空格个数不可控
        tmp = "";
        while (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))) ++i;
        for (; s[i] != '(' && s[i] != ' ' && s[i] != ',' && i < s.length(); ++i)//将该字符串即字段类型存下来
        {
            tmp += s[i];
        }
        if (tmp.length() == 0)  cout << "create 语法错误！" << endl;

        //去除逗号，全都变成空格，s是目前得到的一行
        for (int i = 0; i < s.length(); i++)
            if (s[i] == ',')
                s[i] = ' ';
        if (s[0] == '(')
            s[0] = ' ';

        stringstream ss(s);//将输入的第一行转换成流，按照空格对输入的字符串截断
        string s1;
        vector<string> in;
        //抽取ss中的值到s1，每个循环中ss就被按照空格截断
        while (ss >> s1) in.push_back(s1);

        col_type.push_back(in[0]); //字段名
        col_name.push_back(in[1]); //字段类型
    }
    add_rear();
    SaveFile();
}
//尾部插入记录
void table::add_rear() {
    cout << "要添加的记录数目：" << endl;
    int addnum = 0;
    cin >> addnum;
    vector<Cell> row;
    for (int i = recount; i < recount + addnum; i++) {
        for (int j = 0; j < col_len; j++) {
            Cell ce(col_type[j]);
            string x;
            cin >> x;
            if (col_type[j] == "int") ce.x = stoi(x);
            if (col_type[j] == "double") ce.y = stod(x);
            if (col_type[j] == "string") ce.z = x;
            row.push_back(ce);
        }
        record.insert(make_pair(i, row));
        row.clear();
    }
    recount += addnum;
}
//将数据存储为文件
void table::SaveFile() {
    string filename = table_name + ".csv";
    ofstream ofs;
    ofs.open(filename, ios::out);
    for (int i = 0; i < col_len; i++) {
        ofs << col_name[i] <<"("<<col_type[i]<<")" << ",";
    }
    ofs << endl;
    //记录
    for (int i = 0; i < recount; i++) {
        for (int j = 0; j < col_len; j++) {
            ofs << record[i][j] << ",";
        }
        ofs << endl;
    }
    ofs.close();
    cout << "记录已保存" << endl;
}
//读取记录：
void table::LoadRecord(string name) {
    table_name = name;
    string filename = name + ".csv";
    ifstream ifs(filename,ios::in);//读文件
	if (!ifs.is_open()) {//为空
        cout << "数据表不存在" << endl;
		ifs.close();
		return;
	}
	//文件为空
	char ch;
	ifs >> ch;
	if (ifs.eof()) {
        cout << "该表为空" << endl;
		ifs.close();
		return;
	}
	//不为空
	ifs.putback(ch);//将上面读取的一个字符放回来
	string date;
	int cnt = -1;//recount,-1
    col_len = 0;
	while (ifs >> date) {
		int pos;//查到的位置变量
		int start = 0;
		vector<Cell> v;
        int colum = 0;
		while (true) {
			pos = date.find(",", start);//找,
			if (pos == -1) {
				//没找到
				break;
			}
            if (cnt == -1) {
                int t = date.find("(", start);
                string ty= date.substr(t+1, pos - t-2);
                col_type.push_back(ty);
                col_len++;
                ty= date.substr(start, t-start);
                col_name.push_back(ty);
            }
            else {
                auto temp = date.substr(start, pos - start);
                Cell ce("");
                if (col_type[colum] == "int") {
                    ce.type = "int";
                    ce.x = stoi(date.substr(start, pos - start));
                }
                if (col_type[colum] == "double") {
                    ce.type = "double";
                    ce.y = stod(date.substr(start, pos - start));
                }
                if (col_type[colum] == "string") {
                    ce.type = "string";
                    ce.z = date.substr(start, pos - start);
                }
                colum++;
                v.push_back(ce);
            }
            start = pos + 1;
		}
        if (cnt != -1) {
            record.insert(make_pair(cnt, v));
        }
		cnt++;
	}
    recount = cnt;
	ifs.close();
}
//打印表 show table 表名
void table::Show_table(string op) {
    if (op.substr(0, 10) != "show table") return;
    int k = 11;
    string name = "";
    for (k; k < op.size(); k++) name += op[k];
    LoadRecord(name);
    cout << "\ntable name:" << name <<" "<<recount<<"条记录" << endl;
    for (int i = 0; i < col_len; i++) {
        cout << "\t" << col_name[i] << " ";
    }
    cout << endl;
    //记录
    for (int i = 0; i < recount; i++) {
        cout << i << " ";
        for (int j = 0; j < col_len; j++) cout << "\t"<<record[i][j] ;
        cout << endl;
    }
}
void table::RecreateTree(int col) {
    this->selTree.root = NULL;
    for (int i = 0; i < recount; i++) {
        selTree.InsertintoBST(record[i], col,i);
    }
}
//先假设最多只有一个吧
void table::Select(string op) {
    //op:"查找：select from student where 字段名 like %T 含T的
    if (op.substr(0, 11) != "select from") return;
    int k = 12;
    string name = "";
    for (k; op[k]!=' '&&k<op.size(); k++) name += op[k];
    LoadRecord(name);
    k = k + 6;
    while (op[k] == ' ') k++;//万一有空格
    string colum = "";
    for (k; op[k] != '='&&k<op.size()&&op[k]!=' '; k++) colum += op[k];
    k++;//=
    while (op[k] == ' ') k++;//万一有空格
    //模糊搜索与精准搜索
    string like = "";
    for (int i = k; i < k + 4&&i<op.size(); i++) like += op[i];
    if (like == "like") {   
        k += 5;//直接跳到作引号后一个
        //就当没有含%的字符串
        if (op[k] == '%') {
            string str;//子串
            k++;
            for (k; op[k] != '%' && k < op.size(); k++) str += op[k];
            int col = -1;
            //cout << str << endl;
            for (int i = 0; i < col_len; i++) {//找col对应的列
                if (col_name[i] == colum) {
                    col = i;
                    break;
                }
            }
            if (col == -1) cout << "您输入的字段不存在" << endl;
            else {
                for (int i = 0; i < col_len; i++) {
                    cout << "\t" << col_name[i] << " ";
                }
                cout << endl;
            }
            for (int j = 0; j < recount; j++) {
                if (record[j][col].strmatch(str)) {
                    for (int i = 0; i < col_len; i++) cout <<"\t"<< record[j][i] << " ";
                    cout << endl;
                }
            }
        }
    }
    else{   // 精准搜索
        string value = "";
        for (k; k < op.size(); k++) value += op[k];
        int col = -1;
        Cell key;
        for (int i = 0; i < col_len; i++) {
            if (col_name[i] == colum) {
                col = i;
                key.type = col_type[i];
                if (col_type[i] == "int") key.x = stoi(value);
                if (col_type[i] == "double") key.y = stod(value);
                if (col_type[i] == "string") key.z = value;
                break;
            }
        }
        if (col == -1) {
            cout << "属性名" << colum << "不存在" << endl;
            return;
        }
        if (col != attri) {
            RecreateTree(col);//构造平衡二叉树
            //selTree.levelOrder();//测试一下
            attri = col;
        }
        TreeNode* p = selTree.searchBST(selTree.root, key, col);
        if (p) {
            for (int i = 0; i < col_len; i++) {
                cout << "\t" << col_name[i] << " ";
            }
            cout << endl;
            int row = p->row;
            for (int i = 0; i < col_len; i++) cout <<"\t" << record[row][i] << " ";
            cout << endl;
        }
        else cout << "没找到" << endl;
    }
}
void table::Insert(string op) {
    if (op.substr(0, 9) != "insert to") return;
    int k = 10;
    string filename = "";
    for (k; k<op.size(); k++) filename += op[k];
    LoadRecord(filename);
    cout << "当前表" << filename << "的属性为：" << endl;
    for (int i = 0; i < col_len; i++) {
        cout << col_type[i] << " " << col_name[i] << ",";
    }
    cout << endl;
    add_rear();
    SaveFile();
}
void table::Delete(string op) {
    //op:"delete from student where name=张三"
    if (op.substr(0, 11) != "delete from") return;
    int k = 12;
    string name = "";
    for (k; op[k] != ' '; k++) name += op[k];
    LoadRecord(name);
    k = k + 6;
    while (op[k] == ' ') k++;//万一有空格
    string colum = "";
    for (k; op[k] != '=' && k < op.size(); k++) colum += op[k];
    k++;
    while (op[k] == ' ') k++;//万一有空格
    string value = "";
    for (k; k < op.size(); k++) value += op[k];
    int col = -1;
    Cell key;
    for (int i = 0; i < col_len; i++) {
        if (col_name[i] == colum) {
            col = i;
            key.type = col_type[i];
            if (col_type[i] == "int") key.x = stoi(value);
            if (col_type[i] == "double") key.y = stod(value);
            if (col_type[i] == "string") key.z = value;
            break;
        }
    }
    if (col == -1) {
        cout << "属性名" << colum << "不存在" << endl;
        return;
    }
    if (col != attri) {
        RecreateTree(col);//构造平衡二叉树
        attri = col;
    }
    TreeNode* p = selTree.searchBST(selTree.root, key, col);
    if (p) {
        int row = p->row;
        for (int i = row; i < recount - 1; i++) {
            for (int j = 0; j < col_len; j++) record[i][j] = record[i + 1][j];
        }
    }
    record.erase(recount);
    recount--;
    SaveFile();
}
void table::alter(string op) {
    /*alter table 表名 rename column 字段名 to 新字段名;
    alter table 表名 add column 字段名 字段类型;
    alter table 表名 drop column 字段名;*/
    if (op.substr(0, 11) != "alter table") return;
    int k = 12;
    string name = "";
    for (k; op[k] != ' '; k++) name += op[k];
    LoadRecord(name);
    string opt = "";
    k++;
    while (op[k] !=' ') opt += op[k++];
    k = k + 8;//跳过" column "
    if (opt == "rename") {
        string colum = "";
        for (k; op[k] != ' ' && k < op.size(); k++) colum += op[k];
        string newname = "";
        k += 4;
        for (k; op[k] != ' ' && k < op.size(); k++) newname += op[k];
        for (int i = 0; i < col_len; i++) {
            if (col_name[i] == colum) {
                col_name[i] = newname;
                break;
            }
        }
        cout << "修改成功" << endl;
    }
    else if (opt == "add") {
        string colum = "";
        for (k; op[k] != ' ' && k < op.size(); k++) colum += op[k];
        k++;
        string typ = "";
        for (k; op[k] != ' ' && k < op.size(); k++) typ += op[k];
        col_type.push_back(typ);
        col_name.push_back(colum);
        cout << "请对表中每个元组补充该字段的值（按顺序）" << endl;
        cout << recount << "条记录" << endl;
        for (int i = 0; i < col_len; i++) {
            cout << "\t" << col_name[i] << " ";
        }
        cout << endl;
        //记录
        for (int i = 0; i < recount; i++) {
            for (int j = 0; j < col_len; j++) cout << "\t" << record[i][j];
            cout << endl;
        }
        string t;
        for (int i = 0; i < recount; i++) {
            cin >> t;
            Cell c(typ);
            if (typ == "int") c.x = stoi(t);
            if (typ == "double") c.y = stod(t);
            if (typ == "string") c.z = t;
            record[i].push_back(c);
        }
        col_len++;
    }
    else if (opt == "drop") {
        string colum = "";
        for (k; op[k] != ' ' && k < op.size(); k++) colum += op[k];
        int t = 0;
        for (int i = 0; i < col_len; i++) {
            if (col_name[i] == colum) {
                for (int j = i + 1; j < col_len; j++) {
                    col_name[j - 1] = col_name[j];
                    col_type[j - 1] = col_type[j];
                }
                col_name.pop_back();
                col_type.pop_back();
                t = i;
                break;
            }
        }
        for (int i = 0; i < recount; i++) {
            for (int j = t + 1; j < col_len; j++) record[i][j - 1] = record[i][j];
            record[i].pop_back();
        }
        col_len--;
    }
    else cout << "输入格式错误的命令" << endl;
    SaveFile();
}
int table::Partition(int low, int high, int index) {
    Cell t = record[low][index];
    vector<Cell> row = record[low];
    while (low < high) {
        while (low < high && record[high][index] > t) high--;
        record[low]= record[high];
        while (low < high && record[low][index] < t) low++;
        record[high] = record[low];
    }
    record[low] = row;
    return low;
}
void table::Quicksort(int low, int high,int index) {
    if (low < high) {
        int c = Partition(low,high,index);
        Quicksort(low, c - 1,index);
        Quicksort(c + 1, high,index);
    }
}
void table::sort(string op) {
    //sort table name by 字段名
    if (op.substr(0, 10) != "sort table") return;
    int k = 11;
    string name = "";
    for (k; op[k] != ' '&&k<op.size(); k++) name += op[k];
    LoadRecord(name);
    int index = -1;
    k = k + 4;
    string colum = "";
    for (k; op[k] != ' ' && k < op.size(); k++) colum += op[k];
    for (int i = 0; i < col_len; i++) {
        if (col_name[i] == colum) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        cout << "字段名错误" << endl;
        return;
    }
    Quicksort(0, recount - 1, index);
    SaveFile();
    cout << "\ntable name:" << name << " " << recount << "条记录" << endl;
    for (int i = 0; i < col_len; i++) {
        cout << "\t" << col_name[i] << " ";
    }
    cout << endl;
    //记录
    for (int i = 0; i < recount; i++) {
        for (int j = 0; j < col_len; j++) cout << "\t" << record[i][j];
        cout << endl;
    }
}
// 删除指定的CSV文件
bool table::deleteFile(string op) {
    if (op.substr(0, 10) != "drop table") return false;
    int k = 11;
    string name = "";
    for (k; k < op.size(); k++) name += op[k];
    string filePath = name + ".csv";
    if (remove(filePath.c_str()) == 0) {
        cout << "文件 " << filePath << " 删除成功" << endl;
    }
    else {
        cout << "删除文件失败: " << filePath << endl;
    }
    return true;
}

//退出
void table::exitSystem(string op) {
    if (op == "exit") exit(0);
}