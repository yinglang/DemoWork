#include <iostream>
#include <windows.h>
#include <vector>
#include <conio.h>
#include <ctime>
#include <stdlib.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define NO_DEREC 0
//move range [0, WIDTH) [0,HEIGHT)
#define WIDTH 22
#define HEIGHT 12

using namespace std;

struct position
{
    int x;
    int y;
};

// 设置光标的位置，调用了两个windows的系统函数
void setHandlePos(position pos)
{
    COORD t_pos = {short(pos.x), short(pos.y)};
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition( out , t_pos );
}

int mod(int a, int mod_num)
{
    while( a < 0 )
        a += mod_num;

    while( a >= mod_num)
        a -= mod_num;

    return a;
}

// 贪吃蛇食物对象
class food
{
public:
    position pos;
    bool is_eat;
    food()
    {
        srand(time(0));
        position temppos = in_randomPos();
        while(temppos.x == WIDTH/2-1 && temppos.y == HEIGHT/2)
            temppos = in_randomPos();
        pos = {temppos.x, temppos.y};
    }

    // 改变食物的位置
    position in_randomPos()
    {
        is_eat = 0;
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        position temppos = { x, y};
        return temppos;
    }
};
food m_food;

// 贪吃蛇对象
class snake
{
private:
    // 记录贪吃蛇的身体所在的坐标点
    vector<position> point;

    // 贪吃蛇除头以外的部分移动一格，被后面移动的函数调用
    void move_change()
    {
        if(is_meet_food()) return;
        else{
            last_tail = point[0];
            int i;
            for(i = 0; i < point.size() - 1; i++)
            {
                point[i].x = mod( point[i+1].x, WIDTH);
                point[i].y = mod( point[i+1].y, HEIGHT);
            }
            point[i].x = head.x;
            point[i].y = head.y;
        }
    }

    // 判断是否贪吃蛇的头是否撞到食物
    // 如果贪吃蛇撞到了食物，把它添加到身体里，并重新随机食物的位置
    bool is_meet_food()
    {
        if( head.x == m_food.pos.x && head.y == m_food.pos.y)
        {
            point.push_back( head );

            position tempPos = m_food.in_randomPos();
            while(!check_right_position(tempPos))//or use all map mark
            {
                tempPos = m_food.in_randomPos();
            }
            m_food.pos.x = tempPos.x;
            m_food.pos.y = tempPos.y;
            return 1;
        }
        return 0;
    }

    // 检查pos是否在蛇的身体里
    bool check_right_position(position pos)
    {
        for(int i = 0; i < point.size(); i++)
        {
            if(pos.x == point[i].x && pos.y == point[i].y)
            {
                return 0;
            }
        }
        return 1;
    }
public:
    snake()
    {
        head = { WIDTH/2-1, HEIGHT/2};
        point.push_back( head );
        last_tail = head;
    }
    position last_tail;
    position head;

   // 贪吃蛇向各个方向移动
    void move_up()
    {
        head.y -=1;
        head.y = mod(head.y, HEIGHT);
        if(point.size() >= 2 && head.y == point[point.size()-2].y)
        {
            head.y = mod(head.y + 1, HEIGHT);
            return;
        }
        move_change();
    }

    void move_down()
    {
        head.y += 1;
        head.y = mod(head.y , HEIGHT);
        if(point.size() >= 2 && head.y == point[point.size()-2].y)
        {
            head.y = mod(head.y - 1, HEIGHT);
            return;
        }
        move_change();
    }

    void move_left()
    {
        head.x -= 1;
        head.x = mod(head.x , WIDTH);
        if(point.size() >= 2 && head.x == point[point.size()-2].x)
        {
            head.x = mod(head.x + 1, WIDTH);
            return;
        }
        move_change();
    }

    void move_right()
    {
        head.x += 1;
        head.x = mod(head.x , WIDTH);
        if(point.size() >= 2 && head.x == point[point.size()-2].x)
        {
            head.x = mod(head.x - 1, WIDTH);
            return;
        }
        move_change();
    }

    // 贪吃蛇撞到自己的程序判定
    bool hit_itself()
    {
        for(int i = 0; i < int(point.size()) - 3; i++)
        //point[size()-1] is head
        //if hit point[size()-2] it is back when line
        //if never hit point[size()-3];
        {
            if( point[i].x == head.x && point[i].y == head.y )
            {
                return 1;
            }
        }
        return 0;
    }
};
snake m_snake;

// 游戏的帧刷新函数，每一帧都会调用一次
// derection: 贪吃蛇当前的移动方向 (决定了下一帧贪吃蛇头的位置)
bool refresh(int derection)//if hititself return 0;
{

    if(derection == UP) m_snake.move_up();
    if(derection == DOWN) m_snake.move_down();
    if(derection == LEFT) m_snake.move_left();
    if(derection == RIGHT) m_snake.move_right();

    if(m_snake.hit_itself())
            return 0;

    if(m_food.is_eat == 0)
    {
        setHandlePos(m_food.pos);
        cout << "%" ;
        /*position posi = { 0, 25};
        setHandlePos( posi);
        cout << m_food.pos.x << " " << m_food.pos.y << endl;*/
    }

    setHandlePos(m_snake.last_tail);
    cout << " " ;

    setHandlePos(m_snake.head);
    cout << "*";

    return 1;
}

int write_before()
{
    cout << "_____________________________________________________" << endl;
    cout << "cpp 贪吃蛇" << endl;
    cout << "time : 2014.6" << endl;
    cout << "作者学校：天津大学" << endl;
    cout << "无版权，任意修改" << endl;
    cout << "_____________________________________________________" << endl;
    cout << "游戏说明 ：键盘的上，下，左，右箭头控制，后退是暂停"<<endl;
    cout << "设置速度 ：1 慢 2 中 3 快" << endl;
    cout << "请输入速度 ：";
    int speed;
    cin >> speed;
    system("pause");
    system("cls");
    for(int i = 0; i < HEIGHT; i++)
    {
        position pos = { WIDTH, i };
        setHandlePos( pos );
        cout << "|";
    }
    position pos = { 0, HEIGHT };
    setHandlePos( pos );
    for(int i = 0; i <= WIDTH; i++)
    cout << "-";
    return speed;
}

int main()
{
    int speed = write_before();
    int derection = NO_DEREC;
    refresh( derection );
    while(1)
    {
        // 利用windows系统函数，异步的获取指定的键盘输入
        if(GetAsyncKeyState(VK_UP))             
            derection = UP;
        if(GetAsyncKeyState(VK_DOWN))
            derection = DOWN;
        if(GetAsyncKeyState(VK_LEFT))
            derection = LEFT;
        if(GetAsyncKeyState(VK_RIGHT))
            derection = RIGHT;

        if( !refresh( derection ))
            return 0;

        if(speed == 1)
        Sleep(300);
        if(speed == 2)
            Sleep(100);
        if(speed == 3)
            Sleep(40);
    }
}
