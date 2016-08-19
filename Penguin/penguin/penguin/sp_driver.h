#ifndef SP_DRIVER_H
#define SP_DRIVER_H


/* Define the size of the flash page if not defined in the header files. */
#ifndef APP_SECTION_SIZE
	#error  "APP_SECTION_SIZE must be defined if not defined in header files."
#endif /*FLASH_PAGE_SIZE*/


/* Define the Start of the application table if not defined in the header files. */
#ifndef APP_SECTION_SIZE
	#error  "APPTABLE_SECTION_START must be defined if not defined in header files."
#endif /*APPTABLE_SECTION_START*/


/*! \brief Read a byte from flash.
 *
 *  This function reads one byte from the flash.
 *
 *  \note Both IAR and GCC have functions to do this, but
 *        we include the fucntions for easier use.
 *
 *  \param address Address to the location of the byte to read.
 *
 *  \retval Byte read from flash.
 */
uint8_t SP_ReadByte(uint32_t address);

/*! \brief Read a word from flash.
 *
 *  This function reads one word from the flash.
 *
 *  \note Both IAR and GCC have functions to do this automatically, but
 *        we include the fucntions for easier use.
 *
 *  \param address Address to the location of the word to read.
 *
 *  \retval word read from flash.
 */
uint16_t SP_ReadWord(uint32_t address);



/*! \brief Write lock bits.
 *
 *  This function changes the lock bits.
 *
 *  \note It is only possible to change the lock bits to a higher security level.
 *
 *  \param data  The new value of the lock bits.
 */
void SP_WriteLockBits(uint8_t data);


/*! \brief Erase entire application section.
 *
 *  This function erases the entire application and application table section
 *
 *  \note If the lock bits are set to not allow spm in the application or
 *        application table section the erase is not done.
 */ 
void SP_EraseApplicationSection(void);

/*! \brief Erase page at byte address in application or application table section.
 *
 *  This function erase one page given by the byte address.
 *
 *  \param address Byte address for flash page.
 */
void SP_EraseApplicationPage(uint32_t address);

/*! \brief Erase and write page buffer to application or application table section at byte address.
 *
 *  This function does a combined erase and write to a flash page in the application
 *  or application table section.
 *
 *  \param address Byte address for flash page.
 */
void SP_EraseWriteApplicationPage(uint32_t address);

/*! \brief Write page buffer to application or application table section at byte address.
 *
 *  This function writes the Flash page buffer to a page in the application or
 *  application table section given by the byte address.
 *
 *  \note The page that is written to must be erased before it is written to.
 *
 *  \param address Byte address for flash page.
 */
void SP_WriteApplicationPage(uint32_t address);

/*! \brief Load one word into Flash page buffer.
 *
 *  This function Loads one word into the Flash page buffer.
 *
 *  \param  address   Position in inside the flash page buffer.
 *  \param  data      Value to be put into the buffer.
 */
void SP_LoadFlashWord(uint16_t address, uint16_t data);

/*! \brief Load entire page from SRAM buffer into Flash page buffer.
 *
 *  This function load an entire page from SRAM.
 *
 *	\param data   Pointer to the data to put in buffer.
 *
 *	\note The __near keyword limits the pointer to two bytes which means that
 *        only data up to 64K (internal SRAM) can be used.
 */
void SP_LoadFlashPage(const uint8_t * data);


/*! \brief Flush Flash page buffer.
 *
 *  This function flush the Flash page buffer.
 */
void SP_EraseFlashBuffer(void);


/*! \brief Wait for SPM to finish.
 *
 *   This routine waits for the SPM to finish and clears the command register.
 */
void SP_WaitForSPM(void);

#endif /* SP_DRIVER_H */
