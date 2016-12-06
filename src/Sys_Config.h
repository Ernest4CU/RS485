#ifndef SRC_JLCT_CONFIG_H_
#define SRC_JLCT_CONFIG_H_

#define ROTATE_LEFT(x, s, n) ((x) << (n)) | ((x) >> ((s) - (n)))	//Ñ­»·×óÒÆ
#define ROTATE_RIGHT(x, s, n) ((x) >> (n)) | ((x) << ((s) - (n)))	//Ñ­»·ÓÒÒÆ

#define CPU_F ((double)8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define FlashA_ADR 0x1080
#define FlashB_ADR 0x1000

#endif /* SRC_JLCT_CONFIG_H_ */
