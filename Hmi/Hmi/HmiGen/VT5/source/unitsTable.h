#ifndef _UNITS_H
#define _UNITS_H

#pragma diag_suppress=Pm011

const char * const unitsTable_1[] =
{
"m3/s",   //13
"m3/min",   //14
"m3/h",   //15
"m3/d",   //16
"l/s",   //48
"l/min",   //49
"l/h",   //50
"l/d",   //51
"USgal/s",   //63
"USgal/min",   //64
"USgal/h",   //65
"USgal/d",   //66
"ft3/s",   //29
"ft3/min",   //30
"ft3/h",   //31
"ft3/d",   //32
"kl/s",   //52
"kl/min",   //60
"kl/h",   //61
"kl/d",   //62
"impgal/s",   //83
"impgal/min",   //84
"impgal/h",   //85
"impgal/d",   //86
"bbl/s",   //104
"bbl/min",   //105
"bbl/h",   //106
"bbl/d",   //107
"kft3/s",   //35
"kft3/min",   //36
"kft3/h",   //37
"kft3/d",   //38
"hl/s",   //56
"hl/min",   //57
"hl/h",   //58
"hl/d",   //59
"bls/d",   //127
"bls/h",   //128
"bls/min",   //129
"bls/s",   //130
"\0"
};

const TUSIGN16 unitsCrossReference_1[] =
{
13,   //"m3/s"
14,   //"m3/min"
15,   //"m3/h"
16,   //"m3/d"
48,   //"l/s"
49,   //"l/min"
50,   //"l/h"
51,   //"l/d"
63,   //"USgal/s"
64,   //"USgal/min"
65,   //"USgal/h"
66,   //"USgal/d"
29,   //"ft3/s"
30,   //"ft3/min"
31,   //"ft3/h"
32,   //"ft3/d"
52,   //"kl/s"
60,   //"kl/min"
61,   //"kl/h"
62,   //"kl/d"
83,   //"impgal/s"
84,   //"impgal/min"
85,   //"impgal/h"
86,   //"impgal/d"
104,   //"bbl/s"
105,   //"bbl/min"
106,   //"bbl/h"
107,   //"bbl/d"
35,   //"kft3/s"
36,   //"kft3/min"
37,   //"kft3/h"
38,   //"kft3/d"
56,   //"hl/s"
57,   //"hl/min"
58,   //"hl/h"
59,   //"hl/d"
127,   //"bls/d"
128,   //"bls/h"
129,   //"bls/min"
130,   //"bls/s"
};

#define MAX_UNITS_1 40

const char * const unitsTable_2[] =
{
"g/s",   //1
"g/min",   //2
"g/h",   //3
"g/d",   //4
"kg/s",   //5
"kg/min",   //6
"kg/h",   //7
"kg/d",   //8
"t/s",   //9
"t/min",   //10
"t/h",   //11
"t/d",   //12
"lb/s",   //13
"lb/min",   //14
"lb/h",   //15
"lb/d",   //16
"US t/s",   //17
"US t/min",   //18
"US t/h",   //19
"US t/d",   //20
"UK t/s",   //21
"UK t/min",   //22
"UK t/h",   //23
"UK t/d",   //24
"kip/d",   //25
"kip/h",   //26
"kip/min",   //27
"kip/s",   //28
"oz/s",   //29
"oz/min",   //30
"oz/h",   //31
"oz/d",   //32
"\0"
};

const TUSIGN16 unitsCrossReference_2[] =
{
1,   //"g/s"
2,   //"g/min"
3,   //"g/h"
4,   //"g/d"
5,   //"kg/s"
6,   //"kg/min"
7,   //"kg/h"
8,   //"kg/d"
9,   //"t/s"
10,   //"t/min"
11,   //"t/h"
12,   //"t/d"
13,   //"lb/s"
14,   //"lb/min"
15,   //"lb/h"
16,   //"lb/d"
17,   //"US t/s"
18,   //"US t/min"
19,   //"US t/h"
20,   //"US t/d"
21,   //"UK t/s"
22,   //"UK t/min"
23,   //"UK t/h"
24,   //"UK t/d"
25,   //"kip/d"
26,   //"kip/h"
27,   //"kip/min"
28,   //"kip/s"
29,   //"oz/s"
30,   //"oz/min"
31,   //"oz/h"
32,   //"oz/d"
};

#define MAX_UNITS_2 32

const char * const unitsTable_3[] =
{
"W",   //1
"TW",   //2
"GW",   //3
"MW",   //4
"kW",   //5
"mW",   //6
"uW",   //7
"nW",   //8
"pW",   //9
"hp",   //10
"kcal/s",   //11
"kcal/min",   //12
"kcal/h",   //13
"kcal/d",   //14
"Mcal/s",   //15
"Mcal/min",   //16
"Mcal/h",   //17
"Mcal/s",   //18
"kJ/s",   //19
"kJ/min",   //20
"kJ/h",   //21
"kJ/d",   //22
"MJ/s",   //23
"MJ/min",   //24
"MJ/h",   //25
"MJ/d",   //26
"BTU/s",   //27
"BTU/min",   //28
"BTU/h",   //29
"BTU/d",   //30
"MBTU/s",   //31
"MBTU/min",   //32
"MBTU/h",   //33
"MBTU/d",   //34
"MMBTU/s",   //35
"MMBTU/min",   //36
"MMBTU/h",   //37
"MMBTU/d",   //38
"\0"
};

const TUSIGN16 unitsCrossReference_3[] =
{
1,   //"W"
2,   //"TW"
3,   //"GW"
4,   //"MW"
5,   //"kW"
6,   //"mW"
7,   //"uW"
8,   //"nW"
9,   //"pW"
10,   //"hp"
11,   //"kcal/s"
12,   //"kcal/min"
13,   //"kcal/h"
14,   //"kcal/d"
15,   //"Mcal/s"
16,   //"Mcal/min"
17,   //"Mcal/h"
18,   //"Mcal/s"
19,   //"kJ/s"
20,   //"kJ/min"
21,   //"kJ/h"
22,   //"kJ/d"
23,   //"MJ/s"
24,   //"MJ/min"
25,   //"MJ/h"
26,   //"MJ/d"
27,   //"BTU/s"
28,   //"BTU/min"
29,   //"BTU/h"
30,   //"BTU/d"
31,   //"MBTU/s"
32,   //"MBTU/min"
33,   //"MBTU/h"
34,   //"MBTU/d"
35,   //"MMBTU/s"
36,   //"MMBTU/min"
37,   //"MMBTU/h"
38,   //"MMBTU/d"
};

#define MAX_UNITS_3 38

const char * const unitsTable_4[] =
{
"kg/m3",   //1
"Mg/m3",   //2
"g/cm3",   //3
"g/m3",   //4
"t/m3",   //5
"kg/l",   //6
"g/ml",   //7
"g/l",   //8
"lb/in3",   //9
"lb/ft3",   //10
"lb/USgal",   //11
"US t/yd3",   //12
"lb/gal",   //13
"ug/l",   //14
"ug/m3",   //15
"mg/l",   //16
"\0"
};

const TUSIGN16 unitsCrossReference_4[] =
{
1,   //"kg/m3"
2,   //"Mg/m3"
3,   //"g/cm3"
4,   //"g/m3"
5,   //"t/m3"
6,   //"kg/l"
7,   //"g/ml"
8,   //"g/l"
9,   //"lb/in3"
10,   //"lb/ft3"
11,   //"lb/USgal"
12,   //"US t/yd3"
13,   //"lb/gal"
14,   //"ug/l"
15,   //"ug/m3"
16,   //"mg/l"
};

#define MAX_UNITS_4 16

const char * const unitsTable_5[] =
{
"K",   //1
"\xF8""C",   //2
"\xF8""F",   //3
"\0"
};

const TUSIGN16 unitsCrossReference_5[] =
{
1,   //"K"
2,   //"\xF8""C"
3,   //"\xF8""F"
};

#define MAX_UNITS_5 3

const char * const unitsTable_6[] =
{
"Pa",   //1
"MPa",   //3
"kPa",   //4
"hPa",   //7
"bar",   //8
"mbar",   //9
"psi",   //65
"kg/cm2",   //69
"mm H2O",   //53
"\0"
};

const TUSIGN16 unitsCrossReference_6[] =
{
1,   //"Pa"
3,   //"MPa"
4,   //"kPa"
7,   //"hPa"
8,   //"bar"
9,   //"mbar"
65,   //"psi"
69,   //"kg/cm2"
53,   //"mm H2O"
};

#define MAX_UNITS_6 9

const char * const unitsTable_7[] =
{
"Hz",   //1
"KHz",   //5
"\0"
};

const TUSIGN16 unitsCrossReference_7[] =
{
1,   //"Hz"
5,   //"KHz"
};

#define MAX_UNITS_7 2

const char * const unitsTable_8[] =
{
"s",   //1
"ks",   //2
"ms",   //3
"us",   //4
"min",   //5
"h",   //6
"d",   //7
"\0"
};

const TUSIGN16 unitsCrossReference_8[] =
{
1,   //"s"
2,   //"ks"
3,   //"ms"
4,   //"us"
5,   //"min"
6,   //"h"
7,   //"d"
};

#define MAX_UNITS_8 7

const char * const unitsTable_9[] =
{
"m3",   //1
"dm3",   //2
"cm3",   //3
"mm3",   //4
"Mm3",   //5
"in3",   //6
"ft3",   //7
"yd3",   //8
"mi3",   //9
"Mft3",   //10
"l",   //11
"cl",   //12
"ml",   //13
"hl",   //14
"kl",   //15
"Ml",   //16
"pt",   //17
"qt",   //18
"US gal",   //19
"imp gal",   //20
"bsh",   //21
"bbl",   //22
"bbl lqd",   //23
"bbl fed",   //24
"kgal",   //27
"imp kgal",   //28
"megabarrel",   //29
"ac in",   //30
"ac ft",   //31
"Mgal",   //32
"\0"
};

const TUSIGN16 unitsCrossReference_9[] =
{
1,   //"m3"
2,   //"dm3"
3,   //"cm3"
4,   //"mm3"
5,   //"Mm3"
6,   //"in3"
7,   //"ft3"
8,   //"yd3"
9,   //"mi3"
10,   //"Mft3"
11,   //"l"
12,   //"cl"
13,   //"ml"
14,   //"hl"
15,   //"kl"
16,   //"Ml"
17,   //"pt"
18,   //"qt"
19,   //"US gal"
20,   //"imp gal"
21,   //"bsh"
22,   //"bbl"
23,   //"bbl lqd"
24,   //"bbl fed"
27,   //"kgal"
28,   //"imp kgal"
29,   //"megabarrel"
30,   //"ac in"
31,   //"ac ft"
32,   //"Mgal"
};

#define MAX_UNITS_9 30

const char * const unitsTable_10[] =
{
"kg",   //1
"g",   //2
"t",   //5
"oz",   //6
"lb",   //7
"mg",   //3
"Mg",   //4
"US t",   //8
"UK t",   //9
"\0"
};

const TUSIGN16 unitsCrossReference_10[] =
{
1,   //"kg"
2,   //"g"
5,   //"t"
6,   //"oz"
7,   //"lb"
3,   //"mg"
4,   //"Mg"
8,   //"US t"
9,   //"UK t"
};

#define MAX_UNITS_10 9

const char * const unitsTable_11[] =
{
"J",   //1
"MJ",   //6
"kJ",   //7
"mJ",   //8
"cal",   //9
"kcal",   //10
"Mcal",   //11
"Wh",   //15
"MWh",   //18
"kWh",   //19
"EJ",   //2
"PJ",   //3
"TJ",   //4
"GJ",   //5
"BTU",   //12
"decatherm",   //13
"MMBTU",   //14
"TWh",   //16
"GWh",   //17
"ft.lbf",   //20
"MBTU",   //21
"\0"
};

const TUSIGN16 unitsCrossReference_11[] =
{
1,   //"J"
6,   //"MJ"
7,   //"kJ"
8,   //"mJ"
9,   //"cal"
10,   //"kcal"
11,   //"Mcal"
15,   //"Wh"
18,   //"MWh"
19,   //"kWh"
2,   //"EJ"
3,   //"PJ"
4,   //"TJ"
5,   //"GJ"
12,   //"BTU"
13,   //"decatherm"
14,   //"MMBTU"
16,   //"TWh"
17,   //"GWh"
20,   //"ft.lbf"
21,   //"MBTU"
};

#define MAX_UNITS_11 21


#pragma diag_default=Pm011


#endif
