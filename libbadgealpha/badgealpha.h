#include "simpletools.h"
#include "fdserial.h"

#define L 'L'
#define R 'R'
/*  max supported using IR connection  */
#define IR_BAUD (2400)
/*  matches receiver on DC22 badge  */
#define IR_FREQ (36000)
/*  for terminal io  */
#define T_BAUD (115200)

#define ACK (0)
#define NAK (1)

#define ON 1
#define OFF 0

/*  io pins  */
// programming / terminal
#define RX1 (31)
#define TX1 (30)
// eeprom / i2c
#define SDA (29)
#define SCL (28)
// touch buttons (left)
#define BTN_0 (27)
//define BTN_0 (17)
#define BTN_1 (26)
//define BTN_1 (16)
#define BTN_2 (25)
//define BTN_2 (15)
// touch buttons (right)
#define BTN_5 (17)
//define BTN_5 (27)
#define BTN_4 (16)
//define BTN_4 (26)
#define BTN_3 (15)
//define BTN_3 (25)
// open source logo button
#define BTN_OS (5)
// accelerometer interrupt in
#define ACC_INT (4)
// IR coms
#define IR_OUT (24)
#define IR_IN (23)
// OLED connections 
#define OLED_DAT (22)
#define OLED_CLK (21)
#define OLED_DC (20)
#define OLED_RST (19)
#define OLED_CS (18)
// composite video (J503)  
#define TV_DAC2 (14)
#define TV_DAC1 (13)
#define TV_DAC0 (12)
// audio (J503)
#define AUD_RT (10)
#define AUD_LF (9)
// blue led charlieplex pins 
#define BLU_CP2 (8)
// define BLU_CP2 (3)
#define BLU_CP1 (7)
// define BLU_CP1 (2)
#define BLU_CP0 (6)
// define BLU_CP0 (1)
// rgb led charlieplex pins 
#define RGB_CP2 (3)
// define RGB_CP2 (8)
#define RGB_CP1 (2)
// define RGB_CP1 (7)
#define RGB_CP0 (1)
// define RGB_CP0 (6)
// battery monitor(go/no-go)
#define BATT_MON (0)
/*  io configuration  */
// all bits off   
#define IS_OFF (0)
// all bits on    
#define IS_ON ((-1))
#define IS_LOW (0)
#define IS_HIGH ((-1))
#define IS_INPUT (0)
#define IS_OUTPUT ((-1))
/*  buttons masks  */
// upper right
#define PB0_MASK (1)
// middle right
#define PB1_MASK (2)
// lower right
#define PB2_MASK (4)
// lower left
#define PB3_MASK (8)
// middle left
#define PB4_MASK (16)
// upper left
#define PB5_MASK (32)
// open source logo
#define PB6_MASK (64)
#define OFF_MASK ((PB5_MASK | PB0_MASK))

/*  led control  */
// charlieplex control for blue leds
#define BLUE_0 (0)
#define BLUE_1 (1)
#define BLUE_2 (2)
#define BLUE_3 (3)
#define BLUE_4 (4)
#define BLUE_5 (5)
//define BLUE_0 (5)
//define BLUE_1 (4)
//define BLUE_2 (3)
//define BLUE_3 (2)
//define BLUE_4 (1)
//define BLUE_5 (0)

// charlieplex control for rgb leds
#define RGB_B1 (0)
#define RGB_G1 (1)
#define RGB_R1 (2)
#define RGB_B2 (3)
#define RGB_G2 (4)
#define RGB_R2 (5)

/*  fixed io pins  */
// blue led charlieplex pins 
#define BLU_CP2 (8)
#define BLU_CP1 (7)
#define BLU_CP0 (6)
// rgb led charlieplex pins 
#define RGB_CP2 (3)
#define RGB_CP1 (2)
#define RGB_CP0 (1)
/*  rgb colors  */
//define OFF (0)
#define BLUE (1)
#define GREEN (2)
#define CYAN (3)
#define RED (4)
#define MAGENTA (5)
#define YELLOW (6)
#define WHITE (7)



#define SCR_BLACK (0)
#ifndef SCR_WHITE
#define SCR_WHITE (1)
#endif
#define TYPE_128X32 (32)
#define TYPE_128X64 (64)
#define LCD_BUFFER_SIZE_BOTH_TYPES (1024)
#define SSD1306_LCDWIDTH (128)
#define SSD1306_LCDHEIGHT32 (32)
#define SSD1306_LCDHEIGHT64 (64)
#define SSD1306_LCDCHARMAX (8)
#define SSD1306_SETCONTRAST (129)
#define SSD1306_DISPLAYALLON_RESUME (164)
#define SSD1306_DISPLAYALLON (165)
#define SSD1306_NORMALDISPLAY (166)
#define SSD1306_INVERTDISPLAY (167)
#define SSD1306_DISPLAYOFF (174)
#define SSD1306_DISPLAYON (175)
#define SSD1306_SETDISPLAYOFFSET (211)
#define SSD1306_SETCOMPINS (218)
#define SSD1306_SETVCOMDETECT (219)
#define SSD1306_SETDISPLAYCLOCKDIV (213)
#define SSD1306_SETPRECHARGE (217)
#define SSD1306_SETMULTIPLEX (168)
#define SSD1306_SETLOWCOLUMN (0)
#define SSD1306_SETHIGHCOLUMN (16)
#define SSD1306_SETSTARTLINE (64)
#define SSD1306_MEMORYMODE (32)
#define SSD1306_COMSCANINC (192)
#define SSD1306_COMSCANDEC (200)
#define SSD1306_SEGREMAP (160)
#define SSD1306_CHARGEPUMP (141)
#define SSD1306_EXTERNALVCC (1)
#define SSD1306_SWITCHCAPVCC (2)
// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL (47)
#define SSD1306_DEACTIVATE_SCROLL (46)
#define SSD1306_SET_VERT_SCROLL_AREA (163)
#define SSD1306_RIGHT_HORIZ_SCROLL (38)
#define SSD1306_LEFT_HORIZ_SCROLL (39)
#define SSD1306_VERTRIGHTHORIZSCROLL (41)
#define SSD1306_VERTLEFTHORIZSCROLL (42)

#define STX 2                                                        //serial framing bytes
#define ETX 3


// i2c address - write
#define MMA7660_WR (152)
// i2c address - read 
#define MMA7660_RD (153)
/* 0x40 */
// set when reading could be corrupted
#define ALERT_BIT (64)
// for reading x, y, z, and tilt at once
#define ALERT_XYZT (1077952576)
/*  registers  */
// 6-bit output value X
#define XOUT (0)
// 6-bit output value Y
#define YOUT (1)
// 6-bit output value Z
#define ZOUT (2)
// Tilt Status
#define TILT (3)
// Sampling Rate Status
#define SRST (4)
// Sleep Count
#define SPCNT (5)
// Interrupt Setup
#define INTSU (6)
// Mode
#define MODE (7)
// Auto-Wake/Sleep, P/L SPS, debounce filter
#define SR (8)
// Tap Detection
#define PDET (9)
// Tap Debounce Count
#define PD (10)



/*
INLINE__ int Shr__(unsigned int a, unsigned int b) { return (a>>b); }
*/

//modified 8/24
typedef struct user_info{
  char name[7];
  char email[7];
  char endchar;
} info;  


int screen_getAutoUpdate();                  // Added 8/23 5:00 PM
int screen_autoUpdate(int state);            // Added 8/23 5:00 PM
int badge_setup( void );
int show_screen( char *p_scr );
int display(const char *fmt, ...);
//void display(int32_t data, int format);
int get_bit(int bitNum, int val);
void set_bit(int bitNum, int *val);
void clear_bit(int bitNum, int *val);
void displayControl();
void ir_send(info *my);
void ir_receive();
void message_get(info *target);
int check_inbox(void);
void clear_inbox(void);
void ir_start(void);
void ir_stop(void);

void ee_wipe(void);
int ee_badge_check(void);
void ee_save(info *contact);
void ee_displayContacts(void);
void ee_uploadContacts(fdserial *term);
void ir_txContacts(void);


//
// automatically generated by spin2cpp v1.93 on Tue Aug 11 09:49:47 2015
// spin2cpp --ccode --main demo.spin 
//

#ifndef light_Class_Defined__
#define light_Class_Defined__

  typedef struct light {
  // cog running led driver
    volatile int	cog;
  // active leds (blue in byte0, rgb in byte1)
    volatile int	ledbits;
  // ticks in refresh cycle
    volatile int	cycleticks;
  } light;

  void led(int n, int state);
  void leds_set(int bits);
  void rgb(int side, int color);  
  void rgbs_set(int colorL, int colorR);

  void light_set_rgb1( int bits);
  void light_set_rgb2( int bits);
  void light_set_rgb( int bits);
  void light_set_all( int bits);
  void light_clear( void );
  int light_start( void );
  void light_stop( void );
  
#endif


//
// automatically generated by spin2cpp v1.93 on Tue Aug 11 09:49:47 2015
// spin2cpp --ccode --main demo.spin 
//

#ifndef touch_Class_Defined__
#define touch_Class_Defined__

  typedef struct touch {
  // # of pins scanned
    volatile int	pincount;
  // pointer to list of pins (byte array)
    volatile int	p_pinslist;
  // discharge timing
    volatile int	disch;
  // mask for input pins
    volatile int	pinsmask;
  } touch;

  int touch_start(int count, unsigned char *p_pins, int dms);
  int pad( int pad );
  int pads_get( void );

#endif


#ifndef jm_i2c_Class_Defined__
#define jm_i2c_Class_Defined__

#define SDA1 (29)
#define SCL1 (28)

/*  fixed io pins  */
// programming / debug port

typedef struct jm_i2c {
// buss pins
  volatile int32_t	scl;
  volatile int32_t	sda;
} jm_i2c;

  int32_t jm_i2c_setup( jm_i2c *self );
  int32_t jm_i2c_setupx( jm_i2c *self, int32_t sclpin, int32_t sdapin);
  int32_t jm_i2c_present( jm_i2c *self, int32_t ctrl);
  int32_t jm_i2c_wait( jm_i2c *self, int32_t ctrl);
  int32_t jm_i2c_start( jm_i2c *self );
  int32_t jm_i2c_write( jm_i2c *self, int32_t i2cbyte);
  int32_t jm_i2c_read( jm_i2c *self, int32_t ackbit);
  int32_t jm_i2c_stop( jm_i2c *self );
#endif

//
// automatically generated by spin2cpp v1.93 on Tue Aug 11 09:49:47 2015
// spin2cpp --ccode --main demo.spin 
//

#ifndef tilt_Class_Defined__
#define tilt_Class_Defined__

//include "jm_i2c.h"

  typedef struct tilt {
    jm_i2c	i2c;
  } tilt;

  int tilt_start( int sclpin, int sdapin);
  int tilt_present( void );
  int tilt_getX( void );
  int tilt_getY( void );
  int tilt_getZ( void );
  void tilt_get(int *x, int *y, int *z);
  int tilt_read_tilt( void );
  int tilt_read_axis( int reg);
  int tilt_read_all( int p_axes);
  int tilt_write_reg( int reg, int value);
  int tilt_read_reg( int reg);

#endif


//
// automatically generated by spin2cpp v1.93 on Tue Aug 11 09:49:47 2015
// spin2cpp --ccode --main demo.spin 
//

#ifndef screen_Class_Defined__
#define screen_Class_Defined__

  #define clear screen_clear
  #define point screen_point
  #define BIG 1
  #define SMALL 0

  typedef struct screen {
    volatile int	cog, command;
    volatile int	CS, DC, DATA, CLK, RST, vccstate;
    volatile int	displayWidth, displayHeight, displayType;
    volatile int	AutoUpdate;
    volatile uint8_t	buffer[LCD_BUFFER_SIZE_BOTH_TYPES];
    volatile int charSize;
    volatile int crsrX, crsrY;
  } screen;
  
  // Beginner
  int screen_clear( void );
  int screen_point( int x, int y, int color);
  int line( int x0, int y0, int x1, int y1, int c);
  int box( int x0, int y0, int x1, int y1, int c);

  void char_size(int size);
  void cursor(int col, int row);
  void string(char *str);
  void text(char *str, int count);
  void letter(char c);
  void dec(int n);
  
  int screen_string8x1( char *str, int len);
  int screen_string8x2( char *str, int len, int row);
  int screen_string16x4( char *str, int len, int row, int col);
  int screen_string8x2v2(char *str, int32_t len, int32_t row, int32_t col);
  int screen_string8x2v3(char *str, int32_t len, int32_t row, int32_t col);
  
  int screen_char32x16( int ch, int row, int col);
  int screen_char7x5( int ch, int row, int col);
  int screen_invert( int i);
  

  // Intermediate
  int screen_scrollRight( int scrollStart, int scrollStop);
  int screen_scrollLeft( int scrollStart, int scrollStop);
  int screen_scrollRightDiag( int scrollStart, int scrollStop);
  int screen_scrollLeftDiag( int scrollStart, int scrollStop);
  int screen_scrollStop( void );
  int screen_AutoUpdateOn( void );
  int screen_AutoUpdateOff( void );
  int screen_update( void );

  // Advanced
  screen *screen_dataAddr();                     // Added 8/23 5:12 PM
  int screen_GetDisplayHeight( void );
  int screen_GetDisplayWidth( void );
  int screen_GetDisplayType( void );
  int screen_HIGH( int Pin);
  int screen_LOW( int Pin);
  int screen_ssd1306_Command( int thecmd);
  int screen_ssd1306_Data( int thedata);
  int screen_getBuffer( void );
  int screen_getSplash( void );
  int screen_SHIFTOUT( int Dpin, int Cpin, int CSpin, int Bits, int Value);
  int screen_WRITEBUFF( int Dpin, int Cpin, int CSpin, int Bits, int Addr);
  int screen_start( void );
  int screen_stop( void );
  int screen_init( int ChipSelect, int DataCommand, int TheData, int TheClock, int Reset, int VCC_state, int Type);

#endif


//
// automatically generated by spin2cpp v1.93 on Tue Aug 11 09:49:47 2015
// spin2cpp --ccode --main demo.spin 
//

#ifndef jm_time_Class_Defined__
#define jm_time_Class_Defined__

  typedef struct jm_time {
  // sync point (snapshot of cnt)
    volatile int	sync;
  // milliseconds accumulator
    volatile int	msecs;
  // timing (ticks) accumulator
    volatile int	rawtix;
  // when <> 0, time on hold
    volatile int	holdstate;
  } jm_time;

  int jm_time_start( void);
  int jm_time_startx(int now);
  int jm_time_set(int ms);
  int jm_time_set_secs(int secs);
  int jm_time_millis( void);
  int jm_time_seconds( void);
  int jm_time_adjust(int ms);
  int jm_time_adjust_secs(int secs);
  int jm_time_mark( void);
  int jm_time_sync_point( void);
  int jm_time_hold( void);
  int jm_time_release( void);
  int jm_time_on_hold( void);
  int jm_time_pause(int ms);
  int jm_time_pause_us(int us);

#endif


//
// automatically generated by spin2cpp v1.93 on Tue Aug 11 09:49:47 2015
// spin2cpp --ccode --main demo.spin 
//

#ifndef jm_i2c_Class_Defined__
#define jm_i2c_Class_Defined__

  /*  fixed io pins  */
  // programming / debug port
  
  typedef struct jm_i2c {
  // buss pins
    volatile int	scl;
    volatile int	sda;
  } jm_i2c;
  
  int jm_i2c_setup( jm_i2c *self );
  int jm_i2c_setupx( jm_i2c *self, int sclpin, int sdapin);
  int jm_i2c_present( jm_i2c *self, int ctrl);
  int jm_i2c_wait( jm_i2c *self, int ctrl);
  int jm_i2c_start( jm_i2c *self );
  int jm_i2c_write( jm_i2c *self, int i2cbyte);
  int jm_i2c_read( jm_i2c *self, int ackbit);
  int jm_i2c_stop( jm_i2c *self );
  
#endif

//
// automatically generated by spin2cpp v1.93 on Sun Aug 16 16:35:52 2015
// spin2cpp --ccode --main IrApp.spin 
//

#define MAX_CONTACTS 25                                              //size of contacts db
#define CONTACT_LEN 40 + 1                                          // 40 chars + terminating 0

#ifndef ircom_Class_Defined__
#define ircom_Class_Defined__

#include <stdint.h>

/*  configuration  */
// power of 2  (2..512)
#define BUF_SIZE (128)
#define BUF_MASK ((BUF_SIZE - 1))

typedef struct jm_ir_hdserial {
// cog flag/id
  volatile int32_t	cog;
// rx head index
  volatile int32_t	rxhead;
// rx tail index
  volatile int32_t	rxtail;
// hub address of rxbuf
  volatile int32_t	rxhub;
// tx head index
  volatile int32_t	txhead;
// tx tail index
  volatile int32_t	txtail;
// hub address of txbuf
  volatile int32_t	txhub;
// rx pin (in)
  volatile int32_t	rxpin;
// tx pin (out)
  volatile int32_t	txpin;
// bit timing (ticks)
  volatile int32_t	bitticks;
// ctrx setup for freq
  volatile int32_t	frsetup;
// rx and tx buffers
  volatile uint8_t	rxbuf[BUF_SIZE];
  volatile uint8_t	txbuf[BUF_SIZE];
} jm_ir_hdserial;

  int32_t ircom_start(int32_t rxd, int32_t txd, int32_t baud, int32_t freq);
  void ircom_stop(void);
  int32_t ircom_rx(void);
  int32_t ircom_rxcheck(void);
  int32_t ircom_rxtime(int32_t ms);
  int32_t ircom_rxflush(void);
  int32_t ircom_tx(int32_t c);
  int32_t ircom_str(char *p_zstr);
  int32_t ircom_dec(int32_t value);
  int32_t ircom_rjdec(int32_t val, int32_t width, int32_t pchar);
  int32_t ircom_hex(int32_t value, int32_t digits);
  int32_t ircom_tx_bin(int32_t value, int32_t digits);
  int32_t ircom_txflush(void);
#endif
  

#ifdef __GNUC__
#define INLINE__ static inline
#define PostEffect__(X, Y) __extension__({ int tmp__ = (X); (X) = (Y); tmp__; })
#else
#define INLINE__ static
static int tmp__;
#define PostEffect__(X, Y) (tmp__ = (X), (X) = (Y), tmp__)
#define waitcnt(n) _waitcnt(n)
#define coginit(id, code, par) _coginit((unsigned)(par)>>2, (unsigned)(code)>>2, id)
#define cognew(code, par) coginit(0x8, (code), (par))
#define cogstop(i) _cogstop(i)
#endif

/*
__asm__ volatile( "    .global __clkfreqval\n" );
__asm__ volatile( "    __clkfreqval = 0x4c4b400\n" );
__asm__ volatile( "    .global __clkmodeval\n" );
__asm__ volatile( "    __clkmodeval = 0x6f\n" );
*/


//extern char beanie[LCD_BUFFER_SIZE_BOTH_TYPES];