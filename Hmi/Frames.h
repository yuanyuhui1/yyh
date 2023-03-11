#ifndef _FRAMES_H
#define _FRAMES_H

#ifndef __SUBSYSTEM_IDX_H__
  #error subsystem_idx.h must be included
#endif

#ifndef _STRINGDEFINITIONS
//  #error StringDefinitions.h must be included
#endif

#ifndef _FRAMETABLEDEFINITIONS_H
  #error FrameTableDefinitions.h must be included
#endif

#ifndef _VALIDITYTEST_H
  #error validityTest.h must be included
#endif

#ifndef _SPECIALOPERATION_H
  #error specialOperation.h must be included
#endif

#ifndef __HMI_IDX_H__
  #error hmi_idx.h must be included
#endif


// Device specific include files...
#include "AnalogInput/Interface/AnalogInput_idx.h"
#include "ARM/Interface/arm_idx.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Diagnosis/interface/Diagnosis_idx.h"
#include "DigitalOutput/Interface/DigitalOutput_idx.h"
#include "ElettronicServices/Interface/ElettronicServices_idx.h"
#include "Ethernet/Interface/Ethernet_idx.h"
#include "FrontEndInterface/Interface/FrontEndInterface_idx.h"
#include "HART/Interface/Hart_idx.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Modbus/Interface/Modbus_idx.h"
#include "MVMeasurement/Interface/MVMeasurement_idx.h"
#include "ServicePort/Interface/ServicePort_idx.h"
#include "SVMeasurement/Interface/SVMeasurement_idx.h"
#include "Totalizer/Interface/Totalizer_idx.h"
#include "WebServer/Interface/WebServer_idx.h"

//+@ HMIGen HMIGen custom object include files @+

//-@ HMIGen HMIGen custom object include files @-


// forward Declarations of all frames
__root const FRAME_FORMAT_TYPE f_0;
__root const FRAME_FORMAT_TYPE f_43;
__root const FRAME_FORMAT_TYPE f_81;
__root const FRAME_FORMAT_TYPE f_82;
__root const FRAME_FORMAT_TYPE f_1;
__root const FRAME_FORMAT_TYPE f_5;
__root const FRAME_FORMAT_TYPE f_6;
__root const FRAME_FORMAT_TYPE f_2;
__root const FRAME_FORMAT_TYPE f_7;
__root const FRAME_FORMAT_TYPE f_8;
__root const FRAME_FORMAT_TYPE f_9;
__root const FRAME_FORMAT_TYPE f_10;
__root const FRAME_FORMAT_TYPE f_11;
__root const FRAME_FORMAT_TYPE f_141;
__root const FRAME_FORMAT_TYPE f_44;
__root const FRAME_FORMAT_TYPE f_4;
__root const FRAME_FORMAT_TYPE f_123;
__root const FRAME_FORMAT_TYPE f_124;
__root const FRAME_FORMAT_TYPE f_125;
__root const FRAME_FORMAT_TYPE f_126;
__root const FRAME_FORMAT_TYPE f_303;
__root const FRAME_FORMAT_TYPE f_352;
__root const FRAME_FORMAT_TYPE f_319;
__root const FRAME_FORMAT_TYPE f_318;
__root const FRAME_FORMAT_TYPE f_320;
__root const FRAME_FORMAT_TYPE f_324;
__root const FRAME_FORMAT_TYPE f_321;
__root const FRAME_FORMAT_TYPE f_322;
__root const FRAME_FORMAT_TYPE f_323;
__root const FRAME_FORMAT_TYPE f_326;
__root const FRAME_FORMAT_TYPE f_325;
__root const FRAME_FORMAT_TYPE f_328;
__root const FRAME_FORMAT_TYPE f_327;
__root const FRAME_FORMAT_TYPE f_304;
__root const FRAME_FORMAT_TYPE f_331;
__root const FRAME_FORMAT_TYPE f_332;
__root const FRAME_FORMAT_TYPE f_412;
__root const FRAME_FORMAT_TYPE f_413;
__root const FRAME_FORMAT_TYPE f_305;
__root const FRAME_FORMAT_TYPE f_306;
__root const FRAME_FORMAT_TYPE f_307;
__root const FRAME_FORMAT_TYPE f_308;
__root const FRAME_FORMAT_TYPE f_309;
__root const FRAME_FORMAT_TYPE f_310;
__root const FRAME_FORMAT_TYPE f_402;
__root const FRAME_FORMAT_TYPE f_468;
__root const FRAME_FORMAT_TYPE f_469;
__root const FRAME_FORMAT_TYPE f_127;
__root const FRAME_FORMAT_TYPE f_132;
__root const FRAME_FORMAT_TYPE f_133;
__root const FRAME_FORMAT_TYPE f_311;
__root const FRAME_FORMAT_TYPE f_313;
__root const FRAME_FORMAT_TYPE f_128;
__root const FRAME_FORMAT_TYPE f_312;
__root const FRAME_FORMAT_TYPE f_329;
__root const FRAME_FORMAT_TYPE f_129;
__root const FRAME_FORMAT_TYPE f_314;
__root const FRAME_FORMAT_TYPE f_473;
__root const FRAME_FORMAT_TYPE f_130;
__root const FRAME_FORMAT_TYPE f_45;
__root const FRAME_FORMAT_TYPE f_131;
__root const FRAME_FORMAT_TYPE f_78;
__root const FRAME_FORMAT_TYPE f_315;
__root const FRAME_FORMAT_TYPE f_316;
__root const FRAME_FORMAT_TYPE f_317;
__root const FRAME_FORMAT_TYPE f_21;
__root const FRAME_FORMAT_TYPE f_134;
__root const FRAME_FORMAT_TYPE f_293;
__root const FRAME_FORMAT_TYPE f_294;
__root const FRAME_FORMAT_TYPE f_135;
__root const FRAME_FORMAT_TYPE f_136;
__root const FRAME_FORMAT_TYPE f_330;
__root const FRAME_FORMAT_TYPE f_353;
__root const FRAME_FORMAT_TYPE f_354;
__root const FRAME_FORMAT_TYPE f_355;
__root const FRAME_FORMAT_TYPE f_351;
__root const FRAME_FORMAT_TYPE f_356;
__root const FRAME_FORMAT_TYPE f_357;
__root const FRAME_FORMAT_TYPE f_358;
__root const FRAME_FORMAT_TYPE f_359;
__root const FRAME_FORMAT_TYPE f_139;
__root const FRAME_FORMAT_TYPE f_46;
__root const FRAME_FORMAT_TYPE f_12;
__root const FRAME_FORMAT_TYPE f_15;
__root const FRAME_FORMAT_TYPE f_3;
__root const FRAME_FORMAT_TYPE f_68;
__root const FRAME_FORMAT_TYPE f_73;
__root const FRAME_FORMAT_TYPE f_19;
__root const FRAME_FORMAT_TYPE f_22;
__root const FRAME_FORMAT_TYPE f_90;
__root const FRAME_FORMAT_TYPE f_142;
__root const FRAME_FORMAT_TYPE f_13;
__root const FRAME_FORMAT_TYPE f_14;
__root const FRAME_FORMAT_TYPE f_16;
__root const FRAME_FORMAT_TYPE f_17;
__root const FRAME_FORMAT_TYPE f_18;
__root const FRAME_FORMAT_TYPE f_137;
__root const FRAME_FORMAT_TYPE f_28;
__root const FRAME_FORMAT_TYPE f_29;
__root const FRAME_FORMAT_TYPE f_36;
__root const FRAME_FORMAT_TYPE f_65;
__root const FRAME_FORMAT_TYPE f_66;
__root const FRAME_FORMAT_TYPE f_67;
__root const FRAME_FORMAT_TYPE f_113;
__root const FRAME_FORMAT_TYPE f_507;
__root const FRAME_FORMAT_TYPE f_508;
__root const FRAME_FORMAT_TYPE f_69;
__root const FRAME_FORMAT_TYPE f_70;
__root const FRAME_FORMAT_TYPE f_74;
__root const FRAME_FORMAT_TYPE f_80;
__root const FRAME_FORMAT_TYPE f_83;
__root const FRAME_FORMAT_TYPE f_75;
__root const FRAME_FORMAT_TYPE f_91;
__root const FRAME_FORMAT_TYPE f_118;
__root const FRAME_FORMAT_TYPE f_477;
__root const FRAME_FORMAT_TYPE f_23;
__root const FRAME_FORMAT_TYPE f_24;
__root const FRAME_FORMAT_TYPE f_76;
__root const FRAME_FORMAT_TYPE f_290;
__root const FRAME_FORMAT_TYPE f_122;
__root const FRAME_FORMAT_TYPE f_168;
__root const FRAME_FORMAT_TYPE f_167;
__root const FRAME_FORMAT_TYPE f_291;
__root const FRAME_FORMAT_TYPE f_169;
__root const FRAME_FORMAT_TYPE f_196;
__root const FRAME_FORMAT_TYPE f_292;
__root const FRAME_FORMAT_TYPE f_195;
__root const FRAME_FORMAT_TYPE f_170;
__root const FRAME_FORMAT_TYPE f_171;
__root const FRAME_FORMAT_TYPE f_172;
__root const FRAME_FORMAT_TYPE f_198;
__root const FRAME_FORMAT_TYPE f_197;
__root const FRAME_FORMAT_TYPE f_199;
__root const FRAME_FORMAT_TYPE f_203;
__root const FRAME_FORMAT_TYPE f_417;
__root const FRAME_FORMAT_TYPE f_425;
__root const FRAME_FORMAT_TYPE f_433;
__root const FRAME_FORMAT_TYPE f_104;
__root const FRAME_FORMAT_TYPE f_445;
__root const FRAME_FORMAT_TYPE f_143;
__root const FRAME_FORMAT_TYPE f_144;
__root const FRAME_FORMAT_TYPE f_278;
__root const FRAME_FORMAT_TYPE f_206;
__root const FRAME_FORMAT_TYPE f_207;
__root const FRAME_FORMAT_TYPE f_208;
__root const FRAME_FORMAT_TYPE f_334;
__root const FRAME_FORMAT_TYPE f_26;
__root const FRAME_FORMAT_TYPE f_25;
__root const FRAME_FORMAT_TYPE f_77;
__root const FRAME_FORMAT_TYPE f_47;
__root const FRAME_FORMAT_TYPE f_188;
__root const FRAME_FORMAT_TYPE f_295;
__root const FRAME_FORMAT_TYPE f_189;
__root const FRAME_FORMAT_TYPE f_296;
__root const FRAME_FORMAT_TYPE f_190;
__root const FRAME_FORMAT_TYPE f_27;
__root const FRAME_FORMAT_TYPE f_147;
__root const FRAME_FORMAT_TYPE f_148;
__root const FRAME_FORMAT_TYPE f_146;
__root const FRAME_FORMAT_TYPE f_153;
__root const FRAME_FORMAT_TYPE f_333;
__root const FRAME_FORMAT_TYPE f_150;
__root const FRAME_FORMAT_TYPE f_154;
__root const FRAME_FORMAT_TYPE f_162;
__root const FRAME_FORMAT_TYPE f_158;
__root const FRAME_FORMAT_TYPE f_163;
__root const FRAME_FORMAT_TYPE f_164;
__root const FRAME_FORMAT_TYPE f_165;
__root const FRAME_FORMAT_TYPE f_166;
__root const FRAME_FORMAT_TYPE f_20;
__root const FRAME_FORMAT_TYPE f_117;
__root const FRAME_FORMAT_TYPE f_476;
__root const FRAME_FORMAT_TYPE f_48;
__root const FRAME_FORMAT_TYPE f_286;
__root const FRAME_FORMAT_TYPE f_174;
__root const FRAME_FORMAT_TYPE f_175;
__root const FRAME_FORMAT_TYPE f_288;
__root const FRAME_FORMAT_TYPE f_176;
__root const FRAME_FORMAT_TYPE f_49;
__root const FRAME_FORMAT_TYPE f_287;
__root const FRAME_FORMAT_TYPE f_178;
__root const FRAME_FORMAT_TYPE f_177;
__root const FRAME_FORMAT_TYPE f_289;
__root const FRAME_FORMAT_TYPE f_179;
__root const FRAME_FORMAT_TYPE f_180;
__root const FRAME_FORMAT_TYPE f_181;
__root const FRAME_FORMAT_TYPE f_50;
__root const FRAME_FORMAT_TYPE f_372;
__root const FRAME_FORMAT_TYPE f_373;
__root const FRAME_FORMAT_TYPE f_375;
__root const FRAME_FORMAT_TYPE f_371;
__root const FRAME_FORMAT_TYPE f_374;
__root const FRAME_FORMAT_TYPE f_376;
__root const FRAME_FORMAT_TYPE f_377;
__root const FRAME_FORMAT_TYPE f_378;
__root const FRAME_FORMAT_TYPE f_297;
__root const FRAME_FORMAT_TYPE f_51;
__root const FRAME_FORMAT_TYPE f_52;
__root const FRAME_FORMAT_TYPE f_30;
__root const FRAME_FORMAT_TYPE f_37;
__root const FRAME_FORMAT_TYPE f_53;
__root const FRAME_FORMAT_TYPE f_59;
__root const FRAME_FORMAT_TYPE f_120;
__root const FRAME_FORMAT_TYPE f_71;
__root const FRAME_FORMAT_TYPE f_72;
__root const FRAME_FORMAT_TYPE f_182;
__root const FRAME_FORMAT_TYPE f_121;
__root const FRAME_FORMAT_TYPE f_401;
__root const FRAME_FORMAT_TYPE f_404;
__root const FRAME_FORMAT_TYPE f_405;
__root const FRAME_FORMAT_TYPE f_414;
__root const FRAME_FORMAT_TYPE f_415;
__root const FRAME_FORMAT_TYPE f_406;
__root const FRAME_FORMAT_TYPE f_407;
__root const FRAME_FORMAT_TYPE f_408;
__root const FRAME_FORMAT_TYPE f_409;
__root const FRAME_FORMAT_TYPE f_410;
__root const FRAME_FORMAT_TYPE f_411;
__root const FRAME_FORMAT_TYPE f_403;
__root const FRAME_FORMAT_TYPE f_466;
__root const FRAME_FORMAT_TYPE f_467;
__root const FRAME_FORMAT_TYPE f_472;
__root const FRAME_FORMAT_TYPE f_84;
__root const FRAME_FORMAT_TYPE f_85;
__root const FRAME_FORMAT_TYPE f_86;
__root const FRAME_FORMAT_TYPE f_87;
__root const FRAME_FORMAT_TYPE f_88;
__root const FRAME_FORMAT_TYPE f_89;
__root const FRAME_FORMAT_TYPE f_298;
__root const FRAME_FORMAT_TYPE f_114;
__root const FRAME_FORMAT_TYPE f_115;
__root const FRAME_FORMAT_TYPE f_116;
__root const FRAME_FORMAT_TYPE f_92;
__root const FRAME_FORMAT_TYPE f_93;
__root const FRAME_FORMAT_TYPE f_94;
__root const FRAME_FORMAT_TYPE f_95;
__root const FRAME_FORMAT_TYPE f_96;
__root const FRAME_FORMAT_TYPE f_119;
__root const FRAME_FORMAT_TYPE f_348;
__root const FRAME_FORMAT_TYPE f_277;
__root const FRAME_FORMAT_TYPE f_478;
__root const FRAME_FORMAT_TYPE f_479;
__root const FRAME_FORMAT_TYPE f_480;
__root const FRAME_FORMAT_TYPE f_481;
__root const FRAME_FORMAT_TYPE f_482;
__root const FRAME_FORMAT_TYPE f_483;
__root const FRAME_FORMAT_TYPE f_140;
__root const FRAME_FORMAT_TYPE f_191;
__root const FRAME_FORMAT_TYPE f_302;
__root const FRAME_FORMAT_TYPE f_192;
__root const FRAME_FORMAT_TYPE f_193;
__root const FRAME_FORMAT_TYPE f_194;
__root const FRAME_FORMAT_TYPE f_200;
__root const FRAME_FORMAT_TYPE f_201;
__root const FRAME_FORMAT_TYPE f_202;
__root const FRAME_FORMAT_TYPE f_418;
__root const FRAME_FORMAT_TYPE f_419;
__root const FRAME_FORMAT_TYPE f_420;
__root const FRAME_FORMAT_TYPE f_421;
__root const FRAME_FORMAT_TYPE f_422;
__root const FRAME_FORMAT_TYPE f_423;
__root const FRAME_FORMAT_TYPE f_424;
__root const FRAME_FORMAT_TYPE f_426;
__root const FRAME_FORMAT_TYPE f_427;
__root const FRAME_FORMAT_TYPE f_428;
__root const FRAME_FORMAT_TYPE f_429;
__root const FRAME_FORMAT_TYPE f_430;
__root const FRAME_FORMAT_TYPE f_431;
__root const FRAME_FORMAT_TYPE f_432;
__root const FRAME_FORMAT_TYPE f_434;
__root const FRAME_FORMAT_TYPE f_435;
__root const FRAME_FORMAT_TYPE f_436;
__root const FRAME_FORMAT_TYPE f_437;
__root const FRAME_FORMAT_TYPE f_438;
__root const FRAME_FORMAT_TYPE f_439;
__root const FRAME_FORMAT_TYPE f_440;
__root const FRAME_FORMAT_TYPE f_97;
__root const FRAME_FORMAT_TYPE f_105;
__root const FRAME_FORMAT_TYPE f_446;
__root const FRAME_FORMAT_TYPE f_449;
__root const FRAME_FORMAT_TYPE f_452;
__root const FRAME_FORMAT_TYPE f_455;
__root const FRAME_FORMAT_TYPE f_458;
__root const FRAME_FORMAT_TYPE f_461;
__root const FRAME_FORMAT_TYPE f_204;
__root const FRAME_FORMAT_TYPE f_273;
__root const FRAME_FORMAT_TYPE f_274;
__root const FRAME_FORMAT_TYPE f_441;
__root const FRAME_FORMAT_TYPE f_282;
__root const FRAME_FORMAT_TYPE f_283;
__root const FRAME_FORMAT_TYPE f_284;
__root const FRAME_FORMAT_TYPE f_396;
__root const FRAME_FORMAT_TYPE f_145;
__root const FRAME_FORMAT_TYPE f_300;
__root const FRAME_FORMAT_TYPE f_205;
__root const FRAME_FORMAT_TYPE f_275;
__root const FRAME_FORMAT_TYPE f_276;
__root const FRAME_FORMAT_TYPE f_416;
__root const FRAME_FORMAT_TYPE f_285;
__root const FRAME_FORMAT_TYPE f_335;
__root const FRAME_FORMAT_TYPE f_349;
__root const FRAME_FORMAT_TYPE f_442;
__root const FRAME_FORMAT_TYPE f_443;
__root const FRAME_FORMAT_TYPE f_444;
__root const FRAME_FORMAT_TYPE f_470;
__root const FRAME_FORMAT_TYPE f_279;
__root const FRAME_FORMAT_TYPE f_280;
__root const FRAME_FORMAT_TYPE f_281;
__root const FRAME_FORMAT_TYPE f_213;
__root const FRAME_FORMAT_TYPE f_214;
__root const FRAME_FORMAT_TYPE f_215;
__root const FRAME_FORMAT_TYPE f_227;
__root const FRAME_FORMAT_TYPE f_209;
__root const FRAME_FORMAT_TYPE f_210;
__root const FRAME_FORMAT_TYPE f_211;
__root const FRAME_FORMAT_TYPE f_212;
__root const FRAME_FORMAT_TYPE f_272;
__root const FRAME_FORMAT_TYPE f_271;
__root const FRAME_FORMAT_TYPE f_239;
__root const FRAME_FORMAT_TYPE f_240;
__root const FRAME_FORMAT_TYPE f_241;
__root const FRAME_FORMAT_TYPE f_242;
__root const FRAME_FORMAT_TYPE f_243;
__root const FRAME_FORMAT_TYPE f_244;
__root const FRAME_FORMAT_TYPE f_245;
__root const FRAME_FORMAT_TYPE f_149;
__root const FRAME_FORMAT_TYPE f_151;
__root const FRAME_FORMAT_TYPE f_152;
__root const FRAME_FORMAT_TYPE f_155;
__root const FRAME_FORMAT_TYPE f_173;
__root const FRAME_FORMAT_TYPE f_156;
__root const FRAME_FORMAT_TYPE f_299;
__root const FRAME_FORMAT_TYPE f_157;
__root const FRAME_FORMAT_TYPE f_159;
__root const FRAME_FORMAT_TYPE f_160;
__root const FRAME_FORMAT_TYPE f_161;
__root const FRAME_FORMAT_TYPE f_360;
__root const FRAME_FORMAT_TYPE f_361;
__root const FRAME_FORMAT_TYPE f_362;
__root const FRAME_FORMAT_TYPE f_363;
__root const FRAME_FORMAT_TYPE f_364;
__root const FRAME_FORMAT_TYPE f_365;
__root const FRAME_FORMAT_TYPE f_366;
__root const FRAME_FORMAT_TYPE f_367;
__root const FRAME_FORMAT_TYPE f_368;
__root const FRAME_FORMAT_TYPE f_369;
__root const FRAME_FORMAT_TYPE f_370;
__root const FRAME_FORMAT_TYPE f_379;
__root const FRAME_FORMAT_TYPE f_391;
__root const FRAME_FORMAT_TYPE f_392;
__root const FRAME_FORMAT_TYPE f_393;
__root const FRAME_FORMAT_TYPE f_380;
__root const FRAME_FORMAT_TYPE f_381;
__root const FRAME_FORMAT_TYPE f_382;
__root const FRAME_FORMAT_TYPE f_385;
__root const FRAME_FORMAT_TYPE f_383;
__root const FRAME_FORMAT_TYPE f_384;
__root const FRAME_FORMAT_TYPE f_471;
__root const FRAME_FORMAT_TYPE f_386;
__root const FRAME_FORMAT_TYPE f_387;
__root const FRAME_FORMAT_TYPE f_388;
__root const FRAME_FORMAT_TYPE f_389;
__root const FRAME_FORMAT_TYPE f_390;
__root const FRAME_FORMAT_TYPE f_394;
__root const FRAME_FORMAT_TYPE f_395;
__root const FRAME_FORMAT_TYPE f_301;
__root const FRAME_FORMAT_TYPE f_79;
__root const FRAME_FORMAT_TYPE f_138;
__root const FRAME_FORMAT_TYPE f_465;
__root const FRAME_FORMAT_TYPE f_464;
__root const FRAME_FORMAT_TYPE f_474;
__root const FRAME_FORMAT_TYPE f_475;
__root const FRAME_FORMAT_TYPE f_31;
__root const FRAME_FORMAT_TYPE f_32;
__root const FRAME_FORMAT_TYPE f_33;
__root const FRAME_FORMAT_TYPE f_34;
__root const FRAME_FORMAT_TYPE f_35;
__root const FRAME_FORMAT_TYPE f_38;
__root const FRAME_FORMAT_TYPE f_39;
__root const FRAME_FORMAT_TYPE f_40;
__root const FRAME_FORMAT_TYPE f_41;
__root const FRAME_FORMAT_TYPE f_42;
__root const FRAME_FORMAT_TYPE f_54;
__root const FRAME_FORMAT_TYPE f_55;
__root const FRAME_FORMAT_TYPE f_56;
__root const FRAME_FORMAT_TYPE f_57;
__root const FRAME_FORMAT_TYPE f_58;
__root const FRAME_FORMAT_TYPE f_60;
__root const FRAME_FORMAT_TYPE f_61;
__root const FRAME_FORMAT_TYPE f_62;
__root const FRAME_FORMAT_TYPE f_63;
__root const FRAME_FORMAT_TYPE f_64;
__root const FRAME_FORMAT_TYPE f_397;
__root const FRAME_FORMAT_TYPE f_398;
__root const FRAME_FORMAT_TYPE f_399;
__root const FRAME_FORMAT_TYPE f_400;
__root const FRAME_FORMAT_TYPE f_183;
__root const FRAME_FORMAT_TYPE f_184;
__root const FRAME_FORMAT_TYPE f_185;
__root const FRAME_FORMAT_TYPE f_186;
__root const FRAME_FORMAT_TYPE f_350;
__root const FRAME_FORMAT_TYPE f_187;
__root const FRAME_FORMAT_TYPE f_484;
__root const FRAME_FORMAT_TYPE f_485;
__root const FRAME_FORMAT_TYPE f_486;
__root const FRAME_FORMAT_TYPE f_487;
__root const FRAME_FORMAT_TYPE f_488;
__root const FRAME_FORMAT_TYPE f_489;
__root const FRAME_FORMAT_TYPE f_490;
__root const FRAME_FORMAT_TYPE f_491;
__root const FRAME_FORMAT_TYPE f_492;
__root const FRAME_FORMAT_TYPE f_493;
__root const FRAME_FORMAT_TYPE f_494;
__root const FRAME_FORMAT_TYPE f_495;
__root const FRAME_FORMAT_TYPE f_496;
__root const FRAME_FORMAT_TYPE f_497;
__root const FRAME_FORMAT_TYPE f_498;
__root const FRAME_FORMAT_TYPE f_499;
__root const FRAME_FORMAT_TYPE f_500;
__root const FRAME_FORMAT_TYPE f_501;
__root const FRAME_FORMAT_TYPE f_502;
__root const FRAME_FORMAT_TYPE f_503;
__root const FRAME_FORMAT_TYPE f_504;
__root const FRAME_FORMAT_TYPE f_505;
__root const FRAME_FORMAT_TYPE f_506;
__root const FRAME_FORMAT_TYPE f_98;
__root const FRAME_FORMAT_TYPE f_99;
__root const FRAME_FORMAT_TYPE f_100;
__root const FRAME_FORMAT_TYPE f_101;
__root const FRAME_FORMAT_TYPE f_102;
__root const FRAME_FORMAT_TYPE f_103;
__root const FRAME_FORMAT_TYPE f_106;
__root const FRAME_FORMAT_TYPE f_107;
__root const FRAME_FORMAT_TYPE f_108;
__root const FRAME_FORMAT_TYPE f_109;
__root const FRAME_FORMAT_TYPE f_110;
__root const FRAME_FORMAT_TYPE f_111;
__root const FRAME_FORMAT_TYPE f_112;
__root const FRAME_FORMAT_TYPE f_447;
__root const FRAME_FORMAT_TYPE f_448;
__root const FRAME_FORMAT_TYPE f_450;
__root const FRAME_FORMAT_TYPE f_451;
__root const FRAME_FORMAT_TYPE f_453;
__root const FRAME_FORMAT_TYPE f_454;
__root const FRAME_FORMAT_TYPE f_456;
__root const FRAME_FORMAT_TYPE f_457;
__root const FRAME_FORMAT_TYPE f_459;
__root const FRAME_FORMAT_TYPE f_460;
__root const FRAME_FORMAT_TYPE f_462;
__root const FRAME_FORMAT_TYPE f_463;
__root const FRAME_FORMAT_TYPE f_216;
__root const FRAME_FORMAT_TYPE f_217;
__root const FRAME_FORMAT_TYPE f_218;
__root const FRAME_FORMAT_TYPE f_219;
__root const FRAME_FORMAT_TYPE f_220;
__root const FRAME_FORMAT_TYPE f_221;
__root const FRAME_FORMAT_TYPE f_336;
__root const FRAME_FORMAT_TYPE f_337;
__root const FRAME_FORMAT_TYPE f_338;
__root const FRAME_FORMAT_TYPE f_222;
__root const FRAME_FORMAT_TYPE f_223;
__root const FRAME_FORMAT_TYPE f_224;
__root const FRAME_FORMAT_TYPE f_225;
__root const FRAME_FORMAT_TYPE f_226;
__root const FRAME_FORMAT_TYPE f_339;
__root const FRAME_FORMAT_TYPE f_341;
__root const FRAME_FORMAT_TYPE f_340;
__root const FRAME_FORMAT_TYPE f_228;
__root const FRAME_FORMAT_TYPE f_229;
__root const FRAME_FORMAT_TYPE f_230;
__root const FRAME_FORMAT_TYPE f_231;
__root const FRAME_FORMAT_TYPE f_232;
__root const FRAME_FORMAT_TYPE f_233;
__root const FRAME_FORMAT_TYPE f_342;
__root const FRAME_FORMAT_TYPE f_343;
__root const FRAME_FORMAT_TYPE f_344;
__root const FRAME_FORMAT_TYPE f_234;
__root const FRAME_FORMAT_TYPE f_235;
__root const FRAME_FORMAT_TYPE f_236;
__root const FRAME_FORMAT_TYPE f_237;
__root const FRAME_FORMAT_TYPE f_238;
__root const FRAME_FORMAT_TYPE f_345;
__root const FRAME_FORMAT_TYPE f_347;
__root const FRAME_FORMAT_TYPE f_346;
__root const FRAME_FORMAT_TYPE f_246;
__root const FRAME_FORMAT_TYPE f_247;
__root const FRAME_FORMAT_TYPE f_248;
__root const FRAME_FORMAT_TYPE f_249;
__root const FRAME_FORMAT_TYPE f_250;
__root const FRAME_FORMAT_TYPE f_251;
__root const FRAME_FORMAT_TYPE f_252;
__root const FRAME_FORMAT_TYPE f_253;
__root const FRAME_FORMAT_TYPE f_254;
__root const FRAME_FORMAT_TYPE f_255;
__root const FRAME_FORMAT_TYPE f_256;
__root const FRAME_FORMAT_TYPE f_257;
__root const FRAME_FORMAT_TYPE f_258;
__root const FRAME_FORMAT_TYPE f_259;
__root const FRAME_FORMAT_TYPE f_260;
__root const FRAME_FORMAT_TYPE f_261;
__root const FRAME_FORMAT_TYPE f_262;
__root const FRAME_FORMAT_TYPE f_263;
__root const FRAME_FORMAT_TYPE f_264;
__root const FRAME_FORMAT_TYPE f_265;
__root const FRAME_FORMAT_TYPE f_266;
__root const FRAME_FORMAT_TYPE f_267;
__root const FRAME_FORMAT_TYPE f_268;
__root const FRAME_FORMAT_TYPE f_269;
__root const FRAME_FORMAT_TYPE f_270;
__root const FRAME_FORMAT_TYPE f_OPERATORMENU;
__root const FRAME_FORMAT_TYPE f_SIGNALSVIEWMENU;
__root const FRAME_FORMAT_TYPE f_DIAGNOSTICSVIEWMENU;
__root const FRAME_FORMAT_TYPE f_ACCESSLEVELSELECTION;
__root const FRAME_FORMAT_TYPE f_ACCESSLEVELPASSWORDENTRY;
__root const FRAME_FORMAT_TYPE f_RESETPASSWORDS;

#pragma diag_suppress=Pm022


// Operator Frame Definitions
__root const FRAME_FORMAT_TYPE f_0=
{
FRAME_TYPE_OPERATOR,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_1,
0,
0,
1,
0,
0,
0,
0,
0,
0,0,0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_43=
{
FRAME_TYPE_OPERATOR,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_1,
0,
0,
2,
HMI_IDX,
HMI_IDX_Password,
0,
1,
6,
0,0,0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_81=
{
FRAME_TYPE_OPERATOR,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_1,
0,
0,
3,
HMI_IDX,
HMI_IDX_Password,
0,
1,
6,
0,0,0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_82=
{
FRAME_TYPE_OPERATOR,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_1,
0,
0,
4,
HMI_IDX,
HMI_IDX_Password,
0,
1,
6,
0,0,0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};



// Main Menu (Header) Frame Definitions
__root const FRAME_FORMAT_TYPE f_1=
{
FRAME_TYPE_CONFIG_HEADER,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_44,
0,
19,
IDF_VALID_always,
1,
2,
3,
4,
0,
0,
0,
CFH_EASY_SETUP,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_5=
{
FRAME_TYPE_CONFIG_HEADER,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_12,
0,
20,
IDF_VALID_always,
5,
6,
0,
0,
0,
0,
0,
CFH_DEVICE_INFO,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_6=
{
FRAME_TYPE_CONFIG_HEADER,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_15,
0,
21,
IDF_VALID_always,
7,
8,
9,
0,
0,
0,
0,
CFH_FLOW_DEVICE,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_2=
{
FRAME_TYPE_CONFIG_HEADER,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_3,
0,
22,
IDF_VALID_always,
10,
11,
0,
0,
0,
0,
0,
CFH_DISPLAY,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_7=
{
FRAME_TYPE_CONFIG_HEADER,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_68,
0,
23,
IDF_VALID_always,
12,
13,
14,
15,
16,
17,
0,
CFH_IO,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_8=
{
FRAME_TYPE_CONFIG_HEADER,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_73,
0,
24,
IDF_VALID_always,
18,
19,
0,
0,
0,
0,
0,
CFH_PROCESS_ALARM,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_9=
{
FRAME_TYPE_CONFIG_HEADER,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_19,
0,
25,
IDF_VALID_always,
20,
21,
22,
23,
0,
0,
0,
CFH_COMMUNICATION,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_10=
{
FRAME_TYPE_CONFIG_HEADER,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_22,
0,
26,
IDF_VALID_always,
24,
25,
0,
0,
0,
0,
0,
CFH_DIAGNOSTICS,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_11=
{
FRAME_TYPE_CONFIG_HEADER,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_90,
0,
27,
IDF_VALID_always,
26,
27,
28,
29,
0,
0,
0,
CFH_TOTALIZER,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_141=
{
FRAME_TYPE_CONFIG_HEADER,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_142,
0,
28,
IDF_VALID_always,
30,
31,
32,
31,
0,
0,
0,
CFH_SERVICE,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};



// Configuration Frame Definitions
__root const FRAME_FORMAT_TYPE f_44=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
29,
HMI_IDX,
HMI_IDX_language,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_4=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
46,
CDT_IDX,
CDT_IDX_operatingMode,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
21,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_123=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
60,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_outputMode,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
34,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_124=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
65,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_pulseFactor,
FLOAT_FIXEDUNIT_value,
0,
66,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_125=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
67,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_pulseWidth,
FLOAT_FIXEDUNIT_value,
0,
68,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_126=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
69,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_minFrequency,
FLOAT_FIXEDUNIT_value,
0,
70,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_303=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
71,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_maxFrequency,
FLOAT_FIXEDUNIT_value,
0,
70,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_352=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
72,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_lgcActState,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
38,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_319=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
75,
SVM_IDX,
SVM_IDX_QvObj,
UOM_PF_unit,
0,
USE_UOM_1,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_318=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
76,
MVM_IDX,
MVM_IDX_QmObj,
UOM_PF_unit,
0,
USE_UOM_2,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_320=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
77,
MVM_IDX,
MVM_IDX_QnObj,
UOM_PF_unit,
0,
USE_UOM_1,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_324=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
78,
MVM_IDX,
MVM_IDX_QpDampedObj,
UOM_PF_unit,
0,
USE_UOM_3,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_321=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
79,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
0,
USE_UOM_4,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_322=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
80,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
0,
USE_UOM_5,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_323=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
81,
MAPPER_IDX,
MAPPER_IDX_dvPressureObj,
UOM_PF_unit,
0,
USE_UOM_6,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_326=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
82,
TOTALIZER_IDX,
TOTALIZER_IDX_totQvUnit,
UNITS_unitsCode,
0,
USE_UOM_9,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_325=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
83,
TOTALIZER_IDX,
TOTALIZER_IDX_totQmUnit,
UNITS_unitsCode,
0,
USE_UOM_10,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_328=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
84,
TOTALIZER_IDX,
TOTALIZER_IDX_totQnUnit,
UNITS_unitsCode,
0,
USE_UOM_9,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_327=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
85,
TOTALIZER_IDX,
TOTALIZER_IDX_totEnergyUnit,
UNITS_unitsCode,
0,
USE_UOM_11,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_304=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
86,
AI_IDX,
AI_IDX_aiSelection,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
40,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_331=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
95,
MAPPER_IDX,
MAPPER_IDX_dvTExtRangeObj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
7,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_332=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
96,
MAPPER_IDX,
MAPPER_IDX_dvTExtRangeObj,
UOM_PF_value_1,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
7,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_412=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
97,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeRelativeObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeRelativeObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_413=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
98,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeRelativeObj,
UOM_PF_value_1,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeRelativeObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_305=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
99,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvPressureObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_306=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
100,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeObj,
UOM_PF_value_1,
MAPPER_IDX,
MAPPER_IDX_dvPressureObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_307=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
101,
MAPPER_IDX,
MAPPER_IDX_dvDensityRangeObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
USE_UOM_4,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_308=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
102,
MAPPER_IDX,
MAPPER_IDX_dvDensityRangeObj,
UOM_PF_value_1,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
USE_UOM_4,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_309=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
103,
MAPPER_IDX,
MAPPER_IDX_dvContentRange,
DEV_RNGFLT_hiLim,
0,
104,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_310=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
105,
MAPPER_IDX,
MAPPER_IDX_dvContentRange,
DEV_RNGFLT_loLim,
0,
104,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_402=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
106,
AI_IDX,
AI_IDX_aiExternalCutOffOption,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
48,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_468=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
110,
CDT_IDX,
CDT_IDX_temperature_range_Obj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
7,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_469=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
111,
CDT_IDX,
CDT_IDX_temperature_range_Obj,
UOM_PF_value_1,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
7,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_127=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
112,
MVM_IDX,
MVM_IDX_liquidCorrection,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
51,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_132=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
116,
MVM_IDX,
MVM_IDX_volumeExpandBeta1,
0,
0,
117,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_133=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
118,
MVM_IDX,
MVM_IDX_densityExpandBeta2,
0,
0,
117,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_311=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
119,
MVM_IDX,
MVM_IDX_heatCapacity,
0,
0,
120,
0,
FIXED_TEXT_UNITS,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_313=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
121,
MVM_IDX,
MVM_IDX_gasDensitySelection,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
54,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_128=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
124,
MVM_IDX,
MVM_IDX_gasRef,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
56,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_312=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
129,
MVM_IDX,
MVM_IDX_gasComputeType,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
60,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_329=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
133,
MVM_IDX,
MVM_IDX_calorificEnergy,
0,
0,
134,
0,
FIXED_TEXT_UNITS,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_129=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
135,
MVM_IDX,
MVM_IDX_steamType,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
63,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_314=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
139,
CDT_IDX,
CDT_IDX_actualDensitySelection,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
66,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_473=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
144,
MVM_IDX,
MVM_IDX_steamPwrrFR,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
70,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_130=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
147,
MVM_IDX,
MVM_IDX_densigyRefObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
USE_UOM_4,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_45=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
148,
MAPPER_IDX,
MAPPER_IDX_dvDensityPresetObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
USE_UOM_4,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_131=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
149,
MVM_IDX,
MVM_IDX_refTemperatureObj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_UnitCode,
0,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_78=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
150,
CDT_IDX,
CDT_IDX_temperaturePreset_Obj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_UnitCode,
0,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_315=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
151,
MAPPER_IDX,
MAPPER_IDX_dvTExtPresetObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvTExtObj,
UOM_PF_unit,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_316=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
152,
MAPPER_IDX,
MAPPER_IDX_dvPressurePresetObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvPressurePresetObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_317=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
153,
MAPPER_IDX,
MAPPER_IDX_dvContentPreset,
0,
0,
104,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_21=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
154,
SVM_IDX,
SVM_IDX_QvRangeObj,
UOM_PF_value_0,
SVM_IDX,
SVM_IDX_QvObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_134=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
155,
MVM_IDX,
MVM_IDX_QnRangeObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QnObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_293=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
156,
MVM_IDX,
MVM_IDX_QvPartialRangeObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QnObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_294=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
157,
MVM_IDX,
MVM_IDX_QnPartialRangeObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QnObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_135=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
158,
MVM_IDX,
MVM_IDX_QmRangeObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QmObj,
UOM_PF_unit,
USE_UOM_2,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_136=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
159,
MVM_IDX,
MVM_IDX_QpRangeObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QpObj,
UOM_PF_unit,
USE_UOM_3,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_330=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
160,
SVM_IDX,
SVM_IDX_QvDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_353=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
162,
MVM_IDX,
MVM_IDX_QnDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_354=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
163,
MVM_IDX,
MVM_IDX_QvPartialDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_355=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
164,
MVM_IDX,
MVM_IDX_QnPartialDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_351=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
165,
MVM_IDX,
MVM_IDX_QmDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_356=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
166,
MVM_IDX,
MVM_IDX_QpDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_357=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
167,
CDT_IDX,
CDT_IDX_temperature_range_Obj,
UOM_PF_value_1,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_358=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
168,
CDT_IDX,
CDT_IDX_temperature_range_Obj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_359=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
169,
CDT_IDX,
CDT_IDX_temperature_DampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_139=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((170 * 65536) + 19),
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_writeAutoZero,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
1,
#endif
};

__root const FRAME_FORMAT_TYPE f_46=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
19,
172,
SVM_IDX,
SVM_IDX_QvLowCutOff,
0,
0,
104,
0,
FIXED_TEXT_UNITS,
5,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
2,
#endif
};

__root const FRAME_FORMAT_TYPE f_12=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_13,
20,
0,
0,
0,
1,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_15=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_16,
21,
0,
2,
0,
5,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_3=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_28,
22,
0,
6,
0,
14,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_68=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_69,
23,
0,
15,
0,
16,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_73=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_74,
24,
0,
17,
0,
21,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_19=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_118,
25,
0,
22,
0,
23,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_22=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_23,
26,
0,
24,
0,
42,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_90=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_417,
27,
0,
43,
0,
47,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_142=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_143,
28,
0,
48,
0,
54,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_13=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_26,
173,
0,
55,
0,
67,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_14=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_153,
174,
0,
68,
0,
77,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_16=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_20,
175,
0,
78,
0,
80,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_17=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_48,
173,
0,
81,
0,
94,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_18=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_50,
176,
0,
95,
0,
103,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_137=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_297,
177,
0,
104,
0,
106,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_28=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
22,
29,
HMI_IDX,
HMI_IDX_language,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_29=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_30,
178,
0,
107,
0,
110,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_36=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
22,
179,
HMI_IDX,
HMI_IDX_autoscrollEnable,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
72,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_65=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
22,
180,
HMI_IDX,
HMI_IDX_customDP_1,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
74,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_66=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
22,
181,
HMI_IDX,
HMI_IDX_customDP_2,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
74,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_67=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
22,
182,
HMI_IDX,
HMI_IDX_DateFormat,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
87,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_113=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((183 * 65536) + 22),
HMI_IDX,
HMI_IDX_displayTest,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_507=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
22,
184,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_modeLCDLight,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
90,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_508=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
22,
185,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_switchBLE,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
93,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_69=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_120,
186,
0,
111,
0,
115,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_70=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_401,
187,
0,
116,
0,
130,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_74=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
NULL,
188,
0,
0,
0,
0,
0,
0,0,0,0,
5,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_80=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((189 * 65536) + 24),
MAPPER_IDX,
MAPPER_IDX_diagClearHistory,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_83=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_84,
190,
0,
131,
0,
133,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_75=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_87,
191,
0,
134,
0,
140,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_91=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_92,
192,
0,
141,
0,
147,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_118=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_277,
193,
0,
148,
0,
148,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_477=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_478,
194,
0,
149,
0,
154,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_23=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_140,
195,
0,
155,
0,
156,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_24=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_302,
196,
0,
157,
0,
160,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_76=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
197,
CDT_IDX,
CDT_IDX_simulationMode,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
99,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_290=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
198,
SVM_IDX,
SVM_IDX_QvSimObj,
UOM_PF_unit,
0,
USE_UOM_1,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_122=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
199,
SVM_IDX,
SVM_IDX_QvSimObj,
UOM_PF_value_0,
SVM_IDX,
SVM_IDX_QvSimObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_168=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
200,
SVM_IDX,
SVM_IDX_QvPerSim,
0,
0,
104,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_167=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
201,
MVM_IDX,
MVM_IDX_QmSimObj,
UOM_PF_unit,
0,
USE_UOM_2,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_291=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
202,
MVM_IDX,
MVM_IDX_QmSimObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QmSimObj,
UOM_PF_unit,
USE_UOM_2,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_169=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
203,
MVM_IDX,
MVM_IDX_QmPerSim,
0,
0,
104,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_196=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
204,
CDT_IDX,
CDT_IDX_temperatureSim_Obj,
UOM_PF_unit,
0,
USE_UOM_5,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_292=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
205,
CDT_IDX,
CDT_IDX_temperatureSim_Obj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperatureSim_Obj,
UOM_PF_unit,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_195=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
206,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_elettronicTemperatureSimulation_Obj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperatureSim_Obj,
UOM_PF_unit,
USE_UOM_5,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_170=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
207,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_simuFrequency,
0,
0,
70,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_171=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
208,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_lgcSimCmd,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_172=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
209,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_simuPulsesNum,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
8,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_198=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
210,
AI_IDX,
AI_IDX_aiSimulationData,
0,
0,
70,
1,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_197=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
211,
CDT_IDX,
CDT_IDX_frequency_SimObj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_frequency_SimObj,
UOM_PF_unit,
USE_UOM_7,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_199=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_200,
212,
0,
161,
0,
163,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_203=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
26,
213,
MAPPER_IDX,
MAPPER_IDX_diagSimulationType,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
115,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_417=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_418,
214,
0,
164,
0,
170,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_425=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_426,
215,
0,
171,
0,
177,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_433=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_434,
216,
0,
178,
0,
184,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_104=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_97,
217,
0,
185,
0,
186,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_445=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_446,
218,
0,
187,
0,
192,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_143=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_204,
176,
0,
193,
0,
196,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_144=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_282,
173,
0,
197,
0,
213,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_278=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_279,
219,
0,
214,
0,
216,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_206=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_213,
220,
0,
217,
0,
220,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_207=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_209,
221,
0,
221,
0,
231,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_208=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_244,
222,
0,
232,
0,
233,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_334=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
28,
223,
CDT_IDX,
CDT_IDX_forceReplace,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
167,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_26=
{
FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
224,
CDT_IDX,
CDT_IDX_converterType,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
171,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_25=
{
FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
225,
CDT_IDX,
CDT_IDX_vortexmeterSize,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
173,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_77=
{
FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
226,
CDT_IDX,
CDT_IDX_swirlmeterSize,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
193,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_47=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
227,
SVM_IDX,
SVM_IDX_QvMaxDNObj,
UOM_PF_value_0,
SVM_IDX,
SVM_IDX_QvObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_188=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
228,
MVM_IDX,
MVM_IDX_QvPartialMaxDNObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QvPartialObj,
UOM_PF_unit,
USE_UOM_2,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_295=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
229,
MVM_IDX,
MVM_IDX_QmMaxDNObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QmObj,
UOM_PF_unit,
USE_UOM_2,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_189=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
230,
MVM_IDX,
MVM_IDX_QnMaxDNObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QnObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_296=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
231,
MVM_IDX,
MVM_IDX_QnPartialMaxDNObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QnObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_190=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
232,
MVM_IDX,
MVM_IDX_QpMaxDNObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QpObj,
UOM_PF_unit,
USE_UOM_3,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_27=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
233,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_sensorID,
0,
8,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_147=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
234,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_sensorSapErpNo,
0,
20,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_148=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
235,
CDT_IDX,
CDT_IDX_frontendOperationHour,
0,
0,
188,
1,
FIXED_TEXT_UNITS,
12,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_146=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_149,
236,
0,
234,
0,
236,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_153=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
174,
237,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_tranType,
0,
12,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_333=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
174,
238,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_tranID,
0,
8,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_150=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
174,
234,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_tranSapErpNo,
0,
20,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_154=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_155,
239,
0,
237,
0,
241,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_162=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
174,
240,
CDT_IDX,
CDT_IDX_totalWorkingTimeHour,
0,
0,
188,
1,
FIXED_TEXT_UNITS,
12,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_158=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_159,
236,
0,
242,
0,
244,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_163=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
174,
241,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_manufacturer,
0,
20,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_164=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
174,
242,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_street,
0,
20,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_165=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
174,
243,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_city,
0,
20,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_166=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
174,
244,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_phone,
0,
20,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_20=
{
FRAME_TYPE_STRING_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
175,
245,
HMI_IDX,
HMI_IDX_SetPassword_StandardLevel,
0,
6,
CHARACTER_SET_PASSWORD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_117=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
175,
246,
ARM_IDX,
ARM_IDX_readOnlySwitchStatus,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
205,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_476=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
175,
247,
ARM_IDX,
ARM_IDX_serviceAccountSwitch,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
72,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_48=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
227,
SVM_IDX,
SVM_IDX_QvMaxDNObj,
UOM_PF_value_0,
SVM_IDX,
SVM_IDX_QvObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_286=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
228,
MVM_IDX,
MVM_IDX_QvPartialMaxDNObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QvPartialObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_174=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
229,
MVM_IDX,
MVM_IDX_QmMaxDNObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QmObj,
UOM_PF_unit,
USE_UOM_2,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_175=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
230,
MVM_IDX,
MVM_IDX_QnMaxDNObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QnObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_288=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
231,
MVM_IDX,
MVM_IDX_QnPartialMaxDNObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QnPartialObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_176=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
232,
MVM_IDX,
MVM_IDX_QpMaxDNObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QpObj,
UOM_PF_unit,
USE_UOM_3,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_49=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
248,
SVM_IDX,
SVM_IDX_QvRangeObj,
UOM_PF_value_0,
SVM_IDX,
SVM_IDX_QvObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_287=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
156,
MVM_IDX,
MVM_IDX_QvPartialRangeObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QvPartialObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_178=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
158,
MVM_IDX,
MVM_IDX_QmRangeObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QmDampedObj,
UOM_PF_unit,
USE_UOM_2,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_177=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
155,
MVM_IDX,
MVM_IDX_QnRangeObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QnDampedObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_289=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
249,
MVM_IDX,
MVM_IDX_QnPartialRangeObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QnPartialRangeObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_179=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
159,
MVM_IDX,
MVM_IDX_QpRangeObj,
UOM_PF_value_0,
MVM_IDX,
MVM_IDX_QpDampedObj,
UOM_PF_unit,
USE_UOM_3,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_180=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
250,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_sensorLocationTag,
0,
20,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_181=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
251,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_sensoTag,
0,
20,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_50=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_360,
252,
0,
245,
0,
255,
0,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_372=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
160,
SVM_IDX,
SVM_IDX_QvDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_373=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
163,
MVM_IDX,
MVM_IDX_QvPartialDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_375=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
165,
MVM_IDX,
MVM_IDX_QmDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_371=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
162,
MVM_IDX,
MVM_IDX_QnDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_374=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
253,
MVM_IDX,
MVM_IDX_QnPartialDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_376=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
166,
MVM_IDX,
MVM_IDX_QpDampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_377=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
169,
CDT_IDX,
CDT_IDX_temperature_DampingTime,
0,
0,
161,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_378=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
172,
SVM_IDX,
SVM_IDX_QvLowCutOff,
0,
0,
104,
0,
FIXED_TEXT_UNITS,
5,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_297=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
177,
46,
CDT_IDX,
CDT_IDX_operatingMode,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
21,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_51=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_379,
254,
0,
0,
1,
18,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_52=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_79,
255,
0,
19,
1,
24,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_30=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_31,
256,
0,
25,
1,
29,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_37=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_38,
257,
0,
30,
1,
34,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_53=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_54,
258,
0,
35,
1,
39,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_59=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_60,
259,
0,
40,
1,
44,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_120=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
186,
281,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_outputMode,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
34,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_71=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_397,
282,
0,
45,
1,
46,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_72=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_399,
283,
0,
47,
1,
48,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_182=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_183,
284,
0,
49,
1,
54,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_121=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
186,
72,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_lgcActState,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
38,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_401=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
86,
AI_IDX,
AI_IDX_aiSelection,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
40,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_404=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
95,
MAPPER_IDX,
MAPPER_IDX_dvTExtRangeObj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
7,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_405=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
96,
MAPPER_IDX,
MAPPER_IDX_dvTExtRangeObj,
UOM_PF_value_1,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
7,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_414=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
97,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeRelativeObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeRelativeObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_415=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
98,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeRelativeObj,
UOM_PF_value_1,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeRelativeObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_406=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
99,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvPressureObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_407=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
100,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeObj,
UOM_PF_value_1,
MAPPER_IDX,
MAPPER_IDX_dvPressureObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_408=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
101,
MAPPER_IDX,
MAPPER_IDX_dvDensityRangeObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
USE_UOM_4,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_409=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
102,
MAPPER_IDX,
MAPPER_IDX_dvDensityRangeObj,
UOM_PF_value_1,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
USE_UOM_4,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_410=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
103,
MAPPER_IDX,
MAPPER_IDX_dvContentRange,
DEV_RNGFLT_hiLim,
0,
104,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_411=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
105,
MAPPER_IDX,
MAPPER_IDX_dvContentRange,
DEV_RNGFLT_loLim,
0,
104,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_403=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
106,
AI_IDX,
AI_IDX_aiExternalCutOffOption,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
48,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_466=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
110,
CDT_IDX,
CDT_IDX_temperature_range_Obj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
7,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_467=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
111,
CDT_IDX,
CDT_IDX_temperature_range_Obj,
UOM_PF_value_1,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
7,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_472=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
187,
285,
CDT_IDX,
CDT_IDX_reaction_InputFail,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
207,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_84=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
190,
286,
MAPPER_IDX,
MAPPER_IDX_diagMaintainMask,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_85=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
190,
287,
MAPPER_IDX,
MAPPER_IDX_diagFunCheckMask,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_86=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
190,
288,
MAPPER_IDX,
MAPPER_IDX_diagOffSpecMask,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_87=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
191,
289,
MAPPER_IDX,
MAPPER_IDX_diagMinFlowMask,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_88=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
191,
290,
MAPPER_IDX,
MAPPER_IDX_diagMaxFlowMask,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_89=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
191,
291,
MAPPER_IDX,
MAPPER_IDX_diagFlow103Mask,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_298=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
191,
292,
MAPPER_IDX,
MAPPER_IDX_diagLowCutoffMask,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_114=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
191,
293,
MAPPER_IDX,
MAPPER_IDX_diagTSensorFailMask,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_115=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
191,
294,
MAPPER_IDX,
MAPPER_IDX_diagTSensorOffSpecMask,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_116=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
191,
295,
MAPPER_IDX,
MAPPER_IDX_diagTxSensorOffSpecMask,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_92=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
192,
296,
MVM_IDX,
MVM_IDX_QAlmRange,
RANGEFLT_loLim,
0,
104,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_93=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
192,
297,
MVM_IDX,
MVM_IDX_QAlmRange,
RANGEFLT_hiLim,
0,
104,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_94=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
192,
298,
CDT_IDX,
CDT_IDX_temperatureAlmRange_obj,
UOM_PF_value_1,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_95=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
192,
299,
CDT_IDX,
CDT_IDX_temperatureAlmRange_obj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_96=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
192,
300,
MAPPER_IDX,
MAPPER_IDX_dvPressureAlmRangeObj,
UOM_PF_value_1,
MAPPER_IDX,
MAPPER_IDX_dvPressureObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_119=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
192,
301,
MAPPER_IDX,
MAPPER_IDX_dvPressureAlmRangeObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvPressureObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_348=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
192,
302,
SVM_IDX,
SVM_IDX_ReAlmLimits,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
9,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_277=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
193,
303,
SERVICEPORT_IDX,
SERVICEPORT_IDX_baudRate,
0,
0,
221,
1,
FIXED_TEXT_UNITS,
6,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_478=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_484,
304,
0,
55,
1,
55,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_479=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_485,
305,
0,
56,
1,
63,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_480=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_493,
306,
0,
64,
1,
67,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_481=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_497,
307,
0,
68,
1,
69,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_482=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_499,
308,
0,
70,
1,
72,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_483=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_502,
309,
0,
73,
1,
77,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_140=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((310 * 65536) + 195),
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_writeSelfDiagnosis,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_191=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
195,
311,
CDT_IDX,
CDT_IDX_BothMaintenanceCycle,
0,
0,
188,
1,
FIXED_TEXT_UNITS,
6,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_302=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
196,
312,
SVM_IDX,
SVM_IDX_Re,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
9,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_192=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
196,
313,
MAPPER_IDX,
MAPPER_IDX_dvTExtObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvTExtObj,
UOM_PF_unit,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_193=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
196,
206,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_elettronicTemperature_Obj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
USE_UOM_5,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_194=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
196,
210,
AI_IDX,
AI_IDX_aiTempCurr,
0,
0,
70,
1,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_200=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
212,
327,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_pulseNum,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_201=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
212,
328,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_frequencyValue,
0,
0,
70,
0,
FIXED_TEXT_UNITS,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_202=
{
FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
212,
329,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_logicValue,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
209,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_418=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((365 * 65536) + 214),
TOTALIZER_IDX,
TOTALIZER_IDX_StartAllTotalizr,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_419=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((366 * 65536) + 214),
TOTALIZER_IDX,
TOTALIZER_IDX_StartQvTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_420=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((367 * 65536) + 214),
TOTALIZER_IDX,
TOTALIZER_IDX_StartQnTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_421=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((368 * 65536) + 214),
TOTALIZER_IDX,
TOTALIZER_IDX_StartQmTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_422=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((369 * 65536) + 214),
TOTALIZER_IDX,
TOTALIZER_IDX_StartEnergyTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_423=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((370 * 65536) + 214),
TOTALIZER_IDX,
TOTALIZER_IDX_StartQvPartialTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_424=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((371 * 65536) + 214),
TOTALIZER_IDX,
TOTALIZER_IDX_StartQnPartialTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_426=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((365 * 65536) + 215),
TOTALIZER_IDX,
TOTALIZER_IDX_StopAllTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_427=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((366 * 65536) + 215),
TOTALIZER_IDX,
TOTALIZER_IDX_StopQvTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_428=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((367 * 65536) + 215),
TOTALIZER_IDX,
TOTALIZER_IDX_StopQnTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_429=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((368 * 65536) + 215),
TOTALIZER_IDX,
TOTALIZER_IDX_StopQmTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_430=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((369 * 65536) + 215),
TOTALIZER_IDX,
TOTALIZER_IDX_StopEnergyTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_431=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((370 * 65536) + 215),
TOTALIZER_IDX,
TOTALIZER_IDX_StopQvPartialTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_432=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((371 * 65536) + 215),
TOTALIZER_IDX,
TOTALIZER_IDX_StopQnPartialtotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_434=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((365 * 65536) + 216),
TOTALIZER_IDX,
TOTALIZER_IDX_ResetAllTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_435=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((366 * 65536) + 216),
TOTALIZER_IDX,
TOTALIZER_IDX_ResetQvTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_436=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((367 * 65536) + 216),
TOTALIZER_IDX,
TOTALIZER_IDX_ResetQnTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_437=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((368 * 65536) + 216),
TOTALIZER_IDX,
TOTALIZER_IDX_ResetQmTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_438=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((369 * 65536) + 216),
TOTALIZER_IDX,
TOTALIZER_IDX_ResetEnergyTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_439=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((370 * 65536) + 216),
TOTALIZER_IDX,
TOTALIZER_IDX_ResetQvPartialTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_440=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((371 * 65536) + 216),
TOTALIZER_IDX,
TOTALIZER_IDX_ResetQnPartialTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_97=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_98,
372,
0,
78,
1,
83,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_105=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_106,
373,
0,
84,
1,
90,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_446=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_447,
366,
0,
91,
1,
92,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_449=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_450,
367,
0,
93,
1,
94,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_452=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_453,
368,
0,
95,
1,
96,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_455=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_456,
369,
0,
97,
1,
98,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_458=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_459,
370,
0,
99,
1,
100,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_461=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_462,
371,
0,
101,
1,
102,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_204=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((374 * 65536) + 176),
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_resetCB_Runhour,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
8,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_273=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
375,
CDT_IDX,
CDT_IDX_totalWorkingTimeHour,
0,
0,
188,
1,
FIXED_TEXT_UNITS,
8,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_274=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
311,
CDT_IDX,
CDT_IDX_tranMaintenanceCyc,
0,
0,
188,
1,
FIXED_TEXT_UNITS,
8,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_441=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
176,
376,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_ambientTemperatureMax_obj,
UOM_PF_value_0,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_ambientTemperatureMax_obj,
UOM_PF_unit,
USE_UOM_5,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_282=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
224,
CDT_IDX,
CDT_IDX_converterType,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
171,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_283=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
225,
CDT_IDX,
CDT_IDX_vortexmeterSize,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
173,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_284=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
226,
CDT_IDX,
CDT_IDX_swirlmeterSize,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
193,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_396=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
377,
SVM_IDX,
SVM_IDX_QvMaxDNObj,
UOM_PF_value_0,
SVM_IDX,
SVM_IDX_QvObj,
UOM_PF_unit,
USE_UOM_1,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_145=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
378,
CDT_IDX,
CDT_IDX_intRTDType,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
211,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_300=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
379,
CDT_IDX,
CDT_IDX_temperatureMaxRecord_obj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperatureMaxRecord_obj,
UOM_PF_unit,
USE_UOM_5,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_205=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((374 * 65536) + 173),
CDT_IDX,
CDT_IDX_resetFE_Runhour,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_275=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
375,
CDT_IDX,
CDT_IDX_frontendOperationHour,
0,
0,
188,
1,
FIXED_TEXT_UNITS,
8,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_276=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
311,
CDT_IDX,
CDT_IDX_frontendMaintenance,
0,
0,
188,
1,
FIXED_TEXT_UNITS,
8,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_416=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
380,
CDT_IDX,
CDT_IDX_snr,
0,
0,
247,
1,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_285=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
381,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_snrThreshold,
0,
0,
247,
1,
FIXED_TEXT_UNITS,
8,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_335=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
382,
CDT_IDX,
CDT_IDX_autoselfcheck_Enable,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_349=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
383,
CDT_IDX,
CDT_IDX_spanUpperRange,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
5,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_442=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
384,
CDT_IDX,
CDT_IDX_sigAmplitude,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_443=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
385,
CDT_IDX,
CDT_IDX_vbrAmplitude,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_444=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
386,
CDT_IDX,
CDT_IDX_sigMagnitude,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_470=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
173,
387,
CDT_IDX,
CDT_IDX_calibrationStatus,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
214,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_279=
{
FRAME_TYPE_STRING_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
219,
388,
CDT_IDX,
CDT_IDX_internalRTDPassword,
0,
6,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_280=
{
FRAME_TYPE_STRING_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
219,
389,
CDT_IDX,
CDT_IDX_analogInputPassword,
0,
6,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_281=
{
FRAME_TYPE_STRING_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
219,
390,
CDT_IDX,
CDT_IDX_energyFlowPassword,
0,
6,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_213=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
220,
391,
SVM_IDX,
SVM_IDX_KLinearisation,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
216,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_214=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
220,
392,
SVM_IDX,
SVM_IDX_KSET,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
225,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_215=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_216,
393,
0,
103,
1,
119,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_227=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_228,
394,
0,
120,
1,
136,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_209=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((395 * 65536) + 221),
CDT_IDX,
CDT_IDX_readParameters,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_210=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((396 * 65536) + 221),
CDT_IDX,
CDT_IDX_writeParameters,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_211=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((397 * 65536) + 221),
CDT_IDX,
CDT_IDX_saveCustomerDef,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_212=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((398 * 65536) + 221),
CDT_IDX,
CDT_IDX_saveFactoryDef,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_272=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((399 * 65536) + 221),
CDT_IDX,
CDT_IDX_resetCustomerDef,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_271=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((400 * 65536) + 221),
CDT_IDX,
CDT_IDX_resetFactoryDef,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_239=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_246,
401,
0,
137,
1,
139,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_240=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_249,
402,
0,
140,
1,
145,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_241=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_255,
403,
0,
146,
1,
151,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_242=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_261,
404,
0,
152,
1,
155,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_243=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_265,
405,
0,
156,
1,
161,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_244=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((398 * 65536) + 222),
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_saveAllAsDefault,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
3,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_245=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((400 * 65536) + 222),
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_resetAllToDefault,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
3,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_149=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
236,
445,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_sensorCalDate,
0,
12,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_151=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
236,
446,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_sensorCalCertNo,
0,
20,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_152=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
236,
447,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_sensorCalLocation,
0,
32,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_155=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
239,
448,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_swRevisionHmi_transmitter,
0,
8,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_173=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
239,
449,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_hwRevisionHmi_transmitter,
0,
8,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_156=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
239,
450,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_swRevisionHmi_fe,
0,
8,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_299=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
239,
451,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_hwRevisionHmi_fe,
0,
8,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_157=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
239,
452,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_bootloaderRevision,
0,
8,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_159=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
236,
445,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_tranCalDate,
0,
12,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_160=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
236,
453,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_tranCalCertNo,
0,
20,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_161=
{
FRAME_TYPE_STRING_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
236,
447,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_tranCalLocation,
0,
32,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_360=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
75,
SVM_IDX,
SVM_IDX_QvObj,
UOM_PF_unit,
0,
USE_UOM_1,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_361=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
76,
MVM_IDX,
MVM_IDX_QmObj,
UOM_PF_unit,
0,
USE_UOM_2,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_362=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
456,
MVM_IDX,
MVM_IDX_QnObj,
UOM_PF_unit,
0,
USE_UOM_1,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_363=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
78,
MVM_IDX,
MVM_IDX_QpDampedObj,
UOM_PF_unit,
0,
USE_UOM_3,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_364=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
79,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
0,
USE_UOM_4,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_365=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
80,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
0,
USE_UOM_5,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_366=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
81,
MAPPER_IDX,
MAPPER_IDX_dvPressureObj,
UOM_PF_unit,
0,
USE_UOM_6,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_367=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
457,
TOTALIZER_IDX,
TOTALIZER_IDX_totQvUnit,
UNITS_unitsCode,
0,
USE_UOM_9,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_368=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
458,
TOTALIZER_IDX,
TOTALIZER_IDX_totQmUnit,
UNITS_unitsCode,
0,
USE_UOM_10,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_369=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
459,
TOTALIZER_IDX,
TOTALIZER_IDX_totQnUnit,
UNITS_unitsCode,
0,
USE_UOM_9,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_370=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
252,
85,
TOTALIZER_IDX,
TOTALIZER_IDX_totEnergyUnit,
UNITS_unitsCode,
0,
USE_UOM_11,
0,0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_379=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
112,
MVM_IDX,
MVM_IDX_liquidCorrection,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
51,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_391=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
116,
MVM_IDX,
MVM_IDX_volumeExpandBeta1,
0,
0,
117,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_392=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
118,
MVM_IDX,
MVM_IDX_densityExpandBeta2,
0,
0,
117,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_393=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
460,
MVM_IDX,
MVM_IDX_heatCapacity,
0,
0,
47,
2,
FIXED_TEXT_UNITS,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_380=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
121,
MVM_IDX,
MVM_IDX_gasDensitySelection,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
54,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_381=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
124,
MVM_IDX,
MVM_IDX_gasRef,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
56,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_382=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
129,
MVM_IDX,
MVM_IDX_gasComputeType,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
60,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_385=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
133,
MVM_IDX,
MVM_IDX_calorificEnergy,
0,
0,
134,
0,
FIXED_TEXT_UNITS,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_383=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
135,
MVM_IDX,
MVM_IDX_steamType,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
63,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_384=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
139,
CDT_IDX,
CDT_IDX_actualDensitySelection,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
66,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_471=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
144,
MVM_IDX,
MVM_IDX_steamPwrrFR,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
70,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_386=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
147,
MVM_IDX,
MVM_IDX_densigyRefObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
USE_UOM_4,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_387=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
148,
MAPPER_IDX,
MAPPER_IDX_dvDensityPresetObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
USE_UOM_4,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_388=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
149,
MVM_IDX,
MVM_IDX_refTemperatureObj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_UnitCode,
0,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_389=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
150,
CDT_IDX,
CDT_IDX_temperaturePreset_Obj,
UOM_PF_value_0,
CDT_IDX,
CDT_IDX_temperature_UnitCode,
0,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_390=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
151,
MAPPER_IDX,
MAPPER_IDX_dvTExtPresetObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvTExtObj,
UOM_PF_unit,
USE_UOM_5,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_394=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
152,
MAPPER_IDX,
MAPPER_IDX_dvPressurePresetObj,
UOM_PF_value_0,
MAPPER_IDX,
MAPPER_IDX_dvPressurePresetObj,
UOM_PF_unit,
USE_UOM_6,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_395=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
153,
MAPPER_IDX,
MAPPER_IDX_dvContentPreset,
0,
0,
104,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_301=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
254,
461,
SVM_IDX,
SVM_IDX_dynViscosity,
0,
0,
48,
2,
FIXED_TEXT_UNITS,
10,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_79=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
255,
462,
CDT_IDX,
CDT_IDX_temperatureOffsetCorrectUserDisp,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_138=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((170 * 65536) + 255),
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_writeAutoZero,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_465=
{
FRAME_TYPE_ENUMERATED_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
255,
463,
ELETTRONICSERVICES_IDX,
ELETTRONICSERVICES_IDX_autozeroStatus,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
227,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_464=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
255,
464,
CDT_IDX,
CDT_IDX_lowFlowThresh,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
4,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_474=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
255,
465,
CDT_IDX,
CDT_IDX_ext_algSelection,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
231,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_475=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
255,
401,
CDT_IDX,
CDT_IDX_vbrCorrectionOn,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_31=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
256,
466,
HMI_IDX,
HMI_IDX_displayMode_1,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
235,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_32=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
256,
467,
HMI_IDX,
HMI_IDX_mainOperatorView_1_1,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_33=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
256,
468,
HMI_IDX,
HMI_IDX_mainOperatorView_1_2,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_34=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
256,
469,
HMI_IDX,
HMI_IDX_mainOperatorView_1_3,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_35=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
256,
470,
HMI_IDX,
HMI_IDX_mainOperatorView_1_bargraph,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_38=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
257,
466,
HMI_IDX,
HMI_IDX_displayMode_2,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
235,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_39=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
257,
467,
HMI_IDX,
HMI_IDX_mainOperatorView_2_1,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_40=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
257,
468,
HMI_IDX,
HMI_IDX_mainOperatorView_2_2,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_41=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
257,
469,
HMI_IDX,
HMI_IDX_mainOperatorView_2_3,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_42=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
257,
470,
HMI_IDX,
HMI_IDX_mainOperatorView_2_bargraph,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_54=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
258,
466,
HMI_IDX,
HMI_IDX_displayMode_3,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
235,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_55=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
258,
467,
HMI_IDX,
HMI_IDX_mainOperatorView_3_1,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_56=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
258,
468,
HMI_IDX,
HMI_IDX_mainOperatorView_3_2,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_57=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
258,
469,
HMI_IDX,
HMI_IDX_mainOperatorView_3_3,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_58=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
258,
470,
HMI_IDX,
HMI_IDX_mainOperatorView_3_bargraph,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_60=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
259,
466,
HMI_IDX,
HMI_IDX_displayMode_4,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
235,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_61=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
259,
467,
HMI_IDX,
HMI_IDX_mainOperatorView_4_1,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_62=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
259,
468,
HMI_IDX,
HMI_IDX_mainOperatorView_4_2,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_63=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
259,
469,
HMI_IDX,
HMI_IDX_mainOperatorView_4_3,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_64=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
259,
470,
HMI_IDX,
HMI_IDX_mainOperatorView_4_bargraph,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
248,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_397=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
282,
65,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_pulseFactor,
FLOAT_FIXEDUNIT_value,
0,
66,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_398=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
282,
67,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_pulseWidth,
FLOAT_FIXEDUNIT_value,
0,
68,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_399=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
283,
69,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_minFrequency,
FLOAT_FIXEDUNIT_value,
0,
70,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_400=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
283,
71,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_maxFrequency,
FLOAT_FIXEDUNIT_value,
0,
70,
0,
FIXED_TEXT_UNITS,
8,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_183=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
284,
471,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_generalAlarm,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_184=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
284,
289,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_minFlowrateAlarm,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_185=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
284,
290,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_maxFlowrateAlarm,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_186=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
284,
472,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_maxTemperatureAlarm,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_350=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
284,
473,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_minTemperatureAlarm,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_187=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
284,
474,
DIGITALOUTPUT_IDX,
DIGITALOUTPUT_IDX_lowFlowCutOff,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_484=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
304,
478,
ETH_IDX,
ETH_IDX_selectedMainProtocol,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
19,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_485=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
305,
479,
ETH_IDX,
ETH_IDX_useDhcp,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
22,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_486=
{
FRAME_TYPE_IP_ADDRESS,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
305,
480,
ETH_IDX,
ETH_IDX_hostAddress,
0,
0,
0,
0,
0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_487=
{
FRAME_TYPE_IP_ADDRESS,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
305,
481,
ETH_IDX,
ETH_IDX_subnetMask,
0,
0,
0,
0,
0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_488=
{
FRAME_TYPE_IP_ADDRESS,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
305,
482,
ETH_IDX,
ETH_IDX_gatewayAddress,
0,
0,
0,
0,
0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_489=
{
FRAME_TYPE_IP_ADDRESS,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
305,
483,
ETH_IDX,
ETH_IDX_ntpServerAddress,
0,
0,
0,
0,
0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_490=
{
FRAME_TYPE_STRING_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
305,
484,
ETH_IDX,
ETH_IDX_hostName,
0,
31,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_491=
{
FRAME_TYPE_STRING_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
305,
485,
ETH_IDX,
ETH_IDX_domainName,
0,
31,
CHARACTER_SET_STANDARD,
0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_492=
{
FRAME_TYPE_IP_ADDRESS,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
305,
486,
ETH_IDX,
ETH_IDX_dnsAddress,
0,
0,
0,
0,
0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_493=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
306,
487,
ETH_IDX,
ETH_IDX_enableProfinetIo,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
24,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_494=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_494,
488,
0,
162,
1,
161,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_495=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_495,
489,
0,
162,
1,
161,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_496=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
&f_496,
490,
0,
162,
1,
161,
1,
0,0,0,0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_497=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
307,
487,
ETH_IDX,
ETH_IDX_enableModbusTcp,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
24,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_498=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
307,
491,
MODBUS_IDX,
MODBUS_IDX_modbusIEEEFormat,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
28,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_499=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
308,
487,
ETH_IDX,
ETH_IDX_enableWebserver,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
30,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_500=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((492 * 65536) + 308),
WEBSRV_IDX,
0,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_501=
{
FRAME_TYPE_TIME_AND_DATE_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
308,
493,
WEBSRV_IDX,
WEBSRV_IDX_sessionTimeoutInMinutes,
0,
0,
0,
0,
0,
0,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_502=
{
FRAME_TYPE_INTEGER_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
309,
494,
ETH_IDX,
ETH_IDX_ipStats,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_503=
{
FRAME_TYPE_INTEGER_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
309,
495,
ETH_IDX,
ETH_IDX_port1EthStatsHMI,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_504=
{
FRAME_TYPE_INTEGER_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
309,
496,
ETH_IDX,
ETH_IDX_port1EthStatsHMI,
1,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_505=
{
FRAME_TYPE_INTEGER_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
309,
497,
ETH_IDX,
ETH_IDX_port1EthStatsHMI,
2,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_506=
{
FRAME_TYPE_INTEGER_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
309,
498,
ETH_IDX,
ETH_IDX_port1EthStatsHMI,
3,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_98=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
372,
366,
TOTALIZER_IDX,
TOTALIZER_IDX_totQvPresetValue,
0,
TOTALIZER_IDX,
TOTALIZER_IDX_totQvUnit,
UNITS_unitsCode,
USE_UOM_9,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_99=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
372,
367,
TOTALIZER_IDX,
TOTALIZER_IDX_totQnPresetValue,
0,
TOTALIZER_IDX,
TOTALIZER_IDX_totQnUnit,
UNITS_unitsCode,
USE_UOM_9,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_100=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
372,
368,
TOTALIZER_IDX,
TOTALIZER_IDX_totQmPresetValue,
0,
TOTALIZER_IDX,
TOTALIZER_IDX_totQmUnit,
UNITS_unitsCode,
USE_UOM_10,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_101=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
372,
369,
TOTALIZER_IDX,
TOTALIZER_IDX_totEnergyPresetValue,
0,
TOTALIZER_IDX,
TOTALIZER_IDX_totEnergyUnit,
UNITS_unitsCode,
USE_UOM_11,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_102=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
372,
370,
TOTALIZER_IDX,
TOTALIZER_IDX_totQvPartialPresetValue,
0,
TOTALIZER_IDX,
TOTALIZER_IDX_totQvUnit,
UNITS_unitsCode,
USE_UOM_9,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_103=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
372,
371,
TOTALIZER_IDX,
TOTALIZER_IDX_totQnPartialPresetValue,
0,
TOTALIZER_IDX,
TOTALIZER_IDX_totQnUnit,
UNITS_unitsCode,
USE_UOM_9,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_106=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((365 * 65536) + 373),
TOTALIZER_IDX,
TOTALIZER_IDX_PresetAllTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_107=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((366 * 65536) + 373),
TOTALIZER_IDX,
TOTALIZER_IDX_PresetQvTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_108=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((367 * 65536) + 373),
TOTALIZER_IDX,
TOTALIZER_IDX_PresetQnTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_109=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((368 * 65536) + 373),
TOTALIZER_IDX,
TOTALIZER_IDX_PresetQmTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_110=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((369 * 65536) + 373),
TOTALIZER_IDX,
TOTALIZER_IDX_PresetEnergyTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_111=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((370 * 65536) + 373),
TOTALIZER_IDX,
TOTALIZER_IDX_PresetQvPartialTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_112=
{
FRAME_TYPE_AUTO_ADJUST,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
(const FRAME_FORMAT_TYPE*) ((371 * 65536) + 373),
TOTALIZER_IDX,
TOTALIZER_IDX_PresetQnPartialTotalizer,
0,
NO_DISPLAY_ON_ADJUST,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_447=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
366,
499,
TOTALIZER_IDX,
TOTALIZER_IDX_totQvOverrollCnt,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_448=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
366,
500,
TOTALIZER_IDX,
TOTALIZER_IDX_totOverrollBase,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_450=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
367,
499,
TOTALIZER_IDX,
TOTALIZER_IDX_totQnOverrollCnt,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_451=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
367,
500,
TOTALIZER_IDX,
TOTALIZER_IDX_totOverrollBase,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_453=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
368,
499,
TOTALIZER_IDX,
TOTALIZER_IDX_totQmOverrollCnt,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_454=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
368,
500,
TOTALIZER_IDX,
TOTALIZER_IDX_totOverrollBase,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_456=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
369,
499,
TOTALIZER_IDX,
TOTALIZER_IDX_totEnergyOverrollCnt,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_457=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
369,
500,
TOTALIZER_IDX,
TOTALIZER_IDX_totOverrollBase,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_459=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
370,
499,
TOTALIZER_IDX,
TOTALIZER_IDX_totQvPartialOverrollCnt,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_460=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
370,
500,
TOTALIZER_IDX,
TOTALIZER_IDX_totOverrollBase,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_462=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
371,
499,
TOTALIZER_IDX,
TOTALIZER_IDX_totQnPartialOverrollCnt,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_463=
{
FRAME_TYPE_NUMERICAL_PARAM_NO_EDIT,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
371,
500,
TOTALIZER_IDX,
TOTALIZER_IDX_totOverrollBase,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_216=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
517,
SVM_IDX,
SVM_IDX_KAverageLiquid,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_217=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
518,
SVM_IDX,
SVM_IDX_KLiquid,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_218=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
519,
SVM_IDX,
SVM_IDX_KLiquid,
1,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_219=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
520,
SVM_IDX,
SVM_IDX_KLiquid,
2,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_220=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
521,
SVM_IDX,
SVM_IDX_KLiquid,
3,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_221=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
522,
SVM_IDX,
SVM_IDX_KLiquid,
4,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_336=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
523,
SVM_IDX,
SVM_IDX_KLiquid,
5,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_337=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
524,
SVM_IDX,
SVM_IDX_KLiquid,
6,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_338=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
525,
SVM_IDX,
SVM_IDX_KLiquid,
7,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_222=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
526,
SVM_IDX,
SVM_IDX_KLiquidFreq,
0,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_223=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
527,
SVM_IDX,
SVM_IDX_KLiquidFreq,
1,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_224=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
528,
SVM_IDX,
SVM_IDX_KLiquidFreq,
2,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_225=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
529,
SVM_IDX,
SVM_IDX_KLiquidFreq,
3,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_226=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
530,
SVM_IDX,
SVM_IDX_KLiquidFreq,
4,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_339=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
531,
SVM_IDX,
SVM_IDX_KLiquidFreq,
5,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_341=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
532,
SVM_IDX,
SVM_IDX_KLiquidFreq,
6,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_340=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
393,
533,
SVM_IDX,
SVM_IDX_KLiquidFreq,
7,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_228=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
534,
SVM_IDX,
SVM_IDX_KAverageGas,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_229=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
518,
SVM_IDX,
SVM_IDX_KGas,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_230=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
519,
SVM_IDX,
SVM_IDX_KGas,
1,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_231=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
520,
SVM_IDX,
SVM_IDX_KGas,
2,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_232=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
521,
SVM_IDX,
SVM_IDX_KGas,
3,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_233=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
522,
SVM_IDX,
SVM_IDX_KGas,
4,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_342=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
523,
SVM_IDX,
SVM_IDX_KGas,
5,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_343=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
524,
SVM_IDX,
SVM_IDX_KGas,
6,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_344=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
525,
SVM_IDX,
SVM_IDX_KGas,
7,
0,
171,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_234=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
526,
SVM_IDX,
SVM_IDX_KGasFreq,
0,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_235=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
527,
SVM_IDX,
SVM_IDX_KGasFreq,
1,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_236=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
528,
SVM_IDX,
SVM_IDX_KGasFreq,
2,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_237=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
529,
SVM_IDX,
SVM_IDX_KGasFreq,
3,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_238=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
530,
SVM_IDX,
SVM_IDX_KGasFreq,
4,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_345=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
531,
SVM_IDX,
SVM_IDX_KGasFreq,
5,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_347=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
532,
SVM_IDX,
SVM_IDX_KGasFreq,
6,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_346=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
394,
533,
SVM_IDX,
SVM_IDX_KGasFreq,
7,
0,
70,
0,
FIXED_TEXT_UNITS,
12,
AUTO_RANGING,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_246=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
401,
535,
CDT_IDX,
CDT_IDX_vbrCorrectionOn,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_247=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
401,
536,
CDT_IDX,
CDT_IDX_vbrFilStep,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
4,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_248=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
401,
537,
CDT_IDX,
CDT_IDX_vbrFilNumCoef,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
3,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_249=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
402,
538,
CDT_IDX,
CDT_IDX_lockMinIntPeriod,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
2,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_250=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
402,
539,
CDT_IDX,
CDT_IDX_lockMaxIntPeriod,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
2,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_251=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
402,
540,
CDT_IDX,
CDT_IDX_lockEstimation,
0,
0,
104,
0,
FIXED_TEXT_UNITS,
2,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_252=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
402,
541,
CDT_IDX,
CDT_IDX_accuracyHigh,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
1,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_253=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
402,
542,
CDT_IDX,
CDT_IDX_accuracyMiddle,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
1,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_254=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
402,
543,
CDT_IDX,
CDT_IDX_accuracyLow,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
1,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_255=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
403,
544,
CDT_IDX,
CDT_IDX_analogGain,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
36,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_256=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
403,
545,
CDT_IDX,
CDT_IDX_gainCtrl,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_257=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
403,
546,
CDT_IDX,
CDT_IDX_gainLowThresh,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_258=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
403,
547,
CDT_IDX,
CDT_IDX_gainHighThresh,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
6,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_259=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
403,
548,
CDT_IDX,
CDT_IDX_gainMarginFreq,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
4,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_260=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
403,
549,
CDT_IDX,
CDT_IDX_gainMaxShift,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
1,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_261=
{
FRAME_TYPE_ENUMERATED_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
404,
550,
CDT_IDX,
CDT_IDX_lowFlowThreshOn,
0,
0,
UOM_NOT_APPLICABLE,
0,0,
113,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_262=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
404,
464,
CDT_IDX,
CDT_IDX_lowFlowThresh,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
4,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_263=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
404,
551,
CDT_IDX,
CDT_IDX_lowFlowThreshCoef,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
2,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_264=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
404,
552,
CDT_IDX,
CDT_IDX_noiseSampSize,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
1,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_265=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
405,
553,
CDT_IDX,
CDT_IDX_staticDecimation,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
1,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_266=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
405,
554,
CDT_IDX,
CDT_IDX_middleBufDecimation,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
1,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_267=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
405,
555,
CDT_IDX,
CDT_IDX_maxFlowFreq,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
7,
2,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_268=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
405,
556,
CDT_IDX,
CDT_IDX_minFlowFreq,
0,
0,
171,
0,
FIXED_TEXT_UNITS,
7,
2,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_269=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
405,
557,
CDT_IDX,
CDT_IDX_freqMinMarginPer,
0,
0,
104,
0,
FIXED_TEXT_UNITS,
2,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_270=
{
FRAME_TYPE_NUMERICAL_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
405,
558,
CDT_IDX,
CDT_IDX_freqMaxMarginPer,
0,
0,
104,
0,
FIXED_TEXT_UNITS,
2,
0,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};


#define STARTINGFRAME     &f_0
#define NVTESTFRAME       0
#define OPERATORMENUFRAME &f_OPERATORMENU
#define SIGNALSVIEWFRAME  &f_SIGNALSVIEWMENU
#define DIAGVIEWFRAME     &f_DIAGNOSTICSVIEWMENU
#define ARMLEVELSELECTION &f_ACCESSLEVELSELECTION
#define ARMPASSWORDENTRY  &f_ACCESSLEVELPASSWORDENTRY
#define RESETPASSWORDS    &f_RESETPASSWORDS
#define MODESELECTION     0

const FRAME_FORMAT_TYPE *FIRSTCONFIGHEADER = &f_1;
const FRAME_FORMAT_TYPE *LASTCONFIGHEADER  = &f_141;


const SIGNAL_TABLE_TYPE signalTable[] =
{
SVM_IDX,
SVM_IDX_QDPercentage,
0,
3,
FIXED_TEXT_UNITS,
0,
104,
0,
679,
680,
HART_IDX,
HART_IDX_bolDeviceIsBusy,
0,
HART_IDX,
0,
0,
1,
IDF_VALID_always,

SVM_IDX,
SVM_IDX_QvDampedObj,
UOM_PF_value_0,
CONFIGURABLE_DP_1,
USE_UOM_1,
SVM_IDX,
SVM_IDX_QvObj,
UOM_PF_unit,
681,
682,
SVM_IDX,
SVM_IDX_QvRangeObj,
UOM_PF_value_1,
SVM_IDX,
SVM_IDX_QvRangeObj,
UOM_PF_value_0,
0,
IDF_VALID_always,

SVM_IDX,
SVM_IDX_QvPercentage,
0,
3,
FIXED_TEXT_UNITS,
0,
104,
0,
683,
684,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
1,
IDF_VALID_always,

MVM_IDX,
MVM_IDX_QvPartialDampedObj,
UOM_PF_value_0,
CONFIGURABLE_DP_1,
USE_UOM_1,
MVM_IDX,
MVM_IDX_QvPartialObj,
UOM_PF_unit,
685,
686,
MVM_IDX,
MVM_IDX_QvPartialRangeObj,
UOM_PF_value_1,
MVM_IDX,
MVM_IDX_QvPartialRangeObj,
UOM_PF_value_0,
0,
IDF_VALID_if_SignalsViewValidityTest_1,

MVM_IDX,
MVM_IDX_QmDampedObj,
UOM_PF_value_0,
CONFIGURABLE_DP_1,
USE_UOM_2,
MVM_IDX,
MVM_IDX_QmObj,
UOM_PF_unit,
687,
688,
MVM_IDX,
MVM_IDX_QmRangeObj,
UOM_PF_value_1,
MVM_IDX,
MVM_IDX_QmRangeObj,
UOM_PF_value_0,
0,
IDF_VALID_if_SignalsViewValidityTest_4,

MVM_IDX,
MVM_IDX_QnDampedObj,
UOM_PF_value_0,
CONFIGURABLE_DP_1,
USE_UOM_1,
MVM_IDX,
MVM_IDX_QnObj,
UOM_PF_unit,
689,
690,
MVM_IDX,
MVM_IDX_QnRangeObj,
UOM_PF_value_1,
MVM_IDX,
MVM_IDX_QnRangeObj,
UOM_PF_value_0,
0,
IDF_VALID_if_SignalsViewValidityTest_3,

MVM_IDX,
MVM_IDX_QnPartialDampedObj,
UOM_PF_value_0,
CONFIGURABLE_DP_1,
USE_UOM_1,
MVM_IDX,
MVM_IDX_QnPartialObj,
UOM_PF_unit,
691,
692,
MVM_IDX,
MVM_IDX_QnPartialRangeObj,
UOM_PF_value_1,
MVM_IDX,
MVM_IDX_QnPartialRangeObj,
UOM_PF_value_0,
0,
IDF_VALID_if_SignalsViewValidityTest_2,

MVM_IDX,
MVM_IDX_QpDampedObj,
UOM_PF_value_0,
CONFIGURABLE_DP_1,
USE_UOM_3,
MVM_IDX,
MVM_IDX_QpObj,
UOM_PF_unit,
693,
694,
MVM_IDX,
MVM_IDX_QpRangeObj,
UOM_PF_value_1,
MVM_IDX,
MVM_IDX_QpRangeObj,
UOM_PF_value_0,
0,
IDF_VALID_if_SignalsViewValidityTest_5,

CDT_IDX,
CDT_IDX_temperature_Damped_Obj,
UOM_PF_value_0,
3,
USE_UOM_5,
CDT_IDX,
CDT_IDX_temperature_Obj,
UOM_PF_unit,
695,
589,
CDT_IDX,
CDT_IDX_temperature_range_Obj,
UOM_PF_value_1,
CDT_IDX,
CDT_IDX_temperature_range_Obj,
UOM_PF_value_0,
0,
IDF_VALID_if_SignalsViewValidityTest_8,

MAPPER_IDX,
MAPPER_IDX_dvPressureObj,
UOM_PF_value_0,
3,
USE_UOM_6,
MAPPER_IDX,
MAPPER_IDX_dvPressureObj,
UOM_PF_unit,
696,
697,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeObj,
UOM_PF_value_1,
MAPPER_IDX,
MAPPER_IDX_dvPressureRangeObj,
UOM_PF_value_0,
0,
IDF_VALID_if_SignalsViewValidityTest_6,

MAPPER_IDX,
MAPPER_IDX_dvContent,
0,
3,
FIXED_TEXT_UNITS,
0,
104,
0,
698,
699,
MAPPER_IDX,
MAPPER_IDX_dvContentRange,
DEV_RNGFLT_loLim,
MAPPER_IDX,
MAPPER_IDX_dvContentRange,
DEV_RNGFLT_hiLim,
0,
IDF_VALID_if_SignalsViewValidityTest_1,

MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_value_0,
3,
USE_UOM_4,
MAPPER_IDX,
MAPPER_IDX_dvDensityObj,
UOM_PF_unit,
700,
701,
MAPPER_IDX,
MAPPER_IDX_dvDensityRangeObj,
UOM_PF_value_1,
MAPPER_IDX,
MAPPER_IDX_dvDensityRangeObj,
UOM_PF_value_0,
0,
IDF_VALID_always,

TOTALIZER_IDX,
TOTALIZER_IDX_totDispQv,
TOTDISP_value,
CONFIGURABLE_DP_2,
USE_UOM_9,
TOTALIZER_IDX,
TOTALIZER_IDX_totQvUnit,
UNITS_unitsCode,
702,
703,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_7,

TOTALIZER_IDX,
TOTALIZER_IDX_totDispQn,
TOTDISP_value,
CONFIGURABLE_DP_2,
USE_UOM_9,
TOTALIZER_IDX,
TOTALIZER_IDX_totQnUnit,
UNITS_unitsCode,
704,
705,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_3,

TOTALIZER_IDX,
TOTALIZER_IDX_totDispQm,
TOTDISP_value,
CONFIGURABLE_DP_2,
USE_UOM_10,
TOTALIZER_IDX,
TOTALIZER_IDX_totQmUnit,
UNITS_unitsCode,
706,
707,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_11,

TOTALIZER_IDX,
TOTALIZER_IDX_totDispQvPartial,
TOTDISP_value,
CONFIGURABLE_DP_2,
USE_UOM_9,
TOTALIZER_IDX,
TOTALIZER_IDX_totQvUnit,
UNITS_unitsCode,
708,
709,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_9,

TOTALIZER_IDX,
TOTALIZER_IDX_totDispQnPartial,
TOTDISP_value,
CONFIGURABLE_DP_2,
USE_UOM_9,
TOTALIZER_IDX,
TOTALIZER_IDX_totQnUnit,
UNITS_unitsCode,
710,
711,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_10,

TOTALIZER_IDX,
TOTALIZER_IDX_totDispEnergy,
TOTDISP_value,
CONFIGURABLE_DP_2,
USE_UOM_11,
TOTALIZER_IDX,
TOTALIZER_IDX_totEnergyUnit,
UNITS_unitsCode,
712,
713,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_12,

CDT_IDX,
CDT_IDX_frequency_Damped_Obj,
UOM_PF_value_0,
3,
USE_UOM_7,
CDT_IDX,
CDT_IDX_frequency_Obj,
UOM_PF_unit,
714,
715,
CDT_IDX,
CDT_IDX_frequency_range_Obj,
UOM_PF_value_1,
CDT_IDX,
CDT_IDX_frequency_range_Obj,
UOM_PF_value_0,
0,
IDF_VALID_always,

CDT_IDX,
CDT_IDX_temperatureSensor_Obj,
UOM_PF_value_0,
3,
USE_UOM_5,
CDT_IDX,
CDT_IDX_temperatureSensor_Obj,
UOM_PF_unit,
716,
717,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_always,

TOTALIZER_IDX,
TOTALIZER_IDX_totQvOverrollCnt,
0,
0,
FIXED_TEXT_UNITS,
0,
171,
0,
718,
719,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_7,

TOTALIZER_IDX,
TOTALIZER_IDX_totQnOverrollCnt,
0,
0,
FIXED_TEXT_UNITS,
0,
171,
0,
720,
721,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_3,

TOTALIZER_IDX,
TOTALIZER_IDX_totQmOverrollCnt,
0,
0,
FIXED_TEXT_UNITS,
0,
171,
0,
722,
723,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_11,

TOTALIZER_IDX,
TOTALIZER_IDX_totQvPartialOverrollCnt,
0,
0,
FIXED_TEXT_UNITS,
0,
171,
0,
724,
725,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_9,

TOTALIZER_IDX,
TOTALIZER_IDX_totQnPartialOverrollCnt,
0,
0,
FIXED_TEXT_UNITS,
0,
171,
0,
726,
727,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_10,

TOTALIZER_IDX,
TOTALIZER_IDX_totEnergyOverrollCnt,
0,
0,
FIXED_TEXT_UNITS,
0,
171,
0,
728,
729,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_if_SignalsViewValidityTest_12,

TOTALIZER_IDX,
TOTALIZER_IDX_totOverrollBase,
0,
1,
FIXED_TEXT_UNITS,
0,
171,
0,
730,
731,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_always,

CDT_IDX,
CDT_IDX_temperatureSensor_Obj,
UOM_PF_value_0,
3,
USE_UOM_5,
CDT_IDX,
CDT_IDX_temperatureSensor_Obj,
UOM_PF_unit,
716,
717,
HART_IDX,
0,
0,
HART_IDX,
0,
0,
3,
IDF_VALID_always,

};

// OperatorMenu Frame Definition
__root const FRAME_FORMAT_TYPE f_OPERATORMENU=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
NULL,
732,
0,
162,
1,
168,
1,
0,0,0,0,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_SIGNALSVIEWMENU=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
NULL,
738,
0,
0,
0,
27,
0,
0,0,0,0,
2,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_DIAGNOSTICSVIEWMENU=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
NULL,
26,
0,
0,
0,
0,
0,
0,0,0,0,
3,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};


// Security Access Frame Definition
__root const FRAME_FORMAT_TYPE f_ACCESSLEVELSELECTION=
{
FRAME_TYPE_MENU,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
NULL,
739,
0,
169,
1,
173,
1,
0,0,0,0,
4,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_ACCESSLEVELPASSWORDENTRY=
{
FRAME_TYPE_STRING_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
IDS_BLANK,
744,
HMI_IDX,
HMI_IDX_Password,
0,
6,
5,
0,0,0,
1,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};

__root const FRAME_FORMAT_TYPE f_RESETPASSWORDS=
{
FRAME_TYPE_STRING_PARAM,
#ifdef HMI_USES_SPECIAL_OP_CODES
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
IDF_SPECIAL_NONE,
#endif
//lint -e{708}
0,
IDS_BLANK,
743,
HMI_IDX,
HMI_IDX_Password,
0,
6,
5,
0,0,0,
2,
#ifdef HMI_SUPPORTS_EASYSETUP
0,
#endif
};


#define CUSTOM_UNIT_1_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_1_MAPPED_OBJECT    0

#define CUSTOM_UNIT_2_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_2_MAPPED_OBJECT    0

#define CUSTOM_UNIT_3_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_3_MAPPED_OBJECT    0

#define CUSTOM_UNIT_4_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_4_MAPPED_OBJECT    0

#define CUSTOM_UNIT_5_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_5_MAPPED_OBJECT    0

#define CUSTOM_UNIT_6_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_6_MAPPED_OBJECT    0

#define CUSTOM_UNIT_7_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_7_MAPPED_OBJECT    0

#define CUSTOM_UNIT_8_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_8_MAPPED_OBJECT    0

#define CUSTOM_UNIT_9_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_9_MAPPED_OBJECT    0

#define CUSTOM_UNIT_10_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_10_MAPPED_OBJECT    0

#define CUSTOM_UNIT_11_MAPPED_SUBSYSTEM 0
#define CUSTOM_UNIT_11_MAPPED_OBJECT    0


const STRING_OBJECT_EXTENSION_TYPE StringObjectExtensionPageTable[] =
{
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorID, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 8, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorSapErpNo, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 20, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranType, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 12, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranID, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 8, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranSapErpNo, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 20, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_manufacturer, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 20, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_street, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 20, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_city, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 20, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_phone, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 20, 0, CHARACTER_SET_STANDARD},
    {HMI_IDX, HMI_IDX_SetPassword_StandardLevel, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 6, 0, CHARACTER_SET_PASSWORD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorLocationTag, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 20, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensoTag, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 20, 0, CHARACTER_SET_STANDARD},
    {CDT_IDX, CDT_IDX_internalRTDPassword, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 6, 0, CHARACTER_SET_STANDARD},
    {CDT_IDX, CDT_IDX_analogInputPassword, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 6, 0, CHARACTER_SET_STANDARD},
    {CDT_IDX, CDT_IDX_energyFlowPassword, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 6, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorCalDate, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 12, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorCalCertNo, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 20, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_sensorCalLocation, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 32, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_swRevisionHmi_transmitter, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 8, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_hwRevisionHmi_transmitter, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 8, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_swRevisionHmi_fe, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 8, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_hwRevisionHmi_fe, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 8, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_bootloaderRevision, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 8, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranCalDate, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 12, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranCalCertNo, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 20, 0, CHARACTER_SET_STANDARD},
    {ELETTRONICSERVICES_IDX, ELETTRONICSERVICES_IDX_tranCalLocation, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 32, 0, CHARACTER_SET_STANDARD},
    {ETH_IDX, ETH_IDX_hostName, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 31, 0, CHARACTER_SET_STANDARD},
    {ETH_IDX, ETH_IDX_domainName, HMI_EXTENDED_CHARACTER_PAGE_EUROPEAN, 31, 0, CHARACTER_SET_STANDARD},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};

#pragma diag_default=Pm022


#endif
