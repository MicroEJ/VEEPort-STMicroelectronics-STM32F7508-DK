/*
 * C
 *
 * Copyright 2021 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef _MISRA_2004_CONF_H
#define _MISRA_2004_CONF_H

#ifdef __ICCARM__

#define MISRA_2004_DISABLE_ALL               _Pragma ("diag_suppress= \
    Pm001,Pm002,Pm003,Pm004,Pm005,Pm006,Pm007,Pm008,Pm009,Pm010,Pm011,\
    Pm012,Pm013,Pm014,Pm015,Pm016,Pm017,Pm018,Pm019,Pm020,Pm021,Pm022,\
    Pm023,Pm024,Pm025,Pm026,Pm027,Pm028,Pm029,Pm030,Pm031,Pm032,Pm033,\
    Pm034,Pm035,Pm036,Pm037,Pm038,Pm039,Pm040,Pm041,Pm042,Pm043,Pm044,\
    Pm045,Pm046,Pm047,Pm048,Pm049,Pm050,Pm051,Pm052,Pm053,Pm054,Pm055,\
    Pm056,Pm057,Pm058,Pm059,Pm060,Pm061,Pm062,Pm063,Pm064,Pm065,Pm066,\
    Pm067,Pm068,Pm069,Pm070,Pm071,Pm072,Pm073,Pm074,Pm075,Pm076,Pm077,\
    Pm078,Pm079,Pm080,Pm081,Pm082,Pm083,Pm084,Pm085,Pm086,Pm087,Pm088,\
    Pm089,Pm090,Pm091,Pm092,Pm093,Pm094,Pm095,Pm096,Pm097,Pm098,Pm099,\
    Pm100,Pm101,Pm102,Pm103,Pm104,Pm105,Pm106,Pm107,Pm108,Pm109,Pm110,\
    Pm111,Pm112,Pm113,Pm114,Pm115,Pm116,Pm117,Pm118,Pm119,Pm120,Pm121,\
    Pm122,Pm123,Pm124,Pm125,Pm126,Pm127,Pm128,Pm129,Pm130,Pm131,Pm132,\
    Pm133,Pm134,Pm135,Pm136,Pm137,Pm138,Pm139,Pm140,Pm141,Pm142,Pm143,\
    Pm144,Pm145,Pm146,Pm147,Pm148,Pm149,Pm150,Pm151,Pm152,Pm153,Pm154,\
    Pm155")

#define MISRA_2004_ENABLE_ALL                 _Pragma ("diag_default= \
    Pm001,Pm002,Pm003,Pm004,Pm005,Pm006,Pm007,Pm008,Pm009,Pm010,\
    Pm012,Pm013,Pm014,Pm015,Pm016,Pm017,Pm018,Pm019,Pm020,Pm021,Pm022,\
    Pm023,Pm024,Pm025,Pm026,Pm027,Pm028,Pm029,Pm030,Pm031,Pm032,Pm033,\
    Pm034,Pm035,Pm036,Pm037,Pm038,Pm039,Pm040,Pm041,Pm042,Pm043,Pm044,\
    Pm045,Pm046,Pm047,Pm048,Pm049,Pm050,Pm051,Pm052,Pm053,Pm054,Pm055,\
    Pm056,Pm057,Pm058,Pm059,Pm060,Pm061,Pm062,Pm063,Pm064,Pm065,Pm066,\
    Pm067,Pm068,Pm069,Pm070,Pm071,Pm072,Pm073,Pm074,Pm075,Pm076,Pm077,\
    Pm078,Pm079,Pm080,Pm081,Pm082,Pm083,Pm084,Pm085,Pm086,Pm087,Pm088,\
    Pm089,Pm090,Pm091,Pm092,Pm093,Pm094,Pm095,Pm096,Pm097,Pm098,Pm099,\
    Pm100,Pm101,Pm102,Pm103,Pm104,Pm105,Pm106,Pm107,Pm108,Pm109,Pm110,\
    Pm111,Pm112,Pm113,Pm114,Pm115,Pm116,Pm117,Pm118,Pm119,Pm120,Pm121,\
    Pm122,Pm123,Pm124,Pm125,Pm126,Pm127,Pm128,Pm129,Pm130,Pm131,Pm132,\
    Pm133,Pm134,Pm135,Pm136,Pm137,Pm138,Pm139,Pm140,Pm141,Pm142,Pm143,\
    Pm144,Pm145,Pm146,Pm147,Pm148,Pm149,Pm150,Pm151,Pm152,Pm153,Pm154,\
    Pm155")  
      
MISRA_2004_ENABLE_ALL

/* Project level MISRA C settings
 *
 *	Suppressed messages:
 *
 *	IAR diagnostic message Pm011
 *	MISRA C 2004 rule 6.3
 *	Typedefs that indicate size and signedness should be used in place of the basic numerical types.
 *	This advisory MISRA rule is temporarily suppressed as the boolean type definition (from <stdbool.h>) used to interface with LLMJVM does not meet this criterion.
 *	It might be possible to define a custom-made boolean type (such as typedef uint32_t bool) which would solve this issue.
 * 
 *	IAR diagnostic message Pm140
 *	MISRA rule 11.3
 *	pvTimerGetTimerID returns a void* which has to be cast to an integer so as to be compared against the timer id, therefore MISRA rule 11.3 is broken.
 * 
 */
 
#define MISRA_2004_DISABLE_RULE_6_3           _Pragma ("diag_suppress=Pm011")
#define MISRA_2004_DISABLE_RULE_10_6          _Pragma ("diag_suppress=Pm127")
#define MISRA_2004_DISABLE_RULE_11_3          _Pragma ("diag_suppress=Pm140")
#define MISRA_2004_DISABLE_RULE_14_3          _Pragma ("diag_suppress=Pm050")
#define MISRA_2004_DISABLE_RULE_16_4          _Pragma ("diag_suppress=Pm069")

#else /* Disable MISRA rules check by IAR */

#define MISRA_2004_DISABLE_ALL
#define MISRA_2004_ENABLE_ALL
#define MISRA_2004_DISABLE_RULE_6_3
#define MISRA_2004_DISABLE_RULE_10_6
#define MISRA_2004_DISABLE_RULE_11_3
#define MISRA_2004_DISABLE_RULE_14_3
#define MISRA_2004_DISABLE_RULE_16_4

#endif /* __ICCARM__ */

#endif /* _MISRA_2004_CONF_H */
