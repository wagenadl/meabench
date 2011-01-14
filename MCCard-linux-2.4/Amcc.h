// AMCC.h 
// 
/****************************************************************************/ 
/*   PCI Configuration Space Registers                                      */ 
/****************************************************************************/ 
 
#define PCI_CS_VENDOR_ID         0x00 
#define PCI_CS_DEVICE_ID         0x02 
#define PCI_CS_COMMAND           0x04 
#define PCI_CS_STATUS            0x06 
#define PCI_CS_REVISION_ID       0x08 
#define PCI_CS_CLASS_CODE        0x09 
#define PCI_CS_CACHE_LINE_SIZE   0x0c 
#define PCI_CS_MASTER_LATENCY    0x0d 
#define PCI_CS_HEADER_TYPE       0x0e 
#define PCI_CS_BIST              0x0f 
#define PCI_CS_BASE_ADDRESS_0    0x10 
#define PCI_CS_BASE_ADDRESS_1    0x14 
#define PCI_CS_BASE_ADDRESS_2    0x18 
#define PCI_CS_BASE_ADDRESS_3    0x1c 
#define PCI_CS_BASE_ADDRESS_4    0x20 
#define PCI_CS_BASE_ADDRESS_5    0x24 
#define PCI_CS_EXPANSION_ROM     0x30 
#define PCI_CS_INTERRUPT_LINE    0x3c 
#define PCI_CS_INTERRUPT_PIN     0x3d 
#define PCI_CS_MIN_GNT           0x3e 
#define PCI_CS_MAX_LAT           0x3f 
 
 
/****************************************************************************/ 
/*   AMCC Operation Register Offsets                                        */ 
/****************************************************************************/ 
 
#define AMCC_OP_REG_OMB1         0x00 
#define AMCC_OP_REG_OMB2         0x04 
#define AMCC_OP_REG_OMB3         0x08  
#define AMCC_OP_REG_OMB4         0x0c 
#define AMCC_OP_REG_IMB1         0x10 
#define AMCC_OP_REG_IMB2         0x14 
#define AMCC_OP_REG_IMB3         0x18  
#define AMCC_OP_REG_IMB4         0x1c 
#define AMCC_OP_REG_FIFO         0x20 
#define AMCC_OP_REG_MWAR         0x24 
#define AMCC_OP_REG_MWTC         0x28 
#define AMCC_OP_REG_MRAR         0x2c 
#define AMCC_OP_REG_MRTC         0x30 
#define AMCC_OP_REG_MBEF         0x34 
#define AMCC_OP_REG_INTCSR       0x38 
#define AMCC_OP_REG_MCSR         0x3c 
#define AMCC_OP_REG_MCSR_NVDATA  (AMCC_OP_REG_MCSR + 2) /* Data in byte 2 */ 
#define AMCC_OP_REG_MCSR_NVCMD   (AMCC_OP_REG_MCSR + 3) /* +3Command in byte 3 */ 
 
#define AMCC_OP_REG_AIMB1         0x00 
#define AMCC_OP_REG_AIMB2         0x04 
#define AMCC_OP_REG_AIMB3         0x08  
#define AMCC_OP_REG_AIMB4         0x0C 
#define AMCC_OP_REG_AOMB1         0x10 
#define AMCC_OP_REG_AOMB2         0x14 
#define AMCC_OP_REG_AOMB3         0x18  
#define AMCC_OP_REG_AOMB4         0x1C 
#define AMCC_OP_REG_AFIFO         0x20 
#define AMCC_OP_REG_AMWAR         0x24 
#define AMCC_OP_REG_APTA          0x28 
#define AMCC_OP_REG_APTD          0x2C 
#define AMCC_OP_REG_AMRAR         0x30 
#define AMCC_OP_REG_AMBEF         0x34 
#define AMCC_OP_REG_AINT          0x38 
#define AMCC_OP_REG_AGCSTS        0x3C 
#define AMCC_OP_REG_AMWTC         0x58 
#define AMCC_OP_REG_AMRTC         0x5C 
 
 
 
 
/****************************************************************/ 
/*  A2P = Add-on to PCI FIFO												 */ 
/*  P2A = PCI to Add-on FIFO												 */ 
/****************************************************************/ 
 
/* Interrupt Enables */ 
#define EN_READ_TC_INT		0x00008000L 
#define EN_WRITE_TC_INT		0x00004000L 
 
/* FIFO Flag Reset */ 
#define RESET_A2P_FLAGS		0x04000000L 
#define RESET_P2A_FLAGS		0x02000000L 
 
/* FIFO Management Scheme */ 
#define A2P_REQ_AT_4FULL	0x00000200L 
#define P2A_REQ_AT_4EMPTY	0x00002000L 
 
/* FIFO Relative Priority */ 
#define A2P_HI_PRIORITY		0x00000100L 
#define P2A_HI_PRIORITY		0x00001000L 
 
/* Enable Transfer Count */ 
#define EN_TCOUNT			0x10000000L 
 
/* Enable Bus Mastering */ 
#define EN_A2P_TRANSFERS	0x00000400L 
#define EN_P2A_TRANSFERS	0x00004000L 
 
/* Identify Interrupt Sources */ 
#define ANY_S593X_INT		  0x00800000L 
#define READ_TC_INT			  0x00080000L 
#define WRITE_TC_INT		  0x00040000L 
#define MASTER_ABORT_INT	0x00100000L 
#define TARGET_ABORT_INT	0x00200000L 
#define BUS_MASTER_INT		0x00200000L 
#define WRITE_MAILBOX_INT	0x00020000L 
#define INCOMING_MAILBOX_INT	0x00020000L 
 
 
#define OUTGOING_MAILBOX_INT	0x00010000L 
 
 
 
 
/* -- <eof> --- */
