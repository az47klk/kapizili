#include <button.h>

button::button(int _pin){
    pinMode(_pin, INPUT_PULLUP);
    button::buttonPinNumber = _pin;
    button::pressed = digitalRead(_pin);
}

void button::loopActivator(){
    // get pwm value to a var
    bool tempVal = digitalRead(button::buttonPinNumber); 
    
    if (button::pressed != tempVal){
        delay(100); 
        tempVal = digitalRead(button::buttonPinNumber); 
        if (button::pressed != tempVal){
            if(tempVal){
                button::onPressedOut();
            }
            else {
                button::onPressed();
            }
            button::pressed = tempVal;
        }
    }
}

