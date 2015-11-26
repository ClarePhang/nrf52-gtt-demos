/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @defgroup pwm_example_main main.c
 * @{
 * @ingroup pwm_example
 * 
 * @brief  PWM Example Application main file.
 *
 * This file contains the source code for a sample application using PWM.
 *
 *
 */


// a full cycle, 16-bit, 2's complement sine wave lookup table
// source: http://www.ecse.rpi.edu/courses/cstudio/Silabs/SamplePrograms/C8051F02x/C/DAC1_fgen1.c
uint16_t sine16b[] = {
   0x0000, 0x0324, 0x0647, 0x096a, 0x0c8b, 0x0fab, 0x12c8, 0x15e2, 
   0x18f8, 0x1c0b, 0x1f19, 0x2223, 0x2528, 0x2826, 0x2b1f, 0x2e11,
   0x30fb, 0x33de, 0x36ba, 0x398c, 0x3c56, 0x3f17, 0x41ce, 0x447a, 
   0x471c, 0x49b4, 0x4c3f, 0x4ebf, 0x5133, 0x539b, 0x55f5, 0x5842,
   0x5a82, 0x5cb4, 0x5ed7, 0x60ec, 0x62f2, 0x64e8, 0x66cf, 0x68a6, 
   0x6a6d, 0x6c24, 0x6dca, 0x6f5f, 0x70e2, 0x7255, 0x73b5, 0x7504,
   0x7641, 0x776c, 0x7884, 0x798a, 0x7a7d, 0x7b5d, 0x7c29, 0x7ce3, 
   0x7d8a, 0x7e1d, 0x7e9d, 0x7f09, 0x7f62, 0x7fa7, 0x7fd8, 0x7ff6,
   0x7fff, 0x7ff6, 0x7fd8, 0x7fa7, 0x7f62, 0x7f09, 0x7e9d, 0x7e1d, 
   0x7d8a, 0x7ce3, 0x7c29, 0x7b5d, 0x7a7d, 0x798a, 0x7884, 0x776c,
   0x7641, 0x7504, 0x73b5, 0x7255, 0x70e2, 0x6f5f, 0x6dca, 0x6c24, 
   0x6a6d, 0x68a6, 0x66cf, 0x64e8, 0x62f2, 0x60ec, 0x5ed7, 0x5cb4,
   0x5a82, 0x5842, 0x55f5, 0x539b, 0x5133, 0x4ebf, 0x4c3f, 0x49b4, 
   0x471c, 0x447a, 0x41ce, 0x3f17, 0x3c56, 0x398c, 0x36ba, 0x33de,
   0x30fb, 0x2e11, 0x2b1f, 0x2826, 0x2528, 0x2223, 0x1f19, 0x1c0b, 
   0x18f8, 0x15e2, 0x12c8, 0x0fab, 0x0c8b, 0x096a, 0x0647, 0x0324,
   0x0000, 0xfcdc, 0xf9b9, 0xf696, 0xf375, 0xf055, 0xed38, 0xea1e, 
   0xe708, 0xe3f5, 0xe0e7, 0xdddd, 0xdad8, 0xd7da, 0xd4e1, 0xd1ef,
   0xcf05, 0xcc22, 0xc946, 0xc674, 0xc3aa, 0xc0e9, 0xbe32, 0xbb86, 
   0xb8e4, 0xb64c, 0xb3c1, 0xb141, 0xaecd, 0xac65, 0xaa0b, 0xa7be,
   0xa57e, 0xa34c, 0xa129, 0x9f14, 0x9d0e, 0x9b18, 0x9931, 0x975a, 
   0x9593, 0x93dc, 0x9236, 0x90a1, 0x8f1e, 0x8dab, 0x8c4b, 0x8afc,
   0x89bf, 0x8894, 0x877c, 0x8676, 0x8583, 0x84a3, 0x83d7, 0x831d, 
   0x8276, 0x81e3, 0x8163, 0x80f7, 0x809e, 0x8059, 0x8028, 0x800a,
   0x8000, 0x800a, 0x8028, 0x8059, 0x809e, 0x80f7, 0x8163, 0x81e3, 
   0x8276, 0x831d, 0x83d7, 0x84a3, 0x8583, 0x8676, 0x877c, 0x8894,
   0x89bf, 0x8afc, 0x8c4b, 0x8dab, 0x8f1e, 0x90a1, 0x9236, 0x93dc, 
   0x9593, 0x975a, 0x9931, 0x9b18, 0x9d0e, 0x9f14, 0xa129, 0xa34c,
   0xa57e, 0xa7be, 0xaa0b, 0xac65, 0xaecd, 0xb141, 0xb3c1, 0xb64c, 
   0xb8e4, 0xbb86, 0xbe32, 0xc0e9, 0xc3aa, 0xc674, 0xc946, 0xcc22,
   0xcf05, 0xd1ef, 0xd4e1, 0xd7da, 0xdad8, 0xdddd, 0xe0e7, 0xe3f5, 
   0xe708, 0xea1e, 0xed38, 0xf055, 0xf375, 0xf696, 0xf9b9, 0xfcdc
};

// a full cycle, 15-bit, 2's complement sine wave lookup table
// sine16b[] divided by two
uint16_t sine15b[] = {
0x4000, 	0x4192, 	0x4323, 	0x44B5, 	0x4645, 	0x47D5, 	0x4964, 	0x4A80, 
0x4C7C, 	0x4E05, 	0x4F8C, 	0x5111, 	0x5294, 	0x5413, 	0x558F, 	0x5708, 
0x587D, 	0x59EF, 	0x5B5D, 	0x5CC6, 	0x5E2B, 	0x5F8B, 	0x60E7, 	0x623D, 
0x638E, 	0x64DA, 	0x661F, 	0x675F, 	0x6899, 	0x69CD, 	0x6AFA, 	0x6C21, 
0x6D41, 	0x6E5A, 	0x6F6B, 	0x7076, 	0x7179, 	0x7274, 	0x7367, 	0x7453, 
0x7536, 	0x7612, 	0x76E5, 	0x77AF, 	0x7800, 	0x792A, 	0x79DA, 	0x7A82, 
0x7B20, 	0x7BB6, 	0x7C42, 	0x7CC5, 	0x7D3E, 	0x7DAE, 	0x7E14, 	0x7E71, 
0x7EC5, 	0x7F0E, 	0x7F4E, 	0x7F84, 	0x7FB1, 	0x7FD3, 	0x7FEC, 	0x7FFB, 
0x7FFF, 	0x7FFB, 	0x7FEC, 	0x7FD3, 	0x7FB1, 	0x7F84, 	0x7F4E, 	0x7F0E, 
0x7EC5, 	0x7E71, 	0x7E14, 	0x7DAE, 	0x7D3E, 	0x7CC5, 	0x7C42, 	0x7BB6, 
0x7B20, 	0x7A82, 	0x79DA, 	0x792A, 	0x7800, 	0x77AF, 	0x76E5, 	0x7612, 
0x7536, 	0x7453, 	0x7367, 	0x7274, 	0x7179, 	0x7076, 	0x6F6B, 	0x6E5A, 
0x6D41, 	0x6C21, 	0x6AFA, 	0x69CD, 	0x6899, 	0x675F, 	0x661F, 	0x64DA, 
0x638E, 	0x623D, 	0x60E7, 	0x5F8B, 	0x5E2B, 	0x5CC6, 	0x5B5D, 	0x59EF, 
0x587D, 	0x5708, 	0x558F, 	0x5413, 	0x5294, 	0x5111, 	0x4F8C, 	0x4E05, 
0x4C7C, 	0x4A80, 	0x4964, 	0x47D5, 	0x4645, 	0x44B5, 	0x4323, 	0x4192, 
0x4000, 	0x3E6E, 	0x3CDC, 	0x3B4B, 	0x39BA, 	0x382A, 	0x369C, 	0x350F, 
0x3384, 	0x31FA, 	0x3073, 	0x2EEE, 	0x2D6C, 	0x2BED, 	0x2A70, 	0x28F7, 
0x2782, 	0x2611, 	0x24A3, 	0x233A, 	0x21D5, 	0x2074, 	0x1F19, 	0x1DC3, 
0x1C72, 	0x1B26, 	0x19E0, 	0x18A0, 	0x1766, 	0x1632, 	0x1505, 	0x13DF, 
0x12BF, 	0x11A6, 	0x1094, 	0x0F8A, 	0x0E87, 	0x0D8C, 	0x0C98, 	0x0BAD, 
0x0AC9, 	0x09EE, 	0x091B, 	0x0850, 	0x078F, 	0x06D5, 	0x0625, 	0x057E, 
0x04DF, 	0x044A, 	0x03BE, 	0x033B, 	0x02C1, 	0x0251, 	0x01EB, 	0x018E, 
0x013B, 	0x00F1, 	0x00B1, 	0x007B, 	0x004F, 	0x002C, 	0x0014, 	0x0005, 
0x0000, 	0x0005, 	0x0014, 	0x002C, 	0x004F, 	0x007B, 	0x00B1, 	0x00F1, 
0x013B, 	0x018E, 	0x01EB, 	0x0251, 	0x02C1, 	0x033B, 	0x03BE, 	0x044A, 
0x04DF, 	0x057E, 	0x0625, 	0x06D5, 	0x078F, 	0x0850, 	0x091B, 	0x09EE, 
0x0AC9, 	0x0BAD, 	0x0C98, 	0x0D8C, 	0x0E87, 	0x0F8A, 	0x1094, 	0x11A6, 
0x12BF, 	0x13DF, 	0x1505, 	0x1632, 	0x1766, 	0x18A0, 	0x19E0, 	0x1B26, 
0x1C72, 	0x1DC3, 	0x1F19, 	0x2074, 	0x21D5, 	0x233A, 	0x24A3, 	0x2611, 
0x2782, 	0x28F7, 	0x2A70, 	0x2BED, 	0x2D6C, 	0x2EEE, 	0x3073, 	0x31FA, 
0x3384, 	0x350F, 	0x369C, 	0x382A, 	0x39BA, 	0x3B4B, 	0x3CDC, 	0x3E6E
};

// a full cycle, 15-bit, 2's complement negative sine wave lookup table
// inverted sine15b[]
uint16_t sine15bneg[] = {
0xC000, 	0xC192, 	0xC323, 	0xC4B5, 	0xC645, 	0xC7D5, 	0xC964, 	0xCA80, 
0xCC7C, 	0xCE05, 	0xCF8C, 	0xD111, 	0xD294, 	0xD413, 	0xD58F, 	0xD708, 
0xD87D, 	0xD9EF, 	0xDB5D, 	0xDCC6, 	0xDE2B, 	0xDF8B, 	0xE0E7, 	0xE23D, 
0xE38E, 	0xE4DA, 	0xE61F, 	0xE75F, 	0xE899, 	0xE9CD, 	0xEAFA, 	0xEC21, 
0xED41, 	0xEE5A, 	0xEF6B, 	0xF076, 	0xF179, 	0xF274, 	0xF367, 	0xF453, 
0xF536, 	0xF612, 	0xF6E5, 	0xF7AF, 	0xF800, 	0xF92A, 	0xF9DA, 	0xFA82, 
0xFB20, 	0xFBB6, 	0xFC42, 	0xFCC5, 	0xFD3E, 	0xFDAE, 	0xFE14, 	0xFE71, 
0xFEC5, 	0xFF0E, 	0xFF4E, 	0xFF84, 	0xFFB1, 	0xFFD3, 	0xFFEC, 	0xFFFB, 
0xFFFF, 	0xFFFB, 	0xFFEC, 	0xFFD3, 	0xFFB1, 	0xFF84, 	0xFF4E, 	0xFF0E, 
0xFEC5, 	0xFE71, 	0xFE14, 	0xFDAE, 	0xFD3E, 	0xFCC5, 	0xFC42, 	0xFBB6, 
0xFB20, 	0xFA82, 	0xF9DA, 	0xF92A, 	0xF800, 	0xF7AF, 	0xF6E5, 	0xF612, 
0xF536, 	0xF453, 	0xF367, 	0xF274, 	0xF179, 	0xF076, 	0xEF6B, 	0xEE5A, 
0xED41, 	0xEC21, 	0xEAFA, 	0xE9CD, 	0xE899, 	0xE75F, 	0xE61F, 	0xE4DA, 
0xE38E, 	0xE23D, 	0xE0E7, 	0xDF8B, 	0xDE2B, 	0xDCC6, 	0xDB5D, 	0xD9EF, 
0xD87D, 	0xD708, 	0xD58F, 	0xD413, 	0xD294, 	0xD111, 	0xCF8C, 	0xCE05, 
0xCC7C, 	0xCA80, 	0xC964, 	0xC7D5, 	0xC645, 	0xC4B5, 	0xC323, 	0xC192, 
0xC000, 	0xBE6E, 	0xBCDC, 	0xBB4B, 	0xB9BA, 	0xB82A, 	0xB69C, 	0xB50F, 
0xB384, 	0xB1FA, 	0xB073, 	0xAEEE, 	0xAD6C, 	0xABED, 	0xAA70, 	0xA8F7, 
0xA782, 	0xA611, 	0xA4A3, 	0xA33A, 	0xA1D5, 	0xA074, 	0x9F19, 	0x9DC3, 
0x9C72, 	0x9B26, 	0x99E0, 	0x98A0, 	0x9766, 	0x9632, 	0x9505, 	0x93DF, 
0x92BF, 	0x91A6, 	0x9094, 	0x8F8A, 	0x8E87, 	0x8D8C, 	0x8C98, 	0x8BAD, 
0x8AC9, 	0x89EE, 	0x891B, 	0x8850, 	0x878F, 	0x86D5, 	0x8625, 	0x857E, 
0x84DF, 	0x844A, 	0x83BE, 	0x833B, 	0x82C1, 	0x8251, 	0x81EB, 	0x818E, 
0x813B, 	0x80F1, 	0x80B1, 	0x807B, 	0x804F, 	0x802C, 	0x8014, 	0x8005, 
0x8000, 	0x8005, 	0x8014, 	0x802C, 	0x804F, 	0x807B, 	0x80B1, 	0x80F1, 
0x813B, 	0x818E, 	0x81EB, 	0x8251, 	0x82C1, 	0x833B, 	0x83BE, 	0x844A, 
0x84DF, 	0x857E, 	0x8625, 	0x86D5, 	0x878F, 	0x8850, 	0x891B, 	0x89EE, 
0x8AC9, 	0x8BAD, 	0x8C98, 	0x8D8C, 	0x8E87, 	0x8F8A, 	0x9094, 	0x91A6, 
0x92BF, 	0x93DF, 	0x9505, 	0x9632, 	0x9766, 	0x98A0, 	0x99E0, 	0x9B26, 
0x9C72, 	0x9DC3, 	0x9F19, 	0xA074, 	0xA1D5, 	0xA33A, 	0xA4A3, 	0xA611, 
0xA782, 	0xA8F7, 	0xAA70, 	0xABED, 	0xAD6C, 	0xAEEE, 	0xB073, 	0xB1FA, 
0xB384, 	0xB50F, 	0xB69C, 	0xB82A, 	0xB9BA, 	0xBB4B, 	0xBCDC, 	0xBE6E
};

uint16_t sine15binter[] = {
0x4000, 	0xC000, 	0x4192, 	0xC192, 	0x4323, 	0xC323, 	0x44B5, 	0xC4B5, 	0x4645, 	0xC645, 	0x47D5, 	0xC7D5, 	0x4964, 	0xC964, 	0x4A80, 	0xCA80, 
0x4C7C, 	0xCC7C, 	0x4E05, 	0xCE05, 	0x4F8C, 	0xCF8C, 	0x5111, 	0xD111, 	0x5294, 	0xD294, 	0x5413, 	0xD413, 	0x558F, 	0xD58F, 	0x5708, 	0xD708, 
0x587D, 	0xD87D, 	0x59EF, 	0xD9EF, 	0x5B5D, 	0xDB5D, 	0x5CC6, 	0xDCC6, 	0x5E2B, 	0xDE2B, 	0x5F8B, 	0xDF8B, 	0x60E7, 	0xE0E7, 	0x623D, 	0xE23D, 
0x638E, 	0xE38E, 	0x64DA, 	0xE4DA, 	0x661F, 	0xE61F, 	0x675F, 	0xE75F, 	0x6899, 	0xE899, 	0x69CD, 	0xE9CD, 	0x6AFA, 	0xEAFA, 	0x6C21, 	0xEC21, 
0x6D41, 	0xED41, 	0x6E5A, 	0xEE5A, 	0x6F6B, 	0xEF6B, 	0x7076, 	0xF076, 	0x7179, 	0xF179, 	0x7274, 	0xF274, 	0x7367, 	0xF367, 	0x7453, 	0xF453, 
0x7536, 	0xF536, 	0x7612, 	0xF612, 	0x76E5, 	0xF6E5, 	0x77AF, 	0xF7AF, 	0x7800, 	0xF800, 	0x792A, 	0xF92A, 	0x79DA, 	0xF9DA, 	0x7A82, 	0xFA82, 
0x7B20, 	0xFB20, 	0x7BB6, 	0xFBB6, 	0x7C42, 	0xFC42, 	0x7CC5, 	0xFCC5, 	0x7D3E, 	0xFD3E, 	0x7DAE, 	0xFDAE, 	0x7E14, 	0xFE14, 	0x7E71, 	0xFE71, 
0x7EC5, 	0xFEC5, 	0x7F0E, 	0xFF0E, 	0x7F4E, 	0xFF4E, 	0x7F84, 	0xFF84, 	0x7FB1, 	0xFFB1, 	0x7FD3, 	0xFFD3, 	0x7FEC, 	0xFFEC, 	0x7FFB, 	0xFFFB, 
0x7FFF, 	0xFFFF, 	0x7FFB, 	0xFFFB, 	0x7FEC, 	0xFFEC, 	0x7FD3, 	0xFFD3, 	0x7FB1, 	0xFFB1, 	0x7F84, 	0xFF84, 	0x7F4E, 	0xFF4E, 	0x7F0E, 	0xFF0E, 
0x7EC5, 	0xFEC5, 	0x7E71, 	0xFE71, 	0x7E14, 	0xFE14, 	0x7DAE, 	0xFDAE, 	0x7D3E, 	0xFD3E, 	0x7CC5, 	0xFCC5, 	0x7C42, 	0xFC42, 	0x7BB6, 	0xFBB6, 
0x7B20, 	0xFB20, 	0x7A82, 	0xFA82, 	0x79DA, 	0xF9DA, 	0x792A, 	0xF92A, 	0x7800, 	0xF800, 	0x77AF, 	0xF7AF, 	0x76E5, 	0xF6E5, 	0x7612, 	0xF612, 
0x7536, 	0xF536, 	0x7453, 	0xF453, 	0x7367, 	0xF367, 	0x7274, 	0xF274, 	0x7179, 	0xF179, 	0x7076, 	0xF076, 	0x6F6B, 	0xEF6B, 	0x6E5A, 	0xEE5A, 
0x6D41, 	0xED41, 	0x6C21, 	0xEC21, 	0x6AFA, 	0xEAFA, 	0x69CD, 	0xE9CD, 	0x6899, 	0xE899, 	0x675F, 	0xE75F, 	0x661F, 	0xE61F, 	0x64DA, 	0xE4DA, 
0x638E, 	0xE38E, 	0x623D, 	0xE23D, 	0x60E7, 	0xE0E7, 	0x5F8B, 	0xDF8B, 	0x5E2B, 	0xDE2B, 	0x5CC6, 	0xDCC6, 	0x5B5D, 	0xDB5D, 	0x59EF, 	0xD9EF, 
0x587D, 	0xD87D, 	0x5708, 	0xD708, 	0x558F, 	0xD58F, 	0x5413, 	0xD413, 	0x5294, 	0xD294, 	0x5111, 	0xD111, 	0x4F8C, 	0xCF8C, 	0x4E05, 	0xCE05, 
0x4C7C, 	0xCC7C, 	0x4A80, 	0xCA80, 	0x4964, 	0xC964, 	0x47D5, 	0xC7D5, 	0x4645, 	0xC645, 	0x44B5, 	0xC4B5, 	0x4323, 	0xC323, 	0x4192, 	0xC192, 
0x4000, 	0xC000, 	0x3E6E, 	0xBE6E, 	0x3CDC, 	0xBCDC, 	0x3B4B, 	0xBB4B, 	0x39BA, 	0xB9BA, 	0x382A, 	0xB82A, 	0x369C, 	0xB69C, 	0x350F, 	0xB50F, 
0x3384, 	0xB384, 	0x31FA, 	0xB1FA, 	0x3073, 	0xB073, 	0x2EEE, 	0xAEEE, 	0x2D6C, 	0xAD6C, 	0x2BED, 	0xABED, 	0x2A70, 	0xAA70, 	0x28F7, 	0xA8F7, 
0x2782, 	0xA782, 	0x2611, 	0xA611, 	0x24A3, 	0xA4A3, 	0x233A, 	0xA33A, 	0x21D5, 	0xA1D5, 	0x2074, 	0xA074, 	0x1F19, 	0x9F19, 	0x1DC3, 	0x9DC3, 
0x1C72, 	0x9C72, 	0x1B26, 	0x9B26, 	0x19E0, 	0x99E0, 	0x18A0, 	0x98A0, 	0x1766, 	0x9766, 	0x1632, 	0x9632, 	0x1505, 	0x9505, 	0x13DF, 	0x93DF, 
0x12BF, 	0x92BF, 	0x11A6, 	0x91A6, 	0x1094, 	0x9094, 	0x0F8A, 	0x8F8A, 	0x0E87, 	0x8E87, 	0x0D8C, 	0x8D8C, 	0x0C98, 	0x8C98, 	0x0BAD, 	0x8BAD, 
0x0AC9, 	0x8AC9, 	0x09EE, 	0x89EE, 	0x091B, 	0x891B, 	0x0850, 	0x8850, 	0x078F, 	0x878F, 	0x06D5, 	0x86D5, 	0x0625, 	0x8625, 	0x057E, 	0x857E, 
0x04DF, 	0x84DF, 	0x044A, 	0x844A, 	0x03BE, 	0x83BE, 	0x033B, 	0x833B, 	0x02C1, 	0x82C1, 	0x0251, 	0x8251, 	0x01EB, 	0x81EB, 	0x018E, 	0x818E, 
0x013B, 	0x813B, 	0x00F1, 	0x80F1, 	0x00B1, 	0x80B1, 	0x007B, 	0x807B, 	0x004F, 	0x804F, 	0x002C, 	0x802C, 	0x0014, 	0x8014, 	0x0005, 	0x8005, 
0x0000, 	0x8000, 	0x0005, 	0x8005, 	0x0014, 	0x8014, 	0x002C, 	0x802C, 	0x004F, 	0x804F, 	0x007B, 	0x807B, 	0x00B1, 	0x80B1, 	0x00F1, 	0x80F1, 
0x013B, 	0x813B, 	0x018E, 	0x818E, 	0x01EB, 	0x81EB, 	0x0251, 	0x8251, 	0x02C1, 	0x82C1, 	0x033B, 	0x833B, 	0x03BE, 	0x83BE, 	0x044A, 	0x844A, 
0x04DF, 	0x84DF, 	0x057E, 	0x857E, 	0x0625, 	0x8625, 	0x06D5, 	0x86D5, 	0x078F, 	0x878F, 	0x0850, 	0x8850, 	0x091B, 	0x891B, 	0x09EE, 	0x89EE, 
0x0AC9, 	0x8AC9, 	0x0BAD, 	0x8BAD, 	0x0C98, 	0x8C98, 	0x0D8C, 	0x8D8C, 	0x0E87, 	0x8E87, 	0x0F8A, 	0x8F8A, 	0x1094, 	0x9094, 	0x11A6, 	0x91A6, 
0x12BF, 	0x92BF, 	0x13DF, 	0x93DF, 	0x1505, 	0x9505, 	0x1632, 	0x9632, 	0x1766, 	0x9766, 	0x18A0, 	0x98A0, 	0x19E0, 	0x99E0, 	0x1B26, 	0x9B26, 
0x1C72, 	0x9C72, 	0x1DC3, 	0x9DC3, 	0x1F19, 	0x9F19, 	0x2074, 	0xA074, 	0x21D5, 	0xA1D5, 	0x233A, 	0xA33A, 	0x24A3, 	0xA4A3, 	0x2611, 	0xA611, 
0x2782, 	0xA782, 	0x28F7, 	0xA8F7, 	0x2A70, 	0xAA70, 	0x2BED, 	0xABED, 	0x2D6C, 	0xAD6C, 	0x2EEE, 	0xAEEE, 	0x3073, 	0xB073, 	0x31FA, 	0xB1FA, 
0x3384, 	0xB384, 	0x350F, 	0xB50F, 	0x369C, 	0xB69C, 	0x382A, 	0xB82A, 	0x39BA, 	0xB9BA, 	0x3B4B, 	0xBB4B, 	0x3CDC, 	0xBCDC, 	0x3E6E, 	0xBE6E
};
												
uint16_t sine10b[] = {
0x0200, 	0x020C, 	0x0219, 	0x0225, 	0x0232, 	0x023E, 	0x024B, 	0x0254, 
0x0263, 	0x0270, 	0x027C, 	0x0288, 	0x0294, 	0x02A0, 	0x02AC, 	0x02B8, 
0x02C3, 	0x02CF, 	0x02DA, 	0x02E6, 	0x02F1, 	0x02FC, 	0x0307, 	0x0311, 
0x031C, 	0x0326, 	0x0330, 	0x033A, 	0x0344, 	0x034E, 	0x0357, 	0x0361, 
0x036A, 	0x0372, 	0x037B, 	0x0383, 	0x038B, 	0x0393, 	0x039B, 	0x03A2, 
0x03A9, 	0x03B0, 	0x03B7, 	0x03BD, 	0x03C0, 	0x03C9, 	0x03CE, 	0x03D4, 
0x03D9, 	0x03DD, 	0x03E2, 	0x03E6, 	0x03E9, 	0x03ED, 	0x03F0, 	0x03F3, 
0x03F6, 	0x03F8, 	0x03FA, 	0x03FC, 	0x03FD, 	0x03FE, 	0x03FF, 	0x03FF, 
0x03FF, 	0x03FF, 	0x03FF, 	0x03FE, 	0x03FD, 	0x03FC, 	0x03FA, 	0x03F8, 
0x03F6, 	0x03F3, 	0x03F0, 	0x03ED, 	0x03E9, 	0x03E6, 	0x03E2, 	0x03DD, 
0x03D9, 	0x03D4, 	0x03CE, 	0x03C9, 	0x03C0, 	0x03BD, 	0x03B7, 	0x03B0, 
0x03A9, 	0x03A2, 	0x039B, 	0x0393, 	0x038B, 	0x0383, 	0x037B, 	0x0372, 
0x036A, 	0x0361, 	0x0357, 	0x034E, 	0x0344, 	0x033A, 	0x0330, 	0x0326, 
0x031C, 	0x0311, 	0x0307, 	0x02FC, 	0x02F1, 	0x02E6, 	0x02DA, 	0x02CF, 
0x02C3, 	0x02B8, 	0x02AC, 	0x02A0, 	0x0294, 	0x0288, 	0x027C, 	0x0270, 
0x0263, 	0x0254, 	0x024B, 	0x023E, 	0x0232, 	0x0225, 	0x0219, 	0x020C, 
0x0200, 	0x01F3, 	0x01E6, 	0x01DA, 	0x01CD, 	0x01C1, 	0x01B4, 	0x01A8, 
0x019C, 	0x018F, 	0x0183, 	0x0177, 	0x016B, 	0x015F, 	0x0153, 	0x0147, 
0x013C, 	0x0130, 	0x0125, 	0x0119, 	0x010E, 	0x0103, 	0x00F8, 	0x00EE, 
0x00E3, 	0x00D9, 	0x00CF, 	0x00C5, 	0x00BB, 	0x00B1, 	0x00A8, 	0x009E, 
0x0095, 	0x008D, 	0x0084, 	0x007C, 	0x0074, 	0x006C, 	0x0064, 	0x005D, 
0x0056, 	0x004F, 	0x0048, 	0x0042, 	0x003C, 	0x0036, 	0x0031, 	0x002B, 
0x0026, 	0x0022, 	0x001D, 	0x0019, 	0x0016, 	0x0012, 	0x000F, 	0x000C, 
0x0009, 	0x0007, 	0x0005, 	0x0003, 	0x0002, 	0x0001, 	0x0000, 	0x0000, 
0x0000, 	0x0000, 	0x0000, 	0x0001, 	0x0002, 	0x0003, 	0x0005, 	0x0007, 
0x0009, 	0x000C, 	0x000F, 	0x0012, 	0x0016, 	0x0019, 	0x001D, 	0x0022, 
0x0026, 	0x002B, 	0x0031, 	0x0036, 	0x003C, 	0x0042, 	0x0048, 	0x004F, 
0x0056, 	0x005D, 	0x0064, 	0x006C, 	0x0074, 	0x007C, 	0x0084, 	0x008D, 
0x0095, 	0x009E, 	0x00A8, 	0x00B1, 	0x00BB, 	0x00C5, 	0x00CF, 	0x00D9, 
0x00E3, 	0x00EE, 	0x00F8, 	0x0103, 	0x010E, 	0x0119, 	0x0125, 	0x0130, 
0x013C, 	0x0147, 	0x0153, 	0x015F, 	0x016B, 	0x0177, 	0x0183, 	0x018F, 
0x019C, 	0x01A8, 	0x01B4, 	0x01C1, 	0x01CD, 	0x01DA, 	0x01E6, 	0x01F3
};

uint16_t sine10b_500Hz[] = {
0x0200, 0x0263, 0x02C3, 0x031C, 0x036A, 0x03A9, 0x03D9, 0x03F6,
0x03FF, 0x03F6, 0x03D9, 0x03A9, 0x036A, 0x031C, 0x02C3, 0x0263, 
0x0200, 0x019C, 0x013C, 0x00E3, 0x0095, 0x0056, 0x0026, 0x0009,
0x0000, 0x0009, 0x0026, 0x0056, 0x0095, 0x00E3, 0x013C, 0x019C
};

uint16_t sine10b_1kHz[] = {
0x0200, 0x02C3, 0x036A, 0x03D9,
0x03FF, 0x03D9, 0x036A, 0x02C3, 
0x0200, 0x013C, 0x0095, 0x0026,
0x0000, 0x0026, 0x0095, 0x013C
};

uint16_t sine10b_1kHz_inter[] = {
0x0200, 0x8200, 0x02C3, 0x82C3, 0x036A, 0x836A, 0x03D9, 0x83D9,
0x03FF, 0x83FF, 0x03D9, 0x83D9, 0x036A, 0x836A, 0x02C3, 0x82C3, 
0x0200, 0x8200, 0x013C, 0x813C, 0x0095, 0x8095, 0x0026, 0x8026,
0x0000, 0x8000, 0x0026, 0x8026, 0x0095, 0x8095, 0x013C, 0x813C
};