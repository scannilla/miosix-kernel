/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "filesystem/ioctl.h"
#include "config/miosix_settings.h"

/* Definitions of physical drive number for each drive */
//#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
//#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
//#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

using namespace miosix;


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

//DSTATUS disk_status (
//	BYTE pdrv		/* Physical drive nmuber to identify the drive */
//)
//{
//	DSTATUS stat;
//	int result;
//
//	switch (pdrv) {
//	case DEV_RAM :
//		result = RAM_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case DEV_MMC :
//		result = MMC_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
//
//	case DEV_USB :
//		result = USB_disk_status();
//
//		// translate the reslut code here
//
//		return stat;
//	}
//	return STA_NOINIT;
//}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/
/*
DSTATUS disk_initialize (
	BYTE pdrv				// Physical drive nmuber to identify the drive
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		result = RAM_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		result = MMC_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}
*/


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

/*
DRESULT disk_read (
	BYTE pdrv,		// Physical drive nmuber to identify the drive 
	BYTE *buff,		// Data buffer to store read data
	LBA_t sector,	// Start sector in LBA 
	UINT count		// Number of sectors to read 
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}
*/

DRESULT disk_read (
    intrusive_ref_ptr<FileBase> pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,           /* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..255) */
)
{
    if(pdrv->lseek(static_cast<off_t>(sector)*512,SEEK_SET)<0) return RES_ERROR;
    if(pdrv->read(buff,count*512)!=static_cast<ssize_t>(count)*512) return RES_ERROR;
    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/


/*
DRESULT disk_write (
	BYTE pdrv,			// Physical drive nmuber to identify the drive 
	const BYTE *buff,	// Data to be written
	LBA_t sector,		// Start sector in LBA 
	UINT count			// Number of sectors to write 
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;
}
*/
DRESULT disk_write (
    intrusive_ref_ptr<FileBase> pdrv,		/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count		/* Number of sectors to write (1..255) */
)
{
    if(pdrv->lseek(static_cast<off_t>(sector)*512,SEEK_SET)<0) return RES_ERROR;
    if(pdrv->write(buff,count*512)!=static_cast<ssize_t>(count)*512) return RES_ERROR;
    return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	intrusive_ref_ptr<FileBase> pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	int result;
/*
	switch (pdrv) {
	case DEV_RAM :

		// Process of the command for the RAM drive

		return res;

	case DEV_MMC :

		// Process of the command for the MMC/SD card

		return res;

	case DEV_USB :

		// Process of the command the USB drive

		return res;
	}

	return RES_PARERR;
	*/
	switch(cmd)
    {
        case CTRL_SYNC:
            if(pdrv->ioctl(IOCTL_SYNC,0)==0) return RES_OK; else return RES_ERROR;
        case GET_SECTOR_COUNT:
            return RES_ERROR; //unimplemented, so f_mkfs() does not work
        case GET_BLOCK_SIZE:
            return RES_ERROR; //unimplemented, so f_mkfs() does not work
        default:
            return RES_PARERR;
    }

	return RES_ERROR; // added to avoid the error, it should be unreachanle code
}

/**
 * \internal
 * Return current time, used to save file creation time
 */
 DWORD get_fattime()
 {
     return 0x210000;//TODO: this stub just returns date 01/01/1980 0.00.00
 }

