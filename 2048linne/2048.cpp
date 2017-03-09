#include <iostream>
#include <windows.h>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <conio.h>

using namespace std;

void setCoord(int x, int y)
{
    COORD pos = {short(x), short(y)};
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(out, pos);
}

class Map{
public:
    //constuct initiliza map
    Map(){}

    Map(const Map& m_map)
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                myMap[i][j] = m_map.myMap[i][j];
            }
        }
    }

    void init()
    {
        srand(time(0));
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                myMap[i][j] = 0;
            }
        }

        set_random_num();
        set_random_num();
    }

    void print_map()
    {
        setCoord(30,0);
        cout << "_____________________";
        for(int i = 0; i < 4; i++)
        {
            setCoord(30, i * 2 + 1);
            cout << "|";
            for(int j = 0; j < 4; j++)
            {
                if(myMap[i][j])
                cout << setw(4) << myMap[i][j] << "|";
                else cout << "    |" ;
            }
            setCoord(30, 2 * i + 2);
            cout << "_____________________" << endl;
        }
    }

    bool moveUp()
    {
        bool hasMove = false;

        for(int j = 0; j < 4; j++)
        {
            for(int i = 1; i < 4; i++)
            {
                if(myMap[i][j] != 0)
                {
                    int new_x = i - 1;
                    //find the fisrt place myMap[new_x][y] is not 0 or new_x == -1;
                    while( new_x >= 0 && myMap[new_x][j] == 0 ) new_x--;

                    if(new_x != i - 1)//if  it has moved
                    {
                        //myMap[new_x + 1][j] is the last 0
                        myMap[new_x + 1][j] = myMap[i][j];
                        myMap[i][j] = 0;
                        hasMove = true;
                    }
                }
            }
        }

        //if the next value can be added
        //to deal the condition 16 8 4 4 => 16 8 8 0 => 16 16 0 0 => 32 0 0 0
        // 8 4 4 16 => 8 8 16 0 => 16 16 0 0=> 32 0 0 0 need loop 3 times
        for(int k = 0; k < 3; k++)
        {
            for(int j = 0; j < 4; j++)
            {
                for(int i = 1; i < 4-k; i++)
                {
                    if(myMap[i][j] != 0)
                    {
                        if( myMap[i][j] == myMap[i-1][j])
                        {
                            myMap[i - 1][j] *= 2;
                            myMap[i][j] = 0;
                            hasMove = true;
                        }
                    }
                }
            }
        }

        return hasMove;
    }

    bool moveDown()
    {
        bool hasMove = false;

        for(int j = 0; j < 4; j++)
        {
            for(int i = 2; i >= 0 ; i--)
            {
                if(myMap[i][j] != 0)
                {
                    int new_x = i + 1;
                    //find the fisrt place myMap[new_x][y]  is not 0 or new_x == 4;
                    while( new_x < 4 && myMap[new_x][j] == 0 ) new_x++;

                    if(new_x != i + 1)//if  it has moved
                    {
                        //myMap[new_x + 1][j] is the last 0
                        myMap[new_x - 1][j] = myMap[i][j];
                        myMap[i][j] = 0;
                        hasMove = true;
                    }
                }
            }
        }

        //if the next value can be added
        for(int k = 0; k < 3; k++)
        {
            for(int j = 0; j < 4; j++)
            {
                for(int i = 2; i >= k; i--)
                {
                    if(myMap[i][j] != 0)
                    {
                        if( myMap[i][j] == myMap[i+1][j])
                        {
                            myMap[i + 1][j] *= 2;
                            myMap[i][j] = 0;
                            hasMove = true;
                        }
                    }
                }
            }
        }
        return hasMove;
    }

    bool moveLeft()
    {
        bool hasMove = false;

        for(int i = 0; i < 4; i++)
        {
            for(int j = 1; j < 4; j++)
            {
                if(myMap[i][j] != 0)
                {
                    int new_y = j - 1;
                    //find the fisrt place myMap[i][new_y] is not 0 or new_y == -1;
                    while( new_y >= 0 && myMap[i][new_y] == 0 ) new_y--;

                    if(new_y != j - 1)//if  it has moved
                    {
                        //myMap[i][new_y + 1] is the last 0
                        myMap[i][new_y + 1] = myMap[i][j];
                        myMap[i][j] = 0;
                        hasMove = true;
                    }

                    //if the next value can be added
                    if( new_y >= 0 && myMap[i][new_y] == myMap[i][new_y + 1])
                    {
                        myMap[i][new_y] *= 2;
                        myMap[i][new_y + 1] = 0;
                        hasMove = true;
                    }
                }
            }
        }

        //if the next value can be added
        for(int k = 0; k < 3; k++)
        {
            for(int i = 0; i < 4; i++)
            {
                for(int j = 1; j < 4-k; j++)
                {
                    if(myMap[i][j] != 0)
                    {
                        if( myMap[i][j] == myMap[i][j-1])
                        {
                            myMap[i][j-1] *= 2;
                            myMap[i][j] = 0;
                            hasMove = true;
                        }
                    }
                }
            }
        }
        return hasMove;
    }

    bool moveRight()
    {
        bool hasMove = false;

        for(int i = 0; i < 4; i++)
        {
            for(int j = 2; j >= 0; j--)
            {
                if(myMap[i][j] != 0)
                {
                    int new_y = j + 1;
                    //find the fisrt place myMap[i][new_y] is not 0 or new_y == 4;
                    while( new_y < 4 && myMap[i][new_y] == 0 ) new_y++;

                    if(new_y != j + 1)//if  it has moved
                    {
                        //myMap[i][new_y - 1] is the last 0
                        myMap[i][new_y - 1] = myMap[i][j];
                        myMap[i][j] = 0;
                        hasMove = true;
                    }
                }
            }
        }

        //if the next value can be added
        for(int k = 0; k < 3; k++)
        {
            for(int i = 0; i < 4; i++)
            {
                for(int j = 2; j >= k; j--)
                {
                    if(myMap[i][j] != 0)
                    {
                        if( myMap[i][j] == myMap[i][j + 1])
                        {
                            myMap[i][j + 1] *= 2;
                            myMap[i][j] = 0;
                            hasMove = true;
                        }
                    }
                }
            }
        }
        return hasMove;
    }

    void set_random_num()
    {
        int x = rand() % 4;
        int y = rand() % 4;
        while( myMap[x][y] != 0 )
        {
           x = rand() % 4;
           y = rand() % 4;
        }

        int k = rand() % 4;
        int num = ((k == 0) ? 4 : 2);

        myMap[x][y] = num;
    }

    bool isFail()
    {
        if(isFull())
        {
            //Map temp_map(*this);
            if(Map(*this).moveUp()) return false;
            if(Map(*this).moveDown()) return false;
            if(Map(*this).moveLeft()) return false;
            if(Map(*this).moveRight()) return false;
            return true;
        }
        return false;
    }

    int getMaxNum()
    {
        int max_num = 4;
        for(int i = 0 ;i < 4; i++)
        {
            for(int j = 0 ; j < 4; j++)
            {
                if(max_num < myMap[i][j])
                    max_num = myMap[i][j];
            }
        }
        return max_num;
    }
private:
    int myMap[4][4];
    bool isFull()
    {
        for(int i = 0 ;i < 4; i++)
        {
            for(int j = 0 ; j < 4; j++)
            {
                if(myMap[i][j] == 0)
                    return false;
            }
        }
        return true;
    }
};
//map was the globel
Map theMap;

void mainLoop();
void fail_print();

int main()
{
     int start = 1;
     while(start - '0')
     {
         theMap.init();
         theMap.print_map();
         mainLoop();
         start = _getch();
     }
}

void mainLoop()
{
    int a, b;
    bool hasMove;
    while(1)
    {
        hasMove = false;
        while((a = _getch()) != 224);
        b = _getch();
        if(a == 224 && b == 72) hasMove = theMap.moveUp();
        if(a == 224 && b == 80) hasMove = theMap.moveDown();
        if(a == 224 && b == 75) hasMove = theMap.moveLeft();
        if(a == 224 && b == 77) hasMove = theMap.moveRight();

        if(hasMove == true) theMap.set_random_num();
        theMap.print_map();

        if(theMap.isFail())
        {
            fail_print();
            return;
        }
    }
}

void fail_print()
{
    setCoord(30,10);
    cout << "you lost !  you point is " << theMap.getMaxNum();
    setCoord(30,11);
    cout << "重玩按‘1’ ，结束按‘0’ " << endl;
}
