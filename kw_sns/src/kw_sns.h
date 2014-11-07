#ifndef __KW_SNS__kw_sns__
#define __KW_SNS__kw_sns__

#include <string>
#include "avl_tree.h"

using namespace std;

class kw_sns
{
private:
    
    avl_tree userinfos_by_name;
    avl_tree userinfos_by_snum; // student number

    bool load_user_info(const string fname);
    bool load_friend_info(string fname);
    bool load_memo_info(string fname);

    bool update_user_info(const string fname);
    bool update_friend_info(string fname);
    bool update_memo_info(string fname);
    
    bool save_user_info(const string fname);
    bool save_friend_info(string fname);
    bool save_memo_info(string fname);
    
public:
    void trim(char* s);
    
    bool run_cmd_script(string fname);
    
    bool run_cmd_load(const char* cmdline);
    bool run_cmd_frndlist(string cmdline);
    bool run_cmd_update(string cmdline);
    bool run_cmd_save(const char* cmdline);
};

#endif /* defined(__KW_SNS__kw_sns__) */
