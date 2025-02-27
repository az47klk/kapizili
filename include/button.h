#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>

class button{
    private:
        int buttonPinNumber;
        
        void onPressed();
        void onPressedOut();
    public:
        bool pressed = false;
        button(int _pin);
        void loopActivator();

};

#endif