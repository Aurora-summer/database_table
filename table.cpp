#include "table.h"
table::table() {
    attri = -1;
    col_len = 0;
    recount = 0;
}
//��ӡ����
void table::Show_com() {
    cout << "-----����-----" << endl;
    cout << "�½���" << "create table ����(����1 ����1������)" << endl;
    cout << "��ʾ��show table ����" << endl;
    cout << "ɾ����drop table ����" << endl;
    cout << "���ң�select from ���� where ������[=ֵ]/[like %T]" << endl;
    cout << "��β������ݣ�insert to ����" << endl;
    cout << "ɾ�����ݣ�delete from ���� where ����=ֵ " << endl;
    cout << "alter table ���� rename column �ֶ��� to ���ֶ���"<<endl;
    cout << "alter table ���� add column �ֶ��� �ֶ�����" << endl;
    cout << "alter table ���� drop column �ֶ���" << endl;
    cout << "sort table name by �ֶ���" << endl;
    cout << "�˳���exit" << endl;
    cout << "-----------" << endl;
}
void table::create_table(string op) {
    if (op.substr(0, 12) != "create table") return ;
    col_len = 0;
    string tmp = "";
    int k = 13;
    //��ȡ�������
    table_name = "";
    for (k; op[k] != '\n' && op[k] != '(' && op[k] != ' ' && op[k] != '\r' && k < op.length(); ++k)
        table_name += op[k];
    op.insert(op.size() - 1, 1, '\n');
    //��һ����û������
    string tt = table_name + ".csv";
    ifstream ifs(tt, ios::in);//���ļ�
    if (ifs.is_open()) {//Ϊ��
        ifs.close();
        cout << "���ļ����Ѵ���" << endl;
        return;
    }
    //��ʼ����
    int p;
    for (p = 0; p < op.size(); p++)
    {
        if (op[p] == '(') {
            p++;
            break;
        }
    }
    op.insert(p, 1, '\n');//��p���λ�ò���һ��\n
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
    //��ȡ�����ֶ������ֶ�����
    string s;
    getline(sp, s); //ȫ�ֺ���getline(��������������Ҫ�ŵ��ĵط�)
    while (getline(sp, s) && s != ")")
    {
        col_len++;//���Ը���+1
        int i = 0;
        tmp = "";
        //�ֶ�������
        while (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))) ++i;
        for (i; s[i] != ' ' && i < s.length(); ++i)
        {
            tmp += s[i];
        }
        if (tmp.length() == 0) //ʲô����»���0��
        {
            cout << "create �﷨����" << endl;
        }
        //�ж��ֶ������Ƿ���ȷ
        if ((tmp != "int") &&  (tmp != "double") && (tmp != "string") && (tmp != "date"))
        {
            cout << "�ֶ����ʹ���" << endl;
        }

        //��ֹ�ֶ������ֶ������м�ո�������ɿ�
        tmp = "";
        while (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'))) ++i;
        for (; s[i] != '(' && s[i] != ' ' && s[i] != ',' && i < s.length(); ++i)//�����ַ������ֶ����ʹ�����
        {
            tmp += s[i];
        }
        if (tmp.length() == 0)  cout << "create �﷨����" << endl;

        //ȥ�����ţ�ȫ����ɿո�s��Ŀǰ�õ���һ��
        for (int i = 0; i < s.length(); i++)
            if (s[i] == ',')
                s[i] = ' ';
        if (s[0] == '(')
            s[0] = ' ';

        stringstream ss(s);//������ĵ�һ��ת�����������տո��������ַ����ض�
        string s1;
        vector<string> in;
        //��ȡss�е�ֵ��s1��ÿ��ѭ����ss�ͱ����տո�ض�
        while (ss >> s1) in.push_back(s1);

        col_type.push_back(in[0]); //�ֶ���
        col_name.push_back(in[1]); //�ֶ�����
    }
    add_rear();
    SaveFile();
}
//β�������¼
void table::add_rear() {
    cout << "Ҫ��ӵļ�¼��Ŀ��" << endl;
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
//�����ݴ洢Ϊ�ļ�
void table::SaveFile() {
    string filename = table_name + ".csv";
    ofstream ofs;
    ofs.open(filename, ios::out);
    for (int i = 0; i < col_len; i++) {
        ofs << col_name[i] <<"("<<col_type[i]<<")" << ",";
    }
    ofs << endl;
    //��¼
    for (int i = 0; i < recount; i++) {
        for (int j = 0; j < col_len; j++) {
            ofs << record[i][j] << ",";
        }
        ofs << endl;
    }
    ofs.close();
    cout << "��¼�ѱ���" << endl;
}
//��ȡ��¼��
void table::LoadRecord(string name) {
    table_name = name;
    string filename = name + ".csv";
    ifstream ifs(filename,ios::in);//���ļ�
	if (!ifs.is_open()) {//Ϊ��
        cout << "���ݱ�����" << endl;
		ifs.close();
		return;
	}
	//�ļ�Ϊ��
	char ch;
	ifs >> ch;
	if (ifs.eof()) {
        cout << "�ñ�Ϊ��" << endl;
		ifs.close();
		return;
	}
	//��Ϊ��
	ifs.putback(ch);//�������ȡ��һ���ַ��Ż���
	string date;
	int cnt = -1;//recount,-1
    col_len = 0;
	while (ifs >> date) {
		int pos;//�鵽��λ�ñ���
		int start = 0;
		vector<Cell> v;
        int colum = 0;
		while (true) {
			pos = date.find(",", start);//��,
			if (pos == -1) {
				//û�ҵ�
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
//��ӡ�� show table ����
void table::Show_table(string op) {
    if (op.substr(0, 10) != "show table") return;
    int k = 11;
    string name = "";
    for (k; k < op.size(); k++) name += op[k];
    LoadRecord(name);
    cout << "\ntable name:" << name <<" "<<recount<<"����¼" << endl;
    for (int i = 0; i < col_len; i++) {
        cout << "\t" << col_name[i] << " ";
    }
    cout << endl;
    //��¼
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
//�ȼ������ֻ��һ����
void table::Select(string op) {
    //op:"���ң�select from student where �ֶ��� like %T ��T��
    if (op.substr(0, 11) != "select from") return;
    int k = 12;
    string name = "";
    for (k; op[k]!=' '&&k<op.size(); k++) name += op[k];
    LoadRecord(name);
    k = k + 6;
    while (op[k] == ' ') k++;//��һ�пո�
    string colum = "";
    for (k; op[k] != '='&&k<op.size()&&op[k]!=' '; k++) colum += op[k];
    k++;//=
    while (op[k] == ' ') k++;//��һ�пո�
    //ģ�������뾫׼����
    string like = "";
    for (int i = k; i < k + 4&&i<op.size(); i++) like += op[i];
    if (like == "like") {   
        k += 5;//ֱ�����������ź�һ��
        //�͵�û�к�%���ַ���
        if (op[k] == '%') {
            string str;//�Ӵ�
            k++;
            for (k; op[k] != '%' && k < op.size(); k++) str += op[k];
            int col = -1;
            //cout << str << endl;
            for (int i = 0; i < col_len; i++) {//��col��Ӧ����
                if (col_name[i] == colum) {
                    col = i;
                    break;
                }
            }
            if (col == -1) cout << "��������ֶβ�����" << endl;
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
    else{   // ��׼����
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
            cout << "������" << colum << "������" << endl;
            return;
        }
        if (col != attri) {
            RecreateTree(col);//����ƽ�������
            //selTree.levelOrder();//����һ��
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
        else cout << "û�ҵ�" << endl;
    }
}
void table::Insert(string op) {
    if (op.substr(0, 9) != "insert to") return;
    int k = 10;
    string filename = "";
    for (k; k<op.size(); k++) filename += op[k];
    LoadRecord(filename);
    cout << "��ǰ��" << filename << "������Ϊ��" << endl;
    for (int i = 0; i < col_len; i++) {
        cout << col_type[i] << " " << col_name[i] << ",";
    }
    cout << endl;
    add_rear();
    SaveFile();
}
void table::Delete(string op) {
    //op:"delete from student where name=����"
    if (op.substr(0, 11) != "delete from") return;
    int k = 12;
    string name = "";
    for (k; op[k] != ' '; k++) name += op[k];
    LoadRecord(name);
    k = k + 6;
    while (op[k] == ' ') k++;//��һ�пո�
    string colum = "";
    for (k; op[k] != '=' && k < op.size(); k++) colum += op[k];
    k++;
    while (op[k] == ' ') k++;//��һ�пո�
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
        cout << "������" << colum << "������" << endl;
        return;
    }
    if (col != attri) {
        RecreateTree(col);//����ƽ�������
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
    /*alter table ���� rename column �ֶ��� to ���ֶ���;
    alter table ���� add column �ֶ��� �ֶ�����;
    alter table ���� drop column �ֶ���;*/
    if (op.substr(0, 11) != "alter table") return;
    int k = 12;
    string name = "";
    for (k; op[k] != ' '; k++) name += op[k];
    LoadRecord(name);
    string opt = "";
    k++;
    while (op[k] !=' ') opt += op[k++];
    k = k + 8;//����" column "
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
        cout << "�޸ĳɹ�" << endl;
    }
    else if (opt == "add") {
        string colum = "";
        for (k; op[k] != ' ' && k < op.size(); k++) colum += op[k];
        k++;
        string typ = "";
        for (k; op[k] != ' ' && k < op.size(); k++) typ += op[k];
        col_type.push_back(typ);
        col_name.push_back(colum);
        cout << "��Ա���ÿ��Ԫ�鲹����ֶε�ֵ����˳��" << endl;
        cout << recount << "����¼" << endl;
        for (int i = 0; i < col_len; i++) {
            cout << "\t" << col_name[i] << " ";
        }
        cout << endl;
        //��¼
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
    else cout << "�����ʽ���������" << endl;
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
    //sort table name by �ֶ���
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
        cout << "�ֶ�������" << endl;
        return;
    }
    Quicksort(0, recount - 1, index);
    SaveFile();
    cout << "\ntable name:" << name << " " << recount << "����¼" << endl;
    for (int i = 0; i < col_len; i++) {
        cout << "\t" << col_name[i] << " ";
    }
    cout << endl;
    //��¼
    for (int i = 0; i < recount; i++) {
        for (int j = 0; j < col_len; j++) cout << "\t" << record[i][j];
        cout << endl;
    }
}
// ɾ��ָ����CSV�ļ�
bool table::deleteFile(string op) {
    if (op.substr(0, 10) != "drop table") return false;
    int k = 11;
    string name = "";
    for (k; k < op.size(); k++) name += op[k];
    string filePath = name + ".csv";
    if (remove(filePath.c_str()) == 0) {
        cout << "�ļ� " << filePath << " ɾ���ɹ�" << endl;
    }
    else {
        cout << "ɾ���ļ�ʧ��: " << filePath << endl;
    }
    return true;
}

//�˳�
void table::exitSystem(string op) {
    if (op == "exit") exit(0);
}