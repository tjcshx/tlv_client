/*
 *	crc-itu-t.h - CRC ITU-T V.41 routine
 *
 * Implements the standard CRC ITU-T V.41:
 *   Width 16
 *   Poly  0x0x1021 (x^16 + x^12 + x^15 + 1)
 *   Init  0
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2. See the file COPYING for more details.
 */

#ifndef CRC_ITU_T_H
#define CRC_ITU_T_H

#define MAGIC_CRC           0x1E50

extern const unsigned short  crc_itu_t_table[256];

extern int ushort_to_bytes(unsigned char *bytes, unsigned short val);
extern unsigned short bytes_to_ushort(unsigned char *bytes, int len);

extern unsigned short crc_itu_t(unsigned short crc, const unsigned char *buffer, unsigned int len);

static inline unsigned short crc_itu_t_byte(unsigned short crc, const unsigned char data)
{
	return (crc << 8) ^ crc_itu_t_table[((crc >> 8) ^ data) & 0xff];
}

#endif /* CRC_ITU_T_H */
