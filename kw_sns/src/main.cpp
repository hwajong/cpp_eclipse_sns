#include <iostream>
#include <fstream>
#include <sstream>
#include "avl_tree.h"
#include "linkedlist.h"
#include "kw_sns.h"

using namespace std;

// 메뉴 출력
void _print_menu()
{
    printf("\n");
    printf("---------------------------\n");
    printf("MENU\n");
    printf("---------------------------\n");
    printf("LOAD     - 데이터 파일 로드\n");
    printf("SAVE     - 데이터 파일 저장\n");
    printf("UPDATE   - 데이터 파일 업데이트\n");
    printf("FRNDLIST - 친구리스트 출력\n");
    
    printf("EXIT     - 프로그램 종료\n");
    printf("---------------------------\n");
}

// ---------------------------------------
// M A I N
// ---------------------------------------
int main()
{
    kw_sns sns;

    // command.txt 를 읽어 명령어를 처리한다.
    sns.run_cmd_script("command.txt");
    
    // 메뉴 출력
    _print_menu();
    
    char instr[1024] = {0,};
    while(1)
    {
        printf("CMD > ");
        
        fgets(instr, sizeof(instr), stdin);
        sns.trim(instr);

        // 입력받은 명령어에 따라 해당 명령 수행
        if(strncmp(instr, "EXIT", 4) == 0)
        {
            cout << "\nEXIT !!\n";
            return 0;
        }
        else if(strncmp(instr, "LOAD", 4) == 0)
        {
            sns.run_cmd_load(instr + 4);
        }
        else if(strncmp(instr, "SAVE", 4) == 0)
        {
            sns.run_cmd_save(instr + 4);
        }
        else if(strncmp(instr, "UPDATE", 6) == 0)
        {
            sns.run_cmd_update(instr + 6);
        }
        else if(strncmp(instr, "FRNDLIST", 8) == 0)
        {
            sns.run_cmd_frndlist(instr + 8);
        }
    }
        
    return 0;
}


















