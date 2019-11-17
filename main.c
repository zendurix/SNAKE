#include "stdio.h"
#include "Windows.h"
#include "time.h"
#include "conio.h"

#define TICK_TIME_MS 70 
#define MYLENGTH 50
#define MYHEIGHT 20
#define PRINT_SPACE 1

const char SNAKE_HEAD = 254;
const char SNAKE_BODY = 254; // black square
const char FLOOR = ' ';
const char FRUIT = 'O';


typedef struct
{
	int x, y;
	char BOOLobstacle,
		 BOOLsnake,
		 BOOLfruit,
		 BOOLchanged_print,
		 BOOLhead;
	char print_format;
}Place;


void goto_xy(int x, int y);
void init_field(Place field[MYHEIGHT][MYLENGTH]);
Place* set_snake_random(Place field[MYHEIGHT][MYLENGTH]);
void print_field(Place field[MYHEIGHT][MYLENGTH], int size);
Place* move(char dir, Place* snake, Place field[MYHEIGHT][MYLENGTH], char BOOLgrow);
Place* grow_snake(Place* snake, int* size);
void place_fruit_random(Place field[MYHEIGHT][MYLENGTH]);


int main()
{
	char direction = '4', temp = ' ';
	char BOOLgrow = 0;
	char BOOLgameOver = 0;
	int snake_size = 1;
	int i;

	Place* head;
	Place field[MYHEIGHT][MYLENGTH];
	Place* snake = (Place*)malloc(snake_size * sizeof(Place));
	Place temp_snake;
	   	 
	srand(time(NULL));
	init_field(field);
	head = set_snake_random(field);
	snake[0] = *head;
	place_fruit_random(field);

	while (!BOOLgameOver)
	{
		BOOLgrow = 0;
		print_field(field, snake_size);

		if (head->BOOLfruit)
		{
			snake = grow_snake(snake, &snake_size, BOOLgrow);
			snake[snake_size - 1] = *head;
			head->BOOLfruit = 0;
			place_fruit_random(field);
			BOOLgrow = 1;
		}
		
		fflush(stdin);
		if (_kbhit())
		{
			temp = _getch();
			if (temp == '4' || temp == '8' || temp == '2' || temp == '6')
			{
				if ((direction == '2' && temp == '8') || (direction == '8' && temp == '2') ||
					(direction == '4' && temp == '6') || (direction == '6' && temp == '4'))
				{
					;
				}
				else
				{
					direction = temp;
				}
			}
		}

		for (i = 1; i < snake_size-1; i++)
		{
			if (i == 1)
			{
				field[snake[i].y][snake[i].x].print_format = FLOOR;
				field[snake[i].y][snake[i].x].BOOLsnake = 0;
			}
			field[snake[i].y][snake[i].x].BOOLchanged_print = 1;
			snake[i] = snake[i + 1];
			field[snake[i].y][snake[i].x].print_format = SNAKE_BODY;
			field[snake[i].y][snake[i].x].BOOLsnake = 1;
		}

		if (snake_size == 2)
		{
			temp_snake = snake[snake_size - 1];
		}
		snake[snake_size - 1] = *head;

		head = move(direction, head, field, BOOLgrow, &BOOLgameOver);
		if (snake_size > 1)
		{
			if (snake_size == 2)
			{
				field[temp_snake.y][temp_snake.x].print_format = FLOOR;
				field[temp_snake.y][temp_snake.x].BOOLchanged_print = 1;
				field[temp_snake.y][temp_snake.x].BOOLsnake = 0;
			}
			field[snake[snake_size - 1].y][snake[snake_size - 1].x].print_format = SNAKE_BODY;
			field[snake[snake_size - 1].y][snake[snake_size - 1].x].BOOLsnake = 1;
		}

		Sleep(TICK_TIME_MS);
	}

	free(snake);
	_getch();
	return 0;
}


void goto_xy(int x, int y)
{
	COORD pos = { x, y };
	HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handleOut, pos);
}

void init_field(Place field[MYHEIGHT][MYLENGTH])
{
	int i, j;
	for (i = 0; i < MYHEIGHT; i++)
	{
		for (j = 0; j < MYLENGTH; j++)
		{
			field[i][j].x = j;
			field[i][j].y = i;
			field[i][j].BOOLchanged_print = 1;
			field[i][j].BOOLfruit = 0;
			field[i][j].BOOLobstacle = 0;
			field[i][j].BOOLsnake = 0;
			field[i][j].BOOLhead = 0;
			field[i][j].print_format = FLOOR;
			if (j == 0)
			{
				field[i][j].print_format = 186;
			}
			if (j == MYLENGTH - 1)
			{
				field[i][j].print_format = 186;
			}
			if (i == 0)
			{
				if (j == 0) field[i][j].print_format = 201;
				else if (j == MYLENGTH - 1) field[i][j].print_format = 187;
				else field[i][j].print_format = 205;
			}
			if (i == MYHEIGHT - 1)
			{
				if (j == 0) field[i][j].print_format = 200;
				else if (j == MYLENGTH - 1) field[i][j].print_format = 188;
				else field[i][j].print_format = 205;
			}
		}
	}
}

Place* set_snake_random(Place field[MYHEIGHT][MYLENGTH])
{
	int x, y;
	x = rand() % (MYLENGTH - 5) + 5;
	y = rand() % (MYHEIGHT - 5) + 5;
	field[y][x].BOOLhead = 1;
	field[y][x].BOOLsnake = 1;
	return &field[y][x];
}


void print_field(Place field[MYHEIGHT][MYLENGTH], int size)
{
	int i, j;
	for (i = 0; i < MYHEIGHT; i++)
	{
		for (j = 0; j < MYLENGTH; j++)
		{
			if (field[i][j].BOOLchanged_print)
			{
				if (PRINT_SPACE) 
				{
					goto_xy(j * 2, i);
					printf("%c ", field[i][j].print_format);
				}
				else
				{
					goto_xy(j, i);
					printf("%c", field[i][j].print_format);
				}
				field[i][j].BOOLchanged_print = 0;
			}
		}
		printf("\n");
	}
	goto_xy(MYLENGTH*2 + 3, 0);
	printf("    ");
	goto_xy(MYLENGTH*2 + 3, 0);
	printf("%d", size);

}

Place* move(char dir, Place* snake, Place field[MYHEIGHT][MYLENGTH], char BOOLgrow, char* BOOLgameOver)
{
	int x = snake->x;
	int y = snake->y;
	if (!BOOLgrow)
	{
		snake->BOOLsnake = 0;
		snake->print_format = FLOOR;
	}
	else
	{
		snake->print_format = SNAKE_BODY;
	}
	snake->BOOLchanged_print = 1;

	switch (dir) 
	{
	case '8':
		y--;
		break;
	case '6':
		x++;
		break;
	case '2':
		y++;
		break;
	case '4':
		x--;
		break;
	default:
		break;
	}

	x = (x == MYLENGTH-1) ? 1 : ( (x == 0) ? MYLENGTH - 2 : x );
	y = (y == MYHEIGHT-1) ? 1 : ( (y == 0) ? MYHEIGHT - 2 : y );

	if (field[y][x].BOOLsnake)
	{
		goto_xy(MYLENGTH, MYHEIGHT / 2);
		printf("GAME OVER");
		_getch();
		*BOOLgameOver = 1;
	}

	if (snake->BOOLhead)
	{
		snake->BOOLhead = 0;
		snake = &field[y][x];
		snake->BOOLhead = 1;
		snake->print_format = SNAKE_HEAD;
	}
	else
	{
		snake->print_format = SNAKE_BODY;
	}
	snake = &field[y][x];
	snake->BOOLsnake = 1;
	snake->BOOLchanged_print = 1;
	return snake;
}

Place* grow_snake(Place* snake, int *size)
{
	(*size)++;
	snake = realloc (snake, (*size) * sizeof(Place));
	return snake;
}

void place_fruit_random(Place field[MYHEIGHT][MYLENGTH])
{
	int x, y;
	x = rand() % (MYLENGTH - 4) + 2;
	y = rand() % (MYHEIGHT - 4) + 2;
	field[y][x].BOOLfruit = 1;
	field[y][x].print_format = FRUIT;
	field[y][x].BOOLchanged_print = 1;
}