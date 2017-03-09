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

// ���ù���λ�ã�����������windows��ϵͳ����
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

// ̰����ʳ�����
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

    // �ı�ʳ���λ��
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

// ̰���߶���
class snake
{
private:
    // ��¼̰���ߵ��������ڵ������
    vector<position> point;

    // ̰���߳�ͷ����Ĳ����ƶ�һ�񣬱������ƶ��ĺ�������
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

    // �ж��Ƿ�̰���ߵ�ͷ�Ƿ�ײ��ʳ��
    // ���̰����ײ����ʳ�������ӵ���������������ʳ���λ��
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

    // ���pos�Ƿ����ߵ�������
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

   // ̰��������������ƶ�
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

    // ̰����ײ���Լ��ĳ����ж�
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

// ��Ϸ��֡ˢ�º�����ÿһ֡�������һ��
// derection: ̰���ߵ�ǰ���ƶ����� (��������һ̰֡����ͷ��λ��)
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
    cout << "cpp ̰����" << endl;
    cout << "time : 2014.6" << endl;
    cout << "����ѧУ������ѧ" << endl;
    cout << "�ް�Ȩ�������޸�" << endl;
    cout << "_____________________________________________________" << endl;
    cout << "��Ϸ˵�� �����̵��ϣ��£����Ҽ�ͷ���ƣ���������ͣ"<<endl;
    cout << "�����ٶ� ��1 �� 2 �� 3 ��" << endl;
    cout << "�������ٶ� ��";
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
        // ����windowsϵͳ�������첽�Ļ�ȡָ���ļ�������
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
