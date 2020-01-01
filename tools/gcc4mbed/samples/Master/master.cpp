#include "mbed.h"

SPI master(D5, D4, D3);
AnalogOut cs(A2);

int main() {
    
    
    while(1) {
        cs = 1;

        master.format(8, 0);
        master.frequency(1000000);

        cs = 0;

        int response = master.write(0x01);
    
        cs = 1;
    }
    
}