#include <stdio.h>
#include <string.h>


typedef int tipe_board[9];

int delta_[20000][9];
int final_[20000];
int start_[3];

#define not_set 0x80808080

FILE *fp;

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

int main()
{
	// inisialisasi variabel
	memset(delta_, 0x80, sizeof(delta_));
	memset(final_, 0x80, sizeof(final_));
	memset(start_, 0x80, sizeof(start_));

	fp=fopen("a.txt", "r");

	int n_final;
	int n_start;
	int n_delta;
	int i;

	fscanf(fp, "%d", &n_start);
	for (i = 0; i < n_start; ++i)
	{
		int a, b;
		fscanf(fp, "%d %d", &a, &b);
		start_[a] = b;
	}

	fscanf(fp, "%d", &n_final);
	for (i = 0; i < n_final; ++i)
	{
		int a, b;
		fscanf(fp, "%d %d", &a, &b);
		final_[a] = b;
	}

	fscanf(fp, "%d", &n_delta);
	for (i = 0; i < n_delta; ++i)
	{
		int a, b, c;
		fscanf(fp, "%d %d %d", &a, &b, &c);
		delta_[a][b] = c;
	}
	
	fclose(fp);
	
    printf("Mau pertama (1) atau kedua (2)? ");
    
    int player=0;
    scanf("%d",&player);
    printf("\n");

    int state=0;

	state = start_[player];

    while (final_[state] == not_set) {
        draw_state(state);
        int x;
        scanf("%d", &x);

        if (state == delta_[state][x])
        	printf("Bodo lu! nggak boleh!\n");
		
		state = delta_[state][x];
    }

    switch(final_[state]) {
        case 0:
            draw_state(state);
            printf("Komputer: Sampah, kok seri sih.\n");
            break;
        case 1:
            draw_state(state);
            printf("Komputer: Wlek.. Lu kalah\n");
            break;
        case -1:
        	draw_state(state);
            printf("Komputer: Kok bisa! Nggak ada statenya lho!\n");
            break;
    }
}