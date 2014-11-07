#ifndef KW_SNS_linkedlist2d_h
#define KW_SNS_linkedlist2d_h

#include <string>

using namespace std;

// 2차원 링크드리스트 노드 정의
struct ll2d_node
{
    string snum_friend; // 친구 학번

    ll2d_node* next;
    ll2d_node* next_2d;

    ll2d_node(string snum_friend)
    : snum_friend(snum_friend), next(0), next_2d(0)
    {}
};

// 2차원 링크드리스트 정의
class linkedlist2d
{
private:
    ll2d_node* head;
    
public:
    // 생성자
    linkedlist2d() : head(0)
    {}
    
    // 데이터 삽입
    bool add(string snum_friend)
    {
        if(head == NULL)
        {
            head = new ll2d_node(snum_friend);
            return true;
        }
        
        ll2d_node* last = head;
        while(last->next)
        {
            last = last->next;
        }
        
        last->next = new ll2d_node(snum_friend);
        
        return true;
    }
    
    // 친구 학번으로 노드 검색
    ll2d_node* get(string snum_friend)
    {
        ll2d_node* cur = head;
        while(cur)
        {
            if(cur->snum_friend == snum_friend)
            {
                return cur;
            }
            cur = cur->next;
        }
        
        return NULL;
    }
    
    // 인덱스로 노드 검색
    ll2d_node* get(int index)
    {
        int s = size();
        if(index >= s)
        {
            cout << "**Error - out of index : " << index << endl;
            return NULL;
        }
        
        ll2d_node* cur = head;
        for(int i=0; i<index; i++)
        {
            cur = cur->next;
        }
        
        return cur;
    }
    
    // 친구 학번으로 노드 제거
    bool remove(string snum_friend)
    {
        ll2d_node* pre = NULL;
        ll2d_node* cur = head;
        while(cur)
        {
            if(cur->snum_friend == snum_friend)
            {
                if(pre)
                {
                    pre->next = cur->next;
                }
                else // 첫노드인 경우
                {
                    head = cur->next;
                }
                
                delete cur;
                
                return true;
            }
            pre = cur;
            cur = cur->next;
        }
    
        return false;
    }
    
    // 링크드리스트 사이즈 리턴
    int size()
    {
        int size = 0;
        ll2d_node* cur = head;
        while(cur)
        {
            size++;
            cur = cur->next;
        }
        
        return size;
    }
    
    // 모든 노드 제거
    void clear()
    {
        while(head)
        {
            ll2d_node* next = head->next;
            delete head;
            head = next;
        }
    }
    
    // 디버깅을 위한 데이터 스트링 리턴
    string to_str()
    {
        string ret;
        ll2d_node* cur = head;
        while(cur)
        {
            ret += ("[" + cur->snum_friend + "] ");
            cur = cur->next;
        }
        
        return ret;
    }
    
};


#endif
