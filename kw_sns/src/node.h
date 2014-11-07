#ifndef KW_SNS_node_h
#define KW_SNS_node_h

#include <string>

#include "linkedlist.h"
#include "linkedlist2d.h"

using namespace std;

// 회원 노드 정의
struct user_node
{
    string snum; // student number
    string name; // 학생 이름
    bool is_withdrawing; // 탈퇴중 여부
    
    linkedlist memo_list; // 메모저장을 위한 링크드리스트
    linkedlist2d friend_list; // 친구목록 저장을 위한 2D 링크드리스트
    
    // 생성자
    user_node(string snum, string name, bool is_withdrawing=false)
    : snum(snum), name(name), is_withdrawing(false)
    {};
    
    // 친구학번이 내 친구 목록에 있는지 여부 리턴
    bool is_my_friend(string snum_friend)
    {
        if(friend_list.get(snum_friend))
        {
            return true;
        }
        
        return false;
    }
    
    // 디버깅을 위한 노드데이터 스트링 리턴
    string to_str()
    {
        return snum + " : " + name + "\n\t-memo : " + memo_list.to_str() + "\n\t-frnd : " + friend_list.to_str();
    }
};

#endif
