#include <propeller.h>
#include "badgealpha.h"
#include "simpletools.h"
/*
char altbuf[1024];
*/
/*
for x in 0, (width/2)-1
{
	for y in 0, height-1
	{
	   temp = buffer[width-x, height-y]
	   buffer[width-x, height-y] = buffer[x,y]
	   buffer[x, y] = temp
	}
}
*/

void main()
{
  /*
  badge_setup();
  uint32_t screenbuf = screen_getBuffer();
  char *scrbuf = (char *) screenbuf;
  //char altbuf[1024];
  display("hello");
  pause(1000);

  screen_autoUpdate(OFF);
  //memcpy(scrbuf, beanie, 1024);
  for(int i = 0; i < 1024; i++)
  {
    scrbuf[i] = beanie[i];
  }    
  screen_autoUpdate(ON);

  pause(1000);
  cursor(0,0);
  display("hello again");
  pause(1000);
  screen_clear();
  
  screen_autoUpdate(OFF);
  
  int i, pixBit, pixel;
  for(int x = 0; x < 128; x++)
  {
    for(int y = 0; y < 64; y++)
    {
      i = (x * y) / 8;
      pixBit = (x * y) % 8;
      pixel = (beanie[i] >> (7-pixBit)) & 1;
      point(x, y, pixel);
    }
  }
  while(1);
  memcpy(scrbuf, altbuf, 1024);
  screen_autoUpdate(ON);          

  while(1);  
  */   
}


/*
for x in 0, (width/2)-1
{
	for y in 0, height-1
	{
	   temp = buffer[width-x, height-y]
	   buffer[width-x, height-y] = buffer[x,y]
	   buffer[x, y] = temp
	}
}
*/

/*  
  screen_autoUpdate(OFF);
  
  int i, pixBit, pixel;
  for(int x = 0; x < 128; x++)
  {
    for(int y = 0; y < 64; y++)
    {
      i = (x * y) / 8;
      pixBit = (x * y) % 8;
      pixel = (beanie[i] >> pixBit) & 1;
      
      altbuf[i] |= (pixel << pixBit);
    }
  }
  memcpy(scrbuf, altbuf, 1024);
  screen_autoUpdate(ON);          
*/