#include "Tree.h"
int BSTree::GetDepth(TreeNode* r) {
    if (r==NULL) return 0;
    int le = GetDepth(r->left);
    int ri = GetDepth(r->right);
    if (le > ri) return le + 1;
    else return ri + 1;
}
//���������������
void BSTree::inorderTraversal() {
    struct TreeNode* p = root;
    stack<TreeNode*> stk;
    while (p || !stk.empty()) { //���ǿջ�ջ�л���δ��ȡԪ��
        //���ߵ������
        while (p) {
            stk.push(p);
            p = p->left;
        }
        p = stk.top();//ȡջ��Ԫ��
        for (int i = 0; i < p->node.size(); i++) {
            cout << p->node[i] << " ";
        }
        cout << endl;
        stk.pop();//�Ѵ��������Ԫ�س�ջ
        p = p->right;//�ٶ�����������ͬ������
    }
}
//��α���
void BSTree::levelOrder() {
    queue<TreeNode*> que;
    if (!root) return;
    que.push(root);//��������
    while (!que.empty()) {
        int len = que.size();//�ò�����
        for (int i = 0; i < len; i++) {
            TreeNode* q = que.front();//����Ԫ��
            if (q->left) {
                que.push(q->left);//�����������
            }
            if (q->right) {
                que.push(q->right);//���������
            }
            for (int i = 0; i < q->node.size(); i++) {
                cout << q->node[i] << " ";
            }
            que.pop();//����
        }//for
        cout << endl;
    }//while
}
//�����ڵ㣬�����ؽ��
TreeNode* BSTree::searchBST(TreeNode* r, Cell key, int col) {
    if (!r) return NULL;
    if (r->node[col] == key) return r;
    else if (r->node[col] > key) return searchBST(r->left, key, col);
    else return searchBST(r->right, key, col);
    return NULL;
}
TreeNode* BSTree::leftRotate(TreeNode* r) {
    if (r == nullptr || r->right == nullptr) return r;
    TreeNode* newroot = r->right;
    TreeNode* rightLeft = newroot->left;
    r->right = rightLeft;
    newroot->left = r;
    return newroot;
}

TreeNode* BSTree::rightRotate(TreeNode* r) {   
    if (r == nullptr || r->left == nullptr) return r;
    TreeNode* newroot = r->left;
    TreeNode* leftRight = r->left->right;
    r->left = leftRight;
    newroot->right = r;
    return newroot;
}
TreeNode* BSTree::balanceBST(TreeNode* r) {
    if (r == NULL) return r;
    r->left = balanceBST(r->left);
    r->right = balanceBST(r->right);
    // ����ƽ������
    int balanceFactor = GetDepth(r->left) - GetDepth(r->right);
    if (balanceFactor > 1) {
        int lblance = GetDepth(r->left->left) - GetDepth(r->left->right);
        if (lblance < 0) r->left = leftRotate(r->left);
        r = rightRotate(r);
        return balanceBST(r);
    }
    else if (balanceFactor < -1) {
        // ���������
        int rblance = GetDepth(r->right->left) - GetDepth(r->right->right);
        if (rblance > 0) r->right = rightRotate(r->right);
        r = leftRotate(r);
        return balanceBST(r);
    }
    return r;
}
//���룬���ڵĲ���
void BSTree::InsertintoBST(vector<Cell> ce, int col,int index) {
    if (!root) {
        root = new TreeNode(index,ce);//���¸��ڵ�ĵ�ַ����root
    }
    else {
        TreeNode* p = root;
        while (p) {
            if (p->node[col] < ce[col]) {//����
                if (!p->right) {
                    p->right = new TreeNode(index,ce);//����һ���½ڵ㲢��ֵ
                    break;
                }
                else p = p->right;
            }
            else {
                if (!p->left) {
                    p->left = new TreeNode(index,ce);
                    break;
                }
                else p = p->left;
            }
        }
    }
    //����ֵ���븳��root����
    root=balanceBST(root);
}
//ɾ��
TreeNode* BSTree::deleteNode(TreeNode* r, Cell key, int col) {
    if (r == NULL) return NULL;
    if (key < r->node[col]) r->left = deleteNode(r->left, key, col);//key��������
    else if (key > r->node[col]) r->right = deleteNode(r->right, key, col);//key��������
    else {      //���ڵ�.val==key
        if (!r->left && !r->right)
            return NULL;//�������������ڣ�ֱ��ɾ
        else if (!r->right) r = r->left;   //�����������ڣ�ֱ�Ӱ�����������
        else {
            TreeNode* p = r->right;
            while (p && p->left) p = p->left;//�������������󣨼����ڵ��ұ���С������
            r->node = p->node;//��ֵ�������ڵ�
            r->right = deleteNode(r->right, p->node[col], col);//ɾ���������еĸý��
        }
    }
    return r;
}