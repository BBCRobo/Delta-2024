#ifndef LS_H_
#define LS_H_

#include <Arduino.h>
#include <Define.h>

class LS
{
public:
    LS() {};

    void init();
    void read();
    bool isBlackTile();
    bool isSilverTile();

private:
    int LSs[LS_COUNT] = {LS_1, LS_2, LS_3};
    double LSinit[LS_COUNT] = {910, 920, 910};
    double LSdiff[LS_COUNT] = {500, 500, 500};
    double LSsilverdiff[LS_COUNT] = {200, 200, 200};
};

#endif