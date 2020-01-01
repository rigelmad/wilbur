  #include "mbed.h"
  #include "SerialHalfDuplex.h"
  // p9 and p10 should be wired together to form "a"
  // p28 and p27 should be wired together to form "b"
  // p9/p10 should be wired to p28/p27 as the Half Duplex connection

  SerialHalfDuplex a(PA_9, PA_10);
  SerialHalfDuplex b(PA_11, PA_12);

  void b_rx() { // second device response
      b.putc(b.getc() + 4);
  }

  int main() {
      b.attach(&b_rx);
      for(int c = 'A'; c < 'Z'; c++) {
          a.putc(c);
          printf("sent [%c]\n", c);
          wait(0.5);   // b should respond
          if(a.readable()) {
              printf("received [%c]\n", a.getc());
          }
      }
  }
