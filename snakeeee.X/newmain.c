#include <xc.h>
#include <pic18f4520.h>
#include<stdio.h>
#include<stdlib.h>
#include <time.h>

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on LATAbits.LATA6 and LATAbits.LATA7)
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; LATEbits.LATE3 input pin disabled)
// CONFIG4L
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

int head[6][7];
int direction=1;
int i,j;
int state = 0;//1:running 2:game over 0:ready

int foodx=2,foody=2;
int score=0;
//呼吸燈變數
int count=0;
int duty =10;
int changemode = 0;
int mode=0 ; 

void my_pwm(int );//呼吸燈
//usart
void Myusartwrite(char);
void MyusartRead();
void Myusartinit();
void ISR_Init();
int receive=0;

void interrupt tc_int(void)             // High priority interrupt
{   
    
    
    if(RCIF)//如果完成flag會=1
    {
        //receive= RCREG;
        MyusartRead();
        if(receive!=0)state=1;
        
    }
    
     if(TMR2IE && TMR2IF&&state==2){
        
         my_pwm(duty);//10代表100% 9代表90%...... 
        
        
        //時間到了就要改變亮度 讓他有呼吸燈的感覺
        changemode++;//到某個時間後要變換亮度了
        
        if(changemode==100){
            
            if(mode==1)//變亮的狀態
                duty++;
            else    //變暗的狀態*/
                duty--;
            
            changemode=0;    
            
        }
        
        if(mode==1&&duty==11)//如果加到100%了 要變暗
            mode=0;
        
        if(mode==0&&duty==-1)//如果減到0%了 要變亮
        { 
            mode=1;
            
        }
        TMR2IF=0;//flag要記得設為0
         
         
    }
           
        
    if(TMR1IE && TMR1IF&&state==1){
       
        if(state==1){
         
        if(direction==1){
            j++;
            head[i][j]=1;
            head[i][j-1]=0;
            
        }
        else if(direction==2){
            j--;
            head[i][j]=1;
            head[i][j+1]=0;
        }
        else if(direction==3){
            i--;
            head[i][j]=1;
            head[i+1][j]=0;
        }
        else if(direction==4){
            i++;
            head[i][j]=1;
            head[i-1][j]=0;
        }
        //-----------------------------food
           if(foodx==i&&foody==j)//吃到了 分數+1並改變food位置
     {
        TXREG = '1';
         score++;
        
         foodx =(rand()%4)+1;
         
         foody =(rand()%5)+1;
         
         if(foodx==i&&foody==j){//食物與蛇位置重疊
             foodx=((foodx+1)%4)+1;
             foody=((foody+1)%5)+1;
            
         }
     }
     //--------
   
     //--------
        
        head[foodx][foody]=1;
        
      
     
        
        //-----------------------------
        LATAbits.LATA5=head[1][1];
        LATEbits.LATE0=head[1][2];
        LATEbits.LATE1=head[1][3];
        LATEbits.LATE2=head[1][4];
        LATAbits.LATA7=head[1][5];
        LATAbits.LATA0=head[2][1];
        LATAbits.LATA1=head[2][2];
        LATAbits.LATA2=head[2][3];
        LATAbits.LATA3=head[2][4];
        LATAbits.LATA4=head[2][5];
        LATCbits.LATC0=head[3][1];
        LATCbits.LATC1=head[3][2];
        LATCbits.LATC2=head[3][3];
        LATCbits.LATC3=head[3][4];
        LATCbits.LATC4=head[3][5];
        LATDbits.LATD0=head[4][1];
        LATDbits.LATD1=head[4][2];
        LATDbits.LATD2=head[4][3];
        LATDbits.LATD3=head[4][4];
        LATDbits.LATD4=head[4][5];
        }
        else if(state==2)//lose
        {
     
       
        
        }
        
        TMR1IF=0;
        TMR1=0xBDB;
        
        
    }
    
    
    
}


void main(void) {
    time_t t;
    srand(&t);
    
    TRISD = 0x00;
    TRISA=0x00;
    TRISC=0x00;
    TRISE=0x00;
    TRISBbits.TRISB0=1;
    TRISBbits.TRISB1=1;
    TRISBbits.TRISB2=1;
    TRISBbits.TRISB3=1;
    
    LATD=0x00;
    LATC=0x00;
    LATA=0x00;
    LATE=0x00;

    //設定遊戲快慢
    
    OSCCONbits.IRCF2=0;
    OSCCONbits.IRCF1=1;
    OSCCONbits.IRCF0=1;
    //timer1
    RCONbits.IPEN=0x01;//enable priority
    PIE1bits.TMR1IE=1;
    IPR1bits.TMR1IP=1;
    PIR1bits.TMR1IF=0;
    T1CON=0x95;//10010101
    TMR1=0xBDB;//65535-62500
    //timer2
    /*
    PIE1bits.TMR2IE=1;         //TMR2的enable,flag,priority
   //IPR1bits.TMR2IP=1;
    PIR1bits.TMR2IF=0;*/
    PR2 = 1;
    T2CON=0x04;
    
    //usart
    Myusartinit();
    ISR_Init();

    
    
    
    INTCONbits.GIE=1;
    
    
    
    i=1;
    j=1;
    head[i][j]=1;
    head[foodx][foody]=1;
    LATAbits.LATA5 = head[i][j];//START
    LATAbits.LATA1 = 1;
 
    while(1){
        
     if(RB0==0){
           direction=4;
            
        }
        
      else if(RB1==0){
         direction=1;
            
    }
     else  if(RB2==0){
          direction=2;
         }
    else if(RB3==0){
          direction=3;
           
        }
     
     if(j==0||j==6||i==0||i==5){
     
         state=2;
         //死掉了換timer2動作
        // TXSTAbits.TXEN = 0;             //Enable Tx
         RCSTAbits.SPEN = 0;
         
         PIE1bits.TMR2IE=1;         //TMR2的enable,flag,priority
         IPR1bits.TMR2IP=1;
         PIR1bits.TMR2IF=0;
     
     }
     
     
    
     
    
    }
    
}

void my_pwm(int duty){
    
        if(count<duty){//count本來是0 要一直加 加到duty cycle後都是0
        LATAbits.LATA5=1;
        LATEbits.LATE0=1;
        LATEbits.LATE1=1;
        LATEbits.LATE2=1;
        LATAbits.LATA7=1;
        LATAbits.LATA0=1;
        LATAbits.LATA1=1;
        LATAbits.LATA2=1;
        LATAbits.LATA3=1;
        LATAbits.LATA4=1;
        LATCbits.LATC0=1;
        LATCbits.LATC1=1;
        LATCbits.LATC2=1;
        LATCbits.LATC3=1;
        LATCbits.LATC4=1;
        LATDbits.LATD0=1;
        LATDbits.LATD1=1;
        LATDbits.LATD2=1;
        LATDbits.LATD3=1;
        LATDbits.LATD4=1;
        }
        else{
        LATAbits.LATA5=0;
        LATEbits.LATE0=0;
        LATEbits.LATE1=0;
        LATEbits.LATE2=0;
        LATAbits.LATA7=0;
        LATAbits.LATA0=0;
        LATAbits.LATA1=0;
        LATAbits.LATA2=0;
        LATAbits.LATA3=0;
        LATAbits.LATA4=0;
        LATCbits.LATC0=0;
        LATCbits.LATC1=0;
        LATCbits.LATC2=0;
        LATCbits.LATC3=0;
        LATCbits.LATC4=0;
        LATDbits.LATD0=0;
        LATDbits.LATD1=0;
        LATDbits.LATD2=0;
        LATDbits.LATD3=0;
        LATDbits.LATD4=0;
           
        } 
        
       count++;
        
        if(count>=10){//全部以100%為基準 10代表100%
            count = 0;
        } 

}

//usart相關

void ISR_Init()
{
     RCONbits.IPEN = 1;      //enable Interrupt Priority mode
    INTCONbits.GIEH = 1;    //enable high priority interrupt
    INTCONbits.GIEL = 0;     //enable low priority interrupt
    
            
    return ;
}

void MyusartRead()
{
   receive= RCREG; //讀RCREG 就會進來了

    
    return ;
}


void Myusartinit()
{
    
    
    /*portC pin6/7 is mult with usart setting their tris*/    
    TRISCbits.TRISC6 = 1;            // Setting by data sheet PPT p15
    TRISCbits.TRISC7 = 1;            // PPT p15
    
    //  Setting baud rate
    BAUDCONbits.BRG16 = 1 ;          //Read Baud rate table
    TXSTAbits.BRGH = 1;
    SPBRG = 12;      //這裡要查表 datasheet P209 關鍵字 18-3 找1MHZ 9.6K 
    
   //   Serial enable P15
    TXSTAbits.SYNC = 0;             //choose the async moode
    RCSTAbits.SPEN = 1;              //open serial port
    
    // Interrutp Setting 設定interrupt
    
    //setting TX/RX 
    PIR1bits.TXIF = 0;              
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;             //Enable Tx
    RCSTAbits.CREN = 1;             //Enable Rx
    //setting TX/RX interrupt
    PIE1bits.TXIE = 0;              //disable Tx interrupt 因為他是在Read裡面read完直接write
    IPR1bits.TXIP = 1;              //Setting Tx as high/low priority interrupt
    PIE1bits.RCIE = 1;              //Enable Rx interrupt
    IPR1bits.RCIP = 1;              //Setting Rc as high/low priority interrupt
    
    
    return ;
}