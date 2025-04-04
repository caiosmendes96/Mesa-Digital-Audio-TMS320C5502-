
#include "stdio.h"
#include "ezdsp5502.h"
#include "ezdsp5502_mcbsp.h"
#include "csl_mcbsp.h"
#define AIC3204_I2C_ADDR 0x18
#include "ezdsp5502_i2c.h"
#include "ezdsp5502_i2cgpio.h"

#include <stdint.h>
#include <math.h>

Int16 AIC3204_rget(  Uint16 regnum, Uint16* regval )
{
    Int16  retcode = 0;
    Uint16 cmd[2];

    cmd[0] = regnum & 0x007F;       // 7-bit Device Register
    cmd[1] = 0;

    /* Send AIC3204 register name */
    retcode |= EZDSP5502_I2C_write( AIC3204_I2C_ADDR, cmd, 1 );

    /* Return AIC3204 register value */
    retcode |= EZDSP5502_I2C_read( AIC3204_I2C_ADDR, cmd, 1 );
    *regval = cmd[0];
    EZDSP5502_waitusec( 50 );

    return retcode;
}

Int16 AIC3204_rset( Uint16 regnum, Uint16 regval )
{
    Uint16 cmd[2];
    cmd[0] = regnum & 0x007F;       // 7-bit Device Register
    cmd[1] = regval;                // 8-bit Register Data

    EZDSP5502_waitusec( 100 );

    return EZDSP5502_I2C_write( AIC3204_I2C_ADDR, cmd, 2 );
}



void inputAic3204Config(Uint32 sf){

    /* Set to McBSP1 mode */
    EZDSP5502_I2CGPIO_configLine( BSP_SEL1, OUT );
    EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1, LOW );

    /* Enable McBSP1 */
    EZDSP5502_I2CGPIO_configLine( BSP_SEL1_ENn, OUT );
    EZDSP5502_I2CGPIO_writeLine(  BSP_SEL1_ENn, LOW );
    Uint16 reg5;

        // Determine P value and set R value = 1
        switch (sf)
        {
            case 8000:
                reg5 = (6<<4)|0x01;
                break;
            case 12000:
                reg5 = (4<<4)|0x01;
                break;
            case 16000:
                reg5 = (3<<4)|0x01;
                break;
            case 24000:
                reg5 = (2<<4)|0x01;
                break;
            case 48000:
                default:
                reg5 = (1<<4)|0x01;
                break;
        }
        reg5 |= 0x80;              // Enable PLL for desired AIC3204 sampling frequency

    EZDSP5502_wait( 100 );

    /* ---------------------------------------------------------------- *
        *  Configure AIC3204                                               *
        * ---------------------------------------------------------------- */
       AIC3204_rset( 0, 0 );      // Select page 0
       AIC3204_rset( 1, 1 );      // Reset codec
       AIC3204_rset( 0, 1 );      // Select page 1
       AIC3204_rset( 1, 8 );      // Disable crude AVDD generation from DVDD
       AIC3204_rset( 2, 1 );      // Enable Analog Blocks, use LDO power
       AIC3204_rset( 0, 0 );

       /* PLL and Clocks config and Power Up  */
       AIC3204_rset( 27, 0x0d );  // BCLK and WCLK are set as o/p; AIC3204(Master)
       AIC3204_rset( 28, 0x00 );  // Data ofset = 0
       AIC3204_rset( 4, 3 );      // PLL setting: PLLCLK <- MCLK, CODEC_CLKIN <-PLL CLK // TESTE
       AIC3204_rset( 6, 7 );      // PLL setting: J=7
       AIC3204_rset( 7, 0x06 );   // PLL setting: HI_BYTE(D=1680)
       AIC3204_rset( 8, 0x90 );   // PLL setting: LO_BYTE(D=1680)
       AIC3204_rset( 30, 0x9C );  // For 32 bit clocks per frame in Master mode ONLY            // BCLK=DAC_CLK/N =(12288000/8) = 1.536MHz = 32*fs
       AIC3204_rset( 5, reg5 );   // PLL setting: Power up PLL, R=1, P is determined from sampling freq.
       AIC3204_rset( 13, 0 );     // Hi_Byte(DOSR) for DOSR = 128 decimal or 0x0080 DAC oversamppling
       AIC3204_rset( 14, 0x80 );  // Lo_Byte(DOSR) for DOSR = 128 decimal or 0x0080
       AIC3204_rset( 20, 0x80 );  // AOSR for AOSR = 128 decimal or 0x0080 for decimation filters 1 to 6
       AIC3204_rset( 11, 0x82 );  // Power up NDAC and set NDAC value to 2
       AIC3204_rset( 12, 0x87 );  // Power up MDAC and set MDAC value to 7
       AIC3204_rset( 18, 0x87 );  // Power up NADC and set NADC value to 7
       AIC3204_rset( 19, 0x82 );  // Power up MADC and set MADC value to 2

       /* DAC ROUTING and Power Up */
       AIC3204_rset( 0, 1 );      // Select page 1
       AIC3204_rset( 0x0c, 8 );   // LDAC AFIR routed to HPL
       AIC3204_rset( 0x0d, 8 );   // RDAC AFIR routed to HPR
       AIC3204_rset( 0, 0 );      // Select page 0
       AIC3204_rset( 64, 2 );     // Left vol=right vol
       AIC3204_rset( 65, 0);      // Left DAC gain to 0dB VOL; Right tracks Left
       AIC3204_rset( 63, 0xd4 );  // Power up left,right data paths and set channel
       AIC3204_rset( 0, 1 );      // Select page 1
       AIC3204_rset( 9, 0x30 );   // Power up HPL,HPR
       AIC3204_rset( 0x10, 0x00 );// Unmute HPL , 0dB gain
       AIC3204_rset( 0x11, 0x00 );// Unmute HPR , 0dB gain
       AIC3204_rset( 0, 0 );      // Select page 0
       EZDSP5502_waitusec( 1000000 ); // wait

       /* ADC ROUTING and Power Up */
       AIC3204_rset( 0, 1 );      // Select page 1
       AIC3204_rset( 0x34, 0x30 );// STEREO 1 Jack
                                  // IN2_L to LADC_P through 40 kohm
       AIC3204_rset( 0x37, 0x30 );// IN2_R to RADC_P through 40 kohmm
       AIC3204_rset( 0x36, 3 );   // CM_1 (common mode) to LADC_M through 40 kohm
       AIC3204_rset( 0x39, 0xc0 );// CM_1 (common mode) to RADC_M through 40 kohm
       AIC3204_rset( 0x3b, 50 );   // MIC_PGA_L unmute
       AIC3204_rset( 0x3c, 50 );   // MIC_PGA_R unmute
       AIC3204_rset( 51, 0x48) ; // Aumente a polarização do MIC usando o LDO interno TESTEEE
       AIC3204_rset( 59, 0x3c); // Ative o MICPGA esquerdo, ganho = 30 dB TESTEEE
       AIC3204_rset( 60, 0x3c); // Ative o MICPGA direito, ganho = 30 dB TESTEEE
       AIC3204_rset( 0, 0 );      // Select page 0
       AIC3204_rset( 0x51, 0xc0 );// Powerup Left and Right ADC

       AIC3204_rset( 0x52, 0 );   // Unmute Left and Right ADC
       AIC3204_rset( 0, 0 );
       EZDSP5502_waitusec( 200 ); // Wait

}

void initAic3204(){
    /* Initialize McBSP */
    EZDSP5502_MCBSP_init( );
}
void disableAic3204(){
    EZDSP5502_MCBSP_close();// Disable McBSP
}
