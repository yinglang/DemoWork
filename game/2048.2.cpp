#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <iomanip>
#include <windows.h>
#include <conio.h>

using namespace std;

void setHandlePos(int x, int y)
{
    COORD pos = {short(x), short(y)};
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition ( out , pos );
}

class Map
{
private:
    int num[4][4];
public:
    Map()
    {
        srand(time(0));
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
                num[i][j] = 0;
        }

        random_num();
        random_num();
    }

    void random_num()//random deal
    {
        int i = rand() % 4;
        int j = rand() % 4;
        while( num[i][j] != 0 )
        {
            i = rand() % 4;
            j = rand() % 4;
        }
        int k = rand() % 5;
        if( k == 0 )
            num[i][j] = 4;
        else
            num[i][j] = 2;
    }

    void print()
    {
        setHandlePos( 32, 0 );
        cout << " ____________________";
        for(int i = 0; i < 4; i++)
        {
            setHandlePos( 32, i * 2 + 1 );
            cout << "|" ;
            for(int j = 0; j < 4; j++)
            {
                cout << setw(4) << num[i][j] << "|";
            }
            setHandlePos( 32, i * 2 + 2 );
            cout << "|____|____|____|____|";
        }
    }

    void move_up()
    {
        for(int j = 0; j < 4; j++)
        {
            for(int i = 1; i < 4; i++)
            {
                if(num[i][j] != 0)//for loop zhong bie pan duan
                {
                    int t = i;
                    while( (t--) && num[t][j] == 0){}
                    if(t == -1)
                    {
                        if( num[0][j] == 0 )
                        {
                             num[0][j] = num[i][j];
                             num[i][j] = 0;
                        }
                    }
                    else if(t + 1 != i)
                    {
                         num[t+1][j] = num[i][j];
                         num[i][j] = 0;
                    }
                }
            }
        }

        for(int j = 0; j < 4; j++)
        {
            for(int i = 0; i < 3; i++)
            {
                if( num[i][j] != 0 && num[i][j] == num[i+1][j] )
                {
                    num[i][j] *= 2;
                    num[i+1][j] = 0;
                }
            }

         }

         for(int j = 0; j < 4; j++)
        {
            for(int i = 1; i < 4; i++)
            {
                if(num[i][j] != 0)//for loop zhong bie pan duan
                {
                    int t = i;
                    while( (t--) && num[t][j] == 0){}
                    if(t == -1)
                    {
                        if( num[0][j] == 0 )
                        {
                             num[0][j] = num[i][j];
                             num[i][j] = 0;
                        }
                    }
                    else if(t + 1 != i)
                    {
                         num[t+1][j] = num[i][j];
                         num[i][j] = 0;
                    }
                }
            }
        }
    }

    void move_down()
    {
        for(int j = 0; j < 4; j++)
        {
            for(int i = 2; i >= 0; i--)
            {
                if(num[i][j] != 0)
                {
                    int t = i + 1;
                    while( t < 4 && num[t][j] == 0) { t++ ;}
                    if(t == 4)
                    {
                        if(num[3][j] == 0)
                        {
                            num[3][j] = num[i][j];
                            num[i][j] = 0;
                        }
                    }
                    else if(t - 1 != i)
                    {
                        num[t - 1] [j] = num[i][j];
                        num[i][j] = 0;
                    }
                }
            }
        }

        for(int j = 0; j < 4; j++)
        {
            for(int i = 3; i > 0; i-- )
            {
                if(num[i][j] != 0 && num[i][j] == num[i - 1][j])
                {
                    num[i][j] *= 2;
                    num[i - 1][j] = 0;
                }
            }
        }

        for(int j = 0; j < 4; j++)
        {
            for(int i = 2; i >= 0; i--)
            {
                if(num[i][j] != 0)
                {
                    int t = i + 1;
                    while( t < 4 && num[t][j] == 0) { t++ ;}
                    if(t == 4)
                    {
                        if(num[3][j] == 0)
                        {
                            num[3][j] = num[i][j];
                            num[i][j] = 0;
                        }
                    }
                    else if(t - 1 != i)
                    {
                        num[t - 1] [j] = num[i][j];
                        num[i][j] = 0;
                    }
                }
            }
        }
    }

    void move_left()
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 1; j < 4; j++)
            {
                if(num[i][j] != 0)//for loop zhong bie pan duan
                {
                    int t = j;
                    while( (t--) && num[i][t] == 0){}
                    if(t == -1)
                    {
                        if( num[i][0] == 0 )
                        {
                             num[i][0] = num[i][j];
                             num[i][j] = 0;
                        }
                    }
                    else if(t + 1 != j)
                    {
                         num[i][t+1] = num[i][j];
                         num[i][j] = 0;
                    }
                }
            }
        }

        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                if( num[i][j] != 0 && num[i][j] == num[i][j+1] )
                {
                    num[i][j] *= 2;
                    num[i][j+1] = 0;
                }
            }

         }

         for(int i = 0; i < 4; i++)
        {
            for(int j = 1; j < 4; j++)
            {
                if(num[i][j] != 0)//for loop zhong bie pan duan
                {
                    int t = j;
                    while( (t--) && num[i][t] == 0){}
                    if(t == -1)
                    {
                        if( num[i][0] == 0 )
                        {
                             num[i][0] = num[i][j];
                             num[i][j] = 0;
                        }
                    }
                    else if(t + 1 != j)
                    {
                         num[i][t+1] = num[i][j];
                         num[i][j] = 0;
                    }
                }
            }
        }
    }

    void move_right()
    {
        for(int i = 0; i < 4; i++)//all move
        {
            for(int j = 3; j >= 0; j--)
            {
                if(num[i][j] != 0)
                {
                    int t = j + 1;
                    while( t < 4 && num[i][t] == 0) { t++ ;}
                    //cout << t << endl;
                    if(t == 4)
                    {
                        if(num[i][3] == 0)
                        {
                            num[i][3] = num[i][j];
                            num[i][j] = 0;
                        }
                    }
                    else if(t - 1 != j)
                    {
                        num[i][t-1] = num[i][j];
                        num[i][j] = 0;
                    }
                }
            }
        }

        for(int i = 0; i < 4; i++)//check doule
        {
            for(int j = 3; j > 0; j-- )
            {
                if(num[i][j] != 0 && num[i][j] == num[i][j - 1])
                {
                    num[i][j] *= 2;
                    num[i][j - 1] = 0;
                }
            }
        }

        for(int i = 0; i < 4; i++)//all move
        {
            for(int j = 3; j >= 0; j--)
            {
                if(num[i][j] != 0)
                {
                    int t = j + 1;
                    while( t < 4 && num[i][t] == 0) { t++ ;}
                    if(t == 4)
                    {
                        if(num[i][3] == 0)
                        {
                            num[i][3] = num[i][j];
                            num[i][j] = 0;
                        }
                    }
                    else if(t - 1 != j)
                    {
                        num[i] [t-1] = num[i][j];
                        num[i][j] = 0;
                    }
                }
            }
        }
    }

     bool map_full()
     {
         for(int i = 0; i < 4; i ++)
            for(int j = 0; j < 4; j++)
                if(num[i][j] == 0)
                    return 0;
         return 1;
     }

     bool map_cannon_move()
     {
         //give value to temp[][] and larger it
         int temp[6][6];
         for(int i = 0; i < 4; i ++)
         {
            for(int j = 0; j < 4; j++)
            {
                temp[i + 1][j + 1] = num[i][j];
            }
         }

         for(int j = 0; j < 6 ; j++)
         {
             temp[0][j] = 0;
             temp[5][j] = 0;
         }

         for(int i = 0; i < 4; i++)
         {
             temp[i][0] = 0;
             temp[i][5] = 0;
         }

         for(int i = 0; i < 6; i++)
         {
             for(int j = 0; j < 6; j++)
             {
                 int k = temp[i][j];
                 if(k == temp[i-1][j]) return 0;
                 if(k == temp[i+1][j]) return 0;
                 if(k == temp[i][j-1]) return 0;
                 if(k == temp[i][j+1]) return 0;
             }
         }

         return 1;
     }

     void push_buttom(char derection)
     {
         if(derection == 'w')
         {
             move_up();
         }

         if(derection == 's')
         {
             move_down();
         }
         if(derection == 'a')
         {
             move_left();
         }

         if(derection == 'd')
         {
             move_right();
         }
     }
};

int main()
{
     Map numMap;
     numMap.print();

     char derection;
     while(1)
     {
         derection = _getch();
         numMap.push_buttom(derection);
         if( numMap.map_full() && numMap.map_cannon_move() )
         {
                  return 0;
         }
         else numMap.random_num();

         numMap.print();
     }
}
