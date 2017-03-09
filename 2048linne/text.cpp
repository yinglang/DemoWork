/*#include <iostream>
#include <conio.h>
#include <string>
#include <stdio.h>
#include <stdarg.h>
using namespace std;

void print_num();
void f();
void print_everyNum(char c[3]);
void print_Chinese_word(int a1, int a2);

int main()
{
    char c[3] = "↑";
    cout << c << endl;
    cout << int(c[0])<< " " << int(c[1]) << " " << int(c[2]) << endl;
    char d[2];
    d[0] = _getch();
    d[1] = _getch();
    cout << d << "is " << int(d[0])<< " " << int(d[1]) << endl;
    if(d == "↓") cout << "the cin is " << d << endl;

    int m,n;
    m = _getch();
    n = _getch();
    cout << m << " " << n << endl;

    char e= -129;
    cout << int(e)<< endl;
    print_everyNum("我");
    print_everyNum("是");
    print_everyNum("一");
    print_everyNum("个");
    print_everyNum("好");
    print_everyNum("人");


    //make a chinese word
    print_Chinese_word(206,210);

    string s = "我是一个好人";
    cout << "s is \"" << s << "\";" << endl;
    for(int i = 0 ; i < s.size(); i++)
        cout << "s[" << i << "] == " << int(s[i]) << endl;
    //f();
}

void print_num()
{
    do{
         int ch = _getch();
         //if(ch < 128 && ch >=0) continue;

         cout << "the number is " << ch << " ;";
         //printf("%c",&ch);

         if( ch == 0 ) return;
    }while(1);
}

void f()
{
    char c;
    while((c = getch()) != 'q') {

        printf("c,d,o:\t%2c %-6d %-5o\n", c, c, c);

    }
}

void print_everyNum(char c[3])//string also can
{
    cout << "the c is " << c << " " << int(c[0]) << " " << int(c[1])
         << " " << int(c[2]) << " " << int(c) << endl;
}

void print_Chinese_word(int a1,int a2)
{
    char c[3];
    c[0] = a1;
    c[1] = a2;
    c[2] = 0;
    cout << a1 << " " << a2 << " " << "0 is " << c << endl;
}*/
