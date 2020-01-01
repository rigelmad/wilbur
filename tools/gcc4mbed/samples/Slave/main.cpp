#include "mbed.h"
#include <SPISlave.h>

Serial pc(USBTX, USBRX);

SPISlave slave(D5, D4, D3, A2);

PwmOut m1_left(D7);
PwmOut m1_right(D2);

DigitalOut r_en(D9);
DigitalOut l_en(D8);

float speed = 1;

int main() {
    //configure slave
    //slave.format(8, 0);
    //slave.frequency(1000000);
    
    slave.reply(0x01);

    // specify period first, then everything else (configure motors)
    r_en = 1;
    l_en = 1;
    m1_left.period(0.002f);
    m1_right.period(0.002f);
    pc.printf("finish setup");

    while(1) {
        pc.printf("inside loop");
       /*
        if(slave.receive() == 1) {
            /*
            response = slave.read();

            int speed = (int) response;

            m1_left.write(speed);
            m1_right.write(0.0f);
            
           m1_left.write(0.50f);
           m1_right.write(0.0f);
        }
        */
        
        m1_left.write(0.0f);
        m1_right.write(0.50f); 
        
        wait(1);
        m1_right.write(0.50f); 
        m1_left.write(0.0f);
        wait(1);
        
    }
}
