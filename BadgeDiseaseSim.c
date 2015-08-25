#include "simpletools.h"
#include "badgealpha.h"
#include "fdserial.h"

info my = {{"000000"}, {"000000"}, 0};
info their;
info last = {{" "}, {" "}, 0};

int id_address = 65335;

char handshake[5];
fdserial *port;

int x, y, z;  // accelerometer
int heldatstart = 0;

void screen_img180();
void screen_img(char *imgaddr);

void main()
{
  // Initialize badge and serial connection
  badge_setup();
  simpleterm_close();
  
  // Pull ID from EEPROM
  leds_set(0b111111);
  char id[7];
  ee_getStr(id, 7, id_address);
  strcpy(my.name, id);
  strcpy(my.email, id);
  
  pause(200);
  char_size(SMALL);
  cursor(3, 3);
  display("Uploading?");
  cursor(4, 5);
  display("Hold OSH");
  leds_set(0b000000);
  if (pad(6) == 1) 
  {
     heldatstart = 1;
  }
  pause(200);    
  leds_set(0b100001);
  pause(200);
  leds_set(0b110011);
  pause(200);
  leds_set(0b111111);
  pause(300);
  clear();
  
  if (heldatstart == 1 && pad(6) == 1)
  {
    clear();
    port = fdserial_open(31, 30, 0, 115200);
    // Check for host upload
    int attempt = 0;
    while (attempt < 50)
    {
      dprint(port, "Propeller\n");
      pause(200);  // We need this pause, since the host needs time to respond. 5 x 1 second = 10 second timeout
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
  
  while(1)
  {
    memset(&their, 0, sizeof(info));
    tilt_get(&x, &y, &z);
    if (y < -35)
    {
      clear();
      leds_set(0b000000);
      //screen_autoUpdate(OFF);
      char_size(SMALL);
      cursor(3, 2); 
      display("ID: %s", my.email);
      cursor(0, 5);
      display("Last Interaction");
      cursor(5, 6);
      display(last.name);
      //screen_img180();
      //screen_autoUpdate(ON);
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
      display("INTERACTION!");
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

void screen_img180()
{
  uint32_t screenbuf = screen_getBuffer();
  char *scrbuf = (char *) screenbuf;
  char altbuf[1024];
  memset(altbuf, 0, 1024);
  screen_autoUpdate(OFF);
  int byte, bit, pixel, xp, yp;
  for(int x = 0; x < 128; x++)
  {
    for(int y = 0; y < 64; y++)
    {
      byte = ((y >> 3) << 7) + x;
      bit = y % 8;  
      pixel = 1 & (scrbuf[byte] >> bit);
      
      xp = 127 - x;
      yp = 63 - y;
      
      byte = ((yp >> 3) << 7) + xp;
      bit = yp % 8;  
      altbuf[byte] |= (pixel << bit);
    }
  } 
  memset(scrbuf, 0, 1024);
  memcpy(scrbuf, altbuf, 1024);
  screen_autoUpdate(ON); 
}

/*
void screen_img(char *imgaddr)
{
  uint32_t screenbuf = screen_getBuffer();
  char *scrbuf = (char *) screenbuf;
  screen_autoUpdate(OFF);
  memcpy(scrbuf, imgaddr, 1024);
  screen_autoUpdate(ON); 
}  

*/