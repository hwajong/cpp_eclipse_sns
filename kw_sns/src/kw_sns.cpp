#include <iostream>
#include <fstream>
#include <sstream>

#include "kw_sns.h"

// 스트링 공백 문자 제거
void kw_sns::trim(char* s)
{
    char* p = s;
    int l = (int)strlen(p);
    
    while(l-1 > 0 && !(p[l-1] & 0x80) && isspace(p[l-1])) p[--l] = 0;
    while(*p && !(*p & 0x80) && isspace(*p)) ++p, --l;
    
    memmove(s, p, l + 1);
}

// 커맨드가 저장된 스크립트 파일을 읽어 처리한다.
bool kw_sns::run_cmd_script(string fname)
{
    ifstream ifs(fname);
    
    if(!ifs)
    {
        cout << "can't open file - " << fname << endl;
        return false;
    }
    
    while(!ifs.eof())
    {
        char buff[1024] = {0,};
        ifs.getline(buff, sizeof(buff));
        
        // 공백문자 제거
        trim(buff);
        
        // 빈줄 스킵
        if(strlen(buff) == 0) continue;
        
        // 명령어를 파싱하여 해당 명령 수행.
        if(strncmp(buff, "LOAD", 4) == 0)
        {
            run_cmd_load(buff + 4);
        }
        else if(strncmp(buff, "FRNDLIST", 8) == 0)
        {
            run_cmd_frndlist(buff + 8);
        }
        else if(strncmp(buff, "UPDATE", 6) == 0)
        {
            run_cmd_update(buff + 6);
        }
        else
        {
        	// Unkown Command
            cout << "**Error - Unkown CMD : " << buff << endl;
        }
    }
    
    ifs.close();
    return true;
}

// 회원 정보를 파일에 포맷에 맞춰 출력한다.
bool kw_sns::save_user_info(const string fname)
{
    ofstream ofs(fname);
    
    if(!ofs)
    {
        cout << "can't open file - " << fname << endl;
        return false;
    }
    
    ofs << "==============================\n";
    ofs << "    학번    /   이름\n";
    ofs << "==============================\n";
    
    userinfos_by_snum.print_by_inorder(ofs);
    
    ofs.close();
    
    cout << "*user file created !! - " << fname << endl;
    return true;
}

// 사용자 정보 로드
bool kw_sns::load_user_info(const string fname)
{
    ifstream ifs(fname);
    
    if(!ifs)
    {
        cout << "can't open file - " << fname << endl;
        return false;
    }
    
    int line_count = 0;
    stringstream ss;
    while(!ifs.eof())
    {
        char buff[1024];
        ifs.getline(buff, sizeof(buff));
        
        // 헤더 스킵
        if(line_count++ < 3) continue;
        
        // 공백문자 제거
        trim(buff);
        
        if(strlen(buff) == 0) continue;
        
        // 파싱하기 위하여 스트링 복사
        ss << buff;
        string snum;
        string name;
        for(int i=0; i<2; i++)
        {
            if(ss.eof()) return false;
            
            char buff2[100] = {0,};
            ss.getline(buff2, sizeof(buff2), '/');
            
            trim(buff2);
            
            //cout << buff2 << endl;
            if(i==0) snum = buff2;
            else if(i==1) name = buff2;
            
        }
        
        // 노드를 생성해 그 포인터를 두개의 avl tree 에 저장한다.
        user_node* user = new user_node(snum, name);
        userinfos_by_name.add(user->name, user);
        userinfos_by_snum.add(user->snum, user);
        cout << "*user added !! - " << snum << " : " << name << endl;
        
        ss.clear();
        
        //cout << buff << endl;
    }
    
    cout << "*load_user_info success ! - " << fname << endl;
    ifs.close();
    return true;
}

// 사용자 정보 업데이트
bool kw_sns::update_user_info(const string fname)
{
    ifstream ifs(fname);
    
    if(!ifs)
    {
        cout << "can't open file - " << fname << endl;
        return false;
    }
    
    int line_count = 0;
    stringstream ss;
    while(!ifs.eof())
    {
        char buff[1024];
        ifs.getline(buff, sizeof(buff));
        
        // 헤더 스킵
        if(line_count++ < 3) continue;
        
        // 공백문자 제거
        trim(buff);
        
        if(strlen(buff) == 0) continue;
        
        // 파싱하기 위하여 스트링 복사
        ss << buff;
        string snum;
        string name;
        for(int i=0; i<2; i++)
        {
            if(ss.eof()) return false;
            
            char buff2[100] = {0,};
            ss.getline(buff2, sizeof(buff2), '/');
            
            trim(buff2);
            
            //cout << buff2 << endl;
            if(i==0) snum = buff2;
            else if(i==1) name = buff2;
            
        }
        
        user_node* user = userinfos_by_snum.find(snum);
        if(user) // 탈퇴
        {
            user->is_withdrawing = true;
            // cout << "-- 탈퇴 회원 : " << buff << endl;
            // cout << "*user withdrawing!! - " << snum << " : " << name << endl;

            // 두 avl 트리에서 제거하고 노드 메모리를 헤제한다.
            userinfos_by_snum.del(snum);
            userinfos_by_name.del(user->name);
            delete user;

            cout << "*user removed!! - " << snum << " : " << name << endl;
        }
        else // 신규 가입
        {
        	// 두 avl 트리에 추가한다.
            user_node* user = new user_node(snum, name);
            userinfos_by_name.add(user->name, user);
            userinfos_by_snum.add(user->snum, user);
            
            cout << "*user added!! - " << snum << " : " << name << endl;
        }
        
        ss.clear();
        //cout << buff << endl;
    }
    
    cout << "*update_user_info success ! - " << fname << endl;
    ifs.close();
    return true;
    
}

// 친구정보를 파일에 포멧에 맞춰 출력한다.
bool kw_sns::save_friend_info(string fname)
{
    ofstream ofs(fname);
    
    if(!ofs)
    {
        cout << "can't open file - " << fname << endl;
        return false;
    }
    
    ofs << "==================================\n";
    ofs << "본인학번    /   친구학번\n";
    ofs << "==================================\n";
    
    userinfos_by_snum.print_friends_by_inorder(ofs);
    
    ofs.close();
    
    cout << "*friend file created !! - " << fname << endl;
    return true;
    
}

// 친구정보를 로드한다.
bool kw_sns::load_friend_info(string fname)
{
    ifstream ifs(fname);
    
    if(!ifs)
    {
        cout << "can't open file - " << fname << endl;
        return false;
    }
    
    int line_count = 0;
    stringstream ss;
    while(!ifs.eof())
    {
        char buff[1024];
        ifs.getline(buff, sizeof(buff));
        
        // 헤더 스킵
        if(line_count++ < 3) continue;
        
        // 공백문자 제거
        trim(buff);
        
        if(strlen(buff) == 0) continue;
        
        ss << buff;
        string snum;
        string snum_friend;
        for(int i=0; i<2; i++)
        {
            if(ss.eof()) return false;
            
            char buff2[100] = {0,};
            ss.getline(buff2, sizeof(buff2), '/');
            
            trim(buff2);
            
            // cout << buff2 << endl;
            if(i==0) snum = buff2;
            else if(i==1) snum_friend = buff2;
        }
        
        // friend data insert
        user_node* user = userinfos_by_snum.find(snum);
        if(user)
        {
            user->friend_list.add(snum_friend);
            //cout << user->to_str() << endl;
            cout << "*friend added !! - " << snum_friend << endl;
        }
        else // 해당 회원이 없을 경우
        {
            cout << "**error : " << snum << " is not our member" << endl;
        }
        
        //cout << buff << endl;
        
        ss.clear();
    }
    
    cout << "*load_friend_info success ! - " << fname << endl;
    ifs.close();
    return true;
}

// 친구정보를 업데이트 한다.
bool kw_sns::update_friend_info(string fname)
{
    ifstream ifs(fname);
    
    if(!ifs)
    {
        cout << "can't open file - " << fname << endl;
        return false;
    }
    
    int line_count = 0;
    stringstream ss;
    while(!ifs.eof())
    {
        char buff[1024];
        ifs.getline(buff, sizeof(buff));
        
        // 헤더 스킵
        if(line_count++ < 3) continue;
        
        // 공백문자 제거
        trim(buff);
        
        if(strlen(buff) == 0) continue;
        
        ss << buff;
        string snum;
        string snum_friend;
        for(int i=0; i<2; i++)
        {
            if(ss.eof()) return false;
            
            char buff2[100] = {0,};
            ss.getline(buff2, sizeof(buff2), '/');
            
            trim(buff2);
            
            // cout << buff2 << endl;
            if(i==0) snum = buff2;
            else if(i==1) snum_friend = buff2;
        }
        
        // friend data update
        user_node* user = userinfos_by_snum.find(snum);
        if(user)
        {
            ll2d_node* f = user->friend_list.get(snum_friend);
            if(f) // 기존에 친구였으면 삭제
            {
                user->friend_list.remove(snum_friend);
                cout << "*friend removed !! - " << snum_friend << endl;
            }
            else // 기존에 친구가 아니었으면 친구로 등록
            {
                user->friend_list.add(snum_friend);
                cout << "*friend added !! - " << snum_friend << endl;
            }
            //cout << user->to_str() << endl;
        }
        else
        {
            cout << "**error : " << snum << " is not our member" << endl;
        }
        
        //cout << buff << endl;
        
        ss.clear();
    }
    
    cout << "*update_friend_info success ! - " << fname << endl;
    ifs.close();
    return true;
}

// 메모정보를 파일 포멧에 맞춰 저장한다.
bool kw_sns::save_memo_info(string fname)
{
    ofstream ofs(fname);
    
    if(!ofs)
    {
        cout << "can't open file - " << fname << endl;
        return false;
    }
    
    ofs << "==========================================================\n";
    ofs << "본인학번    /   시간        /   글내용\n";
    ofs << "==========================================================\n";
    
    userinfos_by_snum.print_memo_by_inorder(ofs);
    
    ofs.close();
    
    cout << "*memo file created !! - " << fname << endl;
    return true;
    
}

// 메모정보를 로드한다.
bool kw_sns::load_memo_info(string fname)
{
    ifstream ifs(fname);
    
    if(!ifs)
    {
        cout << "can't open file - " << fname << endl;
        return false;
    }
    
    int line_count = 0;
    stringstream ss;
    while(!ifs.eof())
    {
        char buff[1024*4];
        ifs.getline(buff, sizeof(buff));
        
        // 헤더 스킵
        if(line_count++ < 3) continue;
        
        // 공백문자 제거
        trim(buff);
        
        if(strlen(buff) == 0) continue;
        
        ss << buff;
        string snum;
        string time;
        string memo;
        for(int i=0; i<3; i++)
        {
            if(ss.eof()) return false;
            
            char buff2[1024*4] = {0,};
            ss.getline(buff2, sizeof(buff2), '/');
            
            trim(buff2);
            
            //cout << buff2 << endl;
            if(i==0) snum = buff2;
            else if(i==1) time = buff2;
            else if(i==2) memo = buff2;
        }
        
        // memo data insert
        
        user_node* user = userinfos_by_snum.find(snum);
        if(user)
        {
            user->memo_list.add(time, memo);
            //cout << user->to_str() << endl;
            cout << "*memo added !! - " << time << " : " << memo << endl;
        }
        else
        {
        	// 해당 회원이 없을 경우
            cout << "**error : " << snum << " is not our member" << endl;
        }
        
        //cout << buff << endl;
        
        ss.clear();
    }
    
    cout << "*load_memo_info success ! - " << fname << endl;
    ifs.close();
    return true;
}

// 메모정보를 업데이트 한다.
bool kw_sns::update_memo_info(string fname)
{
    ifstream ifs(fname);
    
    if(!ifs)
    {
        cout << "can't open file - " << fname << endl;
        return false;
    }
    
    int line_count = 0;
    stringstream ss;
    while(!ifs.eof())
    {
        char buff[1024*4];
        ifs.getline(buff, sizeof(buff));
        
        // 헤더 스킵
        if(line_count++ < 3) continue;
        
        // 공백문자 제거
        trim(buff);
        
        if(strlen(buff) == 0) continue;
        
        ss << buff;
        string snum;
        string time;
        string memo;
        for(int i=0; i<3; i++)
        {
            if(ss.eof()) break;
            
            char buff2[1024*4] = {0,};
            ss.getline(buff2, sizeof(buff2), '/');
            
            trim(buff2);
            
            // cout << buff2 << endl;
            if(i==0) snum = buff2;
            else if(i==1) time = buff2;
            else if(i==2) memo = buff2;
        }
        
        user_node* user = userinfos_by_snum.find(snum);
        if(user)
        {
            // memo data update
            if(memo.empty()) // 메모가 없을 경우는 기존의 메모를 지우는 명령이다.
            {
                string memo_removed = user->memo_list.remove(time);
                if(memo_removed.empty())
                {
                	// 지울 메모가 없을 경우
                    cout << "***Error : 없는 메모 날짜\n";
                }
                else
                {
                    cout << "*메모 삭제 : " << memo_removed << endl;
                }
                // 삭제 메모
                
            }
            else
            {
                // 새 메모
                user->memo_list.add(time, memo);
                cout << "*memo added !! - " << time << " : " << memo << endl;
            }
        }
        else
        {
            cout << "**error : " << snum << " is not our member" << endl;
        }
        
        //        cout << buff << endl;
        
        ss.clear();
    }
    
    cout << "*update_memo_info success ! - " << fname << endl;
    ifs.close();
    return true;
}

// LOAD 명령어 처리
bool kw_sns::run_cmd_load(const char* cmdline)
{
    if(strlen(cmdline) == 0)
    {
        cout << "**Error invalid cmd\n";
        return false;
    }
    
    stringstream ss;
    ss << cmdline;
    
    while (true)
    {
        if(ss.eof()) break;
        
        char buff2[1024] = {0,};
        ss.getline(buff2, sizeof(buff2), '\t');
        
        trim(buff2);
        
        if(strlen(buff2) == 0) continue;
        
        // LOAD 명령의 파라미터로 따라온 파일 이름에 따라 해당 명령 수행.
        if(strcmp(buff2, "load_user.txt") == 0)
        {
            load_user_info("load_user.txt");
        }
        else if(strcmp(buff2, "load_frnd.txt") == 0)
        {
            load_friend_info("load_frnd.txt");
        }
        else if(strcmp(buff2, "load_memo.txt") == 0)
        {
            load_memo_info("load_memo.txt");
        }
        else
        {
            cout << "**Error unknown file name - " << buff2 << endl;
        }
        
        //        cout << buff2 << endl;
    }
    
    return true;
}

// SAVE 명령어 처리
bool kw_sns::run_cmd_save(const char* cmdline)
{
    if(strlen(cmdline) == 0)
    {
        cout << "**Error invalid cmd\n";
        return false;
    }
    
    stringstream ss;
    ss << cmdline;
    
    while (true)
    {
        if(ss.eof()) break;
        
        char buff2[1024] = {0,};
        ss.getline(buff2, sizeof(buff2), ' ');
        
        trim(buff2);
        
        if(strlen(buff2) == 0) continue;
        
        // SAVE 명령의 파라미터로 따라온 파일 이름에 따라 해당 명령 수행.
        if(strcmp(buff2, "save_user.txt") == 0)
        {
            save_user_info("save_user.txt");
        }
        else if(strcmp(buff2, "save_frnd.txt") == 0)
        {
            save_friend_info("save_frnd.txt");
        }
        else if(strcmp(buff2, "save_memo.txt") == 0)
        {
            save_memo_info("save_memo.txt");
        }
        else
        {
            cout << "**Error unknown file name - " << buff2 << endl;
        }
    }
    
    return true;
}

// FRNDLIST 명령어 처리
bool kw_sns::run_cmd_frndlist(string cmdline)
{
    char snum[100] = {0,};
    strcpy(snum, cmdline.c_str());
    trim(snum);
    
    // 파라미터로 받은 학번으로 AVL트리에서 회원을 조회하여 정보를 출력한다.
    user_node* user = userinfos_by_snum.find(snum);
    if(user)
    {
        int friend_size = user->friend_list.size();
        cout << "==========================\n";
        cout << user->name << "\t" << user->snum << endl;
        //cout << "friend size : " << friend_size << endl;
        cout << "==========================\n";
        
        for(int i=0; i<friend_size; i++)
        {
            string fno = user->friend_list.get(i)->snum_friend;
            user_node* f = userinfos_by_snum.find(fno);
            if(f)
            {
                if(f->is_my_friend(snum))
                {
                	// 서로가 친구일 경우
                    cout << f->name << "씨는 나와 친구입니다.\n";
                }
                else
                {
                	// 나만 친구일 경우
                    cout << f->name << "씨는 나만 친구입니다.\n";
                }
            }
            else
            {
                cout << "**error - internal error\n";
            }
        }
        
        return true;
    }
    else
    {
        cout << "**error : unknown user - " << snum << endl;
        return false;
    }
}

// UPDATE 명령어 처리
bool kw_sns::run_cmd_update(string cmdline)
{
    stringstream ss;
    ss << cmdline;
    
    while (true)
    {
        if(ss.eof()) break;
        
        char buff2[1024] = {0,};
        ss.getline(buff2, sizeof(buff2), '\t');
        
        trim(buff2);
        
        if(strlen(buff2) == 0) continue;
        
        // UPDATE 명령의 파라미터로 따라온 파일 이름에 따라 해당 명령 수행.
        if(strcmp(buff2, "update_user.txt") == 0)
        {
            update_user_info("update_user.txt");
        }
        else if(strcmp(buff2, "update_frnd.txt") == 0)
        {
            update_friend_info("update_frnd.txt");
        }
        else if(strcmp(buff2, "update_memo.txt") == 0)
        {
            update_memo_info("update_memo.txt");
        }
    }
    
    return true;
}






