#include "simpletools.h"
#include "badgealpha.h"
#include "fdserial.h"

info my = {{"TEST"}, {"123890"}, 0};
info* get;

int addr = 65335;

fdserial *port;

char newname[6];
void save_id_info();

void main()
{
  // Initialize badge and serial connection
  badge_setup();
  simpleterm_close();

  port = fdserial_open(31, 30, 0, 115200);
  
  get_to_get();
  clear();
  char_size(SMALL);
  cursor(0, 1);
  display("WAITING");
  cursor(0, 2);
  display("current name");
  cursor(2, 3); 
  display(get->name);
  

  leds_set(0b111111);
  dprint(port, "Give me a name\n");

  dscan(port, "%s", newname);
  char_size(SMALL);
  cursor(0, 5);
  strcpy(my.name, newname);
  strcpy(my.email, newname);
  display(newname);
  cursor(0, 6);
  //display(my.name);
  save_id_info();
  get_to_get();
  display(get->name);
  cursor(2, 7);
  display("saved");
}


void save_id_info()
{
  int a = addr;
  int ss = 1 + strlen(my.name);
  ee_putStr(my.name, ss, a);
  a += ss;
  ss = 1 + strlen(my.email);
  ee_putStr(my.email, ss, a);
  a += ss;
  ee_putInt(0, a);
}
        

void get_to_get() 
{
  int offset = 0;
  char s[32];
  int sos = sizeof(s);
  memset(s, 0, sizeof(s));
  int a = addr;
  ee_getStr(s, sos, a);
  int ss = 1 + strlen(s);
  strcpy(get->name, s);
  offset += ss;
  ss = 1 + strlen(&s[offset]);
  strcpy(get->email, &s[offset]);
  a += (offset + ss);
}
     
