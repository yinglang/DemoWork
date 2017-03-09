#ifndef GAMEMAP_H
#define GAMEMAP_H

class GameMap
{
private:
    //! Disable Copy
    GameMap(const GameMap &);
    GameMap &operator=(const GameMap &);

public:
    enum Direction{
        D_LEFT = 0,
        D_RIGHT,
        D_UP,
        D_DOWN,
        DIRECTION_MAX
    };


    GameMap(const int dimension, const int maxlimit=-1);

    virtual ~GameMap();

    //! Reset the game map.
    void reset();

    inline int getDimension()const {
        return this->m_dim;
    }

    inline int getMaxLimit()const {
        return this->m_maxlimit;
    }

    inline const int* getData()const {
        return this->m_data;
    }

    inline bool canMove()const{
        for(int i=0;i<DIRECTION_MAX;++i){
            if(canMove(Direction(i))){
                return true;
            }
        }
        return false;
    }

    void move(Direction d);

    inline bool isReachMaxLimit(){
        for(int i=0;i<this->m_dim*this->m_dim; ++i){
            if(this->m_data[i] > m_maxlimit){
                return true;
            }
        }
        return false;
    }

private:
    bool canMove(Direction d) const;

private:
    const int m_dim;
    const int m_maxlimit;
    int* m_data;

    friend class GameMapRow;
};

#endif // GAMEMAP_H
