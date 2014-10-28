#include <stdio.h>
#include <stack>
#include <map>
#include <cstring> 

using namespace std;

typedef int tipe_board[9];

char gridChar(int i) {
    switch(i) {
        case -1:
            return 'X';
        case 0:
            return ' ';
        case 1:
            return 'O';
    }
}
 
void draw_state(int state)
{
    char _map[] = {' ', 'O', 'X'};

    tipe_board board;

    int i = 8;

    while (state)
    {
        board[i] = state%3;
        state/=3;
        i--;
    }

    for (; i >= 0; i--) board[i] = 0;

    printf(" %c | %c | %c\n",_map[board[0]],_map[board[1]],_map[board[2]]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",_map[board[3]],_map[board[4]],_map[board[5]]);
    printf("---+---+---\n");
    printf(" %c | %c | %c\n",_map[board[6]],_map[board[7]],_map[board[8]]);
}

int win(tipe_board board) {
    //determines if a player has won, returns 0 otherwise.
    unsigned wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    for(i = 0; i < 8; ++i) {
        if(board[wins[i][0]] != 0 &&
           board[wins[i][0]] == board[wins[i][1]] &&
           board[wins[i][0]] == board[wins[i][2]])
            return board[wins[i][2]];
    }
    return 0;
}

typedef pair<int, int> memo_state;
map <memo_state, int> memo; 

// konversi board ke state

int board_state(tipe_board board){
    int result = 0;
    int map_[3] = {2, 0, 1};

    for (int i = 0; i < 9; ++i)
        result = result*3+map_[board[i]+1];

    return result;
}

// konversi state ke board

void state_board(int state, tipe_board board)
{
    int map_[3] = {0, 1, -1};
    
    for (int i = 0; i < 9; ++i)
        board[i] = 0;

    int i = 8;
    while (state)
    {
        board[i] = map_[state%3];
        state/=3;
        i--;
    }
}

// AI

int minimax(tipe_board board, int player) {

    memo_state now = make_pair(board_state(board), player);

    if (memo.find(now) == memo.end())
    {
        //How is the position like for player (their turn) on board?
        int winner = win(board);
        if(winner != 0) return winner*player;
     
        int move = -1;
        int score = -2;//Losing moves are preferred to no move
        int i;
        for(i = 0; i < 9; ++i) {//For all moves,
            if(board[i] == 0) {//If legal,
                board[i] = player;//Try the move
                int thisScore = -minimax(board, player*-1);
                if(thisScore > score) {
                    score = thisScore;
                    move = i;
                }//Pick the one that's worst for the opponent
                board[i] = 0;//Reset board after try
            }
        }
        if(move == -1) memo[now]= 0;
        else memo[now] = score;

        return memo[now];
    }
    else
    {
        return memo[now];
    }
}
 
void computerMove(tipe_board board) {
    int move = -1;
    int score = -2;
    int i;
    for(i = 0; i < 9; ++i) {
        if(board[i] == 0) {
            board[i] = 1;
            int tempScore = -minimax(board, -1);
            board[i] = 0;
            if(tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    //returns a score based on minimax tree at a given node.
    board[move] = 1;
}
 
int playerMove(tipe_board board, int move) {
    if (board[move] != 0)
    {
        return 0;
    }
    else
    {
        board[move] = -1;
        return 1;
    }
}

bool tie(tipe_board board)
{
    bool tie = true;
    for (int i = 0; (i < 9) && tie; ++i)
        tie = (board[i] != 0);
    return tie;
}

typedef map < pair<int, int>, int> map_delta;
typedef map <int, int> map_win;

map_delta delta;
map_win win_state;

void pre_compute(tipe_board board)
{
    //draw(board);

    int current_state = board_state(board);

    for (int i = 0; i < 9; ++i)
    {
        if (playerMove(board, i))
        {
            if (!win(board) && !tie(board))
            {
                computerMove(board);

                if (!win(board) && !tie(board))
                {
                    delta[make_pair(current_state, i)] = board_state(board);
                    pre_compute(board);
                }
                else
                {
                    delta[make_pair(current_state, i)] = board_state(board);
                    win_state[board_state(board)] = win(board);
                }
                
            }
            else
            {
                delta[make_pair(current_state, i)] = board_state(board);
                win_state[board_state(board)] = win(board);
            }
        }
        else
        {
            delta[make_pair(current_state, i)] = board_state(board);
        }

        state_board(current_state, board);
    }
}

char win_color[3][20] = {"lightgray", "tomato", "palegreen"};

int main() {
    tipe_board board = {0,0,0,0,0,0,0,0,0};

    int start_point; 

    pre_compute(board);
    //computerMove(board);
    //pre_compute(board);

    start_point = board_state(board);

    printf("digraph finite_state_machine {\nrankdir=LR;\nsize=\"800\"\n");

    for (map_win::iterator it=win_state.begin(); it!=win_state.end(); ++it)
        printf("node [shape = doublecircle, style=filled, fillcolor = %s]; %d;\n", win_color[it->second], it->first);

    printf("node [shape = point ]; qi;\n");
    printf("node [shape = circle style=filled, fillcolor = white];\n");
    printf("qi -> %d;\n", start_point);

    char self_loop[20000][100];

    memset(self_loop, 0, sizeof(self_loop));

    for (map_delta::iterator it=delta.begin(); it!=delta.end(); ++it){
        if ((it->first).first == it->second)
        {
            char current[100];
            
            strcpy(current, self_loop[(it->first).first]);

            if (strlen(current)==0)
                sprintf(self_loop[(it->first).first], "%d", (it->first).second);
            else
                sprintf(self_loop[(it->first).first], "%s, %d", current, (it->first).second);
        }
        else
        {
            printf("%d -> %d [ label = \"%d\" ];\n", (it->first).first, it->second, (it->first).second);
        }
    }

    for (int i = 0; i < 20000; ++i)
        if (strlen(self_loop[i])>0)
            printf("%d -> %d [ label = \"%s\" ];\n", i, i, self_loop[i]);
    
    printf("}\n");
}