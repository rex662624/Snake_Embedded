/*************************************************************************
                           HEADER FILES
 **************************************************************************/
#include<pic18.h>                 
#pragma config OSC = INTIO67      // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = ON       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 1        // Watchdog Timer Postscale Select bits (1:1)

//            
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)
/******************************BUZZER**********************************/
#define _XTAL_FREQ 8000000
#define BUZZER LATCbits.LATC2
#define Max_Notes 114

unsigned char TIMER_H, TIMER_L;
unsigned int Notes[Max_Notes] =
{
440,490,523,494,523,659,494,330,440,392,440,523,392,330,349,330,349,523,523,//19
330,523,494,370,370,494,494,440,494,523,494,523,659,494,330,330,//17
440,392,440,523,392,330,349,523,490,490,523,578,659,523,523,490,440,490,415,//19
440,523,578,659,578,698,784,578,392,578,523,490,523,523,578,659,//18        
659,440,490,523,490,523,578,523,392,392,698,659,578,523,659,440,490,523,490,523,659,//21
490,330,440,392,440,523,392,330,349,523,490,490,523,578,659,523,523,//17
523,490,440,490,415,440
};
unsigned char Durations[Max_Notes] ={
1,1,1,1,1,1,3,1,1,1,1,1,3,1,1,1,1,1,1,
2,1,1,1,1,1,2,1,1,1,1,1,2,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
2,1,1,1,1,1,1,1,1,2,1,1,1,1,2,1,1,1,1,1,1,
2,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,2,//17
1,1,1,1,1,3

};

void Delay_Ms(unsigned int s)
{
unsigned int j;
for(j = 0; j < s; j++)__delay_ms(1);
}

//
// This funcon plays a note with the specified frequency and duraon
//
void Sound_Play(unsigned int freq, unsigned int duration)
{
    float period;
    period = 500000.0/freq;
    period = 65536-period;
    TIMER_H = (char)(period/256);
    TIMER_L = (char)(period -256*TIMER_H);
    TMR0H = TIMER_H;
    TMR0L = TIMER_L;
    T0CONbits.TMR0ON = 1;
    Delay_Ms(duration);
    T0CONbits.TMR0ON = 0;
   
}
//
// This funcon creates seconds delay. The argument specifies the delay me in seconds.
//
void Delay_Seconds(unsigned char s)
{
unsigned char i,j;
for(j = 0; j < s; j++)
{
for(i = 0; i < 100; i++)__delay_ms(10);
}
}
 
                                                                                                                      
 
/*************************************************************************
                       FUNCTION DECLARATIONS
**************************************************************************/
void Delay_us(int us);
void Data(int Value);           
void Cmd(int Value);            
void Send2Lcd(const char Adr, const char *Lcd);
void Myusartinit(); 

/*************************************************************************
                           MAIN FUNCTION
**************************************************************************/
int count=0;

int receive = 0 ; //usart收到的資訊
void main()                     
{
    unsigned char  i;
 /**************設定LED**************/
    TRISC=0X00;                  /* PORTC(control lines) configured as o/p  */
    TRISD=0X00;                  /* PORTD(data lines) configured as o/p     */
    Delay_us(25);              
    Cmd(0X30);                   /* LCD Specification Commands              */
    Delay_us(25);     
    Cmd(0X30);                   /* LCD Specification Commands              */
    Delay_us(25);      
    Cmd(0X30);                   /* LCD Specification Commands              */
    Delay_us(25);             
    Cmd(0X38);                   /* Double Line Display Command        38     */
    Cmd(0X06);                   /* Auto Increment Location Address Command */
    Cmd(0X0C);                   /* Display ON Command                      */
    Cmd(0X01);                   /* Clear Display Command                   */
    Delay_us(1000);              
    Send2Lcd(0x80,"PIC18");  /* Displays string in the first line    rhydoLABZ   0x84*/
    Send2Lcd(0xc0,"Ready To Play");  ;   /* Displays string in the second line   Cochin  0xc5 */
    /*********************設定內部頻率*******************************/
    OSCCONbits.IRCF2= 1;
    OSCCONbits.IRCF1= 1;
    OSCCONbits.IRCF0= 1;
    /************************BUZZER********************************/
    
    TRISC = 0; // Configure PORTC as output
    BUZZER = 0; // Buzzer = 0 to start with
    T0CONbits.TMR0ON = 0; // Timer OFF
    T0CONbits.T08BIT = 0; // Timer in 16 bit mode
    T0CONbits.T0CS = 0; // Use internal clock
    T0CONbits.T0SE = 0; // Low-to-high transion
    T0CONbits.PSA = 0; // Use the prescaler
    T0CONbits.T0PS = 0; // Prescaler = 2
    INTCONbits.TMR0IE = 1; // Enable global and TMR0 interrupts
    /*********************USART*************************************/
    Myusartinit();

    /****************************設定timer**********************************/
    
    RCONbits.IPEN=0x01;//enable priority
    PIE1bits.TMR1IE=1;
    IPR1bits.TMR1IP=1;
    PIR1bits.TMR1IF=0;
    T1CON = 0xB5;//10110101
    INTCONbits.GIE=1;
    TMR1=0x85ED;//65535-62500
    /**********************************************************************/
    
          for(;;) // Endless loop
    {
        if(receive!=0)     
        for(i=0;i<Max_Notes;i++)
           {
            
            Sound_Play(Notes[i], 350 * Durations[i]); // Play the notes
            Delay_Ms(100); // Gap between notes
            }           
            Delay_Seconds(1);//重複播放的時間
    }
}
                                                                                                                   
/*************************************************************************
* Function    : Cmd                                                      *
*                                                                        *
* Description : Function to send a command to LCD                        *
*                                                                        *
* Parameters  : Value - command to be sent                               *
**************************************************************************/
int address1=0X80;
int address2=0XC0;
int mode = 0;
int countout=0;

void interrupt tc_int(void)     // High priority interrupt
{       
    if(TMR1IE && TMR1IF&&receive!=0){
        if(countout==2){
     Cmd(0X01);                   /* Clear Display Command                   */
    Delay_us(1000);       
    Send2Lcd(address1,"PIC18");  /* Displays string in the first line    rhydoLABZ   0x84*/
    Send2Lcd(address2,"Game!");     /* Displays string in the second line   Cochin  0xc5 */
    
    if(count<35&&mode ==0 ){address1++;address2++;}
    else if(count>=0 &&mode ==1){address1--;address2--;}
     count++;
    if(count==35&&mode==0)mode = 1;
    
    if(count==70&&mode==1){mode=0;count=0;}
     
     countout=0;
        
        }else
        {
        countout++;
        }
        
         TMR1IF=0;
        TMR1=0x85ED;
}
    /***Buzzer*****/
 if(TMR0IE && TMR0IF&&receive!=0){
    TMR0H = TIMER_H;
    TMR0L = TIMER_L;
    BUZZER = ~BUZZER;
    INTCONbits.TMR0IF= 0;
    
    }
     /***Usart*****/
    if(RCIF)//如果完成flag會=1
    {
         receive= RCREG;
    }
    
        
 
}

void Cmd(int Value)
{
 PORTD = Value;               /* Write the command to data lines         */
 RC0   = 0;                   /* RS-0(command register)                  */
 RC1   = 1;                   /* E-1(enable)                             */
 Delay_us(25);                
 RC1   = 0;                   /* E-0(enable)                             */
}
 
/*************************************************************************
* Function    : Data                                                     *
*                                                                        *
* Description : Function to display single character on LCD              *
*                                                                        *
* Parameters  : Value - character to be displayed                        *
**************************************************************************/
void Data(int Value)
{
 PORTD = Value;               /* Write the character to data lines       */
 RC0   = 1;                   /* RS-1(data register)                     */
 RC1   = 1;                   /* E-1(enable)                             */
 Delay_us(25);                
 RC1   = 0;                   /* E-0(enable)                             */
}
 
/*************************************************************************
* Function    : Send2LCD                                                 *
*                                                                        *
* Description : Function to display string on LCD                        *
*                                                                        *
* Parameters  : Adr - location                                           *
*               String to be displayed                                   *
**************************************************************************/
void Send2Lcd(const char Adr, const char *Lcd)
{
 Cmd(Adr);                    /* Address of location to display string   */
 while(*Lcd!='\0')            /* Check for termination character         */
 {    
  Data(*Lcd);                 /* Display the character on LCD            */    
  Lcd++;                      /* Increment the pointer                   */
 }
}
 
/*************************************************************************
* Function    : Delay_us                                                 *
*                                                                        *
* Description : Function for 1 microsecond delay                         *
*                                                                        *
* Parameter   : us - delay in microseconds                               *
**************************************************************************/
void Delay_us(int us)
{
 us=us>>1;
  while(us!=1)
 us--;
} 
 
/***************************  初始化USART  ****************************/

void Myusartinit()
{
    
    
    /*portC pin6/7 is mult with usart setting their tris*/    
    TRISCbits.TRISC6 = 1;            // Setting by data sheet PPT p15
    TRISCbits.TRISC7 = 1;            // PPT p15
    
 
    //  Setting baud rate
    BAUDCONbits.BRG16 = 1 ;          //Read Baud rate table
    TXSTAbits.BRGH = 1;
    SPBRG = 207;      //這裡要查表 datasheet P209 關鍵字 18-3 找1MHZ 9.6K 
    
   //   Serial enable P15
    TXSTAbits.SYNC = 0;             //choose the async moode
    RCSTAbits.SPEN = 1;              //open serial port
    
    // Interrutp Setting 設定interrupt
    
    //setting TX/RX 
             
    PIR1bits.RCIF = 0;
  
    RCSTAbits.CREN = 1;             //Enable Rx
    //setting TX/RX interrupt
 
    PIE1bits.RCIE = 1;              //Enable Rx interrupt
    IPR1bits.RCIP = 1;              //Setting Rc as high/low priority interrupt
    
    
    return ;
}

