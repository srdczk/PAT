#include <bits/stdc++.h>

using namespace std;

int cnt = 0;

struct Node;

struct Index {
    int key;
    Node *val;
    Index(): key(0), val(nullptr) {}
};

bool operator<(const Index &a, const int &b) {
    return a.key < b;
}
bool operator<(const int &a, const Index &b) {
    return a < b.key;
}
bool operator==(const Index &a, const int &b) {
    return a.key == b;
}
bool operator==(const int &a, const Index &b) {
    return a == b.key;
}


struct Node {
    Node *parent;
    Node *next;
    Node *pre;
    int n;
    bool isLeaf;
    // �����Ȳ����ٽ��з���
    Index children[5];
    Node(): parent(nullptr), next(nullptr)
            , pre(nullptr), n(0), isLeaf(false) {}
};

Node *root = nullptr;

void init() {
    root = new Node;
    root->isLeaf = true;
}

// ���ҵ� Ҫ����ĵ�
Node *findLeaf(int k) {
    Node *node = root;
    while (!node->isLeaf) {
        node = upper_bound(node->children, node->children + node->n - 1, k)->val;
    }
    return node;
}

void insertLeafNoSplit(Node *node, int k) {
    if (!node->n) {
        node->children[0].key = k;
        node->n = 1;
        return;
    }
    auto insert = upper_bound(node->children, node->children + node->n, k);
    std::copy_backward(insert, node->children + node->n, node->children + node->n + 1);
    insert->key = k;
    ++node->n;
}

void resetParent(Node *node) {
    // ���� ��child �������� parent
    for (int i = 0; i < node->n; ++i) {
        node->children[i].val->parent = node;
    }
}

void insertToIndexNoSplit(Node *node, int k, Node *after) {
    // ����
    // �ҵ� upper_bound ==>
    auto insert = upper_bound(node->children, node->children + node->n - 1, k);
    std::copy_backward(insert, node->children + node->n, node->children + node->n + 1);
    insert->key = k;
    insert->val = (insert + 1)->val;
    (insert + 1)->val = after;
    ++node->n;
}

void insertToIndex(Node *parent, int k, Node *old, Node *after) {
    if (!parent) {
        // ���ĸ��ڵ�
        Node *node = new Node;
        node->n = 2;
        node->children[0].key = k;
        node->children[0].val = old;
        node->children[1].val = after;
        root = node;
        resetParent(root);
        return;
    }
    // ������ һ��
    insertToIndexNoSplit(parent, k, after);
    if (parent->n == 4) {
        Node *node = new Node;
        node->n = 2;
        parent->n = 2;
        node->pre = parent;
        node->next = parent->next;
        parent->next = node;
        node->parent = parent->parent;
        if (node->next) node->next->pre = node;
        // reset һ�� father
        node->children[0].key = parent->children[2].key;
        node->children[0].val = parent->children[2].val;
        node->children[1].val = parent->children[3].val;
        resetParent(node);
        insertToIndex(parent->parent, parent->children[1].key, parent, node);
    }
    // ����Ͳ��ö���
}

void insertToLeaf(Node *node, int k) {
    // �Ȳ��� �� ����
    insertLeafNoSplit(node, k);
    // ������� <ֱ�ӽ��з���>
    if (node->n == 4) {
        // ǰ����, ������
        Node *newNode = new Node;
        newNode->isLeaf = true;
        newNode->next = node->next;
        newNode->pre = node;
        node->next = newNode;
        newNode->parent = node->parent;
        if (newNode->next) newNode->next->pre = newNode;
        newNode->n = 2;
        node->n = 2;
        newNode->children[0] = node->children[2];
        newNode->children[1] = node->children[3];
        // ���ϲ���
        insertToIndex(node->parent, newNode->children[0].key, node, newNode);
    }
}

void print(Node *node) {
    if (node->isLeaf) {
        printf("[");
        for (int i = 0; i < node->n; ++i) {
            if (i) printf(",");
            printf("%d", node->children[i].key);
        }
        printf("]");
    } else {
        printf("[");
        for (int i = 0; i < node->n - 1; ++i) {
            if (i) printf(",");
            printf("%d", node->children[i].key);
        }
        printf("]");
    }
}

int main() {
    init();
    int n;
    scanf("%d", &n);
    while (n--) {
        int x;
        scanf("%d", &x);
        Node *node = findLeaf(x);
//        Key 7 is duplicated
//        [9]
//        [4,7,8][9,10]
        if (binary_search(node->children, node->children + node->n, x)) {
            printf("Key %d is duplicated\n", x);
        } else insertToLeaf(node, x);
    }
    Node *node = root;
    while (node) {
        Node *p = node;
        while (p) {
            print(p);
            p = p->next;
        }
        node = node->children[0].val;
        printf("\n");
    }
    return 0;
}

