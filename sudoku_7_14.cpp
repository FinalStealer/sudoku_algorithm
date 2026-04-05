#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <utility>
#include <set>
#include <stack>
#include <map>
using namespace std;
int n,non_filled;
string gamemode;
bool win = false,if_errors;
vector<vector<int>> answer; // fast_mode :the final answer for the sudoku
vector<vector<bool>> fix; // fast_mode : if fix[i][j] == true -> answer[i][j] cannot be change
vector<vector<unordered_set<int>>> remain; 
// fast_mode : if remain[i][j] = {1,2,5,9} -> answer[i][j] must be 1,2,5 or 9
vector<vector<vector<vector<int>>>>board_block;// a temporary storage to keep the inputs
// vectors and stacks below are for pro mode 
vector<pair<pair<int,int>,int>> path; // pro_mode(debug) : remember all the guesses
stack<pair<vector<vector<unordered_set<int>>>,vector<vector<int>>>> quest; //
stack<int> filled_stack;
stack<vector<vector<bool>>> fix_stack;


void print_out(int type){
    if(type == 2){
        cout << "input the size of the sukodu first ;)" << '\n';
    }
    if(type == 3) cout << "==============================" << '\n';
    if(type == 4){
        for(auto i : quest.top().first){   
            for(auto j : i) {
                cout << "( ";
                for(auto k : j) cout << k << " ";
                cout << ") ";
            }
            cout << '\n';

        }
    }
    if(type == 5){
        for(auto i : quest.top().second) {
            for(auto j : i) cout << j << " ";
            cout << '\n';
        }
    }
    
}


void init(){// deal with the inputs
    print_out(2);
    cin >> n;
    cout << '\n' << "well done !, now please choose your gamemode :" << '\n';
    cin >> gamemode;
    while(true){
        if(gamemode == "fast") break;
        if(gamemode == "pro") break;
        cout << "unknown gamemode..." << '\n';
        cin >> gamemode;
    }
    
}
int convert(char num){
    return num-'0';
}
void info_init(){
    non_filled = n*n*n*n;
    unordered_set<int> sea;
    for(int i=1;i<n*n+1;i++) sea.insert(i);
    remain = vector<vector<unordered_set<int>>>(n*n,vector<unordered_set<int>> (n*n,sea));
    fix= vector<vector<bool>> (n*n,vector<bool>(n*n,false));
    answer = vector<vector<int>> (n*n,vector<int> (n*n,-1));
    auto board_block_temp = vector<vector<vector<int>>>(n,vector<vector<int>> (n,vector<int> (n,-1)));
    for(int i=0;i<n;i++) board_block.push_back(board_block_temp);
    for(int i=0;i<n*n;i++) {
        string stay;
        cin >> stay;
        for(int j=0;j<n*n;j++) answer[i][j] = convert(stay[j]);
        
    }
    for(int i=0;i<n*n;i++) {
        vector<int> temp;
        for(int j=0;j<n*n;j++){
            int cur_r = answer[i][j];
            int cur_c = answer[j][i];
            if((cur_r==0)&&(cur_c==0)) continue;
            for(int k=0;k<n*n;k++){
                remain[i][k].erase(cur_r);
                remain[k][i].erase(cur_c);
            }
            
        }
    }
    for(int i=0;i<n*n;i++) for(int j=0;j<n*n;j++) {
        if(answer[i][j] != 0) {
            remain[i][j] = {answer[i][j]};
            non_filled--;
            fix[i][j] = true;
        }
    }
    //print_out(1);
    if(gamemode == "pro"){
        quest.push({remain,answer});    
        filled_stack.push(non_filled);
        fix_stack.push(fix);
    }
}
void check_row(){
    for(int i=0;i<n*n;i++){
        unordered_set<int> temp;
        unordered_map<int,int> if_repeat;
        map<int,int> erasing_cnt,erasing_pos;
        if(gamemode == "fast"){
            for(int j=0;j<n*n;j++) temp.insert(answer[i][j]);
            for(int j=0;j<n*n;j++) for(auto k : temp) if(!fix[i][j]) {
                remain[i][j].erase(k);
            }
            for(int j=0;j<n*n;j++) for(auto k : remain[i][j]) {
                if(!fix[i][j]){
                    erasing_cnt[k]++;
                    erasing_pos[k] = j;
                }
            }
            for(auto j :erasing_cnt) if(j.second == 1){
                remain[i][erasing_pos[j.first]] = {j.first};
            }
        }
        else if(gamemode == "check"){
            for(int j=0;j<n*n;j++) if(quest.top().second[i][j]!=0)if_repeat[quest.top().second[i][j]]++;
            for(auto j : if_repeat) if(j.second > 1) if_errors = true;
        }else if(gamemode == "pro"){
            for(int j=0;j<n*n;j++) temp.insert(quest.top().second[i][j]);
            for(int j=0;j<n*n;j++) for(auto k : temp) if(!fix_stack.top()[i][j]) quest.top().first[i][j].erase(k);
            for(int j=0;j<n*n;j++) for(auto k : quest.top().first[i][j]) {
                if(!fix_stack.top()[i][j]){
                    erasing_cnt[k]++;
                    erasing_pos[k] = j;
                }
            }
            for(auto j :erasing_cnt) if(j.second == 1) quest.top().first[i][erasing_pos[j.first]] = {j.first};
        }    
    }   
}
void check_col(){
    for(int i=0;i<n*n;i++){
        unordered_set<int> temp;
        unordered_map<int,int> if_repeat;
        map<int,int> erasing_cnt,erasing_pos;
        if(gamemode == "fast"){
            for(int j=0;j<n*n;j++) temp.insert(answer[j][i]);
            for(int j=0;j<n*n;j++) for(auto k : temp) if(!fix[j][i]) {//剩餘消去
                remain[j][i].erase(k);
            }
            for(int j=0;j<n*n;j++) for(auto k : remain[j][i]) {//限定消去
                if(!fix[j][i]){
                    erasing_cnt[k]++;
                    erasing_pos[k] = j;
                }
            }
            for(auto j :erasing_cnt) if(j.second == 1) {//限定消去
                remain[erasing_pos[j.first]][i] = {j.first};
            }
        }else if(gamemode == "check"){
            for(int j=0;j<n*n;j++) if(quest.top().second[j][i]!=0)if_repeat[quest.top().second[j][i]]++;
            for(auto j : if_repeat) if(j.second > 1) if_errors = true;
        }else if(gamemode == "pro"){
            for(int j=0;j<n*n;j++) temp.insert(quest.top().second[j][i]);
            for(int j=0;j<n*n;j++) for(auto k : temp) if(!fix_stack.top()[j][i]) quest.top().first[j][i].erase(k);
            for(int j=0;j<n*n;j++) for(auto k : quest.top().first[j][i]) {
                if(!fix_stack.top()[j][i]){
                    erasing_cnt[k]++;
                    erasing_pos[k] = j;
                }
            }
            for(auto j :erasing_cnt) if(j.second == 1) quest.top().first[erasing_pos[j.first]][i] = {j.first};
        }
        
    }
}
void check_block(){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            unordered_set<int> temp;
            unordered_map<int,int> if_repeat;
            map<int,int> erasing_cnt;
            map<int,pair<int,int>> erasing_pos;
            if(gamemode == "fast"){
                for(int k=i*n;k<i*n+n;k++) for(int l=j*n;l<j*n+n;l++) {//剩餘消去
                    if(answer[k][l] != 0) temp.insert(answer[k][l]);
                }
                for(int k=i*n;k<i*n+n;k++)for(int l=j*n;l<j*n+n;l++){//剩餘消去
                    for(auto m : temp) if(!fix[k][l]) remain[k][l].erase(m);
                } 
                for(int k=i*n;k<i*n+n;k++)for(int l=j*n;l<j*n+n;l++){//限定消去
                    for(auto m : remain[k][l]) if(!fix[k][l]){
                        erasing_cnt[m]++;
                        erasing_pos[m] = {k,l};
                    }
                }
                for(auto k : erasing_cnt) if(k.second == 1) {//限定消去
                    remain[erasing_pos[k.first].first][erasing_pos[k.first].second] = {k.first};
                }
            }
            else if(gamemode == "check"){
                for(int k=i*n;k<i*n+n;k++) for(int l=j*n;l<j*n+n;l++) {
                    if(quest.top().second[k][l] != 0) if_repeat[quest.top().second[k][l]]++;
                //cout << l << " ";
                }
                for(auto k : if_repeat) if(k.second > 1) if_errors = true;
            }else if(gamemode == "pro"){
                for(int k=i*n;k<i*n+n;k++) for(int l=j*n;l<j*n+n;l++) {
                    if(quest.top().second[k][l] != 0) temp.insert(quest.top().second[k][l]);
                //cout << l << " ";
                }
                for(int k=i*n;k<i*n+n;k++)for(int l=j*n;l<j*n+n;l++){
                    for(auto m : temp) if(!fix_stack.top()[k][l]) quest.top().first[k][l].erase(m);
                } 
                for(int k=i*n;k<i*n+n;k++)for(int l=j*n;l<j*n+n;l++){
                    for(auto m : quest.top().first[k][l]) if(!fix_stack.top()[k][l]){
                        erasing_cnt[m]++;
                        erasing_pos[m] = {k,l};
                    }
                }
                for(auto k : erasing_cnt) if(k.second == 1) {
                    quest.top().first[erasing_pos[k.first].first][erasing_pos[k.first].second] = {k.first};
                }
            }
            
        }
    }

}
void fast_solve(){
    int cnt = 1;
    cout << "Now the cpu is burning......" << '\n';
    while (non_filled > 0){
        cnt++;
        check_row();
        check_col();
        check_block();
        for(int i=0;i<n*n;i++) for(int j=0;j<n*n;j++){
            if((remain[i][j].size() == 1)&&(!fix[i][j])){
                //cout << i << " " << j << '\n';  
                for(auto k : remain[i][j]) answer[i][j] = k;
                fix[i][j] = true;
                non_filled--;
            }
        }
        if(cnt > 200) {
            break;
        }
    }
    if(cnt > 200){
        cout << "Oof this sudoku is too hard for fast mode :(" << '\n';
        return;
    }
    cout << "After my FAST calculation, I think the answer is :" << '\n';
    for(auto i : answer) {
        for(auto j : i) cout << j << " ";
        cout << '\n';
    }


}

bool error_occur(vector<vector<int>> ans){
    gamemode = "check";
    if_errors = false;
    check_col();
    check_row();
    check_block();
    gamemode = "pro";
    return if_errors;
}

bool ok(){
    int result = 0;
    for(auto i : quest.top().first) for(auto j : i) if(j.size() == 1) result++;
    return (result < n*n*n*n);
}
void pro_solve(){
    int cnt = 0,last_filled;
    cout << "Now the cpu is burning FIERCELY......" << '\n';
    print_out(3);
    
    while (filled_stack.top() > 0){

        last_filled = filled_stack.top();
        cout << " round " << quest.size() << " : remaining " ;
        cout<< filled_stack.top() << " numbers"<< '\n';
        cnt++;
        check_row();
        check_col();
        check_block();
        for(int i=0;i<n*n;i++) for(int j=0;j<n*n;j++){
            if(quest.top().first[i][j].size() == 0){
                filled_stack.pop();
                fix_stack.pop();
                quest.pop();
                //path.push_back({{10000,10000},100000});
                cout << "stack failed.... killed by set_size()" << '\n';
                return;
            }
            if((quest.top().first[i][j].size() == 1)&&(!fix_stack.top()[i][j])){
                //cout << i << " " << j << '\n';  
                for(auto k : quest.top().first[i][j]) quest.top().second[i][j] = k;
                fix_stack.top()[i][j] = true;
                filled_stack.top()--;
            }
        }
        if(last_filled == filled_stack.top()){
            cout << "numbers repeats !" << '\n';            
            if(error_occur(quest.top().second)) {
                quest.pop();
                filled_stack.pop();
                fix_stack.pop();
                cout << "stack failed....0. :( killed by repeat()" << '\n';
                print_out(3);
                return;
            }
            map<int,pair<pair<int,int>,unordered_set<int>>>awaiting;
            for(int i=0;i<quest.top().first.size();i++) for(int j=0;j<quest.top().first.size();j++) {
                if(quest.top().first[i][j].size() > 1){
                    awaiting[quest.top().first[i][j].size()]={{i,j},quest.top().first[i][j]};
                }
            }
            auto it = awaiting.begin();
            auto i = *it;
            int r = i.second.first.first;
            int c = i.second.first.second;
            auto temp_answer = quest.top().second;
            auto temp_remain = quest.top().first;
            auto temp_fix = fix_stack.top();
            for(auto j : i.second.second){
                temp_answer[r][c] = j;
                temp_remain[r][c] = {j};
                temp_fix[r][c] = true;
                filled_stack.push(last_filled-1);
                quest.push({temp_remain,temp_answer});
                fix_stack.push(temp_fix);                    
                cout << " calling new stack by guessing blank (" << r << ", " << c << ") is " << j << '\n';
                pro_solve();
                if(win) return;
            }
             return;
        }
        
        //cout << filled ;
    }
    if(error_occur(quest.top().second)) {
        quest.pop();
        filled_stack.pop();
        fix_stack.pop();
        //path.push_back({{10000,10000},100000});
        cout << "stack failed..... :( out of range" << '\n';
        print_out(3);
        return;
    }
    else{
        cout << "stack_complete ! (by ending loop" << '\n';
        win = true;
        return;
    }
}
int main(){
    init();
    info_init();
    if(gamemode == "fast") {    
        fast_solve();
    }else{
        pro_solve();
        if(!win){
            cout << "crashed ! searching if have missing answers..." << '\n';
        }
        if(win){
            print_out(3);
            cout << "After my PRO calculation, I think the answer is :" << '\n';
            for(auto i : quest.top().second) {
                for(auto j : i) cout << j << " ";
                cout << '\n';
            }
        }
    }
}


/*


HARDEST:
3
pro
800000000
003600000
070090200
050007000
000045700
000100030
001000068
008500010
090000400



GAME :
3
pro
000080050
060002000
007000300
500000080
090700000
000900000
000300607
300040000
000000900


BOOK:
3
pro
050000002
000409300
800010007
900003008
002000600
300100009
400070003
008205000
600000090


*/



