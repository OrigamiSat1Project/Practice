/// Prolougue

//  OrigamiProjectメンバーにコーディング規約を定めるためのプログラムファイルです．
//  これを参考にして他人が読みやすく，修正しやすいコーディングを行いましょう．
//  Author      :   reo kashiyama
//  Reference   :   (プログラム言語Cの推奨されるスタイルとコーディング技術)[https://www.gfd-dennou.org/arch/comptech/cstyle/cstyle-ja.htm]
//              :   (組み込みソフトウェア開発向けコ－ディング作法ガイド[C言語版])[http://www.ipa.go.jp/files/000005123.pdf]

/// Include files
#include <stdio.h>
#include <math.h>
#include "I2C.h"
#include "OrigamiTypedef.h"

/// config setting (only PIC)
#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = DIG    // SOSC Power Selection and mode Configuration bits (Digital (SCLKI) mode)
#pragma config XINST = OFF      // Extended Instruction Set (Disabled)

// CONFIG1H
#pragma config FOSC = INTIO2    // Oscillator (Internal RC oscillator)
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)

/// Macro, Enum
#define _XTAL_FREQ    16000000   // PICの動作周波数値
#define UHF_BAND 428.34
#define VHF_BAND 144.13
#define UINT_TRUE 1
#define UINT_FALSE 0

enum SequenceFlow{
    InitialBandSetting,
    WaitUplink,
    SaveToEEPROM,
    AckToOBC,
    Warning,
    Emergency
}

/// Global data
const WAIT_TIME_FOR_REBOOT = 50000;
const ULONG MAX_MEMORY_REGION = 777;
const UBYTE MEMORY_ADDRESS    = 0xb6;
static UINT flowIndex = 0;
static UINT isSuccess = UINT_FALSE;
static UINT WarnigLoopCount = 0;
UINT HogeHoge = 0;

/// Method
void setBandOfTransceiver(double, double);
void SaveToEEPROM(UBYTE, UBYTE[]);

/// Main Program
void int main(void) {
    flowIndex = (UINT)SequenceFlow.InitialBandSetting;
    while (UINT_TRUE) {
        switch ((SequenceFlow)flowIndex) {
            case InitialBandSetting:
                isSuccess = setBandOfTransceiver;
                if (isSuccess == UINT_TRUE) {
                    flowIndex = (UINT)SequenceFlow.WaitUplink;
                }else{
                    flowIndex = (UINT)Warning;
                }
                break;
            case WaitUplink:
                flowIndex = (UINT)SequenceFlow.SaveToEEPROM;
                break;
            case SaveToEEPROM:
                flowIndex = (UINT)SequenceFlow.AckToOBC;
                break;
            case AckToOBC:
                flowIndex = (UINT)SequenceFlow.WaitUplink;
                break;
            case Warning:
                WarnigLoopCount ++;
                if (WarningLoopCount >= 3) {
                    flowIndex = (UINT)SequenceFlow.Emergency;
                }
                break;
            case Emergency:
                isSuccess = rebootPowerOfBus();
                if (isSuccess == UINT_TRUE) {
                    flowIndex = (UINT)SequenceFlow.InitialBandSetting;
                }else{
                    flowIndex = (UINT)SequenceFlow.Warning;
                }
                break;
        }
    }

    return;
}

/*
 *	set band of transceiver, then flow move on
 *	arg      :   Uplink band, Downklink band
 *	return   :   failure -> false, success -> true
 *	TODO     :   setting band
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
UINT setBandOfTransceiver(double _uplink_band, double _downlink_band){
    if(flowIndex != (UINT)SequenceFlow.InitialBandSetting) {
        return UINT_FALSE;
    }

    return UINT_TRUE;
}

/*
 *	if loop count too much, reboot bus power
 *	arg      :   void
 *	return   :   failure -> false, success -> true
 *	TODO     :   switch bus power
 *	FIXME    :   not yet
 *	XXX      :   not yet
 */
UINT rebootPowerOfBus(void){
    if(flowIndex != (UINT)SequenceFlow.Emergency) {
        return UINT_FALSE;
    }

    __delay_ms(WAIT_TIME_FOR_REBOOT);
    return UINT_TRUE;
}
