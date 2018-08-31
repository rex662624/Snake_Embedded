#include <xc.h>
#include <pic18f4520.h>
// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on LATAbits.LATA6 and LATAbits.LATA7)
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
// CONFIG3H
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; LATEbits.LATE3 input pin disabled)
// CONFIG4L
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

int count=0;
int firstdigit=-1;
int gamestart=0;
int point=0;
int pointa;
int pointb;

int a[10]={0x03	,0x9F,0x25,0x0D,0x99,0x49,0xC1,0x1B,0x01,0x19};

void Myusartinit()
{
    
    
    TRISCbits.TRISC6 = 1;            // Setting by data sheet 
    TRISCbits.TRISC7 = 1;            //  
    
    /*
           Serial Setting      
     * 1.   Setting Baud rate
     * 2.   choose sync/async mode 
     * 3.   enable Serial port (configures RX/DT and TX/CK pins as serial port pins)
     * 3.5  enable Tx, Rx Interrupt(optional)
     * 4.   Enable Tx & RX
     */      
    //  Setting baud rate
    BAUDCONbits.BRG16 = 1 ;          //Read Baud rate table
    TXSTAbits.BRGH = 1;
    SPBRG = 25;      
    
   //   Serial enable
    TXSTAbits.SYNC = 0;             //choose the async moode
    RCSTAbits.SPEN = 1;              //open serial port
    
    // Interrutp Setting 
    
    //setting TX/RX
    PIR1bits.TXIF = 0;
    PIR1bits.RCIF = 0;
    TXSTAbits.TXEN = 1;             //Enable Tx
    RCSTAbits.CREN = 1;             //Enable Rx
    //setting TX/RX interrupt
    PIE1bits.TXIE = 0;              //disable Tx interrupt
    IPR1bits.TXIP = 1;              //Setting Tx as high/low priority interrupt
    PIE1bits.RCIE = 1;              //Enable Rx interrupt
    IPR1bits.RCIP = 1;              //Setting Rc as high/low priority interrupt
    
    
    return ;
}
void main(void) {
    
     
    RCONbits.IPEN=0x01;//enable priority
    TRISBbits.TRISB5=0;
    TRISBbits.TRISB4=0;
    TRISBbits.TRISB3=0;
    TRISBbits.TRISB2=0;
    TRISBbits.TRISB1=0;
    TRISBbits.TRISB0=0;
    TRISEbits.TRISE0=0;
    TRISCbits.TRISC5=0;
    TRISCbits.TRISC4=0;
    TRISCbits.TRISC3=0;
    TRISCbits.TRISC2=0;
    TRISCbits.TRISC1=0;
    TRISCbits.TRISC0=1;
    TRISA=0x00;//閮剖?LED
    TRISD=0x00;//閮剖?LED
    LATA=0xFF;
    LATD=0xFF;
    LATCbits.LATC5=1;
    LATCbits.LATC4=1;
    LATCbits.LATC3=1;
    LATCbits.LATC2=1;
    LATCbits.LATC1=1;
    LATBbits.LATB5=1;
    LATBbits.LATB4=1;
    LATBbits.LATB3=1;
    LATBbits.LATB2=1;
    LATBbits.LATB1=1;
    LATBbits.LATB0=1;
    LATEbits.LATE0=1;
    LATCbits.LATC0=0;
    //----------
        LATCbits.LATC5=0;
        LATCbits.LATC4=0;
        LATCbits.LATC3=0;
        LATCbits.LATC2=0;
        LATCbits.LATC1=0;
        LATAbits.LATA0=0;
        LATDbits.LATD0=1;
        
        LATBbits.LATB5=0;
        LATBbits.LATB4=0;
        LATBbits.LATB3=0;
        LATBbits.LATB2=0;
        LATBbits.LATB1=0;
        LATBbits.LATB0=0;
        LATEbits.LATE0=1;
    
    //----------
    PIE1bits.TMR1IE=1;
    IPR1bits.TMR1IP=1;
    PIR1bits.TMR1IF=0;
    Myusartinit();
    T1CON=0x95;//10010101
    INTCONbits.GIE=1;
    TMR1=0xBDB;//65535-62500
    point=0;
    RCIF=0;
    while(1);
}

char g;
int judge=0;
void interrupt tc_int(void)     // High priority interrupt
{
    pointa=point%10;
    pointb=(point/10)%10;
    TXREG = gamestart;
    if(RCIF)//憒?摰?flag??1 
    {
        if(judge==0)
        {  g = RCREG;
            point=1;
            judge=1;
        }
         
        else{
            g = RCREG;
            if(point<88)
                 point++;
            else{
             point = 0;
            }
       }
         
    }
     
    
    
    if(RC0==1){
        gamestart = 1;
    }
    if(gamestart==1){
        
        if(pointa==0){
        LATCbits.LATC5=0;
        LATCbits.LATC4=0;
        LATCbits.LATC3=0;
        LATCbits.LATC2=0;
        LATCbits.LATC1=0;
        LATAbits.LATA0=0;
        LATDbits.LATD0=1;
        }   
        else if(pointa==1){
        LATCbits.LATC5=1;
        LATCbits.LATC4=0;
        LATCbits.LATC3=0;
        LATCbits.LATC2=1;
        LATCbits.LATC1=1;
        LATAbits.LATA0=1;
        LATDbits.LATD0=1;
        }   
        else if(pointa==2){
        LATCbits.LATC5=0;
        LATCbits.LATC4=0;
        LATCbits.LATC3=1;
        LATCbits.LATC2=0;
        LATCbits.LATC1=0;
        LATAbits.LATA0=1;
        LATDbits.LATD0=0;
        }  
        else if(pointa==3){
        LATCbits.LATC5=0;
        LATCbits.LATC4=0;
        LATCbits.LATC3=0;
        LATCbits.LATC2=0;
        LATCbits.LATC1=1;
        LATAbits.LATA0=1;
        LATDbits.LATD0=0;
        }  
        else if(pointa==4){
        LATCbits.LATC5=1;
        LATCbits.LATC4=0;
        LATCbits.LATC3=0;
        LATCbits.LATC2=1;
        LATCbits.LATC1=1;
        LATAbits.LATA0=0;
        LATDbits.LATD0=0;
        }  
        else if(pointa==5){
        LATCbits.LATC5=0;
        LATCbits.LATC4=1;
        LATCbits.LATC3=0;
        LATCbits.LATC2=0;
        LATCbits.LATC1=1;
        LATAbits.LATA0=0;
        LATDbits.LATD0=0;
        }  
        else if(pointa==6){
        LATCbits.LATC5=1;
        LATCbits.LATC4=1;
        LATCbits.LATC3=0;
        LATCbits.LATC2=0;
        LATCbits.LATC1=0;
        LATAbits.LATA0=0;
        LATDbits.LATD0=0;
        }  
        else if(pointa==7){
        LATCbits.LATC5=0;
        LATCbits.LATC4=0;
        LATCbits.LATC3=0;
        LATCbits.LATC2=1;
        LATCbits.LATC1=1;
        LATAbits.LATA0=0;
        LATDbits.LATD0=1;
        }  
        else if(pointa==8){
        LATCbits.LATC5=0;
        LATCbits.LATC4=0;
        LATCbits.LATC3=0;
        LATCbits.LATC2=0;
        LATCbits.LATC1=0;
        LATAbits.LATA0=0;
        LATDbits.LATD0=0;
        }  
        else if(pointa==9){
        LATCbits.LATC5=0;
        LATCbits.LATC4=0;
        LATCbits.LATC3=0;
        LATCbits.LATC2=1;
        LATCbits.LATC1=1;
        LATAbits.LATA0=0;
        LATDbits.LATD0=0;
        }  
        
        
        
        
        if(pointb==0){
        LATBbits.LATB5=0;
        LATBbits.LATB4=0;
        LATBbits.LATB3=0;
        LATBbits.LATB2=0;
        LATBbits.LATB1=0;
        LATBbits.LATB0=0;
        LATEbits.LATE0=1;
        }   
        else if(pointb==1){
        LATBbits.LATB5=1;
        LATBbits.LATB4=0;
        LATBbits.LATB3=0;
        LATBbits.LATB2=1;
        LATBbits.LATB1=1;
        LATBbits.LATB0=1;
        LATEbits.LATE0=1;
        }   
        else if(pointb==2){
        LATBbits.LATB5=0;
        LATBbits.LATB4=0;
        LATBbits.LATB3=1;
        LATBbits.LATB2=0;
        LATBbits.LATB1=0;
        LATBbits.LATB0=1;
        LATEbits.LATE0=0;
        }  
        else if(pointb==3){
        LATBbits.LATB5=0;
        LATBbits.LATB4=0;
        LATBbits.LATB3=0;
        LATBbits.LATB2=0;
        LATBbits.LATB1=1;
        LATBbits.LATB0=1;
        LATEbits.LATE0=0;
        }  
        else if(pointb==4){
        LATBbits.LATB5=1;
        LATBbits.LATB4=0;
        LATBbits.LATB3=0;
        LATBbits.LATB2=1;
        LATBbits.LATB1=1;
        LATBbits.LATB0=0;
        LATEbits.LATE0=0;
        }  
        else if(pointb==5){
        LATBbits.LATB5=0;
        LATBbits.LATB4=1;
        LATBbits.LATB3=0;
        LATBbits.LATB2=0;
        LATBbits.LATB1=1;
        LATBbits.LATB0=0;
        LATEbits.LATE0=0;
        }  
        else if(pointb==6){
        LATBbits.LATB5=1;
        LATBbits.LATB4=1;
        LATBbits.LATB3=0;
        LATBbits.LATB2=0;
        LATBbits.LATB1=0;
        LATBbits.LATB0=0;
        LATEbits.LATE0=0;
        }  
        else if(pointb==7){
        LATBbits.LATB5=0;
        LATBbits.LATB4=0;
        LATBbits.LATB3=0;
        LATBbits.LATB2=1;
        LATBbits.LATB1=1;
        LATBbits.LATB0=0;
        LATEbits.LATE0=1;
        }  
        else if(pointb==8){
        LATBbits.LATB5=0;
        LATBbits.LATB4=0;
        LATBbits.LATB3=0;
        LATBbits.LATB2=0;
        LATBbits.LATB1=0;
        LATBbits.LATB0=0;
        LATEbits.LATE0=0;
        }  
        else if(pointb==9){
        LATBbits.LATB5=0;
        LATBbits.LATB4=0;
        LATBbits.LATB3=0;
        LATBbits.LATB2=1;
        LATBbits.LATB1=1;
        LATBbits.LATB0=0;
        LATEbits.LATE0=0;
        }  
       
        
        
        if(firstdigit==0){
            LATAbits.LATA7=0;
            LATAbits.LATA6=0;
            LATAbits.LATA5=0;
            LATAbits.LATA4=1;
            LATAbits.LATA3=1;
            LATAbits.LATA2=0;
            LATAbits.LATA1=0;
        }
        else if(firstdigit==1){
            LATAbits.LATA7=0;
            LATAbits.LATA6=0;
            LATAbits.LATA5=0;
            LATAbits.LATA4=0;
            LATAbits.LATA3=0;
            LATAbits.LATA2=0;
            LATAbits.LATA1=0;
        }
        else if(firstdigit==2){
            LATAbits.LATA7=0;
            LATAbits.LATA6=0;
            LATAbits.LATA5=0;
            LATAbits.LATA4=1;
            LATAbits.LATA3=1;
            LATAbits.LATA2=0;
            LATAbits.LATA1=1;
        }
        else if(firstdigit==3){
            LATAbits.LATA7=1;
            LATAbits.LATA6=1;
            LATAbits.LATA5=0;
            LATAbits.LATA4=0;
            LATAbits.LATA3=0;
            LATAbits.LATA2=0;
            LATAbits.LATA1=0;
        }
        else if(firstdigit==4){
            LATAbits.LATA7=0;
            LATAbits.LATA6=1;
            LATAbits.LATA5=0;
            LATAbits.LATA4=0;
            LATAbits.LATA3=1;
            LATAbits.LATA2=0;
            LATAbits.LATA1=0;
        }
        else if(firstdigit==5){
            LATAbits.LATA7=1;
            LATAbits.LATA6=0;
            LATAbits.LATA5=0;
            LATAbits.LATA4=1;
            LATAbits.LATA3=1;
            LATAbits.LATA2=0;
            LATAbits.LATA1=0; 
            }
        else if(firstdigit==6){
            LATAbits.LATA7=0;
            LATAbits.LATA6=0;
            LATAbits.LATA5=0;
            LATAbits.LATA4=0;
            LATAbits.LATA3=1;
            LATAbits.LATA2=1;
            LATAbits.LATA1=0;  
            }
        else if(firstdigit==7){
            LATAbits.LATA7=0;
            LATAbits.LATA6=0;
            LATAbits.LATA5=1;
            LATAbits.LATA4=0;
            LATAbits.LATA3=0;
            LATAbits.LATA2=1;
            LATAbits.LATA1=0;  
            }
        else if(firstdigit==8){
            LATAbits.LATA7=1;
            LATAbits.LATA6=0;
            LATAbits.LATA5=0;
            LATAbits.LATA4=1;
            LATAbits.LATA3=1;
            LATAbits.LATA2=1;
            LATAbits.LATA1=1; 
            }
        else if(firstdigit==9){
            LATAbits.LATA7=0;
            LATAbits.LATA6=0;
            LATAbits.LATA5=0;
            LATAbits.LATA4=0;
            LATAbits.LATA3=0;
            LATAbits.LATA2=0;
            LATAbits.LATA1=1;   
            if(count==0){
                gamestart=0;
            }
       }
        else{
            LATAbits.LATA7=1;
            LATAbits.LATA6=1;
            LATAbits.LATA5=1;
            LATAbits.LATA4=1;
            LATAbits.LATA3=1;
            LATAbits.LATA2=1;
            LATAbits.LATA1=1;   
            
        }
            
       
        
        
       
        
        
    if(TMR1IE && TMR1IF){
        
         
    
        if(count==0){
            count++;
            firstdigit++;
        }
        else if(count==1){
            LATDbits.LATD7=0;
            LATDbits.LATD6=0;
            LATDbits.LATD5=0;
            LATDbits.LATD4=1;
            LATDbits.LATD3=1;
            LATDbits.LATD2=0;
            LATDbits.LATD1=0;
            count++;  
        }
        else if(count==2){
            count++;
        }
        else if(count==3){
            LATDbits.LATD7=0;
            LATDbits.LATD6=0;
            LATDbits.LATD5=0;
            LATDbits.LATD4=0;
            LATDbits.LATD3=0;
            LATDbits.LATD2=0;
            LATDbits.LATD1=0;
            count++;
        }
        else if(count==4){
            count++;
        }
        else if(count==5){
            LATDbits.LATD7=0;
            LATDbits.LATD6=0;
            LATDbits.LATD5=0;
            LATDbits.LATD4=1;
            LATDbits.LATD3=1;
            LATDbits.LATD2=0;
            LATDbits.LATD1=1;
            count++;
        }
        else if(count==6){
            count++;
        }
        else if(count==7){
            LATDbits.LATD7=1;
            LATDbits.LATD6=1;
            LATDbits.LATD5=0;
            LATDbits.LATD4=0;
            LATDbits.LATD3=0;
            LATDbits.LATD2=0;
            LATDbits.LATD1=0;
            count++;
        }
        else if(count==8){
            count++;
        }
        else if(count==9){
            LATDbits.LATD7=0;
            LATDbits.LATD6=1;
            LATDbits.LATD5=0;
            LATDbits.LATD4=0;
            LATDbits.LATD3=1;
            LATDbits.LATD2=0;
            LATDbits.LATD1=0;
            count++;
        }
        else if(count==10){
            count++;
        }
        else if(count==11){
            LATDbits.LATD7=1;
            LATDbits.LATD6=0;
            LATDbits.LATD5=0;
            LATDbits.LATD4=1;
            LATDbits.LATD3=1;
            LATDbits.LATD2=0;
            LATDbits.LATD1=0; 
            count++;
        }
        else if(count==12){
            count++;
        }
        else if(count==13){
            LATDbits.LATD7=0;
            LATDbits.LATD6=0;
            LATDbits.LATD5=0;
            LATDbits.LATD4=0;
            LATDbits.LATD3=1;
            LATDbits.LATD2=1;
            LATDbits.LATD1=0;  
            count++;
        }
        else if(count==14){
            count++;
        }
        else if(count==15){
            LATDbits.LATD7=0;
            LATDbits.LATD6=0;
            LATDbits.LATD5=1;
            LATDbits.LATD4=0;
            LATDbits.LATD3=0;
            LATDbits.LATD2=1;
            LATDbits.LATD1=0; 
            count++;
        }
        else if(count==16){
            count++;
        }
        else if(count==17){
            LATDbits.LATD7=1;
            LATDbits.LATD6=0;
            LATDbits.LATD5=0;
            LATDbits.LATD4=1;
            LATDbits.LATD3=1;
            LATDbits.LATD2=1;
            LATDbits.LATD1=1; 
            count++;
        }
        else if(count==18){
            count++;
        }
        else if(count==19){
            LATDbits.LATD7=0;
            LATDbits.LATD6=0;
            LATDbits.LATD5=0;
            LATDbits.LATD4=0;
            LATDbits.LATD3=0;
            LATDbits.LATD2=0;
            LATDbits.LATD1=1;  
            count=0;   
        }  
    }
        TMR1IF=0;
        TMR1=0xBDB;
        
    }
    else{
        count=0;
        TMR1IF=0;
        TMR1=0xBDB;
        firstdigit=-1;
        LATA=0xFF;
        LATD=0xFF;
        LATCbits.LATC5=1;
        LATCbits.LATC4=1;
        LATCbits.LATC3=1;
        LATCbits.LATC2=1;
        LATCbits.LATC1=1;
        LATCbits.LATC0=0;
        LATBbits.LATB5=1;
        LATBbits.LATB4=1;
        LATBbits.LATB3=1;
        LATBbits.LATB2=1;
        LATBbits.LATB1=1;
        LATBbits.LATB0=1;
        LATEbits.LATE0=1;
        
    }
    
    
    
}
      