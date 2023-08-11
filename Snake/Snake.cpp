// Snake.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <chrono>
using namespace std;


void draw_field(char border_char, int field_size, char** field)
{
    for (int i = 0; i < field_size + 2; i++)
    {
        cout << border_char << " ";
    }
    cout << "\n";

    for (int y = 0; y < field_size; y++)
    {
        cout << border_char << " ";
        for (int x = 0; x < field_size; x++)
        {
            cout << field[y][x] << " ";

        }
        cout << border_char;
        cout << "\n";
    }

    for (int i = 0; i < field_size + 2; i++)
    {
        cout << border_char << " ";
    }
}


void spawn_fruit(char** field, char fruit, int field_size, char space)
{
    srand(time(0));
    bool spawned = false;
    while (!spawned) {
        int x = rand() % field_size;
        int y = rand() % field_size;
        if (field[y][x] == space)
        {
            field[y][x] = fruit;
            spawned = true;
        }
    }
}

void spawn_snake(char** field, char fruit, int field_size, char space, char snake_head, char snake_body, int* location)
{
    int spawn_location[2], tail_location[2];
    spawn_location[0] = 0;
    spawn_location[1] = 1;
    location[0] = spawn_location[0];
    location[1] = spawn_location[1];
    field[spawn_location[0]][spawn_location[1] - 1] = snake_body;
    field[spawn_location[0]][spawn_location[1]] = snake_head;
}

bool move(char** field, char fruit, int field_size, char space, char snake_head, char snake_body, int* location, int* score, int* direction, bool* game, int* tail, int* tail_tip_location)
{

    bool add = false;
    int desired_position[2], last_location[2];
    desired_position[0] = location[0];
    desired_position[1] = location[1];

    last_location[0] = location[0];
    last_location[1] = location[1];



    field[location[0]][location[1]] = snake_body;



    if (GetAsyncKeyState('W'))
    {
        *direction = 1;
    }
    if (GetAsyncKeyState('S'))
    {
        *direction = 2;
    }
    if (GetAsyncKeyState('A'))
    {
        *direction = 3;
    }
    if (GetAsyncKeyState('D'))
    {
        *direction = 4;
    }

    if (*direction == 1)
    {
        if (desired_position[0] - 1 >= 0)
        {
           desired_position[0] --;
        }
        else
        {
            desired_position[0] = field_size - 1;
        }
    }
    else if (*direction == 2)
    {
        if (desired_position[0] + 1 < field_size)
        {
            desired_position[0] ++;
           
        }
        else
        {
            desired_position[0] = 0;
        }
    }
    else if (*direction == 3)
    {
        if (desired_position[1] - 1 >= 0)
        {
            desired_position[1] --;
        }
        else
        {
            desired_position[1] = field_size - 1;
        }
    }
    else if (*direction == 4)
    {
        if (desired_position[1] + 1 < field_size)
        {
            desired_position[1] ++;
        }
        else
        {
            desired_position[1] = 0;
        }
    }

    if (field[desired_position[0]][desired_position[1]] == space || field[desired_position[0]][desired_position[1]] == fruit)
    {


        
        if (field[desired_position[0]][desired_position[1]] == fruit)
        {
            *score += 1;
  
            add = true;
            spawn_fruit(field, fruit, field_size, space);
            if (*score % 10 == 0)
            {
                spawn_fruit(field, fruit, field_size, space);
            }

        }


        else
        {


            field[tail[0]][tail[1]] = space;
            for (int i = 0; i < *score+1; i++)
            {
                tail[i*2] = tail[(i*2)+2];     // 0 2       2 4       4 6
                tail[(i*2)+1] = tail[(i*2)+3]; // 1 3       3 5       5 7
            }

            tail[(*score * 2) + 2] = last_location[0];
            tail[(*score * 2) + 3] = last_location[1];

        }
        

        field[desired_position[0]][desired_position[1]] = snake_head;
        location[0] = desired_position[0];
        location[1] = desired_position[1];
        tail_tip_location[0] = last_location[0];
        tail_tip_location[1] = last_location[1];
    }
    else
    {
        *game = false;
    }
    return add;
}

void create_field(int field_size, char** field, char empty_space_char) {

    for (int y = 0; y < field_size; y++)
    {
        for (int x = 0; x < field_size; x++)
        {
            field[y][x] = empty_space_char;
        }
    }
}

int main()

{
    const int field_size = 20;
    char field_border_char = '#';
    char empty_space_char = ' ';
    char fruit_char = 'Q';
    char snake_head = 'O';
    char snake_body = 'o';

    int score = 0;

    int start_time = time(0);
    int update_time = 1;
    int updated = -1;

    int direction = 4;

    int timer;

    int* tail = new int[4];

    tail[0] = 0;
    tail[1] = 0;
    tail[2] = 1;
    tail[3] = 0;

    int tail_tip_location[2] = {0, 0};


    int snake_location[2];

    bool game = true;
    int length;

    char** field = new char* [field_size];

    for (int i = 0; i < field_size; i++)
    {
        field[i] = new char[field_size];

    }


    create_field(field_size, field, empty_space_char);

    spawn_snake(field, fruit_char, field_size, empty_space_char, snake_head, snake_body, snake_location);
    spawn_fruit(field, fruit_char, field_size, empty_space_char);
    draw_field(field_border_char, field_size, field);
    while (game)
    {
        if (GetKeyState('X'))
        {
            game = false;
        }

        timer = time(0) - start_time;

        if (timer % update_time == 0 && updated != timer)
        {
            system("CLS");
            updated = timer;


            if (move(field, fruit_char, field_size, empty_space_char, snake_head, snake_body, snake_location, &score, &direction, &game, tail, tail_tip_location))
            {
                length = ((score + 2) * 2);
                int* foo = new int[length];

                for (int i = 0; i < length - 2; i++)
                {
                    foo[i] = tail[i];
                }
                foo[4 + ((score - 1)*2)] = tail_tip_location[0];
                foo[5 + ((score - 1)*2)] = tail_tip_location[1];
                tail = foo;
            }
            draw_field(field_border_char, field_size, field);
            cout << "\n Score: " << score;
        }
    }
    for (int i = 0; i < field_size; i++)
    {
        delete[] field[i];
    }
    delete[] tail;

}