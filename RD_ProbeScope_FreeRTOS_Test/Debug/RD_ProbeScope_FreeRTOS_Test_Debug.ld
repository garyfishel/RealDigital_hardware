/*
 * GENERATED FILE - DO NOT EDIT
 * Copyright (c) 2008-2013 Code Red Technologies Ltd,
 * Copyright 2015, 2018-2019 NXP
 * (c) NXP Semiconductors 2013-2023
 * Generated linker script file for LPC4370
 * Created from linkscript.ldt by FMCreateLinkLibraries
 * Using Freemarker v2.3.30
 * MCUXpresso IDE v11.7.0 [Build 9198] [2023-01-17] on Oct 2, 2023, 2:07:12 PM
 */

INCLUDE "RD_ProbeScope_FreeRTOS_Test_Debug_library.ld"
INCLUDE "RD_ProbeScope_FreeRTOS_Test_Debug_memory.ld"

ENTRY(ResetISR)

SECTIONS
{
     /* MAIN TEXT SECTION */
    .text : ALIGN(4)
    {
        FILL(0xff)
        __vectors_start__ = ABSOLUTE(.) ;
        KEEP(*(.isr_vector))
        /* Global Section Table */
        . = ALIGN(4) ;
        __section_table_start = .;
        __data_section_table = .;
        LONG(LOADADDR(.data));
        LONG(    ADDR(.data));
        LONG(  SIZEOF(.data));
        LONG(LOADADDR(.data_RAM2));
        LONG(    ADDR(.data_RAM2));
        LONG(  SIZEOF(.data_RAM2));
        LONG(LOADADDR(.data_RAM3));
        LONG(    ADDR(.data_RAM3));
        LONG(  SIZEOF(.data_RAM3));
        LONG(LOADADDR(.data_RAM4));
        LONG(    ADDR(.data_RAM4));
        LONG(  SIZEOF(.data_RAM4));
        LONG(LOADADDR(.data_RAM5));
        LONG(    ADDR(.data_RAM5));
        LONG(  SIZEOF(.data_RAM5));
        LONG(LOADADDR(.data_RAM6));
        LONG(    ADDR(.data_RAM6));
        LONG(  SIZEOF(.data_RAM6));
        LONG(LOADADDR(.data_RAM7));
        LONG(    ADDR(.data_RAM7));
        LONG(  SIZEOF(.data_RAM7));
        __data_section_table_end = .;
        __bss_section_table = .;
        LONG(    ADDR(.bss));
        LONG(  SIZEOF(.bss));
        LONG(    ADDR(.bss_RAM2));
        LONG(  SIZEOF(.bss_RAM2));
        LONG(    ADDR(.bss_RAM3));
        LONG(  SIZEOF(.bss_RAM3));
        LONG(    ADDR(.bss_RAM4));
        LONG(  SIZEOF(.bss_RAM4));
        LONG(    ADDR(.bss_RAM5));
        LONG(  SIZEOF(.bss_RAM5));
        LONG(    ADDR(.bss_RAM6));
        LONG(  SIZEOF(.bss_RAM6));
        LONG(    ADDR(.bss_RAM7));
        LONG(  SIZEOF(.bss_RAM7));
        __bss_section_table_end = .;
        __section_table_end = . ;
        /* End of Global Section Table */

        *(.after_vectors*)

        /* Code Read Protection data */
        . = 0x000002FC ;
        PROVIDE(__CRP_WORD_START__ = .) ;
        KEEP(*(.crp))
        PROVIDE(__CRP_WORD_END__ = .) ;
        ASSERT(!(__CRP_WORD_START__ == __CRP_WORD_END__), "Linker CRP Enabled, but no CRP_WORD provided within application");
        /* End of Code Read Protection */
       *(.text*)
       KEEP(*/tasks.o(.rodata*))  /* FreeRTOS Debug Config */
       *(.rodata .rodata.* .constdata .constdata.*)
       . = ALIGN(4);
    } > RamLoc128
    /*
     * for exception handling/unwind - some Newlib functions (in common
     * with C++ and STDC++) use this.
     */
    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > RamLoc128

    .ARM.exidx : ALIGN(4)
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > RamLoc128
 
    _etext = .;
        
    /* DATA section for RamLoc72 */

    .data_RAM2 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM2 = .) ;
        PROVIDE(__start_data_RamLoc72 = .) ;
        *(.ramfunc.$RAM2)
        *(.ramfunc.$RamLoc72)
        *(.data.$RAM2)
        *(.data.$RamLoc72)
        *(.data.$RAM2.*)
        *(.data.$RamLoc72.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM2 = .) ;
        PROVIDE(__end_data_RamLoc72 = .) ;
     } > RamLoc72 AT>RamLoc128

    /* DATA section for RamAHB32 */

    .data_RAM3 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM3 = .) ;
        PROVIDE(__start_data_RamAHB32 = .) ;
        *(.ramfunc.$RAM3)
        *(.ramfunc.$RamAHB32)
        *(.data.$RAM3)
        *(.data.$RamAHB32)
        *(.data.$RAM3.*)
        *(.data.$RamAHB32.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM3 = .) ;
        PROVIDE(__end_data_RamAHB32 = .) ;
     } > RamAHB32 AT>RamLoc128

    /* DATA section for RamAHB16 */

    .data_RAM4 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM4 = .) ;
        PROVIDE(__start_data_RamAHB16 = .) ;
        *(.ramfunc.$RAM4)
        *(.ramfunc.$RamAHB16)
        *(.data.$RAM4)
        *(.data.$RamAHB16)
        *(.data.$RAM4.*)
        *(.data.$RamAHB16.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM4 = .) ;
        PROVIDE(__end_data_RamAHB16 = .) ;
     } > RamAHB16 AT>RamLoc128

    /* DATA section for RamAHB_ETB16 */

    .data_RAM5 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM5 = .) ;
        PROVIDE(__start_data_RamAHB_ETB16 = .) ;
        *(.ramfunc.$RAM5)
        *(.ramfunc.$RamAHB_ETB16)
        *(.data.$RAM5)
        *(.data.$RamAHB_ETB16)
        *(.data.$RAM5.*)
        *(.data.$RamAHB_ETB16.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM5 = .) ;
        PROVIDE(__end_data_RamAHB_ETB16 = .) ;
     } > RamAHB_ETB16 AT>RamLoc128

    /* DATA section for RamM0Sub16 */

    .data_RAM6 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM6 = .) ;
        PROVIDE(__start_data_RamM0Sub16 = .) ;
        *(.ramfunc.$RAM6)
        *(.ramfunc.$RamM0Sub16)
        *(.data.$RAM6)
        *(.data.$RamM0Sub16)
        *(.data.$RAM6.*)
        *(.data.$RamM0Sub16.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM6 = .) ;
        PROVIDE(__end_data_RamM0Sub16 = .) ;
     } > RamM0Sub16 AT>RamLoc128

    /* DATA section for RamM0Sub2 */

    .data_RAM7 : ALIGN(4)
    {
        FILL(0xff)
        PROVIDE(__start_data_RAM7 = .) ;
        PROVIDE(__start_data_RamM0Sub2 = .) ;
        *(.ramfunc.$RAM7)
        *(.ramfunc.$RamM0Sub2)
        *(.data.$RAM7)
        *(.data.$RamM0Sub2)
        *(.data.$RAM7.*)
        *(.data.$RamM0Sub2.*)
        . = ALIGN(4) ;
        PROVIDE(__end_data_RAM7 = .) ;
        PROVIDE(__end_data_RamM0Sub2 = .) ;
     } > RamM0Sub2 AT>RamLoc128

    /* MAIN DATA SECTION */
    .uninit_RESERVED (NOLOAD) : ALIGN(4)
    {
        _start_uninit_RESERVED = .;
        KEEP(*(.bss.$RESERVED*))
       . = ALIGN(4) ;
        _end_uninit_RESERVED = .;
    } > RamLoc128 AT> RamLoc128

    /* Main DATA section (RamLoc128) */
    .data : ALIGN(4)
    {
       FILL(0xff)
       _data = . ;
       PROVIDE(__start_data_RAM = .) ;
       PROVIDE(__start_data_RamLoc128 = .) ;
       *(vtable)
       *(.ramfunc*)
       KEEP(*(CodeQuickAccess))
       KEEP(*(DataQuickAccess))
       *(RamFunction)
       *(.data*)
       . = ALIGN(4) ;
       _edata = . ;
       PROVIDE(__end_data_RAM = .) ;
       PROVIDE(__end_data_RamLoc128 = .) ;
    } > RamLoc128 AT>RamLoc128

    /* BSS section for RamLoc72 */
    .bss_RAM2 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM2 = .) ;
       PROVIDE(__start_bss_RamLoc72 = .) ;
       *(.bss.$RAM2)
       *(.bss.$RamLoc72)
       *(.bss.$RAM2.*)
       *(.bss.$RamLoc72.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM2 = .) ;
       PROVIDE(__end_bss_RamLoc72 = .) ;
    } > RamLoc72 AT> RamLoc72

    /* BSS section for RamAHB32 */
    .bss_RAM3 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM3 = .) ;
       PROVIDE(__start_bss_RamAHB32 = .) ;
       *(.bss.$RAM3)
       *(.bss.$RamAHB32)
       *(.bss.$RAM3.*)
       *(.bss.$RamAHB32.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM3 = .) ;
       PROVIDE(__end_bss_RamAHB32 = .) ;
    } > RamAHB32 AT> RamAHB32

    /* BSS section for RamAHB16 */
    .bss_RAM4 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM4 = .) ;
       PROVIDE(__start_bss_RamAHB16 = .) ;
       *(.bss.$RAM4)
       *(.bss.$RamAHB16)
       *(.bss.$RAM4.*)
       *(.bss.$RamAHB16.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM4 = .) ;
       PROVIDE(__end_bss_RamAHB16 = .) ;
    } > RamAHB16 AT> RamAHB16

    /* BSS section for RamAHB_ETB16 */
    .bss_RAM5 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM5 = .) ;
       PROVIDE(__start_bss_RamAHB_ETB16 = .) ;
       *(.bss.$RAM5)
       *(.bss.$RamAHB_ETB16)
       *(.bss.$RAM5.*)
       *(.bss.$RamAHB_ETB16.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM5 = .) ;
       PROVIDE(__end_bss_RamAHB_ETB16 = .) ;
    } > RamAHB_ETB16 AT> RamAHB_ETB16

    /* BSS section for RamM0Sub16 */
    .bss_RAM6 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM6 = .) ;
       PROVIDE(__start_bss_RamM0Sub16 = .) ;
       *(.bss.$RAM6)
       *(.bss.$RamM0Sub16)
       *(.bss.$RAM6.*)
       *(.bss.$RamM0Sub16.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM6 = .) ;
       PROVIDE(__end_bss_RamM0Sub16 = .) ;
    } > RamM0Sub16 AT> RamM0Sub16

    /* BSS section for RamM0Sub2 */
    .bss_RAM7 : ALIGN(4)
    {
       PROVIDE(__start_bss_RAM7 = .) ;
       PROVIDE(__start_bss_RamM0Sub2 = .) ;
       *(.bss.$RAM7)
       *(.bss.$RamM0Sub2)
       *(.bss.$RAM7.*)
       *(.bss.$RamM0Sub2.*)
       . = ALIGN (. != 0 ? 4 : 1) ; /* avoid empty segment */
       PROVIDE(__end_bss_RAM7 = .) ;
       PROVIDE(__end_bss_RamM0Sub2 = .) ;
    } > RamM0Sub2 AT> RamM0Sub2

    /* MAIN BSS SECTION */
    .bss : ALIGN(4)
    {
        _bss = .;
        PROVIDE(__start_bss_RAM = .) ;
        PROVIDE(__start_bss_RamLoc128 = .) ;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4) ;
        _ebss = .;
        PROVIDE(__end_bss_RAM = .) ;
        PROVIDE(__end_bss_RamLoc128 = .) ;
        PROVIDE(end = .);
    } > RamLoc128 AT> RamLoc128

    /* NOINIT section for RamLoc72 */
    .noinit_RAM2 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM2 = .) ;
       PROVIDE(__start_noinit_RamLoc72 = .) ;
       *(.noinit.$RAM2)
       *(.noinit.$RamLoc72)
       *(.noinit.$RAM2.*)
       *(.noinit.$RamLoc72.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM2 = .) ;
       PROVIDE(__end_noinit_RamLoc72 = .) ;
    } > RamLoc72 AT> RamLoc72

    /* NOINIT section for RamAHB32 */
    .noinit_RAM3 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM3 = .) ;
       PROVIDE(__start_noinit_RamAHB32 = .) ;
       *(.noinit.$RAM3)
       *(.noinit.$RamAHB32)
       *(.noinit.$RAM3.*)
       *(.noinit.$RamAHB32.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM3 = .) ;
       PROVIDE(__end_noinit_RamAHB32 = .) ;
    } > RamAHB32 AT> RamAHB32

    /* NOINIT section for RamAHB16 */
    .noinit_RAM4 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM4 = .) ;
       PROVIDE(__start_noinit_RamAHB16 = .) ;
       *(.noinit.$RAM4)
       *(.noinit.$RamAHB16)
       *(.noinit.$RAM4.*)
       *(.noinit.$RamAHB16.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM4 = .) ;
       PROVIDE(__end_noinit_RamAHB16 = .) ;
    } > RamAHB16 AT> RamAHB16

    /* NOINIT section for RamAHB_ETB16 */
    .noinit_RAM5 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM5 = .) ;
       PROVIDE(__start_noinit_RamAHB_ETB16 = .) ;
       *(.noinit.$RAM5)
       *(.noinit.$RamAHB_ETB16)
       *(.noinit.$RAM5.*)
       *(.noinit.$RamAHB_ETB16.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM5 = .) ;
       PROVIDE(__end_noinit_RamAHB_ETB16 = .) ;
    } > RamAHB_ETB16 AT> RamAHB_ETB16

    /* NOINIT section for RamM0Sub16 */
    .noinit_RAM6 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM6 = .) ;
       PROVIDE(__start_noinit_RamM0Sub16 = .) ;
       *(.noinit.$RAM6)
       *(.noinit.$RamM0Sub16)
       *(.noinit.$RAM6.*)
       *(.noinit.$RamM0Sub16.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM6 = .) ;
       PROVIDE(__end_noinit_RamM0Sub16 = .) ;
    } > RamM0Sub16 AT> RamM0Sub16

    /* NOINIT section for RamM0Sub2 */
    .noinit_RAM7 (NOLOAD) : ALIGN(4)
    {
       PROVIDE(__start_noinit_RAM7 = .) ;
       PROVIDE(__start_noinit_RamM0Sub2 = .) ;
       *(.noinit.$RAM7)
       *(.noinit.$RamM0Sub2)
       *(.noinit.$RAM7.*)
       *(.noinit.$RamM0Sub2.*)
       . = ALIGN(4) ;
       PROVIDE(__end_noinit_RAM7 = .) ;
       PROVIDE(__end_noinit_RamM0Sub2 = .) ;
    } > RamM0Sub2 AT> RamM0Sub2

    /* DEFAULT NOINIT SECTION */
    .noinit (NOLOAD): ALIGN(4)
    {
        _noinit = .;
        PROVIDE(__start_noinit_RAM = .) ;
        PROVIDE(__start_noinit_RamLoc128 = .) ;
        *(.noinit*)
         . = ALIGN(4) ;
        _end_noinit = .;
       PROVIDE(__end_noinit_RAM = .) ;
       PROVIDE(__end_noinit_RamLoc128 = .) ;        
    } > RamLoc128 AT> RamLoc128

    PROVIDE(_pvHeapStart = DEFINED(__user_heap_base) ? __user_heap_base : .);

    PROVIDE(_vStackTop = DEFINED(__user_stack_top) ? __user_stack_top : __top_RamLoc128 - 0);

    /* ## Create checksum value (used in startup) ## */
    PROVIDE(__valid_user_code_checksum = 0 - 
                                         (_vStackTop 
                                         + (ResetISR + 1) 
                                         + (NMI_Handler + 1) 
                                         + (HardFault_Handler + 1) 
                                         + (( DEFINED(MemManage_Handler) ? MemManage_Handler : 0 ) + 1)   /* MemManage_Handler may not be defined */
                                         + (( DEFINED(BusFault_Handler) ? BusFault_Handler : 0 ) + 1)     /* BusFault_Handler may not be defined */
                                         + (( DEFINED(UsageFault_Handler) ? UsageFault_Handler : 0 ) + 1) /* UsageFault_Handler may not be defined */
                                         ) );

    /* Provide basic symbols giving location and size of main text
     * block, including initial values of RW data sections. Note that
     * these will need extending to give a complete picture with
     * complex images (e.g multiple Flash banks).
     */
    _image_start = LOADADDR(.text);
    _image_end = LOADADDR(.data) + SIZEOF(.data);
    _image_size = _image_end - _image_start;
}