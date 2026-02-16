/*
 * Case-insensitive CRC32 function originally added to zlib by Corsix
 * for hashing file paths/identifiers in a case-insensitive manner.
 * Extracted to a standalone file so we can use system zlib.
 *
 * The algorithm is identical to standard CRC32 but each input byte
 * is OR'd with 0x20 before hashing, which forces ASCII uppercase
 * letters to lowercase (and doesn't affect digits/most punctuation).
 */

#include "rainman/util/crc32_case_idt.h"

/* Standard CRC32 polynomial (same as zlib) */
static unsigned long crc_table[256];
static int crc_table_ready = 0;

static void make_crc_table(void)
{
	unsigned long c;
	int n, k;
	for (n = 0; n < 256; n++)
	{
		c = (unsigned long)n;
		for (k = 0; k < 8; k++)
		{
			if (c & 1)
				c = 0xedb88320UL ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
	crc_table_ready = 1;
}

unsigned long crc32_case_idt(unsigned long crc, const unsigned char *buf, unsigned int len)
{
	if (buf == 0)
		return 0UL;

	if (!crc_table_ready)
		make_crc_table();

	crc = crc ^ 0xffffffffUL;
	while (len >= 8)
	{
		crc = crc_table[((int)crc ^ ((*buf++) | 0x20)) & 0xff] ^ (crc >> 8);
		crc = crc_table[((int)crc ^ ((*buf++) | 0x20)) & 0xff] ^ (crc >> 8);
		crc = crc_table[((int)crc ^ ((*buf++) | 0x20)) & 0xff] ^ (crc >> 8);
		crc = crc_table[((int)crc ^ ((*buf++) | 0x20)) & 0xff] ^ (crc >> 8);
		crc = crc_table[((int)crc ^ ((*buf++) | 0x20)) & 0xff] ^ (crc >> 8);
		crc = crc_table[((int)crc ^ ((*buf++) | 0x20)) & 0xff] ^ (crc >> 8);
		crc = crc_table[((int)crc ^ ((*buf++) | 0x20)) & 0xff] ^ (crc >> 8);
		crc = crc_table[((int)crc ^ ((*buf++) | 0x20)) & 0xff] ^ (crc >> 8);
		len -= 8;
	}
	if (len)
		do
		{
			crc = crc_table[((int)crc ^ ((*buf++) | 0x20)) & 0xff] ^ (crc >> 8);
		} while (--len);
	return crc ^ 0xffffffffUL;
}
