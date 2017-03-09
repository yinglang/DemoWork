#include "gamemap.h"
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

static int InitRandomSeed(){
    int seed = std::time(0);
    std::srand(seed);
    return seed;
}

static int RANDOM_SEED = InitRandomSeed();

class GameMapRow {
public:
    GameMap* self;
    GameMap::Direction d;
    int idx;
    GameMapRow(GameMap* self, GameMap::Direction d, int idx):
        self(self),d(d), idx(idx)
    {
        assert(idx < self->m_dim);
    }

    int size() const {
        return self->m_dim;
    }

    int& operator [](const int& i) {
        int* begin;
        switch(d){
            case GameMap::D_LEFT: // Left Operation
                begin = idx*self->m_dim+self->m_data;
                return begin[i];
            case GameMap::D_RIGHT: // Right Operation
                begin = idx*self->m_dim+self->m_data + self->m_dim - 1;
                return *(begin - i);
            case GameMap::D_UP:
                begin = self->m_data+idx;
                return *(begin + self->m_dim*i);
            case GameMap::D_DOWN:
                begin = self->m_data + self->m_dim*(self->m_dim -1) + idx;
                return *(begin - self->m_dim*i);
            default:
                assert(false);
                begin = 0;
                return *begin;
        }
    }

    bool canMove(){
        GameMapRow& r = *this;
        bool z_not_zero = r[0];
        for(int i=1;i<r.size();++i){
            if(r[i]==0 && z_not_zero){
                z_not_zero = false;
            }
            else if(!z_not_zero && r[i]){
                return true;
            }
        }

        for (int i=0;i<r.size()-1;++i){
            if(r[i]==r[i+1]&&r[i]){
                return true;
            }
        }
        return false;
    }

    void move(){
        this->bubbleUpZero();
        this->mergeValue();
        this->bubbleUpZero();
    }
private:
    void bubbleUpZero(){
        GameMapRow& r = *this;
        //! Bubble up zero
        for(int i=0;i<r.size();++i){
            if(r[i]){ // i is not zero, then swap i with the first zero value.
                for(int j=0; j<i;++j){
                    if(r[j]==0){ // First Zero Value
                        std::swap(r[i],r[j]);
                        break;
                    }
                }
            }
        }
    }

    void mergeValue(){
        GameMapRow& r = *this;
        for(int i=0;i<r.size() -1;++i){
            if(r[i]==r[i+1]&&r[i]){
                ++r[i];
                r[i+1] = 0;
                ++i;
            }
        }
    }

};



GameMap::GameMap(const int dimension, const int maxlimit)
    :m_dim(dimension), m_maxlimit(maxlimit)
{
    this->m_data = new int [m_dim*m_dim];
}

GameMap::~GameMap()
{
    delete [] this->m_data;
}

void GameMap::reset()
{
    using namespace std;
    memset(this->m_data, 0, m_dim*m_dim*sizeof(int));
    int pos_a = rand() %(this->m_dim*this->m_dim);
    int pos_b;
    do {
        pos_b = rand() %(this->m_dim*this->m_dim);
    }while(pos_a == pos_b);
    this->m_data[pos_a] = 1;
    this->m_data[pos_b] = 1;
}

void GameMap::move(GameMap::Direction d)
{
    if(!this->canMove(d)){
        return;
    }

    for(int i=0;i<m_dim;++i){
        GameMapRow r(this, d, i);
        r.move();
    }
    // Random New Position
    int new_pos;
    do {
        new_pos = rand() %(this->m_dim*this->m_dim);
    } while(this->m_data[new_pos]);
    this->m_data[new_pos] = 1;
}

bool GameMap::canMove(GameMap::Direction d) const
{
    for(int i=0;i<m_dim;++i){
        GameMapRow r(const_cast<GameMap*>(this),d, i);
        if(r.canMove()){
            return true;
        }
    }
    return false;
}
