/*#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <memory>
// 2的概率大于4
#define random_2_4 (rand()%5==4 ? 4:2)
#define random_x(x) (rand()%x)
using namespace std;

// 矩阵大小
const int MAX = 4;
const int MAXMAX = MAX*MAX;
// 初始数据个数
const int INIT_SIZE = 2;
// 数据矩阵
int _matrix[MAX][MAX];
// 合并临时数组
int current[MAX];
// 数据计数
int _count;
// 按键操作 => 不使用char...防止不必要的失败... => ch[0]即可...
string ch;
// 得分...
int score;


// 打印数组
void print();
// 操作说明
void help();
// 初始操作
void init();
// 随机位置 => 随机数据2/4
bool random_data_one();

// 上左下右
bool b_up();
bool b_left();
bool b_down();
bool b_right();

void _up();
void _left();
void _down();
void _right();

int main()
{
    init();
    print();
    while (b_up() || b_left() || b_down() || b_right())
    {
        help();
        switch (ch[0])
        {
        case 'w':
            _up();
            break;
        case 'a':
            _left();
            break;
        case 's':
            _down();
            break;
        case 'd':
            _right();
            break;
        default:
            cout <<"!!!!!!!!!!!!无效输入..."<< endl << endl;
            break;
        }
        print();
    }
    cout << " Game Over !! " << endl;
    cout << "最终得分Socre => " << score << endl;
    system("pause");
    system("pause");
    system("pause");
    return 0;
}

void print()
{
    cout << "-------------------------------------" << endl;
    cout << "得分Socre => " << score << endl;
    cout << "-------------------------------------" << endl;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            if (_matrix[i][j])
                cout << setw(5) << _matrix[i][j] << " |";
            else
                cout << setw(7) << " |";
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl << endl;;
}

void help()
{
    cout << "wasd => 上左下右" << endl;
    cout << "请输入: ";
    cin >> ch;
}
void init()
{
    _count = 0;
    score = 0;
    srand((int)time(0));
    for (int i = 0; i < INIT_SIZE; i++)
        random_data_one();
}

bool random_data_one()
{
    if (_count == MAXMAX)
        return false;
    int left = MAXMAX - _count;
    int tmp = random_x(left);
    int num = random_2_4;
    int k = 0;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            if (_matrix[i][j] == 0)
            {
                if (k++ == tmp)
                {
                    _matrix[i][j] = num;
                    break;
                }
            }
        }
    }
    ++_count;
    return true;
}

bool b_up()
{
    for (int k = 0; k < MAX; k++)
    {
        // 特殊情况...这一列有空元素...
        bool flag = false;
        for (int i = 0; i < MAX - 1; i++)
        {
            if (_matrix[i][k] == 0)
                flag = true;
            else
            {
                int j = i + 1;
                while (j < MAX)
                {
                    if (_matrix[j][k])
                    {
                        if (_matrix[i][k] == _matrix[j][k])
                            return true;
                        else
                            break;
                    }
                    else{
                        ++j;
                    }
                }
            }
        }
        if (flag)
        {
            // 空元素上方有元素则为True...
            // 使用左右夹击法...
            int i = 0, j = MAX - 1;
            while (i < MAX)
            {
                if (_matrix[i][k])
                    ++i;
                else
                    break;
            }
            while (j >= 0)
            {
                if (_matrix[j][k] == 0)
                    --j;
                else
                    break;
            }
            if (i < j)
                return true;
        }
    }
    return false;
}
bool b_left()
{
    for (int k = 0; k < MAX; k++)
    {
        // 特殊情况...这一行有空元素...
        bool flag = false;
        for (int i = 0; i < MAX - 1; i++)
        {
            if (_matrix[k][i] == 0)
                flag = true;
            else
            {
                int j = i + 1;
                while (j < MAX)
                {
                    if (_matrix[k][j])
                    {
                        if (_matrix[k][i] == _matrix[k][j])
                            return true;
                        else
                            break;
                    }
                    else{
                        ++j;
                    }
                }
            }
        }
        if (flag)
        {
            // 空元素右边有元素则为True...
            // 使用左右夹击法...
            int i = 0, j = MAX - 1;
            // i 是空元素位置，j是非空元素位置
            while (i < MAX)
            {
                if (_matrix[k][i])
                    ++i;
                else
                    break;
            }
            while (j >= 0)
            {
                if (_matrix[k][j] == 0)
                    --j;
                else
                    break;
            }
            if (i < j)
                return true;
        }
    }
    return false;
}
bool b_down()
{
    for (int k = 0; k < MAX; k++)
    {
        // 特殊情况...这一列有空元素...
        bool flag = false;
        for (int i = MAX - 1; i > 0; i--)
        {
            if (_matrix[i][k] == 0)
                flag = true;
            else
            {
                int j = i - 1;
                while (j >= 0)
                {
                    if (_matrix[j][k])
                    {
                        if (_matrix[i][k] == _matrix[j][k])
                            return true;
                        else
                            break;
                    }
                    else{
                        --j;
                    }
                }
            }
        }
        if (flag)
        {
            // 空元素上方有元素则为True... => 下边第一个空元素在上边第一个非空元素下边即可...
            // 使用左右夹击法...
            int i = 0, j = MAX - 1;
            // i 是非空位置，j是空元素位置
            while (i < MAX)
            {
                if (_matrix[i][k] == 0)
                    ++i;
                else
                    break;
            }
            while (j >= 0)
            {
                if (_matrix[j][k])
                    --j;
                else
                    break;
            }
            if (i < j)
                return true;
        }
    }
    return false;
}
bool b_right()
{
    for (int k = 0; k < MAX; k++)
    {
        // 特殊情况...这一行这一行有空元素...
        bool flag = false;
        for (int i = MAX - 1; i > 0; i--)
        {
            if (_matrix[k][i] == 0)
                flag = true;
            else
            {
                int j = i - 1;
                while (j >= 0)
                {
                    if (_matrix[k][j])
                    {
                        if (_matrix[k][i] == _matrix[k][j])
                            return true;
                        else
                            break;
                    }
                    else{
                        --j;
                    }
                }
            }
        }
        if (flag)
        {
            // 空元素左边有元素则为True... => 右边第一个空元素在左边第一个非空元素右边即可...
            // 使用左右夹击法...
            int i = 0, j = MAX - 1;
            // i 是非空位置，j是空元素位置
            while (i < MAX)
            {
                if (_matrix[k][i] == 0)
                    ++i;
                else
                    break;
            }
            while (j >= 0)
            {
                if (_matrix[k][j])
                    --j;
                else
                    break;
            }
            if (i < j)
                return true;
        }
    }
    return false;
}

void _up()
{
    cout << "按下了上键" << endl << endl;
    if (b_up())
    {
        cout << "可以向上合并" << endl;
        for (int i = 0; i < MAX; i++)
        {
            memset(current, 0, sizeof(int)*MAX);
            int ii = 0;
            for (int j = 0; j < MAX; j++)
            {
                if (_matrix[j][i])
                    current[ii++] = _matrix[j][i];
            }
            for (int k = 0; k < ii - 1; k++)
            {
                if (current[k] == current[k + 1])
                {
                    current[k] *= 2;
                    score += current[k];
                    current[k + 1] = 0;
                    ++k;
                    --_count;
                }
            }
            ii = 0;
            for (int j = 0; j < MAX; j++)
            {
                if (current[j])
                    _matrix[ii++][i] = current[j];
            }
            for (; ii < MAX; ii++)
                _matrix[ii][i] = 0;
        }
        random_data_one();
    }
    else{
        cout << "!!!!!!!!!!!向上无效" << endl << endl;
    }
}

void _left()
{
    cout << "按下了左键" << endl << endl;
    if (b_left())
    {
        cout << "可以向左合并" << endl << endl;
        for (int i = 0; i < MAX; i++)
        {
            memset(current, 0, sizeof(int)*MAX);
            int ii = 0;
            for (int j = 0; j < MAX; j++)
            {
                if (_matrix[i][j])
                    current[ii++] = _matrix[i][j];
            }
            for (int k = 0; k < ii - 1; k++)
            {
                if (current[k] == current[k + 1])
                {
                    current[k] *= 2;
                    score += current[k];
                    current[k + 1] = 0;
                    ++k;
                    --_count;
                }
            }
            ii = 0;
            for (int j = 0; j < MAX; j++)
            {
                if (current[j])
                    _matrix[i][ii++] = current[j];
            }
            for (; ii < MAX; ii++)
                _matrix[i][ii] = 0;
        }
        random_data_one();
    }
    else{
        cout << "!!!!!!!!!!!向左无效" << endl << endl;
    }
}

void _down()
{
    cout << "按下了下键" << endl << endl;
    if (b_down())
    {
        cout << "可以向下合并" << endl;
        for (int i = 0; i < MAX; i++)
        {
            memset(current, 0, sizeof(int)*MAX);
            int ii = 0;
            for (int j = MAX - 1; j >= 0; j--)
            {
                if (_matrix[j][i])
                    current[ii++] = _matrix[j][i];
            }
            for (int k = 0; k < ii - 1; k++)
            {
                if (current[k] == current[k + 1])
                {
                    current[k] *= 2;
                    score += current[k];
                    current[k + 1] = 0;
                    ++k;
                    --_count;
                }
            }
            ii = MAX - 1;
            for (int j = 0; j < MAX; j++)
            {
                if (current[j])
                    _matrix[ii--][i] = current[j];
            }
            for (; ii >= 0; ii--)
                _matrix[ii][i] = 0;
        }
        random_data_one();
    }
    else{
        cout << "!!!!!!!!!!!向下无效" << endl << endl;
    }
}

void _right()
{
    cout << "按下了右键" << endl << endl;
    if (b_right())
    {
        cout << "可以向右合并" << endl;
        for (int i = 0; i < MAX; i++)
        {
            memset(current, 0, sizeof(int)*MAX);
            int ii = 0;
            for (int j = MAX - 1; j >= 0; j--)
            {
                if (_matrix[i][j])
                    current[ii++] = _matrix[i][j];
            }
            for (int k = 0; k < ii - 1; k++)
            {
                if (current[k] == current[k + 1])
                {
                    current[k] *= 2;
                    score += current[k];
                    current[k + 1] = 0;
                    ++k;
                    --_count;
                }
            }
            ii = MAX - 1;
            for (int j = 0; j < MAX; j++)
            {
                if (current[j])
                    _matrix[i][ii--] = current[j];
            }
            for (; ii >= 0; ii--)
                _matrix[i][ii] = 0;
        }
        random_data_one();
    }
    else{
        cout << "!!!!!!!!!!!向右无效" << endl << endl;
    }
}</memory></string></ctime></cstdlib></iomanip></iostream>*/
