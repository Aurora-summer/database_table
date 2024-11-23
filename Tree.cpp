#include "Tree.h"
int BSTree::GetDepth(TreeNode* r) {
    if (r==NULL) return 0;
    int le = GetDepth(r->left);
    int ri = GetDepth(r->right);
    if (le > ri) return le + 1;
    else return ri + 1;
}
//二叉树的中序遍历
void BSTree::inorderTraversal() {
    struct TreeNode* p = root;
    stack<TreeNode*> stk;
    while (p || !stk.empty()) { //树非空或栈中还有未读取元素
        //先走到最左边
        while (p) {
            stk.push(p);
            p = p->left;
        }
        p = stk.top();//取栈顶元素
        for (int i = 0; i < p->node.size(); i++) {
            cout << p->node[i] << " ";
        }
        cout << endl;
        stk.pop();//已存入数组的元素出栈
        p = p->right;//再对右子树进行同样操作
    }
}
//层次遍历
void BSTree::levelOrder() {
    queue<TreeNode*> que;
    if (!root) return;
    que.push(root);//根结点入队
    while (!que.empty()) {
        int len = que.size();//该层结点数
        for (int i = 0; i < len; i++) {
            TreeNode* q = que.front();//本行元素
            if (q->left) {
                que.push(q->left);//左子树入队列
            }
            if (q->right) {
                que.push(q->right);//右子树入队
            }
            for (int i = 0; i < q->node.size(); i++) {
                cout << q->node[i] << " ";
            }
            que.pop();//出队
        }//for
        cout << endl;
    }//while
}
//搜索节点，并返回结点
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
    // 计算平衡因子
    int balanceFactor = GetDepth(r->left) - GetDepth(r->right);
    if (balanceFactor > 1) {
        int lblance = GetDepth(r->left->left) - GetDepth(r->left->right);
        if (lblance < 0) r->left = leftRotate(r->left);
        r = rightRotate(r);
        return balanceBST(r);
    }
    else if (balanceFactor < -1) {
        // 检查右子树
        int rblance = GetDepth(r->right->left) - GetDepth(r->right->right);
        if (rblance > 0) r->right = rightRotate(r->right);
        r = leftRotate(r);
        return balanceBST(r);
    }
    return r;
}
//插入，等于的不行
void BSTree::InsertintoBST(vector<Cell> ce, int col,int index) {
    if (!root) {
        root = new TreeNode(index,ce);//把新根节点的地址赋给root
    }
    else {
        TreeNode* p = root;
        while (p) {
            if (p->node[col] < ce[col]) {//往右
                if (!p->right) {
                    p->right = new TreeNode(index,ce);//创建一个新节点并赋值
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
    //返回值必须赋给root！！
    root=balanceBST(root);
}
//删除
TreeNode* BSTree::deleteNode(TreeNode* r, Cell key, int col) {
    if (r == NULL) return NULL;
    if (key < r->node[col]) r->left = deleteNode(r->left, key, col);//key在左子树
    else if (key > r->node[col]) r->right = deleteNode(r->right, key, col);//key在右子树
    else {      //根节点.val==key
        if (!r->left && !r->right)
            return NULL;//左右子树不存在，直接删
        else if (!r->right) r = r->left;   //右子树不存在，直接把左子树接上
        else {
            TreeNode* p = r->right;
            while (p && p->left) p = p->left;//到右子树的最左（即根节点右边最小的数）
            r->node = p->node;//把值赋给根节点
            r->right = deleteNode(r->right, p->node[col], col);//删除右子树中的该结点
        }
    }
    return r;
}