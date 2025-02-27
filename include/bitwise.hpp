#ifndef __bitwise_47_hpp__
#define __bitwise_47_hpp__

bool getbit(int _input, int numOfBit){
    if (numOfBit > 7) return -1;
    else{
        int aa = _input;
        aa |= (1u << numOfBit);
        if (_input == aa) return 1;
        else return 0;
    }
}

int changebit(int __input, int numOfBit, bool endbit){
    int _input = __input;
    if (endbit == 1){
        _input |= (1u << numOfBit);
    }
    else {
        _input &= ~(1u << numOfBit);
    }
    return _input;
}

#endif