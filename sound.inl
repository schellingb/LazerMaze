/*
  Lazer Maze
  Copyright (C) 2018 Bernhard Schelling

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCTheme_OrderTable[] = {
	0x000000001, 0x000000001, 0x010000001, 0x011000001, 0x011000002, 0x011000003, 0x011000001, 0x011000004,
	0x001000005,
};
static const unsigned char IMCTheme_PatternData[] = {
	0x54, 0, 0, 0, 0x45, 0, 0, 0, 0x49, 0, 0, 0, 0x59, 0, 0, 0,
	0x54, 0, 0x54, 0, 0x5B, 0, 0x59, 0, 0x55, 0, 0x54, 0, 0x50, 0, 0, 0,
	0x57, 0, 0x57, 0, 0x55, 0, 0x60, 0, 0x5B, 0, 0x59, 0, 0x57, 0, 0x55, 0,
	0x4B, 0x4B, 0, 0, 0x49, 0x49, 0, 0, 0x47, 0x47, 0, 0, 0x45, 0x45, 0, 0,
	0x44, 0, 0, 0, 0x44, 0, 0, 0, 0x44, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0x42, 0, 0, 0, 0x42, 0, 0, 0, 0x44, 0, 0, 0x45, 0, 0,
	0x50, 0x50, 0, 0, 0x50, 0x50, 0, 0, 0x50, 0x50, 0, 0, 0x50, 0x50, 0, 0,
};
static const unsigned char IMCTheme_PatternLookupTable[] = { 0, 5, 5, 5, 5, 5, 5, 6, };
static const TImcSongEnvelope IMCTheme_EnvList[] = {
	{ 0, 256, 19, 8, 15, 255, true, 255, },
	{ 50, 100, 201, 24, 255, 255, true, 255, },
	{ 0, 256, 2, 27, 13, 255, false, 255, },
	{ 0, 256, 157, 25, 15, 255, false, 0, },
	{ 0, 256, 93, 8, 16, 255, true, 255, },
	{ 196, 256, 52, 3, 21, 255, true, 255, },
	{ 0, 256, 260, 8, 16, 255, true, 255, },
	{ 0, 128, 435, 7, 8, 255, true, 255, },
	{ 0, 256, 325, 8, 16, 255, true, 255, },
	{ 0, 256, 653, 8, 8, 255, true, 255, },
	{ 0, 256, 2703, 8, 16, 255, true, 255, },
	{ 0, 386, 65, 8, 16, 255, true, 255, },
	{ 0, 256, 32, 8, 16, 255, true, 255, },
	{ 128, 256, 130, 8, 16, 255, true, 255, },
	{ 0, 128, 196, 8, 16, 255, true, 255, },
	{ 0, 256, 129, 5, 19, 255, true, 255, },
	{ 0, 256, 196, 8, 16, 255, true, 255, },
};
static TImcSongEnvelopeCounter IMCTheme_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 50 }, { 2, 0, 18 },
	{ 3, 0, 2 }, { -1, -1, 400 }, { 4, 6, 256 }, { 5, 6, 244 },
	{ 6, 6, 256 }, { 7, 6, 127 }, { 8, 6, 256 }, { 9, 6, 256 },
	{ 10, 6, 256 }, { 11, 7, 386 }, { 12, 7, 256 }, { 13, 7, 256 },
	{ 14, 7, 128 }, { 15, 7, 238 }, { 16, 7, 256 },
};
static const TImcSongOscillator IMCTheme_OscillatorList[] = {
	{ 7, 0, IMCSONGOSCTYPE_SINE, 0, -1, 130, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 0, -1, 86, 1, 1 },
	{ 7, 0, IMCSONGOSCTYPE_SINE, 0, -1, 148, 1, 1 },
	{ 9, 0, IMCSONGOSCTYPE_SQUARE, 0, 0, 72, 1, 1 },
	{ 7, 0, IMCSONGOSCTYPE_SAW, 0, 1, 60, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
	{ 10, 86, IMCSONGOSCTYPE_SQUARE, 5, -1, 84, 0, 0 },
	{ 8, 1, IMCSONGOSCTYPE_SQUARE, 5, -1, 50, 0, 0 },
	{ 8, 1, IMCSONGOSCTYPE_SAW, 5, 9, 255, 0, 0 },
	{ 9, 150, IMCSONGOSCTYPE_SQUARE, 5, 10, 238, 0, 0 },
	{ 5, 66, IMCSONGOSCTYPE_SINE, 6, -1, 98, 6, 7 },
	{ 7, 0, IMCSONGOSCTYPE_SINE, 6, -1, 255, 8, 9 },
	{ 6, 0, IMCSONGOSCTYPE_SINE, 6, -1, 192, 10, 11 },
	{ 9, 150, IMCSONGOSCTYPE_SINE, 6, -1, 255, 12, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 6, 16, 255, 1, 1 },
	{ 5, 150, IMCSONGOSCTYPE_SINE, 7, -1, 255, 14, 15 },
	{ 9, 0, IMCSONGOSCTYPE_NOISE, 7, -1, 255, 16, 1 },
	{ 5, 200, IMCSONGOSCTYPE_SINE, 7, -1, 160, 17, 1 },
	{ 5, 31, IMCSONGOSCTYPE_SINE, 7, -1, 255, 18, 1 },
};
static const TImcSongEffect IMCTheme_EffectList[] = {
	{ 0, 0, 101, 0, IMCSONGEFFECTTYPE_FLANGE, 2, 0 },
	{ 53, 0, 1, 0, IMCSONGEFFECTTYPE_LOWPASS, 1, 0 },
	{ 255, 165, 1, 0, IMCSONGEFFECTTYPE_RESONANCE, 3, 4 },
	{ 124, 0, 24804, 0, IMCSONGEFFECTTYPE_DELAY, 0, 0 },
	{ 189, 128, 1, 6, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
	{ 193, 0, 1, 7, IMCSONGEFFECTTYPE_LOWPASS, 1, 0 },
	{ 226, 177, 1, 7, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
};
static unsigned char IMCTheme_ChannelVol[8] = { 84, 100, 100, 100, 100, 33, 30, 33 };
static const unsigned char IMCTheme_ChannelEnvCounter[8] = { 0, 0, 0, 0, 0, 0, 5, 13 };
static const bool IMCTheme_ChannelStopNote[8] = { true, false, false, false, false, true, true, true };
static TImcSongData imcDataIMCTheme = {
	/*LEN*/ 0x9, /*ROWLENSAMPLES*/ 8268, /*ENVLISTSIZE*/ 17, /*ENVCOUNTERLISTSIZE*/ 19, /*OSCLISTSIZE*/ 22, /*EFFECTLISTSIZE*/ 7, /*VOL*/ 50,
	IMCTheme_OrderTable, IMCTheme_PatternData, IMCTheme_PatternLookupTable, IMCTheme_EnvList, IMCTheme_EnvCounterList, IMCTheme_OscillatorList, IMCTheme_EffectList,
	IMCTheme_ChannelVol, IMCTheme_ChannelEnvCounter, IMCTheme_ChannelStopNote };
static ZL_SynthImcTrack imcTheme(&imcDataIMCTheme);

//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCEnding_OrderTable[] = {
	0x000000011, 0x000000022,
};
static const unsigned char IMCEnding_PatternData[] = {
	0x50, 0, 0x50, 0x50, 0x50, 0, 0x54, 0, 0x54, 0x54, 0x54, 0, 0x57, 0, 0x5B, 0,
	0x60, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0x50, 0, 0x50, 0x50, 0x50, 0, 0x54, 0, 0x54, 0x54, 0x54, 0, 0x57, 0, 0x5B, 0,
	0x62, 0x64, 0x62, 0x60, 0x64, 0x64, 0x62, 0x60, 0x64, 0x62, 0x64, 0x60, 0, 0, 0, 0,
};
static const unsigned char IMCEnding_PatternLookupTable[] = { 0, 2, 4, 4, 4, 4, 4, 4, };
static const TImcSongEnvelope IMCEnding_EnvList[] = {
	{ 0, 256, 97, 8, 16, 0, true, 255, },
	{ 0, 256, 379, 8, 15, 255, true, 255, },
	{ 0, 256, 17, 8, 15, 255, true, 255, },
	{ 0, 256, 173, 1, 23, 255, true, 255, },
	{ 128, 256, 174, 8, 16, 16, true, 255, },
	{ 0, 256, 2092, 8, 16, 16, true, 255, },
	{ 0, 256, 523, 8, 16, 255, true, 255, },
};
static TImcSongEnvelopeCounter IMCEnding_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 254 }, { 2, 0, 256 },
	{ 3, 1, 158 }, { 4, 1, 256 }, { 5, 1, 256 }, { 6, 1, 256 },
};
static const TImcSongOscillator IMCEnding_OscillatorList[] = {
	{ 8, 0, IMCSONGOSCTYPE_SAW, 0, -1, 168, 1, 1 },
	{ 10, 0, IMCSONGOSCTYPE_SAW, 0, -1, 96, 1, 1 },
	{ 7, 127, IMCSONGOSCTYPE_SAW, 0, -1, 158, 2, 1 },
	{ 6, 0, IMCSONGOSCTYPE_SINE, 0, 0, 16, 1, 1 },
	{ 6, 0, IMCSONGOSCTYPE_SINE, 0, 1, 50, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 0, 2, 100, 1, 1 },
	{ 5, 15, IMCSONGOSCTYPE_SINE, 1, -1, 102, 1, 5 },
	{ 8, 0, IMCSONGOSCTYPE_NOISE, 1, -1, 142, 6, 1 },
	{ 5, 227, IMCSONGOSCTYPE_SINE, 1, -1, 92, 7, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
static const TImcSongEffect IMCEnding_EffectList[] = {
	{ 70, 49, 1, 0, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
	{ 3810, 1522, 1, 0, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 3 },
	{ 50, 0, 1, 1, IMCSONGEFFECTTYPE_LOWPASS, 1, 0 },
	{ 3556, 2358, 1, 1, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
};
static unsigned char IMCEnding_ChannelVol[8] = { 115, 133, 100, 100, 100, 100, 100, 100 };
static const unsigned char IMCEnding_ChannelEnvCounter[8] = { 0, 4, 0, 0, 0, 0, 0, 0 };
static const bool IMCEnding_ChannelStopNote[8] = { false, true, false, false, false, false, false, false };
static TImcSongData imcDataIMCEnding = {
	/*LEN*/ 0x2, /*ROWLENSAMPLES*/ 6615, /*ENVLISTSIZE*/ 7, /*ENVCOUNTERLISTSIZE*/ 8, /*OSCLISTSIZE*/ 15, /*EFFECTLISTSIZE*/ 4, /*VOL*/ 20,
	IMCEnding_OrderTable, IMCEnding_PatternData, IMCEnding_PatternLookupTable, IMCEnding_EnvList, IMCEnding_EnvCounterList, IMCEnding_OscillatorList, IMCEnding_EffectList,
	IMCEnding_ChannelVol, IMCEnding_ChannelEnvCounter, IMCEnding_ChannelStopNote };
static ZL_SynthImcTrack imcEnding(&imcDataIMCEnding, false);

//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCSfxMove_OrderTable[] = {
	0x000000001,
};
static const unsigned char IMCSfxMove_PatternData[] = {
	0x40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const unsigned char IMCSfxMove_PatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
static const TImcSongEnvelope IMCSfxMove_EnvList[] = {
	{ 0, 256, 65, 8, 16, 4, true, 255, },
	{ 50, 150, 418, 8, 255, 255, true, 255, },
	{ 100, 200, 21, 8, 255, 255, false, 255, },
};
static TImcSongEnvelopeCounter IMCSfxMove_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 150 }, { 2, 0, 200 },
};
static const TImcSongOscillator IMCSfxMove_OscillatorList[] = {
	{ 7, 66, IMCSONGOSCTYPE_SAW, 0, -1, 255, 1, 2 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
static const TImcSongEffect IMCSfxMove_EffectList[] = {
	{ 41, 95, 1, 0, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
	{ 0, 0, 201, 0, IMCSONGEFFECTTYPE_FLANGE, 3, 0 },
};
static unsigned char IMCSfxMove_ChannelVol[8] = { 66, 100, 100, 100, 100, 100, 100, 100 };
static const unsigned char IMCSfxMove_ChannelEnvCounter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const bool IMCSfxMove_ChannelStopNote[8] = { false, false, false, false, false, false, false, false };
TImcSongData imcDataIMCSfxMove = {
	/*LEN*/ 0x1, /*ROWLENSAMPLES*/ 5512, /*ENVLISTSIZE*/ 3, /*ENVCOUNTERLISTSIZE*/ 4, /*OSCLISTSIZE*/ 8, /*EFFECTLISTSIZE*/ 2, /*VOL*/ 150,
	IMCSfxMove_OrderTable, IMCSfxMove_PatternData, IMCSfxMove_PatternLookupTable, IMCSfxMove_EnvList, IMCSfxMove_EnvCounterList, IMCSfxMove_OscillatorList, IMCSfxMove_EffectList,
	IMCSfxMove_ChannelVol, IMCSfxMove_ChannelEnvCounter, IMCSfxMove_ChannelStopNote };
static ZL_SynthImcTrack imcSfxMove(&imcDataIMCSfxMove);

//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCSfxPrism_OrderTable[] = {
	0x000000001,
};
static const unsigned char IMCSfxPrism_PatternData[] = {
	0x40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const unsigned char IMCSfxPrism_PatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
static const TImcSongEnvelope IMCSfxPrism_EnvList[] = {
	{ 0, 256, 65, 8, 16, 4, true, 255, },
	{ 100, 200, 697, 8, 255, 255, false, 255, },
};
static TImcSongEnvelopeCounter IMCSfxPrism_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 200 },
};
static const TImcSongOscillator IMCSfxPrism_OscillatorList[] = {
	{ 8, 0, IMCSONGOSCTYPE_SINE, 0, -1, 100, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
static const TImcSongEffect IMCSfxPrism_EffectList[] = {
	{ 0, 0, 201, 0, IMCSONGEFFECTTYPE_FLANGE, 2, 0 },
};
static unsigned char IMCSfxPrism_ChannelVol[8] = { 100, 100, 100, 100, 100, 100, 100, 100 };
static const unsigned char IMCSfxPrism_ChannelEnvCounter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const bool IMCSfxPrism_ChannelStopNote[8] = { false, false, false, false, false, false, false, false };
static TImcSongData imcDataIMCSfxPrism = {
	/*LEN*/ 0x1, /*ROWLENSAMPLES*/ 5512, /*ENVLISTSIZE*/ 2, /*ENVCOUNTERLISTSIZE*/ 3, /*OSCLISTSIZE*/ 8, /*EFFECTLISTSIZE*/ 1, /*VOL*/ 100,
	IMCSfxPrism_OrderTable, IMCSfxPrism_PatternData, IMCSfxPrism_PatternLookupTable, IMCSfxPrism_EnvList, IMCSfxPrism_EnvCounterList, IMCSfxPrism_OscillatorList, IMCSfxPrism_EffectList,
	IMCSfxPrism_ChannelVol, IMCSfxPrism_ChannelEnvCounter, IMCSfxPrism_ChannelStopNote };
static ZL_SynthImcTrack imcSfxPrism(&imcDataIMCSfxPrism);

//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCSfxActivate_OrderTable[] = {
	0x000000001,
};
static const unsigned char IMCSfxActivate_PatternData[] = {
	0x50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const unsigned char IMCSfxActivate_PatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
static const TImcSongEnvelope IMCSfxActivate_EnvList[] = {
	{ 0, 256, 29, 8, 16, 255, true, 255, },
	{ 0, 256, 21, 28, 255, 255, true, 255, },
};
static TImcSongEnvelopeCounter IMCSfxActivate_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { -1, -1, 50 }, { 1, 0, 32 },
};
static const TImcSongOscillator IMCSfxActivate_OscillatorList[] = {
	{ 5, 15, IMCSONGOSCTYPE_SINE, 0, -1, 162, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 0, 0, 172, 2, 3 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
static const TImcSongEffect IMCSfxActivate_EffectList[] = {
	{ 6731, 1070, 1, 0, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
};
static unsigned char IMCSfxActivate_ChannelVol[8] = { 104, 100, 100, 100, 100, 100, 100, 100 };
static const unsigned char IMCSfxActivate_ChannelEnvCounter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const bool IMCSfxActivate_ChannelStopNote[8] = { true, false, false, false, false, false, false, false };
static TImcSongData imcDataIMCSfxActivate = {
	/*LEN*/ 0x1, /*ROWLENSAMPLES*/ 16537, /*ENVLISTSIZE*/ 2, /*ENVCOUNTERLISTSIZE*/ 4, /*OSCLISTSIZE*/ 9, /*EFFECTLISTSIZE*/ 1, /*VOL*/ 80,
	IMCSfxActivate_OrderTable, IMCSfxActivate_PatternData, IMCSfxActivate_PatternLookupTable, IMCSfxActivate_EnvList, IMCSfxActivate_EnvCounterList, IMCSfxActivate_OscillatorList, IMCSfxActivate_EffectList,
	IMCSfxActivate_ChannelVol, IMCSfxActivate_ChannelEnvCounter, IMCSfxActivate_ChannelStopNote };
static ZL_SynthImcTrack imcSfxActivate(&imcDataIMCSfxActivate, false);

//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCSfxDoor_OrderTable[] = {
	0x000000111,
};
static const unsigned char IMCSfxDoor_PatternData[] = {
	0, 0, 0x50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0x50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0x40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const unsigned char IMCSfxDoor_PatternLookupTable[] = { 0, 1, 2, 3, 3, 3, 3, 3, };
static const TImcSongEnvelope IMCSfxDoor_EnvList[] = {
	{ 0, 256, 45, 8, 16, 255, true, 255, },
	{ 0, 256, 33, 30, 10, 255, true, 255, },
	{ 100, 250, 871, 8, 255, 255, true, 255, },
	{ 0, 256, 33, 8, 16, 255, true, 255, },
	{ 0, 256, 697, 24, 255, 255, true, 255, },
	{ 0, 256, 17, 0, 8, 255, true, 255, },
	{ 0, 256, 204, 8, 16, 4, true, 255, },
	{ 0, 256, 209, 8, 16, 255, true, 255, },
	{ 64, 256, 261, 8, 15, 255, true, 255, },
	{ 0, 256, 3226, 8, 16, 255, true, 255, },
};
static TImcSongEnvelopeCounter IMCSfxDoor_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 72 }, { 2, 0, 250 },
	{ 3, 1, 256 }, { 4, 1, 0 }, { 5, 1, 128 }, { 6, 2, 256 },
	{ 7, 2, 256 }, { 8, 2, 256 }, { 9, 2, 256 },
};
static const TImcSongOscillator IMCSfxDoor_OscillatorList[] = {
	{ 6, 106, IMCSONGOSCTYPE_SAW, 0, -1, 174, 1, 2 },
	{ 8, 227, IMCSONGOSCTYPE_SAW, 1, -1, 255, 5, 6 },
	{ 6, 127, IMCSONGOSCTYPE_SINE, 2, -1, 206, 8, 9 },
	{ 8, 0, IMCSONGOSCTYPE_NOISE, 2, -1, 186, 10, 1 },
	{ 7, 0, IMCSONGOSCTYPE_NOISE, 2, 2, 152, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
static const TImcSongEffect IMCSfxDoor_EffectList[] = {
	{ 2413, 2064, 1, 0, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 3 },
	{ 47, 255, 1, 1, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
	{ 9906, 843, 1, 2, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
	{ 142, 58, 1, 2, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
};
static unsigned char IMCSfxDoor_ChannelVol[8] = { 99, 97, 71, 100, 100, 100, 100, 100 };
static const unsigned char IMCSfxDoor_ChannelEnvCounter[8] = { 0, 4, 7, 0, 0, 0, 0, 0 };
static const bool IMCSfxDoor_ChannelStopNote[8] = { true, true, false, false, false, false, false, false };
static TImcSongData imcDataIMCSfxDoor = {
	/*LEN*/ 0x1, /*ROWLENSAMPLES*/ 3307, /*ENVLISTSIZE*/ 10, /*ENVCOUNTERLISTSIZE*/ 11, /*OSCLISTSIZE*/ 10, /*EFFECTLISTSIZE*/ 4, /*VOL*/ 70,
	IMCSfxDoor_OrderTable, IMCSfxDoor_PatternData, IMCSfxDoor_PatternLookupTable, IMCSfxDoor_EnvList, IMCSfxDoor_EnvCounterList, IMCSfxDoor_OscillatorList, IMCSfxDoor_EffectList,
	IMCSfxDoor_ChannelVol, IMCSfxDoor_ChannelEnvCounter, IMCSfxDoor_ChannelStopNote };
static ZL_SynthImcTrack imcSfxDoor(&imcDataIMCSfxDoor, false);

//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCSfxUnlock_OrderTable[] = {
	0x000000111,
};
static const unsigned char IMCSfxUnlock_PatternData[] = {
	0, 0, 0, 0x70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0x70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0x75, 0, 0x62, 0x65, 0x69, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const unsigned char IMCSfxUnlock_PatternLookupTable[] = { 0, 1, 2, 3, 3, 3, 3, 3, };
static const TImcSongEnvelope IMCSfxUnlock_EnvList[] = {
	{ 0, 256, 45, 8, 16, 255, true, 255, },
	{ 0, 256, 33, 30, 10, 255, true, 255, },
	{ 100, 250, 17439, 8, 255, 255, true, 255, },
	{ 0, 256, 33, 8, 16, 255, true, 255, },
	{ 0, 256, 1743, 24, 255, 255, true, 255, },
	{ 0, 256, 17, 0, 8, 255, true, 255, },
	{ 0, 256, 204, 8, 16, 4, true, 255, },
	{ 0, 256, 209, 8, 16, 255, true, 255, },
	{ 64, 256, 261, 8, 15, 255, true, 255, },
	{ 0, 256, 3226, 8, 16, 255, true, 255, },
};
static TImcSongEnvelopeCounter IMCSfxUnlock_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 72 }, { 2, 0, 250 },
	{ 3, 1, 256 }, { 4, 1, 0 }, { 5, 1, 128 }, { 6, 2, 256 },
	{ 7, 2, 256 }, { 8, 2, 256 }, { 9, 2, 256 },
};
static const TImcSongOscillator IMCSfxUnlock_OscillatorList[] = {
	{ 6, 106, IMCSONGOSCTYPE_SAW, 0, -1, 174, 1, 2 },
	{ 8, 227, IMCSONGOSCTYPE_SAW, 1, -1, 255, 5, 6 },
	{ 6, 127, IMCSONGOSCTYPE_SINE, 2, -1, 206, 8, 9 },
	{ 8, 0, IMCSONGOSCTYPE_NOISE, 2, -1, 186, 10, 1 },
	{ 7, 0, IMCSONGOSCTYPE_NOISE, 2, 2, 152, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
static const TImcSongEffect IMCSfxUnlock_EffectList[] = {
	{ 10287, 270, 1, 0, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 3 },
	{ 47, 255, 1, 1, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
	{ 9906, 843, 1, 2, IMCSONGEFFECTTYPE_OVERDRIVE, 0, 1 },
	{ 142, 58, 1, 2, IMCSONGEFFECTTYPE_RESONANCE, 1, 1 },
};
static unsigned char IMCSfxUnlock_ChannelVol[8] = { 99, 163, 115, 100, 100, 100, 100, 100 };
static const unsigned char IMCSfxUnlock_ChannelEnvCounter[8] = { 0, 4, 7, 0, 0, 0, 0, 0 };
static const bool IMCSfxUnlock_ChannelStopNote[8] = { true, true, false, false, false, false, false, false };
TImcSongData imcDataIMCSfxUnlock = {
	/*LEN*/ 0x1, /*ROWLENSAMPLES*/ 3307, /*ENVLISTSIZE*/ 10, /*ENVCOUNTERLISTSIZE*/ 11, /*OSCLISTSIZE*/ 10, /*EFFECTLISTSIZE*/ 4, /*VOL*/ 80,
	IMCSfxUnlock_OrderTable, IMCSfxUnlock_PatternData, IMCSfxUnlock_PatternLookupTable, IMCSfxUnlock_EnvList, IMCSfxUnlock_EnvCounterList, IMCSfxUnlock_OscillatorList, IMCSfxUnlock_EffectList,
	IMCSfxUnlock_ChannelVol, IMCSfxUnlock_ChannelEnvCounter, IMCSfxUnlock_ChannelStopNote };
static ZL_SynthImcTrack imcSfxUnlock(&imcDataIMCSfxUnlock, false);
//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCSfxOperate_OrderTable[] = {
	0x000000001,
};
static const unsigned char IMCSfxOperate_PatternData[] = {
	0x44, 0x42, 0x54, 0x52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const unsigned char IMCSfxOperate_PatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
static const TImcSongEnvelope IMCSfxOperate_EnvList[] = {
	{ 0, 256, 277, 8, 16, 255, true, 255, },
	{ 0, 256, 21, 8, 16, 255, true, 255, },
};
static TImcSongEnvelopeCounter IMCSfxOperate_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 256 },
};
static const TImcSongOscillator IMCSfxOperate_OscillatorList[] = {
	{ 6, 0, IMCSONGOSCTYPE_SINE, 0, -1, 128, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 0, -1, 146, 1, 1 },
	{ 9, 0, IMCSONGOSCTYPE_SINE, 0, -1, 112, 1, 2 },
};
static unsigned char IMCSfxOperate_ChannelVol[8] = { 76, 84, 100, 100, 100, 100, 100, 100 };
static const unsigned char IMCSfxOperate_ChannelEnvCounter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const bool IMCSfxOperate_ChannelStopNote[8] = { true, true, false, false, false, false, false, false };
static TImcSongData imcDataIMCSfxOperate = {
	/*LEN*/ 0x1, /*ROWLENSAMPLES*/ 2594, /*ENVLISTSIZE*/ 2, /*ENVCOUNTERLISTSIZE*/ 3, /*OSCLISTSIZE*/ 3, /*EFFECTLISTSIZE*/ 0, /*VOL*/ 130,
	IMCSfxOperate_OrderTable, IMCSfxOperate_PatternData, IMCSfxOperate_PatternLookupTable, IMCSfxOperate_EnvList, IMCSfxOperate_EnvCounterList, IMCSfxOperate_OscillatorList, NULL,
	IMCSfxOperate_ChannelVol, IMCSfxOperate_ChannelEnvCounter, IMCSfxOperate_ChannelStopNote };
static ZL_SynthImcTrack imcSfxOperateOn(&imcDataIMCSfxOperate, false);

//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCSfxOperateOff_OrderTable[] = {
	0x000000001,
};
static const unsigned char IMCSfxOperateOff_PatternData[] = {
	0x52, 0x54, 0x42, 0x44, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const unsigned char IMCSfxOperateOff_PatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
static const TImcSongEnvelope IMCSfxOperateOff_EnvList[] = {
	{ 0, 256, 277, 8, 16, 255, true, 255, },
	{ 0, 256, 21, 8, 16, 255, true, 255, },
};
static TImcSongEnvelopeCounter IMCSfxOperateOff_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 256 },
};
static const TImcSongOscillator IMCSfxOperateOff_OscillatorList[] = {
	{ 6, 0, IMCSONGOSCTYPE_SINE, 0, -1, 128, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 0, -1, 146, 1, 1 },
	{ 9, 0, IMCSONGOSCTYPE_SINE, 0, -1, 112, 1, 2 },
};
static unsigned char IMCSfxOperateOff_ChannelVol[8] = { 76, 84, 100, 100, 100, 100, 100, 100 };
static const unsigned char IMCSfxOperateOff_ChannelEnvCounter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const bool IMCSfxOperateOff_ChannelStopNote[8] = { true, true, false, false, false, false, false, false };
static TImcSongData imcDataIMCSfxOperateOff = {
	/*LEN*/ 0x1, /*ROWLENSAMPLES*/ 2594, /*ENVLISTSIZE*/ 2, /*ENVCOUNTERLISTSIZE*/ 3, /*OSCLISTSIZE*/ 3, /*EFFECTLISTSIZE*/ 0, /*VOL*/ 130,
	IMCSfxOperateOff_OrderTable, IMCSfxOperateOff_PatternData, IMCSfxOperateOff_PatternLookupTable, IMCSfxOperateOff_EnvList, IMCSfxOperateOff_EnvCounterList, IMCSfxOperateOff_OscillatorList, NULL,
	IMCSfxOperateOff_ChannelVol, IMCSfxOperateOff_ChannelEnvCounter, IMCSfxOperateOff_ChannelStopNote };
static ZL_SynthImcTrack imcSfxOperateOff(&imcDataIMCSfxOperateOff, false);

//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCSfxLazer_OrderTable[] = {
	0x000000001,
};
static const unsigned char IMCSfxLazer_PatternData[] = {
	0x50, 0x52, 0x54, 0x55, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const unsigned char IMCSfxLazer_PatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
static const TImcSongEnvelope IMCSfxLazer_EnvList[] = {
	{ 0, 256, 92, 8, 16, 255, true, 255, },
	{ 0, 256, 21, 8, 16, 255, true, 255, },
	{ 0, 256, 635, 8, 255, 255, true, 255, },
};
static TImcSongEnvelopeCounter IMCSfxLazer_EnvCounterList[] = {
	{ 0, 0, 256 }, { -1, -1, 256 }, { 1, 0, 256 }, { 2, 0, 256 },
};
static const TImcSongOscillator IMCSfxLazer_OscillatorList[] = {
	{ 6, 0, IMCSONGOSCTYPE_SINE, 0, -1, 0, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 0, -1, 0, 1, 1 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 0, -1, 108, 1, 2 },
	{ 8, 200, IMCSONGOSCTYPE_SQUARE, 0, 2, 92, 1, 1 },
};
static const TImcSongEffect IMCSfxLazer_EffectList[] = {
	{ 27, 255, 1, 0, IMCSONGEFFECTTYPE_RESONANCE, 3, 1 },
};
static unsigned char IMCSfxLazer_ChannelVol[8] = { 76, 84, 100, 100, 100, 100, 100, 100 };
static const unsigned char IMCSfxLazer_ChannelEnvCounter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const bool IMCSfxLazer_ChannelStopNote[8] = { true, true, false, false, false, false, false, false };
static TImcSongData imcDataIMCSfxLazer = {
	/*LEN*/ 0x1, /*ROWLENSAMPLES*/ 2594, /*ENVLISTSIZE*/ 3, /*ENVCOUNTERLISTSIZE*/ 4, /*OSCLISTSIZE*/ 4, /*EFFECTLISTSIZE*/ 1, /*VOL*/ 63,
	IMCSfxLazer_OrderTable, IMCSfxLazer_PatternData, IMCSfxLazer_PatternLookupTable, IMCSfxLazer_EnvList, IMCSfxLazer_EnvCounterList, IMCSfxLazer_OscillatorList, IMCSfxLazer_EffectList,
	IMCSfxLazer_ChannelVol, IMCSfxLazer_ChannelEnvCounter, IMCSfxLazer_ChannelStopNote };
static ZL_SynthImcTrack imcSfxLazer(&imcDataIMCSfxLazer, false);

//-----------------------------------------------------------------------------------------------------------------------

static const unsigned int IMCSfxKey_OrderTable[] = {
	0x000000001,
};
static const unsigned char IMCSfxKey_PatternData[] = {
	0x62, 0x64, 0x65, 0, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const unsigned char IMCSfxKey_PatternLookupTable[] = { 0, 1, 1, 1, 1, 1, 1, 1, };
static const TImcSongEnvelope IMCSfxKey_EnvList[] = {
	{ 0, 256, 65, 8, 16, 4, true, 255, },
	{ 0, 256, 370, 8, 12, 255, true, 255, },
};
static TImcSongEnvelopeCounter IMCSfxKey_EnvCounterList[] = {
	{ 0, 0, 256 }, { 1, 0, 256 }, { -1, -1, 256 },
};
static const TImcSongOscillator IMCSfxKey_OscillatorList[] = {
	{ 9, 66, IMCSONGOSCTYPE_SQUARE, 0, -1, 126, 1, 2 },
	{ 7, 66, IMCSONGOSCTYPE_SAW, 0, 0, 36, 2, 2 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 1, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 2, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 3, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 4, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 5, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 6, -1, 100, 0, 0 },
	{ 8, 0, IMCSONGOSCTYPE_SINE, 7, -1, 100, 0, 0 },
};
static unsigned char IMCSfxKey_ChannelVol[8] = { 51, 100, 100, 100, 100, 100, 100, 100 };
static const unsigned char IMCSfxKey_ChannelEnvCounter[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static const bool IMCSfxKey_ChannelStopNote[8] = { false, false, false, false, false, false, false, false };
static TImcSongData imcDataIMCSfxKey = {
	/*LEN*/ 0x1, /*ROWLENSAMPLES*/ 2594, /*ENVLISTSIZE*/ 2, /*ENVCOUNTERLISTSIZE*/ 3, /*OSCLISTSIZE*/ 9, /*EFFECTLISTSIZE*/ 0, /*VOL*/ 100,
	IMCSfxKey_OrderTable, IMCSfxKey_PatternData, IMCSfxKey_PatternLookupTable, IMCSfxKey_EnvList, IMCSfxKey_EnvCounterList, IMCSfxKey_OscillatorList, NULL,
	IMCSfxKey_ChannelVol, IMCSfxKey_ChannelEnvCounter, IMCSfxKey_ChannelStopNote };
static ZL_SynthImcTrack imcSfxKey(&imcDataIMCSfxKey, false);

//-----------------------------------------------------------------------------------------------------------------------

