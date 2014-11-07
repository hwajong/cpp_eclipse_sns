#include <iostream>
#include <algorithm>
#include <string>

#include "node.h"

#ifndef __avl_tree_h__
#define __avl_tree_h__

using namespace std;

// AVL 트리노드 정
struct avl_node
{
    string key;      // 키데이터 정렬의 기준이 된다.
    user_node* val;  // user_node 포인터를 저장.
    
    avl_node* left;
    avl_node* right;

    int h; // AVL 트리의 높이 저장.

    // 생성자
    avl_node(string key, user_node* val)
        : key(key), val(val), left(0), right(0), h(1)
    {};
    
};

// AVL 클래스 정의
class avl_tree
{
private:
    avl_node* root;
    
    // 노드의 높이 리턴
    int height(const avl_node* n) { return n? n->h : 0; }

    // 노드의 벨런스값 리턴
    int balance(const avl_node* n) { return height(n->right) - height(n->left); }

    // 노드의 높이값 재설정
    void reheight(avl_node* n) { n->h = std::max(height(n->left), height(n->right)) + 1; }

    // 트리의 균형을 유지하기 위한 메소드 정의
    avl_node* turn_left(avl_node* n);
    avl_node* turn_right(avl_node* n);
    avl_node* rebalance(avl_node* n);
    
    // 데이터 인서트
    avl_node* insert(avl_node* n, string key, user_node* val);

    // 최소값 노드 제거
    avl_node* remove_min(avl_node* n, avl_node* iter);

    // 노드 제거 by key
    avl_node* remove(avl_node* n, string key);

    // 노드 서치 by key
    avl_node* search(avl_node* n, string key);

    // 하위 노드를 포함해 모두 제거
    void tree_delete(avl_node* n);
    
    // 순서대로 노드값 출력 - 리커전
    void _print_by_inorder(avl_node* n, ostream& out);

    // 순서대로 친구정보 출력 - 리커전
    void _print_friends_by_inorder(avl_node* n, ostream& out);

    // 순서대로 메모정보 출력 - 리커전
    void _print_memo_by_inorder(avl_node* n, ostream& out);
    
public:

    // 생성자 - root 값은 null로 초기화
    avl_tree(): root(0) {};

    // 소멸자
    ~avl_tree(){ if(root) tree_delete(root); }

    // 노드 추가
    void add(string key, user_node* val) { root = insert(root, key, val); }

    // 노드 제거
    void del(string key){ root = remove(root, key); }

    // 노드 서치
    user_node* find(string key);

    // 순서대로 노드값 출력
    void print_by_inorder(ostream& out);

    // 순서대로 친구정보 출력
    void print_friends_by_inorder(ostream& out);

    // 순서대로 메모정보 출력
    void print_memo_by_inorder(ostream& out);
};

#endif
