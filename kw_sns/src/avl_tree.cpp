#include "avl_tree.h"

// 왼쪽으로 회전
avl_node* avl_tree::turn_left(avl_node* n)
{
    avl_node* new_root = n->right;
    n->right = new_root->left;
    new_root->left = n;
    reheight(n);
    reheight(new_root);
    return new_root;
}

// 오른쪽 회전
avl_node* avl_tree::turn_right(avl_node* n)
{
    avl_node* new_root = n->left;
    n->left = new_root->right;
    new_root->right = n;
    reheight(n);
    reheight(new_root);
    return new_root;
}

// 균형을 잡는다.
avl_node* avl_tree::rebalance(avl_node* n)
{
    if(!n) return 0;
    reheight(n);
    if(balance(n) == 2)
    {
        if(balance(n->right) < 0)
            n->right = turn_right(n->right);
        return turn_left(n);
    }
    
    if(balance(n) == -2)
    {
        if(balance(n->left) > 0)
            n->left = turn_left(n->left);
        return turn_right(n);
    }
    
    return n;
}

// 데이터 인서트
avl_node* avl_tree::insert(avl_node* n, string key, user_node* val)
{
    if(!n)
    {
        try { n = new avl_node(key, val); }
        catch(std::bad_alloc&)
        {
            std::cerr << "Can't insert element in avl tree" << std::endl;
            exit(1);
        }
        return n;
    }
    
    if(key < n->key)
        n->left = insert(n->left, key, val);
    else
        n->right = insert(n->right, key, val);
    return rebalance(n);
}

// 최소값 노드 제거
avl_node* avl_tree::remove_min(avl_node* n, avl_node* iter)
{
    if(iter->left)
        iter->left = remove_min(n, iter->left);
    else
    {
        
        avl_node* r = iter->right;
        n->key = iter->key;
        n->val = iter->val;
        delete iter;
        iter = r;
    }
    
    return rebalance(iter);
}

// 노드 제거 by key
avl_node* avl_tree::remove(avl_node* n, string key)
{
    if(!n) return 0;
    if(key < n->key)
        n->left = remove(n->left, key);
    else if( n->key < key)
        n->right = remove(n->right, key);
    else
    {
        avl_node *l = n->left, *r = n->right;
        if(!l && !r)
        {
            delete n;
            return 0;
        }
        
        if(!r)
            return l;
        n->right = remove_min(n, r);
    }
    return rebalance(n);
}

// 노드 서치 by key
avl_node* avl_tree::search(avl_node* n, string key)
{
    if(!n)
        return 0;
    
    avl_node* iter;
    for(iter=n; iter; )
    {
        if(key < iter->key)
            iter = iter->left;
        else if(iter->key < key)
            iter = iter->right;
        else
            return iter;
    }
    return 0;
}

// 하위 노드를 포함해 모두 제거
void avl_tree::tree_delete(avl_node* n)
{
    if(n->left)
        tree_delete(n->left);
    if(n->right)
        tree_delete(n->right);
    delete n;
}

// 노드 서치
user_node* avl_tree::find(string key)
{
    avl_node* n = search(root, key);
    if(n)
        return n->val;
    else
        return 0;
}

// 순서대로 노드값 출력 - 리커전
void avl_tree::_print_by_inorder(avl_node* n, ostream& out)
{
    if(n->left) _print_by_inorder(n->left, out);
    
    out << n->val->snum << "  /   " << n->val->name << endl;
    
    if(n->right) _print_by_inorder(n->right, out);
}

// 순서대로 노드값 출력
void avl_tree::print_by_inorder(ostream& out)
{
    if(!root)
    {
        cerr << "avl tree is empty!\n";
        return;
    }
    
    _print_by_inorder(root, out);
}

// 순서대로 친구정보 출력 - 리커전
void avl_tree::_print_friends_by_inorder(avl_node* n, ostream& out)
{
    if(n->left) _print_friends_by_inorder(n->left, out);
    
    int fsize = n->val->friend_list.size();
    for(int i=0; i<fsize; i++)
    {
    	// 파일 포맷에 맞게 출력한다.
        out << n->val->snum << "  /   " << n->val->friend_list.get(i)->snum_friend << endl;
    }
        
    if(n->right) _print_friends_by_inorder(n->right, out);
}

// 순서대로 친구정보 출력
void avl_tree::print_friends_by_inorder(ostream& out)
{
    if(!root)
    {
        cerr << "avl tree is empty!\n";
        return;
    }
    
    _print_friends_by_inorder(root, out);
}

// 순서대로 메모정보 출력 - 리커전
void avl_tree::_print_memo_by_inorder(avl_node* n, ostream& out)
{
    if(n->left) _print_memo_by_inorder(n->left, out);
    
    int msize = n->val->memo_list.size();
    for(int i=0; i<msize; i++)
    {
    	// 파일 포맷에 맞게 출력한다.
        out << n->val->snum << "  /   " << n->val->memo_list.get(i)->time << "  /   "
        << n->val->memo_list.get(i)->memo << endl;
    }
    
    if(n->right) _print_memo_by_inorder(n->right, out);
}

// 순서대로 메모정보 출력
void avl_tree::print_memo_by_inorder(ostream& out)
{
    if(!root)
    {
        cerr << "avl tree is empty!\n";
        return;
    }
    
    _print_memo_by_inorder(root, out);
}














