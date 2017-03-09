/*#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <memory>
// 2�ĸ��ʴ���4
#define random_2_4 (rand()%5==4 ? 4:2)
#define random_x(x) (rand()%x)
using namespace std;

// �����С
const int MAX = 4;
const int MAXMAX = MAX*MAX;
// ��ʼ���ݸ���
const int INIT_SIZE = 2;
// ���ݾ���
int _matrix[MAX][MAX];
// �ϲ���ʱ����
int current[MAX];
// ���ݼ���
int _count;
// �������� => ��ʹ��char...��ֹ����Ҫ��ʧ��... => ch[0]����...
string ch;
// �÷�...
int score;


// ��ӡ����
void print();
// ����˵��
void help();
// ��ʼ����
void init();
// ���λ�� => �������2/4
bool random_data_one();

// ��������
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
            cout <<"!!!!!!!!!!!!��Ч����..."<< endl << endl;
            break;
        }
        print();
    }
    cout << " Game Over !! " << endl;
    cout << "���յ÷�Socre => " << score << endl;
    system("pause");
    system("pause");
    system("pause");
    return 0;
}

void print()
{
    cout << "-------------------------------------" << endl;
    cout << "�÷�Socre => " << score << endl;
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
    cout << "wasd => ��������" << endl;
    cout << "������: ";
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
        // �������...��һ���п�Ԫ��...
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
            // ��Ԫ���Ϸ���Ԫ����ΪTrue...
            // ʹ�����Ҽл���...
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
        // �������...��һ���п�Ԫ��...
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
            // ��Ԫ���ұ���Ԫ����ΪTrue...
            // ʹ�����Ҽл���...
            int i = 0, j = MAX - 1;
            // i �ǿ�Ԫ��λ�ã�j�Ƿǿ�Ԫ��λ��
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
        // �������...��һ���п�Ԫ��...
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
            // ��Ԫ���Ϸ���Ԫ����ΪTrue... => �±ߵ�һ����Ԫ�����ϱߵ�һ���ǿ�Ԫ���±߼���...
            // ʹ�����Ҽл���...
            int i = 0, j = MAX - 1;
            // i �Ƿǿ�λ�ã�j�ǿ�Ԫ��λ��
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
        // �������...��һ����һ���п�Ԫ��...
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
            // ��Ԫ�������Ԫ����ΪTrue... => �ұߵ�һ����Ԫ������ߵ�һ���ǿ�Ԫ���ұ߼���...
            // ʹ�����Ҽл���...
            int i = 0, j = MAX - 1;
            // i �Ƿǿ�λ�ã�j�ǿ�Ԫ��λ��
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
    cout << "�������ϼ�" << endl << endl;
    if (b_up())
    {
        cout << "�������Ϻϲ�" << endl;
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
        cout << "!!!!!!!!!!!������Ч" << endl << endl;
    }
}

void _left()
{
    cout << "���������" << endl << endl;
    if (b_left())
    {
        cout << "��������ϲ�" << endl << endl;
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
        cout << "!!!!!!!!!!!������Ч" << endl << endl;
    }
}

void _down()
{
    cout << "�������¼�" << endl << endl;
    if (b_down())
    {
        cout << "�������ºϲ�" << endl;
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
        cout << "!!!!!!!!!!!������Ч" << endl << endl;
    }
}

void _right()
{
    cout << "�������Ҽ�" << endl << endl;
    if (b_right())
    {
        cout << "�������Һϲ�" << endl;
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
        cout << "!!!!!!!!!!!������Ч" << endl << endl;
    }
}</memory></string></ctime></cstdlib></iomanip></iostream>*/
