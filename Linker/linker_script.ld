/**
* @file    stm32f446re.ld
* @brief   Linker script for STM32F446RE (512KB Flash, 128KB RAM)
* @details Defines memory regions, section placement, stack, heap, and
*          startup vector table. Used by GNU linker for project build.
*/ 

/**
 * @brief Entry point symbol for reset handler.
 * @details This symbol must exist in the startup file.
 */
ENTRY(Reset_Handler)

/**
 * @brief Memory regions as per STM32F446RE datasheet.
 */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K   /* Program memory */
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K   /* SRAM1 + SRAM2 */
}

/**
 * @brief Section placement defines how each section is mapped to memory.
 */
SECTIONS
{
  /**
   * @section Code_Section
   * @brief Vector Table and Code (.isr_vector + .text)
   */
  .text :
  {
    KEEP(*(.isr_vector))      /* Interrupt Vector Table (Reset, ISRs) */
    *(.text*)                 /* All code (.text, .text.Reset_Handler, etc.) */
    *(.rodata*)               /* Read-only data (const, lookup tables) */
    *(.glue_7) *(.glue_7t)    /* ARM-Thumb interworking support */
    *(.eh_frame)              /* Exception handling data (C++) */

    . = ALIGN(4);             /* Align next address on 4-byte boundary */
    _etext = .;               /* Marks end of code section */
  } > FLASH

  /**
   * @section Data_Section
   * @brief Initialized Data (.data)
   * @details Data copied from Flash (_sidata) to RAM (_sdata → _edata)
   */
  .data : AT(_etext)
  {
    _sidata = LOADADDR(.data); /* Address in Flash where .data is stored */

    . = ALIGN(4);
    _sdata = .;                /* Start of .data in RAM */
    *(.data*)
    . = ALIGN(4);
    _edata = .;                /* End of .data in RAM */
  } > RAM

  /**
   * @section BSS_Section
   * @brief Uninitialized Data (.bss)
   * @details Zero-initialized at runtime (_sbss → _ebss)
   */
  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    _sbss = .;                 /* Start of .bss in RAM */
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;                 /* End of .bss in RAM */
  } > RAM

  /**
   * @section Heap_Stack
   * @brief Heap and Stack region
   * @details Reserve space for dynamic memory (heap) and stack.
   */
  ._user_heap_stack (NOLOAD):
  {
    . = ALIGN(8);
    _sheap = .;                /* Heap start */
    . = . + 0x00001000;        /* Reserve 4 KB for heap */
    _eheap = .;

    . = ALIGN(8);
    _sstack = .;               /* Stack start (grows down) */
    . = . + 0x00002000;        /* Reserve 8 KB for stack */
    _estack = ORIGIN(RAM) + LENGTH(RAM); /* Top of stack */
  } > RAM

  /**
   * @section Discard_Debug
   * @brief Discard Debug Sections (optional)
   */
  /DISCARD/ :
  {
    *(.comment)
    *(.note*)
  }
}

/**
 * @brief End of linker script
 */
