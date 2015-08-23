/*
  Beginner API Tests.c
  Explores API support of beginner learning curves.
*/

#include "simpletools.h"
#include "badgealpha.h"

int x, y, z;

void main()
{
  clear();
  char_size(BIG); 
  print("LED light control\n\n");
  display("  LEDs  ");
  pause(500);
  
  led(0, ON);
  pause(200);
  led(0, OFF);
  pause(200);
  led(5, ON);
  pause(200);
  led(5, OFF);
  
  pause(500);
  
  for(int n = 0; n < 6; n++)
  {
    led(n, ON);
    pause(200);
  }
  
  for(int n = 5; n >= 0; n--)
  {
    led(n, OFF);
    pause(200);
  }
  
  for(int n = 0; n < 6; n++)
  {
    leds_set(0b101010);
    pause(200);
    leds_set(0b010101);
    pause(200);
  }
  
  leds_set(0b111111);
  pause(500);

  // rgb light control
  // OFF (0), BLUE (1), GREEN (2), CYAN (3),
  // RED (4), MAGENTA (5), YELLOW (6), WHITE (7)

  print("RGB light control\n\n");
  cursor(2, 1);
  display("RGBs");

  rgb(L, YELLOW);
  rgb(R, BLUE);
  pause(400);
  rgb(L, RED);
  rgb(R, WHITE);
  pause(400);
  rgb(L, WHITE);
  rgb(R, GREEN);
  pause(400);
  
  for(int n = OFF; n < WHITE*2; n++)
  {
    rgbs_set(n, 8-n);
    pause(150);
  }      
  
  rgbs_set(OFF, OFF);
  leds_set(0b000000);

  pause(500);


  // Touch pad monitoring
  print("Blue lights indicate touch pads.\n");
  print("Press OSH to exit.\n\n");
  clear();
  display("TOUCHPAD");
  pause(500);
  char_size(SMALL);
  cursor(0, 4);
  display("PAD:   6543210");
  cursor(0, 5);
  display("STATE:");
  cursor(0, 7);
  display("EXIT: Press OSH");
  int states;
  while(1)
  {
    states = pads_get();
    leds_set(states);
    cursor(7, 5);
    display("%07b", states);
    if(states == 0b1000000) break;
  } 
  char_size(BIG);
  cursor(0, 0);    
  display("PADS    Done!   ");     
  pause(800);
  leds_set(0b000000);


  // Accelerometer
  print("Tilt with accelerometer\n\n");
  
  clear();
  display("TiltTest");
  char_size(SMALL);
  cursor(0, 7);
  display("Exit: Press OSH");

  while(pad(6) == 0)
  {
    print("%c", HOME);
    tilt_get(&x, &y, &z);
    cursor(5, 4);
    display("x = %3d " , x);
    cursor(5, 5);
    display("y = %3d ", y);
    cursor(5, 6);
    display("z = %3d ", z);
    pause(100);
  }

  // Display
  print("Display\n\n");
  clear();
  cursor(0,0);
  char_size(BIG);
  display("DISPLAY");
  char_size(SMALL);
  pause(1500);
  clear();
  
  for(int i = 0; i < 8; i++)
  {
    cursor(i, i);
    display("0123456789ABCDEF");
    pause(1000);
  }    
    
  clear();
  for(x = 0; x < 128; x++)
  {
    y = 64 - (x * x / 32);
    point( x, y, 1);
    point(128-x, y, 1);
    pause(20);
  }
  
  char_size(BIG);
  cursor(2, 1);
  display("Bye!");
}


