#ifndef KW_SNS_linkedlist_h
#define KW_SNS_linkedlist_h

#include <string>

using namespace std;

// 노드 정의
struct ll_node
{
    string time; // 메모를 남긴 시간 저장
    string memo; // 메모 스트링 저장

    ll_node* next;
    
    ll_node(string time, string memo) : time(time), memo(memo), next(0) {}
};

// 링크드리스트 정의
class linkedlist
{
private:
    ll_node* head;
    
public:
    // 생성자
    linkedlist() : head(0)
    {}
    
    // 메모 데이터 삽입
    bool add(string time, string memo)
    {
        if(head == NULL)
        {
            head = new ll_node(time, memo);
            return true;
        }
        
        ll_node* last = head;
        while(last->next)
        {
            last = last->next;
        }
        
        last->next = new ll_node(time, memo);
        
        return true;
    }
    
    // 링크드리스트 사이즈 리턴
    int size()
    {
        int size = 0;
        ll_node* cur = head;
        while(cur)
        {
            size++;
            cur = cur->next;
        }
        
        return size;
    }
    
    // index 에 위치한 노드 리턴
    ll_node* get(int index)
    {
        int s = size();
        if(index >= s)
        {
            cout << "**Error - out of index : " << index << endl;
            return NULL;
        }
        
        ll_node* cur = head;
        for(int i=0; i<index; i++)
        {
            cur = cur->next;
        }
        
        return cur;
    }
    
    // 노드 제거 by time
    string remove(string time)
    {
        ll_node* pre = NULL;
        ll_node* cur = head;
        while(cur)
        {
            if(cur->time == time)
            {
                if(pre)
                {
                    pre->next = cur->next;
                }
                else // 첫노드인 경우
                {
                    head = cur->next;
                }
                
                string ret = cur->memo;
                delete cur;
                
                return ret;
            }
            pre = cur;
            cur = cur->next;
        }
        
        return "";
    }
    
    // 모든 노드 제거
    void clear()
    {
        while(head)
        {
            ll_node* next = head->next;
            delete head;
            head = next;
        }
    }
    
    // 디버깅을 위한 데이터 스트링 리턴
    string to_str()
    {
        string ret;
        ll_node* cur = head;
        while(cur)
        {
            ret += ("[" + cur->time + ":" + cur->memo + "] ");
            cur = cur->next;
        }
        
        return ret;
    }
    
};


#endif
