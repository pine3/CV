#include<iostream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <set>
using namespace std;

struct AVLNode {
    AVLNode *left, *right; 
    string key;
    string value;
    int height;
    AVLNode (string _key, string _value) : key (_key), value (_value){
       left = right = NULL;
       height = 1;
    }
    AVLNode(string const &_key) : key(_key) {
       left = right = NULL;
       height = 1;
    }
    int getBalanceFactor() const {
        int r = right == NULL ? 0 : right->height;
        int l = left == NULL ? 0 : left->height;
        return r - l;
    }
    void fix() {
        int r = right == NULL ? 0 : right->height;
        int l = left == NULL ? 0 : left->height;
        height = (r > l ? r : l) + 1;
    }
    
    AVLNode *balance() {
        fix();
        switch (getBalanceFactor()) {
            case -2:
                if (left->getBalanceFactor() > 0)
                    left = left->rotateLeft();
                return rotateRight();
            case 2:
                if (right->getBalanceFactor() < 0)
                    right = right->rotateRight();
                return rotateLeft();
            default: return this;
        }
    }

    AVLNode *find(string const &_key) {
        if (key == _key) return this;
        if (key > _key) return left == NULL ? NULL : left->find(_key);
        return right == NULL ? NULL : right->find(_key);
    }

    AVLNode *rotateLeft() {
        AVLNode *t = right;
        right = t->left;
        t->left = this;
        fix();
        t->fix();
        return t;
    }

    AVLNode *rotateRight() {
        AVLNode *t = left;
        left = t->right;
        t->right = this;
        fix();
        t->fix();
        return t;
    }

    void print(int indent){
        if (right != NULL) 
            right->print(indent + 1);
        for (int i = 0; i < indent; i++) 
            printf("    ");
        printf("%s\n", key.c_str());
        if (left != NULL)
            left->print(indent+1);
    }

    AVLNode *insert(string const &_key) {
         if (_key < key)
            left = left == NULL ? new AVLNode(_key) : left->insert(_key);
         else
            right = right == NULL ? new AVLNode(_key) : right->insert(_key);
         return balance();
    }

    AVLNode *insert(string const &_key, string _value) {
        if (_key < key)
            left = left == NULL ? new AVLNode(_key, _value) : left->insert(_key, _value);
        else
            right = right == NULL ? new AVLNode(_key, _value) : right->insert(_key, _value);
        return balance();
    }

    AVLNode *findMinimum() {
        return left != NULL ? left->findMinimum() : this;
    }   

    AVLNode *removeMinimum() {
        if (left == NULL) return right;
        left = left->removeMinimum();
        return balance();
    }

    static AVLNode *remove(AVLNode *p, string const &_key) {
        if (p == NULL) return NULL;
        if (_key < p->key) {
            p->left = remove(p->left, _key);
            return p->balance();
        } else if (_key > p->key) {
            p->right = remove(p->right, _key);
            return p->balance();
        } else {
            AVLNode *l = p->left;
            AVLNode *r = p->right;
            delete p;
            if (r == NULL) return l;
            AVLNode *min = r->findMinimum();
            min->right = r->removeMinimum();
            min->left = l;
            return min->balance();
        }
    }
};

struct AVLTree {
    AVLNode *root;
    
    AVLTree() {
        root = NULL;
    }

    void print() const {
        if (root != NULL) root->print(0);
    }

    bool insert(string const &_key) {
        if (root == NULL) root = new AVLNode(_key);
        else root = root->insert(_key);
        return true;
    }

    bool insert(string const &_key, string _value) {
        if (root == NULL) root = new AVLNode(_key, _value);
        else root = root->insert(_key, _value);
        return true;
    }


    bool find(string const &_key) {
        if (root == NULL) return false;
        AVLNode *f = root->find(_key);
        return f == NULL ? false : true;
    }

    bool find(string const &_key, string *val) {
        if (root == NULL) return false;
        AVLNode *f = root->find(_key);
        if(f != NULL) *val = f->value; 
        return f == NULL ? false :  true;
    }
    
    bool edit(string const &_key, string val) {
        if (root == NULL) return false;
        AVLNode *f = root->find(_key);
        if (f != NULL) f->value = val;
        return f == NULL ? false : true;
    }

    bool remove(string const &_key) {
        root = AVLNode::remove(root, _key);
        return true;
    }

    ~AVLTree() {
        remove(root);
    }

    void remove(AVLNode *t) {
        if (t == NULL) return;
        remove(t->left);
        remove(t->right);
        delete t;
    }
};
    
int main() {
    AVLTree t;
    int n;
    string s,name, num;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> s;
        if (s == "ADD") {
            cin >> name >> num;
            if (!t.find(name)) 
                t.insert(name,num);
            else{ 
                cout << "ERROR\n";
            }
        } else if (s == "DELETE"){
            cin >> name;
            if (t.find(name)) 
                t.remove(name);
            else
                cout << "ERROR\n";
        } else if (s == "EDITPHONE") {
            cin >> name >> num;
            if (t.edit(name, num)) 
                ;
            else
                cout <<"ERROR\n";
        } else if (s == "PRINT") {
            cin >> name;
            if (t.find(name ,&num)) 
                cout << name << " " << num << "\n";
            else
                cout << "ERROR\n";
        }
    }
    return 0;
}

