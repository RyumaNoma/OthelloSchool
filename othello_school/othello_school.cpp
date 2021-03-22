#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 10
#define T 100

struct INPUT_DATA
{
	int x;
	int y;
};

void make_board(int board[N][N]);
void print_board(int board[N][N]);
int check_finish(int t, int board[N][N]);
int check_pass(int k, int board[N][N]);
struct INPUT_DATA input_key(int k, int board[N][N]);
void change_board(int k, int x, int y, int board[N][N]);
int check_change(int k, int x, int y, int board[N][N]);
int check_pass(int k, int board[N][N]);

void copy(const int src[N][N], int dest[N][N]);
int count_stone(int board[N][N], int k);
int count_stone_based_on_score(int k, int board[N][N]);

struct INPUT_DATA randomAI(int k, int board[N][N]);
struct INPUT_DATA maxAI(int k, int board[N][N]);
struct INPUT_DATA maxAI_based_on_score(int k, int board[N][N]);
double utility(int board[N][N], int k, struct INPUT_DATA coordinate, int depth, int max_depth);
struct INPUT_DATA Ntesaki(int k, int board[N][N], int search_depth);
int play_out(int start, int board[N][N]);
struct INPUT_DATA monte(int k, int board[N][N]);

int open_rate(int k, struct INPUT_DATA input, int board[N][N], int cmp_board[N][N]);
struct INPUT_DATA openAI(int k, int board[N][N]);

int open_u(int board[N][N], int k, struct INPUT_DATA input, int depth, int max_depth);
struct INPUT_DATA NopenAI(int k, int board[N][N], int search_depth);

struct INPUT_DATA sekiAI(int k, int board[N][N]);

int surround(int k, int board[N][N]);
struct INPUT_DATA yashioAI(int k, int board[N][N]);

const int score[8][8] = {
	{ 10000,    5,  100,   10,   10,  100,    5, 10000 },
	{ 5,    1,   15,   20,   20,   15,    1,    5 },
	{ 100,   15,   25,   50,   50,   25,   15,  100 },
	{ 10,   20,   50,  200,  200,   50,   20,   10 },
	{ 10,   20,   50,  200,  200,   50,   20,   10 },
	{ 100,   15,   25,   50,   50,   25,   15,  100 },
	{ 5,    1,   15,   20,   20,   15,    1,    5 },
	{ 10000,    5,  100,   10,   10,  100,    5, 10000 }
};

// 白色 ...  1 ... O
// 黒色 ... -1 ... X
int main(void) {

	//乱数の初期化
	srand(time(NULL));
	int first_win = 0;

	int GAMES = 100;
	scanf_s("%d", &GAMES);

	int sente = 100;
	int gote = 100;

	printf("sente:");
	scanf_s("%d", &sente);
	printf("gote:");
	scanf_s("%d", &gote);

	for (int match = 0; match < GAMES; match++)
	{
		int board[N][N] = { 0 };
		int t;
		int k;
		struct INPUT_DATA input_data;

		make_board(board);
		print_board(board);

		t = 0;
		k = -1;
		while (check_finish(t, board) != 0)
		{
			t++;
			if (check_pass(k, board) != 0)
			{

				if (t % 2)
				{
					switch (sente)
					{
					case 0:
						input_data = randomAI(k, board);
						break;
					case 1:
						input_data = maxAI(k, board);
						break;
					case 2:
						input_data = maxAI_based_on_score(k, board);
						break;
					case 3:
						input_data = Ntesaki(k, board, 2);
						break;
					case 4:
						input_data = monte(k, board);
						break;
					case 5:
						input_data = openAI(k, board);
						break;
					case 6:
						input_data = NopenAI(k, board, 3);
						break;
					case 7:
						input_data = sekiAI(k, board);
						break;
					case 8:
						input_data = yashioAI(k, board);
						break;
					default:
						input_data = input_key(k, board);
						break;
					}
				}
				else
				{
					switch (gote)
					{
					case 0:
						input_data = randomAI(k, board);
						break;
					case 1:
						input_data = maxAI(k, board);
						break;
					case 2:
						input_data = maxAI_based_on_score(k, board);
						break;
					case 3:
						input_data = Ntesaki(k, board, 2);
						break;
					case 4:
						input_data = monte(k, board);
						break;
					case 5:
						input_data = openAI(k, board);
						break;
					case 6:
						input_data = NopenAI(k, board, 3);
						break;
					case 7:
						input_data = sekiAI(k, board);
						break;
					case 8:
						input_data = yashioAI(k, board);
						break;
					default:
						input_data = input_key(k, board);
						break;
					}
				}

				printf("(%c, %d)に置きました\n", char(input_data.x + 'a' - 1), input_data.y);

				change_board(k, input_data.x, input_data.y, board);
				printf("%dターン\n", t);
				print_board(board);
			}
			k = k * -1;
		}

		printf("第%d試合\n", match + 1);
		print_board(board);
		int a, b;
		printf("先手 : %d\n", a = count_stone(board, -1));
		printf("後手 : %d\n", b = count_stone(board, 1));

		if (a > b)
		{
			first_win++;
		}
	}

	putchar('\n');
	puts("最終結果");
	printf("先手 : %d\n", first_win);
	printf("後手 : %d\n", GAMES - first_win);
}

void make_board(int board[N][N])
{
	// 壁を作る
	for (int i = 0; i < N; i++)
	{
		board[0][i] = 2;
		board[N - 1][i] = 2;
		board[i][0] = 2;
		board[i][N - 1] = 2;
	}

	// 初期の4コマを置く
	board[4][4] = 1;
	board[5][5] = 1;
	board[4][5] = -1;
	board[5][4] = -1;
}

void print_board(int board[N][N])
{
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (board[y][x] == 1)
			{
				printf("O");
			}
			else if (board[y][x] == -1)
			{
				printf("X");
			}
			else if (board[y][x] == 2)
			{
				if (y == 0 && 1 <= x && x <= 8)
				{
					printf("%c", (char)x + 'a' - 1);
				}
				else if (x == 0 && 1 <= y && y <= 8)
				{
					printf("%d", y);
				}
				else
				{
					printf("+");
				}
			}
			else
			{
				printf(" ");
			}
			printf(" ");
		}
		printf("\n");
	}
}

// 0 ... finish
// 1 ... continue
int check_finish(int t, int board[N][N])
{
	if (t >= T) return 0;

	if (check_pass(1, board) == 0 && check_pass(-1, board) == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

// 0 ... pass
// 1 ... put
int check_pass(int k, int board[N][N])
{
	for (size_t y = 1; y <= 8; y++)
	{
		for (size_t x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				return 1;
			}
		}
	}

	return 0;
}

struct INPUT_DATA input_key(int k, int board[N][N])
{
	struct INPUT_DATA input;
	do
	{
		char x[10] = { '\0' };
		int y;
		printf("X (小文字) : ");
		scanf_s("%s", x);
		printf("Y : ");
		scanf_s("%d", &y);

		input = { (int)x[0] - 'a' + 1, y };
	} while (check_change(k, input.x, input.y, board) == 0 && printf("そこは置けません\nもう一度入力してください\n"));

	return input;
}

void change_board(int k, int x, int y, int board[N][N])
{
	int I = y;
	int J = x;
	int l = k * -1;

	board[I][J] = k;

	int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

	for (int idx = 0; idx < 8; idx++)
	{
		while (board[I + dy[idx]][J + dx[idx]] == l)
		{
			I += dy[idx];
			J += dx[idx];
		}

		if (board[I + dy[idx]][J + dx[idx]] == k)
		{
			I = y;
			J = x;
			while (board[I + dy[idx]][J + dx[idx]] == l)
			{
				board[I + dy[idx]][J + dx[idx]] = k;

				I += dy[idx];
				J += dx[idx];
			}
		}
		I = y;
		J = x;
	}
}

// 0 ... No
// 1以上 ... Yes
int check_change(int k, int x, int y, int board[N][N])
{
	if (x < 1 || 8 < x ||
		y < 1 || 8 < y)
	{
		return 0;
	}

	int I = y;
	int J = x;
	// 敵の番号
	int l = k * -1;

	int dx[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	int dy[] = { -1, -1, 0, 1, 1, 1, 0, -1 };

	int check = 0;

	if (board[y][x] != 0)
	{
		return check;
	}

	for (int idx = 0; idx < 8; idx++)
	{
		while (board[I + dy[idx]][J + dx[idx]] == l)
		{
			check = 1;
			I += dy[idx];
			J += dx[idx];
		}
		if (board[I + dy[idx]][J + dx[idx]] == k && check == 1)
		{
			check = 1;
			return check;
		}
		I = y;
		J = x;
		check = 0;
	}
	return check;
}

void copy(const int src[N][N], int dest[N][N])
{
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			dest[y][x] = src[y][x];
		}
	}
}

int count_stone(int board[N][N], int k)
{
	int count = 0;
	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (board[y][x] == k)
			{
				count++;
			}
		}
	}

	return count;
}

int count_stone_based_on_score(int k, int board[N][N])
{
	int sum = 0;
	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (board[y][x] == k)
			{
				sum += score[y - 1][x - 1];
			}
		}
	}

	return sum;
}

int count_can_put(int board[N][N], int k)
{
	int count = 0;

	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				count++;
			}
		}
	}

	return count;
}

struct INPUT_DATA randomAI(int k, int board[N][N])

{
	// search algorithm
	INPUT_DATA can_put_list[60];
	int how_many_can_put = 0;
	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				can_put_list[how_many_can_put] = INPUT_DATA{ x, y };
				how_many_can_put++;
			}
		}
	}

	//random
	int rand_number = rand() % how_many_can_put;
	return can_put_list[rand_number];
}

struct INPUT_DATA maxAI(int k, int board[N][N])
{
	// search algorithm
	INPUT_DATA can_put_list[60];
	int can_change_num[60] = { 0 };
	int how_many_can_put = 0;
	int max_change = 0;

	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				int virtual_board[N][N];
				copy(board, virtual_board);
				change_board(k, x, y, virtual_board);

				can_put_list[how_many_can_put] = INPUT_DATA{ x, y };
				can_change_num[how_many_can_put] = count_stone(virtual_board, k);
				max_change = (max_change < can_change_num[how_many_can_put]) ? can_change_num[how_many_can_put] : max_change;

				how_many_can_put++;
			}
		}
	}

	INPUT_DATA can_put_max_change[60];
	int max_change_num = 0;

	for (int i = 0; i < how_many_can_put; i++)
	{
		if (can_change_num[i] == max_change)
		{
			can_put_max_change[max_change_num] = can_put_list[i];
			max_change_num++;
		}
	}

	int r = rand() % max_change_num;
	return can_put_max_change[r];
}

struct INPUT_DATA maxAI_based_on_score(int k, int board[N][N])
{
	// search algorithm
	INPUT_DATA can_put_list[60];
	int can_change_num[60] = { 0 };
	int how_many_can_put = 0;
	int max_change = 0;

	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				int virtual_board[N][N];
				copy(board, virtual_board);
				change_board(k, x, y, virtual_board);

				can_put_list[how_many_can_put] = INPUT_DATA{ x, y };
				can_change_num[how_many_can_put] = count_stone_based_on_score(k, virtual_board);
				max_change = (max_change < can_change_num[how_many_can_put]) ? can_change_num[how_many_can_put] : max_change;

				how_many_can_put++;
			}
		}
	}

	INPUT_DATA can_put_max_change[60];
	int max_change_num = 0;

	for (int i = 0; i < how_many_can_put; i++)
	{
		if (can_change_num[i] == max_change)
		{
			can_put_max_change[max_change_num] = can_put_list[i];
			max_change_num++;
		}
	}

	int r = rand() % max_change_num;
	return can_put_max_change[r];
}

// N手先score_board
double utility(int board[N][N], int k, struct INPUT_DATA coordinate, int depth, int max_depth)
{
	int setter = (depth % 2) ? k : -k;
	int v_board[N][N];
	copy(board, v_board);
	change_board(setter, coordinate.x, coordinate.y, v_board);

	if (depth == max_depth)
	{
		return (double)count_stone_based_on_score(k, v_board);
	}

	double sum = 0;
	int count = 0;
	int next_setter = -setter;
	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			struct INPUT_DATA next = { x, y };
			if (check_change(next_setter, x, y, v_board))
			{
				sum += utility(v_board, k, next, depth + 1, max_depth);
				count++;
			}
		}
	}

	if (count > 0)
	{
		return sum / count;
	}
	else
	{
		return double(count_stone_based_on_score(k, v_board));
	}

}

struct INPUT_DATA Ntesaki(int k, int board[N][N], int search_depth)
{
	INPUT_DATA can_put_list[60];
	double utility_points[60] = { 0 };
	int can_put_num = 0;
	double max = 0;

	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				struct INPUT_DATA c = { x, y };
				double utility_point = utility(board, k, c, 1, search_depth);

				can_put_list[can_put_num] = c;
				utility_points[can_put_num] = utility_point;
				max = (max < utility_point) ? utility_point : max;
				can_put_num++;
			}
		}
	}

	INPUT_DATA max_list[60];
	int max_utility_num = 0;

	for (int i = 0; i < can_put_num; i++)
	{
		if (utility_points[i] == max)
		{
			max_list[max_utility_num] = can_put_list[i];
			max_utility_num++;
		}
	}

	int r = rand() % max_utility_num;
	return max_list[r];
}

int play_out(int start, int board[N][N])
{
	int v_board[N][N];
	copy(board, v_board);

	int now = start;
	while (check_finish(0, v_board))
	{
		if (check_pass(now, v_board))
		{
			struct INPUT_DATA input_data = randomAI(now, v_board);
			change_board(now, input_data.x, input_data.y, v_board);
		}
		now *= -1;
	}

	int a = count_stone(v_board, 1);
	int b = count_stone(v_board, -1);

	return ((a > b) ? 1 : -1);
}

struct INPUT_DATA monte(int k, int board[N][N])
{
	int enemy = -k;
	const int game_count = 1000;
	struct INPUT_DATA list[60];
	int win_list[60];
	int how_many_can_put = 0;

	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				// v_boardに打つ
				int v_board[N][N];
				copy(board, v_board);
				change_board(k, x, y, v_board);

				// game_count回プレイアウトする
				int win = 0;
				for (int i = 0; i < game_count; i++)
				{
					int winner = play_out(enemy, v_board);
					if (winner == k)
					{
						win++;
					}
				}

				// 勝利数を格納
				win_list[how_many_can_put] = win;
				list[how_many_can_put] = { x,y };
				how_many_can_put++;
			}
		}
	}

	// 昇順にバブルソート
	int max = 0;
	int max_i = 0;
	for (int i = 0; i < how_many_can_put; i++)
	{
		if (max < win_list[i])
		{
			max = win_list[i];
			max_i = i;
		}
	}

	return list[max_i];
}

int open_rate(int k, struct INPUT_DATA input, int board[N][N], int cmp_board[N][N])
{
	int count = 0;
	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (x == input.x && y == input.y)
			{
				continue;
			}
			// 自分の駒に変化していたら
			if (board[y][x] != cmp_board[y][x] && board[y][x] == k)
			{
				// 周囲8方位を調べる
				for (int dy = -1; dy < 2; dy++)
				{
					for (int dx = -1; dx < 2; dx++)
					{
						// 置いたマスなら
						if (input.x == x + dx && input.y == y + dy)
						{
							count++;
						}
						// 空いてるなら
						if (board[y + dy][x + dx] == 0)
						{
							count++;
						}
					}
				}
			}
		}
	}

	return count;
}

struct INPUT_DATA openAI(int k, int board[N][N])
{
	// search algorithm
	INPUT_DATA can_put_list[60];
	int score[60] = { 0 };
	int how_many_can_put = 0;
	int min_change = 10000;

	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				int v_board[N][N];
				copy(board, v_board);
				change_board(k, x, y, v_board);

				can_put_list[how_many_can_put] = INPUT_DATA{ x, y };
				score[how_many_can_put] = open_rate(k, INPUT_DATA{ x, y }, v_board, board);
				printf("(%c, %d) : %d\n", (char)(x + 'a' - 1), y, score[how_many_can_put]);
				min_change = (min_change > score[how_many_can_put]) ? score[how_many_can_put] : min_change;

				how_many_can_put++;
			}
		}
	}

	INPUT_DATA can_put_min_change[60];
	int min_change_num = 0;

	for (int i = 0; i < how_many_can_put; i++)
	{
		if (score[i] == min_change)
		{
			can_put_min_change[min_change_num] = can_put_list[i];
			min_change_num++;
		}
	}

	int r = rand() % min_change_num;
	return can_put_min_change[r];
}

int open_u(int board[N][N], int k, struct INPUT_DATA input, int depth, int max_depth)
{
	int v_board[N][N];
	copy(board, v_board);
	change_board(k, input.x, input.y, v_board);
	int now_open = open_rate(k, input, v_board, board);

	if (depth == max_depth)
	{
		return now_open;
	}

	int min = 10000;
	int count = 0;
	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(-k, x, y, v_board))
			{
				struct INPUT_DATA next = { x, y };
				int p = open_u(v_board, -k, next, depth + 1, max_depth);
				min = (min > p) ? p : min;
				count++;
			}
		}
	}

	if (count > 0)
	{
		if (depth % 2)
		{
			return now_open + min;
		}
		else
		{
			return now_open - min;
		}
	}
	else
	{
		return now_open;
	}
}

struct INPUT_DATA NopenAI(int k, int board[N][N], int search_depth)
{
	INPUT_DATA can_put_list[60];
	int utility_points[60] = { 0 };
	int can_put_num = 0;
	double min = 10000;

	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				struct INPUT_DATA c = { x, y };
				int utility_point = open_u(board, k, c, 1, 2);
				printf("(%c,%d) : %d\n", (char)(x + 'a' - 1), y, utility_point);

				can_put_list[can_put_num] = c;
				utility_points[can_put_num] = utility_point;
				min = (min > utility_point) ? utility_point : min;
				can_put_num++;
			}
		}
	}

	INPUT_DATA min_list[60];
	int min_utility_num = 0;

	for (int i = 0; i < can_put_num; i++)
	{
		if (utility_points[i] == min)
		{
			min_list[min_utility_num] = can_put_list[i];
			min_utility_num++;
		}
	}

	int r = rand() % min_utility_num;
	return min_list[r];
}

struct INPUT_DATA sekiAI(int k, int board[N][N])
{
	// search algorithm
	INPUT_DATA can_put_list[60];
	long long int score[60] = { 0 };
	int how_many_can_put = 0;
	long long int max_change = 0;

	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				int v_board[N][N];
				copy(board, v_board);
				change_board(k, x, y, v_board);

				can_put_list[how_many_can_put] = INPUT_DATA{ x, y };
				score[how_many_can_put] = (100 - open_rate(k, INPUT_DATA{ x, y }, v_board, board)) * count_stone_based_on_score(k, v_board);
				//printf("(%c, %d) : %d\n", (char)(x + 'a' - 1), y, score[how_many_can_put]);
				max_change = (max_change < score[how_many_can_put]) ? score[how_many_can_put] : max_change;

				how_many_can_put++;
			}
		}
	}

	INPUT_DATA can_put_max_change[60];
	int max_change_num = 0;

	for (int i = 0; i < how_many_can_put; i++)
	{
		if (score[i] == max_change)
		{
			can_put_max_change[max_change_num] = can_put_list[i];
			max_change_num++;
		}
	}

	int r = rand() % max_change_num;
	return can_put_max_change[r];
}

int surround(int k, int board[N][N])
{
	int count = 0;
	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (board[y][x] == k)
			{
				int flag = 1;
				for (int dy = -1; dy < 2; dy++)
				{
					for (int dx = -1; dx < 2; dx++)
					{
						if (dx == 0 && dy == 0) continue;
						int nowX = x;
						int nowY = y;
						while (board[nowY + dy][nowX + dx] == k)
						{
							nowX += dx;
							nowY += dy;
						}


						if (board[nowY + dy][nowX + dx] == 0)
						{
							flag = 0;
							break;
						}
					}
					if (flag == 0) break;
				}

				if (flag) count++;
			}
		}
	}

	return count;
}

struct INPUT_DATA yashioAI(int k, int board[N][N])
{
	// search algorithm
	INPUT_DATA can_put_list[60];
	int score[60] = { 0 };
	int how_many_can_put = 0;
	int max_change = 0;

	for (int y = 1; y <= 8; y++)
	{
		for (int x = 1; x <= 8; x++)
		{
			if (check_change(k, x, y, board))
			{
				int v_board[N][N];
				copy(board, v_board);
				change_board(k, x, y, v_board);

				can_put_list[how_many_can_put] = INPUT_DATA{ x, y };
				printf("(%d, %d) : ", x, y);
				score[how_many_can_put] = surround(k, v_board);
				printf("%d\n", score[how_many_can_put]);
				max_change = (max_change < score[how_many_can_put]) ? score[how_many_can_put] : max_change;

				how_many_can_put++;
			}
		}
	}

	INPUT_DATA can_put_max_change[60];
	int max_change_num = 0;

	for (int i = 0; i < how_many_can_put; i++)
	{
		if (score[i] == max_change)
		{
			can_put_max_change[max_change_num] = can_put_list[i];
			max_change_num++;
		}
	}

	int r = rand() % max_change_num;
	return can_put_max_change[r];
}