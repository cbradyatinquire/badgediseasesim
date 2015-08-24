#include "simpletools.h"
#include "badgealpha.h"
#include "fdserial.h"

info my = {{"000000"}, {"000000"}, 0};
info their;
info last = {{" "}, {" "}, 0};

char handshake[5];
fdserial *port;

int x, y, z;  // accelerometer
int heldatstart = 0;

void main()
{
  // Initialize badge and serial connection
  badge_setup();
  simpleterm_close();
  leds_set(0b111111);
  pause(200);
  char_size(SMALL);
  cursor(3, 3);
  display("Uploading?");
  cursor(4, 5);
  display("Hold OSH");
  leds_set(0b000000);
  //pause(1000);
  //if (pad(6) == 1) 
  //{
  //   heldatstart = 1;
  //}
  pause(200);    
  leds_set(0b100001);
  pause(200);
  leds_set(0b110011);
  pause(200);
  leds_set(0b111111);
  pause(300);
  clear();
  
  //if (heldatstart == 1 && pad(6) == 1)
  if (pad(6) == 1)
  {
    clear();
    port = fdserial_open(31, 30, 0, 115200);
    // Check for host upload
    int attempt = 0;
    while (attempt < 5)
    {
      dprint(port, "Propeller\n");
      pause(200);
      if (fdserial_rxCount(port) == 0)
      {
        attempt++;
        continue;
      }      
      else if (fdserial_rxCount(port) < 5)
      {
        fdserial_rxFlush(port);
        attempt++;
        continue;
      }
      else dscan(port, "%s", handshake);
      // Attempt handshake and listen to response
      if (strcmp(handshake, "H0st") == 0)
      {
        char_size(SMALL);
        cursor(3, 3);
        display("CONNECTED!");
        cursor(2, 4);
        display("Uploading...");
        ee_uploadContacts(port);
        cursor(0, 5);
        display("Upload complete!");
        return;
      }      
      attempt++;
    }
  }
        
  // Need to dprint for it to work?
  //dprint(port, "IR Test\n");
  //fdserial_close(port);
  
  leds_set(0b100001);
  ir_start();
  pause(500);
  clear();
  
  // Pull ID from EEPROM
  
  
  while(1)
  {
    memset(&their, 0, sizeof(info));
    tilt_get(&x, &y, &z);
    if (y < -35)
    {
      clear();
      leds_set(0b000000);
      char_size(SMALL);
      cursor(3, 2); 
      display("ID: %s", my.email);
      cursor(0, 5);
      display("Last Interaction");
      cursor(5, 6);
      display(last.name);
      while(y < -35)
      {
        tilt_get(&x, &y, &z);
        pause(200);
      }
      clear();
    }
    else if (pad(1) == 1 && pad(4) == 1)
    {
      clear();
      cursor(3, 3);
      display("Sending...");
      led(4, ON); 
      led(1, ON);
      rgb(L, BLUE);
      ir_send(&my);
      rgb(L, OFF);
      cursor(6, 5);
      display("DONE");
      while (pad(1) == 1 && pad(4) == 1);
      led(4, OFF);
      led(1, OFF);
      clear();
    }
    else if(check_inbox() == 1)
    {
      clear();
      memset(&last, 0, sizeof(info));
      message_get(&their);
      last = their;
      ee_save(&their);
      cursor(2, 1);
      display("New Contact!");
      cursor(3, 4);
      display("ID: %s", their.email);
      cursor(0, 7);
      display("OSH to Continue.");
      rgb(L, OFF);
      while(pad(6) != 1);     
      rgb(R, OFF);
      clear();
    }
    else
    {
      char_size(SMALL);
      cursor(3, 2); 
      display("ID: %s", my.name);
      cursor(5, 6);
      display("Ready.");
      leds_set(0b101101);
      pause(200);
    }      
  }    
}  