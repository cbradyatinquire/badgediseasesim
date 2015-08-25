#include "simpletools.h"
#include "badgealpha.h"

int32_t 	cpcog;

unsigned char TPCount = 7;
unsigned char TPPins[] = {BTN_OS, BTN_5, BTN_4, BTN_3, BTN_2, BTN_1, BTN_0};
unsigned char TPDischarge = 15;

int badgeLastRecordAddr;
int badgeRecordCount;
int badgeInfoAnchor;

// char ssss[20] = {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};                   // removed 8/23 4:36 PM

static screen badgeScreen;   // Added 8/23, 4:30 PM
static screen *oled;         // Added 8/23, 4:30 PM

info my;

//__attribute__((constructor))
int32_t badge_setup(void)
{
  // Setup IO and objects for application
  // clear all pins (this cog)
  //jm_io_start(&self->io, 0, 0);
  // setup touchpad interface
  //touch_start((*(uint8_t *)&dat[0]), (int32_t)(&(*(uint8_t *)&dat[1])), (*(uint8_t *)&dat[8]));
  touch_start(TPCount, TPPins, TPDischarge);
  // setup timing/delays for clock speed
  //jm_time_start();
  // connect to boot eeprom
  //jm_24xx512_start(0);
  // connect to accelerometer
  tilt_start(SCL, SDA);
  //start charlieplexing driver
  cpcog = light_start();
  //ircom_start(IR_IN, IR_OUT, 2400, 38500);
  //cog_run(ir_receive, 1024);
  //    ' start buffered oled driver
  //screen_init(OLED_CS, OLED_DC, OLED_DAT, OLED_CLK, OLED_RST, SSD1306_SWITCHCAPVCC, TYPE_128X64);
  // ir.start(IR_IN, IR_OUT, IR_BAUD, IR_FREQ)                      ' start IR serial  
  // start serial for terminal
  cog_run(displayControl, 512);
  print(" %c", CLS);
  char_size(BIG);
  oled = &badgeScreen;                   // Added 8/23 4:30 PM
  screen_autoUpdate(ON);                 // Added 8/23 4:30 PM
  return 0;
}

int screen_getAutoUpdate()                // <Added 8/23 4:43 PM>...
{
  return oled->AutoUpdate;
}

int screen_autoUpdate(int state)
{
  oled->AutoUpdate = state;
}                                         // </Added 8/23 4:43 PM>

int cogIRcom, *cogIRmgr;
void ir_start(void)
{
  cogIRcom = ircom_start(IR_IN, IR_OUT, 2400, 38500);
  cogIRmgr = cog_run(ir_receive, 512);
}  

void ir_stop(void)
{
  void ircom_stop(void);
  cog_end(cogIRmgr);
}  

int32_t show_screen(char *p_scr)
{
  // Displays two 8-line messages on OLED
  // -- uses Parallax font
  // -- both lines must be 8 characters wide
  //screen_AutoUpdateOff();                   // commented 8/23 5:24 PM
  screen_string8x2((p_scr + 0), 8, 0);
  screen_string8x2((p_scr + 9), 8, 1);
  screen_update();
  return 0;
}

void dec(int n)
{
  char s[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  sprint(s, "%d", n);
  //print("%s", s);
  string(s);
}  

#define CHAR 0
#define STR  1
#define DEC  2

int display(const char *fmt, ...)
{
  char buf[256];
  va_list args;
  int r;
  va_start(args, fmt);
  r = _dosprnt(fmt, args, buf);
  va_end(args);
  //putStr(buf);
  string(buf);
  return r;
}

int get_bit(int bitNum, int val)
{
  return val >> bitNum & 1;
}

void set_bit(int bitNum, int *val)
{
  *val |= (1 << bitNum);
}

void clear_bit(int bitNum, int *val)
{
  *val &= (~(1 << bitNum));
}  

void displayControl()
{
  screen_init(OLED_CS, OLED_DC, OLED_DAT, OLED_CLK, OLED_RST, SSD1306_SWITCHCAPVCC, TYPE_128X64);
  screen_autoUpdate(ON);                      // Changed 8/23 5:16 PM
  
  while (1)
  {
    if(oled->AutoUpdate);                     // Added 8/23 5:30 PM
      screen_update();
  }
}


info incoming;
info *get;
volatile int inbox = 0;

void ir_send(info *my)
{
  ircom_tx(STX);
  pause(1);
  ircom_str(my->name);
  ircom_tx(0);
  pause(1);
  ircom_str(my->email);
  ircom_tx(0);
  pause(1);
  ircom_tx(ETX);
  pause(1);
  ircom_txflush();
  pause(20);
}  


void ir_receive()
{
  get = &incoming;
  int temp = 0;
  inbox = 0;
  char s[48];
  memset(s, 0, 48);
  while(1)
  {
    memset(get, 0, sizeof(info));
    while(temp != STX)
    {
      //temp = ircom_rxget();                               ///
      temp = ircom_rxtime(10);                              ///
      //led(0, ON);
    }   
    //led(0, OFF);
    rgb(L, RED); 
    int i = 0;
    while(1)
    {
      //led(1, ON);                                // Commented 8/23 4:46 PM
      temp = ircom_rxtime(10);
      if(temp == ETX || temp == -1)
      {
        break;
      }
      else
      {
        s[i++] = (char) temp;       
      }
    }  
    //led(1, OFF);                                // Commented 8/23 4:46 PM
    //char s[16];
    //memcpy(s, &get.name, 16);
    strcpy(get->name, s);
    int offset = strlen(s) + 1;
    strcpy(get->email, &s[offset]);
    if(strncmp(get->name, "hotspot", 7) == 0)
    {
      //led(2, ON);                                // Commented 8/23 4:46 PM
      ee_displayContacts();                             ///
      ir_txContacts();
      inbox = 0;
      //led(2, OFF);                                // Commented 8/23 4:46 PM
    }
    else
    {
      //led(3, ON);                                // Commented 8/23 4:46 PM
      inbox = 1;
      rgb(L, GREEN);
      while(inbox);
      //led(3, OFF);                                // Commented 8/23 4:46 PM
      rgb(R, GREEN);
      //ircom_rxflush();                                 //////
    }      
    ircom_rxflush();                                   ///
  }    
}


void ir_txContacts(void)
{
  //led(3, ON);                                // Commented 8/23 4:46 PM
  ee_badge_check();
  //led(3, OFF);                                // Commented 8/23 4:46 PM
  info record;
  memset(&record, 0, sizeof(info));
  char s[48];
  int a = badgeInfoAnchor + 8;
  int sos = sizeof(s);
  memset(s, 0, sizeof(s));
  // int a = badgeLastRecordAddr;
  for(int i = 0; i < badgeRecordCount; i++)
  {
    int offset = 0;
    ee_getStr(s, sos, a);
    int ss = 1 + strlen(s);
    strcpy(record.name, s);
    offset += ss;
    ss = 1 + strlen(&s[offset]);
    strcpy(record.email, &s[offset]);
    a += (offset + ss);
    ir_send(&record);
    pause(100);
  } 
  info done = {{"txDone"}, {"done@domain.com"}};
  ir_send(&done);  
}    


void message_get(info *target)
{
  info *get = &incoming;
  memcpy(target, get, sizeof(info));
  memset(get, 0, sizeof(info));
  inbox = 0; 
}  

int check_inbox(void )
{
  return inbox;
}  

void clear_inbox(void)                          ////
{
  get = &incoming;
  memset(get, 0, sizeof(info));
  inbox = 0;
}                                               /////

void ee_wipe(void)
{
  for(int i = 0; i < 64; i++)
  {
    ee_putByte(255, 32768 + i);
  }    
  badgeLastRecordAddr = 0;
  badgeRecordCount = 0;
  badgeInfoAnchor = 0;
  ee_badge_check();                              ///
}

int dev_ee_show(void)                            //changed 8/23 4:54 PM
{
  for(int i = 0; i < 200; i++)
  {
    int c = ee_getByte(32768+i);
    if((c >= ' ' && c <= 'z'))
    {
      print("%c", c);
    }
    else
    {
      print("[%d]", c);
    }
  } 
  int addr = badgeInfoAnchor;
  int recCnt = ee_getInt(addr);
  int lastRec = ee_getInt(addr + 4);
  print("\n\nrecCnt = %d, lastRec = %d\n", recCnt, lastRec);
  while(1);           
}

int ee_badge_check(void)
{
  //if(badgeRecordCount) return badgeLastRecordAddr;
  //led(4, ON);                                // Commented 8/23 4:46 PM
  badgeInfoAnchor = 32768 + 16;
  char s[64];
  memset(s, 0, sizeof(s));
  int ss;
  char p[] = "Parallax eBadge";
  ss = 1 + strlen(p);
  int a = 32768;
  ee_getStr(s, 16, a);
  //led(4, OFF);                                // Commented 8/23 4:46 PM
  //led(5, ON);                                // Commented 8/23 4:46 PM
//  clear();
//  display("%s", p);
//  pause(1000);
  if(!strcmp(s, p))
  {
    //led(0, ON);
    a = badgeInfoAnchor;
    badgeRecordCount = ee_getInt(a);
    a += 4;
    badgeLastRecordAddr = ee_getInt(a);
    a += 4;
    //led(0, OFF);                                // Commented 8/23 4:46 PM
  }
  else
  {    
    //led(4, ON);                                // Commented 8/23 4:46 PM
    a = 32768;
    memcpy(p, "Parallax eBadge", 16);      ///
    //ee_putStr(p, 16, a);                 /////
    for(int i = 0; i < 16; i++)            //////
    {
      ee_putByte(p[i], a++);
    }                                      ///////
    //led(4, OFF);                                // Commented 8/23 4:46 PM
    a = badgeInfoAnchor + 8;
    ss = 1 + strlen(my.name);
    ee_putStr(my.name, ss, a);
    a += ss;
    ss = 1 + strlen(my.email);
    ee_putStr(my.email, ss, a);
    a += ss;
    badgeLastRecordAddr = a;
    badgeRecordCount = 1;
    ee_putInt(badgeRecordCount, badgeInfoAnchor);
    a+=4;
    ee_putInt(badgeLastRecordAddr, badgeInfoAnchor + 4);
    a += 4;
  }  
  //led(5, OFF);                                // Commented 8/23 4:46 PM
  return badgeLastRecordAddr;
} 
 
void ee_save(info *contact)
{
  int a = ee_badge_check();
  //int a = badgeLastRecordAddr;
  int ss = 1 + strlen(contact->name);
  ee_putStr(contact->name, ss, a);
  a += ss;
  ss = 1 + strlen(contact->email);
  ee_putStr(contact->email, ss, a);
  a += ss;
  ee_putInt(a, badgeInfoAnchor + 4);
  badgeRecordCount += 1;
  ee_putInt(badgeRecordCount, badgeInfoAnchor);
}    

void ee_displayContacts(void)
{
  ee_badge_check();
  info record;
  char s[48];
  int a = badgeInfoAnchor + 8;
  int sos = sizeof(s);
  memset(s, 0, sizeof(s));
  // int a = badgeLastRecordAddr;
  for(int i = 0; i < badgeRecordCount; i++)
  {
    int offset = 0;
    ee_getStr(s, sos, a);
    int ss = 1 + strlen(s);
    strcpy(record.name, s);
    offset += ss;
    ss = 1 + strlen(&s[offset]);
    strcpy(record.email, &s[offset]);
    a += (offset + ss);
    print("name: %s\n", record.name);
    print("email: %s\n", record.email);
  }    
}    


void ee_uploadContacts(fdserial *term)
{
  ee_badge_check();
  info record;
  char s[48];
  int a = badgeInfoAnchor + 8;
  int sos = sizeof(s);
  memset(s, 0, sizeof(s));
  // int a = badgeLastRecordAddr;
  fdserial_txChar(term, badgeRecordCount);
  dprint(term, "\n");
  for(int i = 0; i < badgeRecordCount; i++)
  {
    int offset = 0;
    ee_getStr(s, sos, a);
    int ss = 1 + strlen(s);
    strcpy(record.name, s);
    offset += ss;
    ss = 1 + strlen(&s[offset]);
    strcpy(record.email, &s[offset]);
    a += (offset + ss);
    dprint(term, record.name);
    dprint(term, "\n");
    dprint(term, record.email);
    dprint(term, "\n");
  }
  ee_wipe();
}    










/*
void ir_receive()
{
  get = &incoming;
  int temp = 0;
  inbox = 0;
  char s[48];
  memset(s, 0, 48);

  while(1)
  {
    memset(get, 0, sizeof(info));
    while(temp != STX)
    {
      temp = ircom_rxcheck();
    }   
    rgb(L, RED); 
    int i = 0;
    
    while(1)
    {
      temp = ircom_rxtime(10);
      if(temp == ETX || temp == -1)
      {
        break;
      }
      else
      {
        s[i++] = (char) temp;       
      }
    }  
    inbox = 1;
    strcpy(get->name, s);
    int offset = strlen(s) + 1;
    strcpy(get->email, &s[offset]);
    rgb(L, GREEN);
    while(inbox);
    rgb(R, GREEN);
    ircom_rxflush();
  }    
}
*/
