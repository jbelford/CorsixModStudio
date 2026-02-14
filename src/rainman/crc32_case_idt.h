#ifndef CRC32_CASE_IDT_H
#define CRC32_CASE_IDT_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Case-insensitive CRC32: same as standard CRC32 but each byte is
 * OR'd with 0x20 before hashing (forces ASCII letters to lowercase).
 * Used for hashing file paths and identifiers in Relic game formats.
 */
unsigned long crc32_case_idt(unsigned long crc, const unsigned char* buf, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif /* CRC32_CASE_IDT_H */
