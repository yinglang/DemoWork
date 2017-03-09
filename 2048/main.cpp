#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <string>
#include "gamemap.h"
using namespace std;
GameMap::Direction GetDirection()
{
    int ret = -1;

    do
    {
        int ch = getch();

        if( isascii( ch ) )
            continue;

        ch = getch();

        switch( ch )
        {
            case 72:
                ret = GameMap::D_UP;
                break;
            case 75:
                ret = GameMap::D_LEFT;
                break;
            case 77:
                ret = GameMap::D_RIGHT;
                break;
            case 80:
                ret = GameMap::D_DOWN;
                break;
            default:
                break;
        }
    }
    while ( ret < 0);

    return GameMap::Direction(ret);
}

static std::string label[] = {
  "0","2","4","8","16","32","64","128","256","512","1024","2048","4096"
};

void Display(const GameMap& m){
    const int * data = m.getData();
    const int dim = m.getDimension();
    for(int i=0;i<dim;++i){
        for(int j=0;j<dim;++j){
            cout<<label[data[i*dim+j]]<<'\t';
        }
        cout<<endl;
    }
}

int main()
{
    GameMap m(4);
    m.reset();
    do{
        Display(m);
        GameMap::Direction d = GetDirection();
        m.move(d);
        system("cls");
    }while(m.canMove());

    return 0;
}

