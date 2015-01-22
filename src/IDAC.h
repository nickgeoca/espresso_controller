
#ifndef __SI32_IDAC_A_REGISTERS_H__
#define __SI32_IDAC_A_REGISTERS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct SI32_IDAC_A_CONTROL_Struct
{
   union
   {
      struct
      {
         // Output Update Trigger
         volatile uint32_t OUPDT: 3;
         // Edge Trigger Source Select
         volatile uint32_t ETRIG: 3;
         // Output Mode
         volatile uint32_t OUTMD: 2;
         // Data Input Format
         volatile uint32_t INFMT: 2;
         // DMA Run
         volatile uint32_t DMARUN: 1;
         // Data Justification Select
         volatile uint32_t JSEL: 1;
         // Data Buffer Reset
         volatile uint32_t BUFRESET: 1;
         // Trigger Source Inhibit
         volatile uint32_t TRIGINH: 1;
                  uint32_t reserved0: 2;
         // Wrap Mode Enable
         volatile uint32_t WRAPEN: 1;
                  uint32_t reserved1: 3;
         // FIFO Overrun Interrupt Enable
         volatile uint32_t ORIEN: 1;
         // FIFO Underrun Interrupt Enable
         volatile uint32_t URIEN: 1;
         // FIFO Went Empty Interrupt Enable
         volatile uint32_t WEIEN: 1;
                  uint32_t reserved2: 6;
         // IDAC Debug Mode
         volatile uint32_t DBGMD: 1;
         // Load Resistor Enable
         volatile uint32_t LOADEN: 1;
         // IDAC Enable
         volatile uint32_t IDACEN: 1;
      };
      volatile uint32_t U32;
   };
};



struct SI32_IDAC_A_DATA_Struct
{
   union
   {
      struct
      {
         // Output Data
         volatile uint32_t DATA_BITS;
      };
      volatile uint32_t U32;
   };
};

struct SI32_IDAC_A_BUFSTATUS_Struct
{
   union
   {
      struct
      {
         // FIFO Level
         volatile uint32_t LEVEL: 3;
                  uint32_t reserved0: 1;
         // FIFO Overrun Interrupt Flag
         volatile uint32_t ORI: 1;
         // FIFO Underrun Interrupt Flag
         volatile uint32_t URI: 1;
         // FIFO Went Empty Interrupt Flag
         volatile uint32_t WEI: 1;
                  uint32_t reserved1: 25;
      };
      volatile uint32_t U32;
   };
};


struct SI32_IDAC_A_BUFFER10_Struct
{
   union
   {
      struct
      {
         // FIFO Buffer Entry 0
         volatile uint16_t BUFFER0;
         // FIFO Buffer Entry 1
         volatile uint16_t BUFFER1;
      };
      volatile uint32_t U32;
   };
};



struct SI32_IDAC_A_BUFFER32_Struct
{
   union
   {
      struct
      {
         // FIFO Buffer Entry 2
         volatile uint16_t BUFFER2;
         // FIFO Buffer Entry 3
         volatile uint16_t BUFFER3;
      };
      volatile uint32_t U32;
   };
};


struct SI32_IDAC_A_GAINADJ_Struct
{
   union
   {
      struct
      {
         // Output Current Gain Adjust
         volatile uint32_t GAINADJ_BITS: 5;
                  uint32_t reserved0: 27;
      };
      volatile uint32_t U32;
   };
};


typedef struct SI32_IDAC_A_Struct
{
   struct SI32_IDAC_A_CONTROL_Struct               CONTROL        ; // Base Address + 0x0
   volatile uint32_t                               CONTROL_SET;
   volatile uint32_t                               CONTROL_CLR;
   uint32_t                                        reserved0;
   struct SI32_IDAC_A_DATA_Struct                  DATA           ; // Base Address + 0x10
   uint32_t                                        reserved1;
   uint32_t                                        reserved2;
   uint32_t                                        reserved3;
   struct SI32_IDAC_A_BUFSTATUS_Struct             BUFSTATUS      ; // Base Address + 0x20
   volatile uint32_t                               BUFSTATUS_SET;
   volatile uint32_t                               BUFSTATUS_CLR;
   uint32_t                                        reserved4;
   struct SI32_IDAC_A_BUFFER10_Struct              BUFFER10       ; // Base Address + 0x30
   uint32_t                                        reserved5;
   uint32_t                                        reserved6;
   uint32_t                                        reserved7;
   struct SI32_IDAC_A_BUFFER32_Struct              BUFFER32       ; // Base Address + 0x40
   uint32_t                                        reserved8;
   uint32_t                                        reserved9;
   uint32_t                                        reserved10;
   struct SI32_IDAC_A_GAINADJ_Struct               GAINADJ        ; // Base Address + 0x50
   uint32_t                                        reserved11;
   uint32_t                                        reserved12;
   uint32_t                                        reserved13;
} SI32_IDAC_A_Type;

#ifdef __cplusplus
}
#endif

// DACs
#define SI32_IDAC_0 ((SI32_IDAC_A_Type*)0x40031000)
#define SI32_IDAC_1 ((SI32_IDAC_A_Type*)0x40032000)


#endif // __SI32_IDAC_A_REGISTERS_H__

//-eof--------------------------------------------------------------------------


