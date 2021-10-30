// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC (1MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)
// CONFIG2
#pragma config MCLRE = ON    // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = OFF   // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON    // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO     // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF     // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = ON  // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON   // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)
// CONFIG3
#pragma config WDTCPS = WDTCPS_31 // WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF         // WDT operating mode (WDT enabled regardless of sleep; SWDTEN ignored)
#pragma config WDTCWS = WDTCWS_7  // WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC        // WDT input clock selector (Software Control)
// CONFIG4
#pragma config WRT = OFF         // UserNVM self-write protection bits (Write protection off)
#pragma config SCANE = available // Scanner Enable bit (Scanner module is available for use)
#pragma config LVP = ON          // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.)
// CONFIG5
#pragma config CP = OFF  // UserNVM Program memory code protection bit (Program Memory code protection disabled)
#pragma config CPD = OFF // DataNVM code protection bit (Data EEPROM code protection disabled)

#define _XTAL_FREQ 32000000

#include <xc.h>
#include <stdio.h>

//ロードセル用ADコンバータ用の設定
#define CLK LATCbits.LATC3
#define DAT PORTCbits.RC4

signed long weight_dat;  //ADCから読み込んだカウント値を入れる
signed long weight_zero; //秤に何も載せないときのカウント値

long scale_count_value(int n)
{
    CLK = 0;
    long weight_count = 0; //読み込んだカウンタ値を入れる
    long weight_add = 0;   //カウンタ値の積算値を入れる
    for (int j = 0; j < n; j++)
    {
        while (DAT == 0)
            ;
        while (DAT == 1)
            ;
        __delay_us(10);
        for (int i = 0; i < 24; i++)
        { //24ビット分読み込む
            weight_count <<= 1;
            CLK = 1;
            __delay_us(10);
            CLK = 0;
            weight_count += DAT;
            __delay_us(10);
        }
        for (int i = 0; i < 1; i++)
        { //i=1:ChAGain128　2:ChBGain32　3:ChAGain64
            CLK = 1;
            __delay_us(10);
            CLK = 0;
            __delay_us(10);
        }
        weight_add += weight_count;
        weight_count = 0;
    }
    weight_count = weight_add / n;
    return weight_count;
}

float scale_convert_gram(signed long count)
{ //ADCのカウント値をグラムに変換
    float temp = count - weight_zero;
    temp = temp / 3035;
    return temp;
}

void main(void)
{
    //初期設定
    TRISA = 0b00000001; //インプットは1　　RA0,RA1は未使用
    ANSELA = 0b00000000;
    TRISB = 0b00111110;
    ANSELB = 0b00000000;
    TRISC = 0b10010100; //RC0,RC1は未使用、RC2は1PPS用入力、(RC3,RC4はI2C)、RC5はブザー、RC6,RC7はEUSART、RC7はRXで入力設定
    //ロードセル用ADコンバータと通信するようにRC3クロック出力,RC4はデータ入力
    ANSELC = 0b00000000; //EISARTのRXピンのANSELは「0」
    //ODCONC=0xFF;

    weight_zero = scale_count_value(50); //0グラム時のカウンタ値を読み込む(50回平均)

    while (1)
    {
        gps_read();

        //秤で重さ計測
        weight_dat = scale_count_value(8);                  //重さを計測しカウンタ値を読み込む(8回平均)。平均値を増やせば精度も高くなると思うけどGPS時計が1秒ごとに動いているので、1秒未満となる8回にした。
        float weight_gram = scale_convert_gram(weight_dat); //カウンタ値を重さのグラムに変換
        printf(" %3.2fg         ", weight_gram);
        if (sw1 == 1)
        { //SW1が押されたら、ゼロ校正を行う
            printf("Zero calibration");
            weight_zero = scale_count_value(50);
            sw1 = 0;
        }
        //秤で重さ計測ここまで↑↑↑
    }
}