/*
  Lazer Maze
  Copyright (C) 2018-2019 Bernhard Schelling

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

#if defined(ZILLALOG) //MAP EDITING MAP EDITING MAP EDITING MAP EDITING MAP EDITING MAP EDITING
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <ZL_Application.h>
#include <ZL_Display.h>
#include <ZL_Surface.h>
#include <ZL_Signal.h>
#include <ZL_Audio.h>
#include <ZL_Font.h>
#include <ZL_Scene.h>
#include <ZL_Input.h>
#include <ZL_SynthImc.h>
#include <vector>

#ifdef __SMARTPHONE__
bool showTouchUI = true;
#else
bool showTouchUI = false;
#endif

static ZL_Font fntBig;
static ZL_Surface srfTiles, srfItems, srfFloor;
#include "sound.inl"

static enum { PHASE_FADEIN, PHASE_PLAY, PHASE_ASKQUIT, PHASE_FINISH } Phase;
static ticks_t PhaseTicks;

enum { MAP_W = 80, MAP_H = 45 };

#include "map.inl"

static ZL_Surface srfMaps[5];
static const ZL_Rectf recMaps[5] = {
	ZL_Rectf(            0,             0,   MAP_W,   MAP_H),
	ZL_Rectf(            0,             0, MAP_W/5, MAP_H/5),
	ZL_Rectf(MAP_W-MAP_W/5,             0,   MAP_W, MAP_H/5),
	ZL_Rectf(            0, MAP_H-MAP_H/5, MAP_W/5,   MAP_H),
	ZL_Rectf(MAP_W-MAP_W/5, MAP_H-MAP_H/5,   MAP_W,   MAP_H),
};
static const ZL_Rectf recMapClamp = recMaps[0] - KINDA_SMALL_NUMBER;
static const ZL_Vector PlayerSize = ZLV(.3f, .4f), PlayerStartPos = ZLV(MAP_W*.5, MAP_H*.48);

static const ZL_Color colFloor[5] = {
	ZLHSV(.000, .0, .4), //DARK WHITE
	ZLHSV(.000, .9, .4), //DARK RED
	ZLHSV(.833, .9, .4), //DARK GREEN
	ZLHSV(.333, .9, .4), //DARK ORANGE
	ZLHSV(.116, .9, .4), //DARK MAGENTA
};
static const ZL_Color colWalls[5][4] = {
	{ ZLHSV(.000, .1, 0.8), ZLHSV(.333, .1, 0.8), ZLHSV(.666, .1, 0.8), ZLHSV(.833, .1, 0.8) }, //MULTI-COLOR
	{ ZLHSV(.000, .9, 1.0), ZLHSV(.025, .9, 1.0), ZLHSV(.050, .9, 1.0), ZLHSV(.970, .9, 1.0) }, //RED-ISH
	{ ZLHSV(.833, .9, 1.0), ZLHSV(.858, .9, 1.0), ZLHSV(.883, .9, 1.0), ZLHSV(.803, .9, 1.0) }, //GREEN-ISH
	{ ZLHSV(.333, .9, 1.0), ZLHSV(.358, .9, 1.0), ZLHSV(.383, .9, 1.0), ZLHSV(.303, .9, 1.0) }, //ORANGE-ISH
	{ ZLHSV(.116, .9, 1.0), ZLHSV(.126, .9, 1.0), ZLHSV(.136, .9, 1.0), ZLHSV(.146, .9, 1.0) }, //MAGENTA-ISH
};
static const ZL_Color colKeyLocks[4] = {
	ZLRGB(1.0,0.0,0.0), //RED
	ZLRGB(0.0,1.0,0.0), //GREEN
	ZLRGB(1.0,0.7,0.0), //ORANGE
	ZLRGB(1.0,0.0,1.0), //MAGENTA
};

enum
{
	COL_NONE    = 0x0,
	COL_RED     = 0x1,
	COL_GREEN   = 0x2,
	COL_YELLOW  = 0x3,
	COL_BLUE    = 0x4,
	COL_MAGENTA = 0x5,
	COL_CYAN    = 0x6,
	COL_WHITE   = 0x7,
};

static const ZL_Color colLazer[] = {
	 ZLRGBA(1,1,1,.3),     //NONE
	 ZLRGB(1.0, 0.0, 0.0), //COL_RED
	 ZLRGB(0.0, 0.9, 0.0), //COL_GREEN
	 ZLRGB(1.0, 0.8, 0.0), //COL_YELLOW
	 ZLRGB(0.2, 0.3, 1.0), //COL_BLUE
	 ZLRGB(0.9, 0.1, 1.0), //COL_MAGENTA
	 ZLRGB(0.0, 1.0, 0.9), //COL_CYAN
	 ZLRGB(0.9, 0.8, 0.7), //COL_WHITE
};

static ZL_Vector MapPoints[5];
static ZL_Vector MapPointDirections[5];
static std::vector<struct sThing> MapThings[5];
static ZL_Rectf recOrtho(PlayerStartPos, recMaps[0].Extents() * .015f);

#define MAPTILE(map, i) MapMain[map][i]
#define MAPPOSXLEFTFROMTILE(map, i)  (recMaps[map].left + (recMaps[map].right - recMaps[map].left) * (s(i % MAP_W)    ) / s(MAP_W))
#define MAPPOSXRIGHTFROMTILE(map, i) (recMaps[map].left + (recMaps[map].right - recMaps[map].left) * (s(i % MAP_W) + 1) / s(MAP_W))
#define MAPPOSYLOWFROMTILE(map, i)   (recMaps[map].low  + (recMaps[map].high  - recMaps[map].low ) * (s(i / MAP_W)    ) / s(MAP_H))
#define MAPPOSYHIGHFROMTILE(map, i)  (recMaps[map].low  + (recMaps[map].high  - recMaps[map].low ) * (s(i / MAP_W) + 1) / s(MAP_H))
#define MAPPOSFROMTILE(map, i) ZLV(MAPPOSXLEFTFROMTILE(map, i), MAPPOSYLOWFROMTILE(map, i))
#define MAPTILEFROMSUBPOS(map, x, y) ((int)(y) * MAP_W + (int)(x))

enum
{
	TILE_EMPTY           = ' ',
	TILE_PRISM           = 'P',
	TILE_DOOR            = 'D',
	TILE_ORIENTATION     = 'O',
	TILE_WALL            = '#',
	TILE_KEY             = 'K',
	TILE_LOCK            = 'L',
	TILE_TRAP            = 'T',
	TILE_KEYLOCK_1       = '+',
	TILE_KEYLOCK_2       = '-',
	TILE_KEYLOCK_3       = '*',
	TILE_KEYLOCK_4       = '/',
	TILE_FINISH          = 'F',
	TILE_COL_RED         = 'r',
	TILE_COL_GREEN       = 'g',
	TILE_COL_BLUE        = 'b',
	TILE_COL_YELLOW      = 'y', 
	TILE_COL_MAGENTA     = 'm', 
	TILE_COL_CYAN        = 'c', 
	TILE_COL_WHITE       = 'w',
	_TILE_EMITTER_MIN    = 'B',
	_TILE_EMITTER_MAX    = 'Y',
	_TILE_ACTIVATOR_MIN  = 'b',
	_TILE_ACTIVATOR_MAX  = 'y',
	TILE_PASSAGESUBMAP_1 = '1',
	TILE_PASSAGESUBMAP_2 = '2',
	TILE_PASSAGESUBMAP_3 = '3',
	TILE_PASSAGESUBMAP_4 = '4',
	_TILE_PASSAGE_MIN    = '1',
	_TILE_PASSAGE_MAX    = '4',
	_TILE_MAX_LAZERPASS  = '9', //contains EMPTY, KEYLOCKs, PASSAGESUBMAPs
};

struct sThing
{
	int TileIndex;
	enum EType { THING_EMITTER, THING_PRISM, THING_ACTIVATOR, THING_DOOR, THING_KEY, THING_LOCK, THING_TRAP, THING_FINISH, _THING_NONE } Type;
	ZL_Vector Pos, Dir, Trg;
	int Col, InCol;
	float Active;

	static void Setup(char* MapTile)
	{
		Remove(MapTile);
		EType Type = 
			  *MapTile == TILE_PRISM       ? THING_PRISM
			: *MapTile == TILE_DOOR        ? THING_DOOR
			: *MapTile == TILE_KEY         ? THING_KEY
			: *MapTile == TILE_LOCK        ? THING_LOCK
			: *MapTile == TILE_TRAP        ? THING_TRAP
			: *MapTile == TILE_EMPTY       ? _THING_NONE
			: *MapTile == TILE_ORIENTATION ? _THING_NONE
			: *MapTile == TILE_KEYLOCK_1   ? _THING_NONE
			: *MapTile == TILE_KEYLOCK_2   ? _THING_NONE
			: *MapTile == TILE_KEYLOCK_3   ? _THING_NONE
			: *MapTile == TILE_KEYLOCK_4   ? _THING_NONE
			: *MapTile == TILE_FINISH      ? THING_FINISH
			: (*MapTile >= _TILE_PASSAGE_MIN   && *MapTile <= _TILE_PASSAGE_MAX  ) ? _THING_NONE
			: (*MapTile >= _TILE_ACTIVATOR_MIN && *MapTile <= _TILE_ACTIVATOR_MAX) ? THING_ACTIVATOR
			: (*MapTile >= _TILE_EMITTER_MIN   && *MapTile <= _TILE_EMITTER_MAX  ) ? THING_EMITTER
			: (ZL_VERIFY(false) ? _THING_NONE : _THING_NONE);
		if (Type == _THING_NONE) return;
		int Map = (int)((MapTile - *MapMain) / sizeof(*MapMain)), TileIndex = (int)(MapTile - MapMain[Map]);
		char tW = MapTile[-1], tE = MapTile[1], tS = MapTile[-MAP_W], tN = MapTile[MAP_W], ct = *MapTile|0x20;
		auto it = std::lower_bound(MapThings[Map].begin(), MapThings[Map].end(), TileIndex, ByTileIndex);
		sThing* res = &*(it != MapThings[Map].end() && it->TileIndex == TileIndex ? it : MapThings[Map].insert(it, sThing()));
		res->TileIndex = TileIndex;
		res->Type = Type;
		res->Pos = ZLV(TileIndex % MAP_W, TileIndex / MAP_W); //MAPPOSFROMTILE(Map, TileIndex);
		res->Dir = ZLV((tW==TILE_ORIENTATION? -1 : (tE==TILE_ORIENTATION? 1 : 0)), (tS==TILE_ORIENTATION? -1 : (tN==TILE_ORIENTATION? 1 : 0))).SetLength(1);
		res->Active = 0;
		if (res->Type == THING_KEY || res->Type == THING_LOCK || res->Type == THING_TRAP)
		{
			if      (tW==TILE_KEYLOCK_1 || tE==TILE_KEYLOCK_1 || tS==TILE_KEYLOCK_1 || tN==TILE_KEYLOCK_1) res->Col = 0;
			else if (tW==TILE_KEYLOCK_2 || tE==TILE_KEYLOCK_2 || tS==TILE_KEYLOCK_2 || tN==TILE_KEYLOCK_2) res->Col = 1;
			else if (tW==TILE_KEYLOCK_3 || tE==TILE_KEYLOCK_3 || tS==TILE_KEYLOCK_3 || tN==TILE_KEYLOCK_3) res->Col = 2;
			else if (tW==TILE_KEYLOCK_4 || tE==TILE_KEYLOCK_4 || tS==TILE_KEYLOCK_4 || tN==TILE_KEYLOCK_4) res->Col = 3;
			else { ZL_ASSERT(false); }
		}
		else if (res->Type == THING_ACTIVATOR || res->Type == THING_EMITTER)
		{
			res->Col = 
				  ct == TILE_COL_RED     ? COL_RED
				: ct == TILE_COL_GREEN   ? COL_GREEN
				: ct == TILE_COL_BLUE    ? COL_BLUE
				: ct == TILE_COL_YELLOW  ? COL_YELLOW
				: ct == TILE_COL_MAGENTA ? COL_MAGENTA
				: ct == TILE_COL_CYAN    ? COL_CYAN
				: ct == TILE_COL_WHITE   ? COL_WHITE
				: (ZL_VERIFY(false) ? COL_WHITE : COL_WHITE);
		}
		else if (res->Type == THING_DOOR || res->Type == THING_FINISH || res->Type == THING_PRISM) { }
		else { ZL_ASSERT(false); }
	}

	static void Remove(const char* MapTile)
	{
		int Map = (int)((MapTile - *MapMain) / sizeof(*MapMain)), TileIndex = (int)(MapTile - MapMain[Map]);
		auto it = std::lower_bound(MapThings[Map].begin(), MapThings[Map].end(), TileIndex, ByTileIndex);
		if (it != MapThings[Map].end() && it->TileIndex == TileIndex) MapThings[Map].erase(it);
	}

	static sThing* Get(const char* MapTile)
	{
		int Map = (int)((MapTile - *MapMain) / sizeof(*MapMain)), TileIndex = (int)(MapTile - MapMain[Map]);
		auto it = std::lower_bound(MapThings[Map].begin(), MapThings[Map].end(), TileIndex, ByTileIndex);
		return (it != MapThings[Map].end() && it->TileIndex == TileIndex ? &*it : NULL);
	}

	static sThing* Get(int Map, int TileIndex)
	{
		auto it = std::lower_bound(MapThings[Map].begin(), MapThings[Map].end(), TileIndex, ByTileIndex);
		return (it != MapThings[Map].end() && it->TileIndex == TileIndex ? &*it : NULL);
	}

	static inline bool ByTileIndex(sThing& a, int TileIndex) { return a.TileIndex < TileIndex; }

	void DoLazer(int map, int recurse = 0, const int* LazerCol = NULL)
	{
		ZL_Vector PosOff = ZLV(Pos.x+.5f, Pos.y+.5f);
		Active = ZL_Math::Clamp(Active + ZLELAPSEDF(50), 1.f, MAP_W+MAP_H);
		for (float f = .71f; f < Active; f += .5f)
		{
			ZL_Vector TestPos = PosOff + Dir * f;
			const char *ptile = &MapMain[map][(int)TestPos.y * MAP_W + (int)TestPos.x], ttile = *ptile;
			ZL_ASSERT(ptile >= MapMain[map] && ptile < MapMain[map+1]);
			Trg = PosOff + Dir * f;
			if ((ttile <= _TILE_MAX_LAZERPASS && ttile != TILE_WALL) || ttile == TILE_ORIENTATION) continue;
			else if (ttile == TILE_DOOR || ttile == TILE_LOCK)
			{
				if (sThing::Get(ptile)->Active) continue;
			}
			else if (ttile == TILE_TRAP)
			{
				if (!sThing::Get(ptile)->Active) continue;
			}
			if (ttile == TILE_PRISM && recurse < 100)
			{
				sThing* Prism = sThing::Get(ptile);
				ZL_ASSERT(Prism && Prism != this);
				if (Prism)
				{
					Trg = ZL_Vector::Lerp(Trg, ZLV(Prism->Pos.x+.5f, Prism->Pos.y+.5f), .5f);
					if (!LazerCol) LazerCol = &Col;
					Prism->InCol |= *LazerCol;
					if (!(Prism->Col & *LazerCol)) Prism->Active = 0;
					Prism->DoLazer(map, recurse + 1, LazerCol);
				}
			}
			if (ttile >= 'a' && ttile <= 'z')
			{
				sThing* Activator = sThing::Get(ptile);
				ZL_ASSERT(Activator);
				if (Activator)
				{
					if (!LazerCol) LazerCol = &Col;
					Activator->InCol |= *LazerCol;
				}
			}
			/*if (f > Active)*/ Active = f;
			break;
		}
	}
};

struct sPlayer
{
	ZL_Vector Vel, Pos, SubPos;
	int Map, KeyRing;
	float Scale, SubMapDepth;
	float PrismRotSpeed;
	char* TileTouchingPrism;
	sThing* OperatingPrism;

	void UpdatePos(bool CheckCollision)
	{
		Pos = recMapClamp.Clamp(Pos);
		for (int i = 4; i >= 0; i--) if (recMaps[i].Contains(Pos)) { Map = i; break; }
		SubPos = ZL_Rectf::Map(Pos, recMaps[Map], recMaps[0]);
		SubMapDepth = (Map ? ZL_Math::ClosestPointOnLine(Pos, MapPoints[Map], MapPoints[Map] + ZLV(1, 0)).GetDistance(Pos) : 0);
		Scale = ZL_Math::Lerp(1.f, .2f, ZL_Math::Clamp(SubMapDepth, 0, 1));
		if (!CheckCollision) return;

		TileTouchingPrism = NULL;
		for (int i = 0; i != 4; i++)
		{
			int AroundTileIndex;
			if      (i == 0) AroundTileIndex = MAPTILEFROMSUBPOS(Map, ZL_Math::Clamp(SubPos.x - PlayerSize.x, recMapClamp.left, recMapClamp.right), SubPos.y);
			else if (i == 1) AroundTileIndex = MAPTILEFROMSUBPOS(Map, ZL_Math::Clamp(SubPos.x + PlayerSize.x, recMapClamp.left, recMapClamp.right), SubPos.y);
			else if (i == 2) AroundTileIndex = MAPTILEFROMSUBPOS(Map, SubPos.x, ZL_Math::Clamp(SubPos.y - PlayerSize.y,  recMapClamp.low,  recMapClamp.high));
			else             AroundTileIndex = MAPTILEFROMSUBPOS(Map, SubPos.x, ZL_Math::Clamp(SubPos.y + PlayerSize.y,  recMapClamp.low,  recMapClamp.high));
			char *pAroundTile = &MapMain[Map][AroundTileIndex], AroundTile = *pAroundTile;
			if (AroundTile == TILE_DOOR)
			{
				sThing* Door = sThing::Get(pAroundTile);
				if (Door && Door->Active) continue;
			}
			else if (AroundTile == TILE_KEY)
			{
				sThing* Key = sThing::Get(pAroundTile);
				if (!Key->Active)
				{
					Key->Active = 1;
					KeyRing |= 1<<(Key->Col);
					for (sThing& it : MapThings[Map]) if (it.Type == sThing::THING_TRAP) it.Active = 0.01f;
					imcSfxKey.Play(true);
				}
				continue;
			}
			else if (AroundTile == TILE_LOCK)
			{
				sThing* Lock = sThing::Get(pAroundTile);
				if (!Lock->Active && (KeyRing & (1<<(Lock->Col)))) { Lock->Active = 0.01f; imcSfxUnlock.Play(true); }
				if (Lock->Active) continue;
			}
			else if (AroundTile == TILE_TRAP)
			{
				sThing* Trap = sThing::Get(pAroundTile);
				if (!Trap->Active) continue;
			}
			else if (AroundTile == TILE_FINISH)
			{
				if (Phase == PHASE_PLAY)
				{
					imcTheme.Stop();
					imcEnding.Play();
					Phase = PHASE_FINISH;
					PhaseTicks = ZLTICKS;
				}
				continue;
			}
			else if (AroundTile == TILE_PRISM)
			{
				TileTouchingPrism = pAroundTile;
				continue;
			}
			else if (AroundTile != TILE_WALL && AroundTile < _TILE_EMITTER_MAX)
			{
				continue;
			}
			ZL_Vector Fix;
			if      (i == 0) Fix.x = MAPPOSXRIGHTFROMTILE(Map, AroundTileIndex) - (Pos.x - PlayerSize.x * Scale - KINDA_SMALL_NUMBER);
			else if (i == 1) Fix.x =  MAPPOSXLEFTFROMTILE(Map, AroundTileIndex) - (Pos.x + PlayerSize.x * Scale + KINDA_SMALL_NUMBER);
			else if (i == 2) Fix.y =  MAPPOSYHIGHFROMTILE(Map, AroundTileIndex) - (Pos.y - PlayerSize.y * Scale - KINDA_SMALL_NUMBER);
			else             Fix.y =   MAPPOSYLOWFROMTILE(Map, AroundTileIndex) - (Pos.y + PlayerSize.y * Scale + KINDA_SMALL_NUMBER);
			Pos += Fix;
			Vel *= ZLV(!Fix.x, !Fix.y);
			UpdatePos(false);
			i = -1;
		}
	}
};

static void FillMapBackgroundTextures()
{
	ZL_SeededRand rnd(0);
	for (int map = 0; map <= 4; map++)
	{
		char* pMap = MapMain[map], *pTile = pMap;
		if (!srfMaps[map]) srfMaps[map] = ZL_Surface(1024, 1024, false);

		srfMaps[map].RenderToBegin();
		ZL_Display::Scale(1024.0f / MAP_W, 1024.0f / MAP_H);

		srfFloor.DrawTo(recMaps[0], 0.05f, 0.05f, colFloor[map]);
		srfTiles.BatchRenderBegin(true);
		srfTiles.SetTilesetIndex(map ? 1 : 0);
		for (int y = 0; y != MAP_H; y++)
			for (int x = 0; x != MAP_W; x++, pTile++)
			{
				if (*pTile != TILE_WALL) continue;
				ZL_Color colWall = colWalls[map][rnd.Int(3)];
				if (map == 0)
				{
					int ClosestSubmap = 0; float Closest = FLT_MAX;
					for (int submap = 1; submap != 5; submap++) { float dst = recMaps[submap].Distance(ZLV(x, y)).GetLength(); if (dst < Closest) { ClosestSubmap = submap; Closest = dst; } }
					colWall = ZL_Color::Lerp(colWalls[ClosestSubmap][rnd.Int(3)], colWall, ZL_Math::Clamp(Closest*.08f, .6f, 1.f));
				}
				srfTiles.DrawTo((float)x, (float)y, (float)(x+1), (float)(y+1), colWall);
			}

		for (sThing& it : MapThings[map])
		{
			if (it.Type == sThing::THING_ACTIVATOR)
			{
				srfTiles.SetTilesetIndex(2).DrawTo(it.Pos.x+0, it.Pos.y+0, it.Pos.x+1, it.Pos.y+1, colWalls[map][rnd.Int(3)]);
			}
		}
		srfTiles.BatchRenderEnd();
		srfMaps[map].RenderToEnd();
	}
}

static void DrawTextBordered(const ZL_Vector& p, const char* txt, scalar scale, const ZL_Color& colfill, const ZL_Color& colborder, ZL_Font& fnt, int border, ZL_Origin::Type origin)
{
	for (int i = 0; i < 9; i++) if (i != 4) fnt.Draw(p.x+(border*((i%3)-1)), p.y+8+(border*((i/3)-1)), txt, scale, scale, colborder, origin);
	fnt.Draw(p.x  , p.y+8  , txt, scale, scale, colfill, origin);
}

static struct sPlayer Player;

static void Init()
{
	srfTiles = ZL_Surface("Data/tiles.png").SetTilesetClipping(2, 2);
	srfItems = ZL_Surface("Data/items.png").SetTilesetClipping(2, 4).SetOrigin(ZL_Origin::Center);
	srfFloor = ZL_Surface("Data/floor.png").SetTextureRepeatMode();
	Player.Pos = PlayerStartPos;
	Player.UpdatePos(false);
	for (int i = 0; i != MAP_W*MAP_H; i++)
	{
		if (MAPTILE(0, i) >= '0' && MAPTILE(0, i) <= '9')
		{
			MapPoints[MAPTILE(0, i) - '0'] = MAPPOSFROMTILE(0, i) + ZLV(0,(i < MAP_W*MAP_H/2 ? 1 : 0));
			MapPointDirections[MAPTILE(0, i) - '0'] = (MAPTILE(0, i+1) == TILE_WALL ? ZLV(0,1) : ZLV(1,0));
		}
	}
	for (int map = 0; map != 5; map++)
		for (int i = 0; i != MAP_W*MAP_H; i++)
			if (MapMain[map][i] != TILE_EMPTY && MapMain[map][i] != TILE_WALL)
				sThing::Setup(&MapMain[map][i]);
	FillMapBackgroundTextures();

	imcTheme.Play();

	Phase = PHASE_FADEIN;
	PhaseTicks = (ticks_t)-1;

	#if !defined(ZILLALOG)
//	ZL_Display::SetFullscreen(true);
//	ZL_Display::SetPointerLock(true);
	#else
	Phase = PHASE_PLAY;
	#endif
}

static void Update()
{
	if (showTouchUI && ZL_Input::KeyDownCount()) showTouchUI = false;
	else if (!showTouchUI && ZL_Input::Down(ZL_BUTTON_LEFT)) showTouchUI = true;

	if (ZL_Input::Down(ZLK_ESCAPE))
	{
		if (Phase == PHASE_PLAY) Phase = PHASE_ASKQUIT;
		#ifndef __WEBAPP__
		else ZL_Application::Quit();
		#endif
	}

	if (Phase == PHASE_FADEIN)
	{
		ticks_t PhaseTime = ZLSINCE(PhaseTicks);
		if (PhaseTime < 350) return;
		recOrtho = ZL_Rectf::Lerp(recOrtho, recMaps[0], ZLELAPSEDF(PhaseTime*.001f));
		if (PhaseTime < 2250) return;
		Phase = PHASE_PLAY;
		PhaseTicks = ZLTICKS;
	}

	if (Phase == PHASE_FINISH || Phase == PHASE_ASKQUIT)
	{
		if (imcSfxMove.IsPlaying()) imcSfxMove.Stop();
		if (Phase == PHASE_ASKQUIT && ZL_Input::Up(ZLK_SPACE)) Phase = PHASE_PLAY;
		else if (Phase == PHASE_ASKQUIT && ZL_Input::Clicked()) Phase = PHASE_PLAY;
		else return;
	}

	bool actionHit = ((Player.OperatingPrism || Player.TileTouchingPrism) && ZL_Input::Down(ZLK_SPACE));

	ZL_Vector touchPosPlayerOnScreen = ZL_Rectf::Map(Player.Pos, recOrtho, ZL_Rectf(0, 0, ZLWIDTH, ZLHEIGHT));
	ZL_Rectf touchRecAction = ZL_Rectf(touchPosPlayerOnScreen, ZLHEIGHT*.05f);
	ZL_Vector touchDrag = (ZL_Input::HeldOutside(touchRecAction) ? ((ZL_Input::Pointer() - touchPosPlayerOnScreen).AddLength(-ZLHEIGHT*.05f) / (ZLHEIGHT*.2f)).SetMaxLength(1) : ZL_Vector::Zero);
	actionHit |= ((Player.OperatingPrism || Player.TileTouchingPrism) && ZL_Input::Clicked(touchRecAction));

	if (Player.OperatingPrism == NULL)
	{
		Player.Vel = ZLV((ZL_Input::Held(ZLK_D)||ZL_Input::Held(ZLK_RIGHT)) - (ZL_Input::Held(ZLK_A)||ZL_Input::Held(ZLK_LEFT)), (ZL_Input::Held(ZLK_W)||ZL_Input::Held(ZLK_UP)) - (ZL_Input::Held(ZLK_S)||ZL_Input::Held(ZLK_DOWN)));
		if (!Player.Vel) Player.Vel = touchDrag;

		bool IsMoving = !!Player.Vel;
		if (IsMoving)
		{
			float MoveDist = ZLELAPSEDF(10);
			while (MoveDist)
			{
				float MoveAmount = (MoveDist > .1f ? .1f : MoveDist);
				MoveDist -= MoveAmount;
				Player.Pos += Player.Vel * (MoveAmount * Player.Scale);
				Player.UpdatePos(true);
			}
		}

		if (Player.TileTouchingPrism && actionHit)
		{
			Player.PrismRotSpeed = 0;
			Player.OperatingPrism = sThing::Get(Player.TileTouchingPrism);
			ZL_ASSERT(Player.OperatingPrism);
			imcSfxOperateOn.Play(true);
		}

		if (imcSfxPrism.IsPlaying()) imcSfxPrism.Stop();
		if (imcSfxMove.IsPlaying()) { if (!IsMoving) imcSfxMove.Stop/*Pause*/(); }
		else                        { if ( IsMoving) imcSfxMove.Play();  }
	}
	else
	{
		float PrismRot = s((ZL_Input::Held(ZLK_A)||ZL_Input::Held(ZLK_LEFT)) - (ZL_Input::Held(ZLK_D)||ZL_Input::Held(ZLK_RIGHT)));
		if (!PrismRot) PrismRot = -touchDrag.x;

		if (PrismRot)
		{
			Player.PrismRotSpeed = ZL_Math::Clamp(Player.PrismRotSpeed + ZLELAPSEDF(5), .5f, 5);
			Player.OperatingPrism->Dir.Rotate(ZLELAPSEDF(PrismRot * Player.PrismRotSpeed));
		}
		else Player.PrismRotSpeed = 0;
		if (actionHit)
		{
			Player.OperatingPrism = NULL;
			imcSfxOperateOff.Play(true);
		}

		if (imcSfxMove.IsPlaying()) imcSfxMove.Stop();
		if (imcSfxPrism.IsPlaying()) { if (!PrismRot) imcSfxPrism.Stop/*Pause*/(); }
		else                         { if ( PrismRot) imcSfxPrism.Play();  }
	}

	ZL_Rectf recOrthoTarget = ZL_Rectf::Lerp(recMaps[0], recMaps[Player.Map], ZL_Math::Clamp01(Player.SubMapDepth));
	recOrtho = ZL_Rectf::Lerp(recOrtho, recOrthoTarget, ZLELAPSEDF(3));
}

static void Draw()
{
	ZL_Display::ClearFill(ZLRGB(.1,.1,.1));

	ZL_Display::PushOrtho(recOrtho);

	for (int map = 0; map <= 4; map++)
	{
		ZL_Display::PushMatrix();
		ZL_Display::Translate(recMaps[map].left, recMaps[map].low);
		ZL_Display::Scale(recMaps[map].Width() / MAP_W);

		srfMaps[map].DrawTo(recMaps[0]);
		for (sThing& it : MapThings[map])
		{
			if (it.Type == sThing::THING_PRISM || it.Type == sThing::THING_ACTIVATOR)
			{
				it.InCol = 0;
			}
		}
		for (sThing& it : MapThings[map])
		{
			if (it.Type == sThing::THING_EMITTER)
			{
				it.DoLazer(map);
				ZL_Display::FillWideLine(it.Pos.x+.5f, it.Pos.y+.5f, it.Trg.x, it.Trg.y, .11f, colLazer[it.Col]+ZLRGBA(.5,.5,.5,0));
				ZL_Display::FillWideLine(it.Pos.x+.5f, it.Pos.y+.5f, it.Trg.x, it.Trg.y, .1f, colLazer[it.Col]);
				srfItems.SetTilesetIndex(3).Draw(it.Pos.x+.5f, it.Pos.y+.5f, it.Dir.GetAngle(), .04f, .04f, colLazer[it.Col]);
			}
		}
		for (sThing& it : MapThings[map])
		{
			if (it.Type == sThing::THING_PRISM)
			{
				if (it.Col != it.InCol)
				{
					it.Col = it.InCol;
					if (it.Col && ZLSINCE(PhaseTicks) > 2000) imcSfxLazer.Play(true);
					it.Active = 0; //reset lazer distance
				}
				if (it.Col)
				{
					ZL_Display::FillWideLine(it.Pos.x+.5f, it.Pos.y+.5f, it.Trg.x, it.Trg.y, .11f, colLazer[it.Col]+ZLRGBA(.5,.5,.5,0));
					ZL_Display::FillWideLine(it.Pos.x+.5f, it.Pos.y+.5f, it.Trg.x, it.Trg.y, .1f, colLazer[it.Col]);
				}
				srfItems.SetTilesetIndex(2).Draw(it.Pos.x+.5f, it.Pos.y+.5f, it.Dir.GetAngle(), .034f, .034f, (it.Col ? colLazer[it.Col] + ZLRGBA(.4,.4,.4,0) : ZLWHITE));
				if (Player.OperatingPrism == &it) ZL_Display::DrawCircle(it.Pos.x+.5f, it.Pos.y+.5f, 1, ZL_Color::White, ZLRGBA(1,1,1,.2));
			}
			else if (it.Type == sThing::THING_DOOR)
			{
				if (it.Active && it.Active != 1) it.Active = ZL_Math::Min(1, it.Active + ZLELAPSEDF(2));
				float d = .49f - (it.Active * .3f);
				srfTiles.SetTilesetIndex(0).DrawTo(it.Pos.x+.01f, it.Pos.y+.01f, it.Pos.x+.01f+d, it.Pos.y+.01f+d);
				srfTiles.SetTilesetIndex(0).DrawTo(it.Pos.x+.99f, it.Pos.y+.01f, it.Pos.x+.99f-d, it.Pos.y+.01f+d);
				srfTiles.SetTilesetIndex(0).DrawTo(it.Pos.x+.99f, it.Pos.y+.99f, it.Pos.x+.99f-d, it.Pos.y+.99f-d);
				srfTiles.SetTilesetIndex(0).DrawTo(it.Pos.x+.01f, it.Pos.y+.99f, it.Pos.x+.01f+d, it.Pos.y+.99f-d);
			}
			else if (it.Type == sThing::THING_TRAP)
			{
				if (it.Active && it.Active != 1) it.Active = ZL_Math::Min(1, it.Active + ZLELAPSEDF(2));
				float d = (it.Active * .98f);
				srfTiles.SetTilesetIndex(2).DrawTo(it.Pos.x+.28f, it.Pos.y+.01f, it.Pos.x+.38f, it.Pos.y+.01f+d, colKeyLocks[it.Col]);
				srfTiles.SetTilesetIndex(2).DrawTo(it.Pos.x+.61f, it.Pos.y+.01f, it.Pos.x+.71f, it.Pos.y+.01f+d, colKeyLocks[it.Col]);
				srfTiles.SetTilesetIndex(2).DrawTo(it.Pos.x+.01f, it.Pos.y+.28f, it.Pos.x+.01f+d, it.Pos.y+.38f, colKeyLocks[it.Col]);
				srfTiles.SetTilesetIndex(2).DrawTo(it.Pos.x+.01f, it.Pos.y+.61f, it.Pos.x+.01f+d, it.Pos.y+.71f, colKeyLocks[it.Col]);
			}
			else if (it.Type == sThing::THING_ACTIVATOR)
			{
				float NewActive = (it.InCol == it.Col ? ZL_Math::Min(1, it.Active + ZLELAPSED) : 0);
				srfItems.SetTilesetIndex(NewActive != 1 ? 4 : 5).DrawTo(it.Pos.x+0, it.Pos.y+0, it.Pos.x+1, it.Pos.y+1, colLazer[it.Col]);
				if (it.Active != NewActive)
				{
					if (it.Active < .1f && NewActive >= .1f) imcSfxActivate.Play(true);
					if (NewActive > 0 && NewActive < 1)
						ZL_Display::FillCircle(it.Pos.x+.5f, it.Pos.y+.5f, .4f*NewActive, colLazer[it.Col]);
					if (NewActive == 0 || NewActive == 1)
					{
						float DoorActive = (NewActive ? .01f : 0);
						sThing* pDoor;
						if ((pDoor = sThing::Get(map, it.TileIndex + 1))     && pDoor->Type == sThing::THING_DOOR) pDoor->Active = DoorActive;
						if ((pDoor = sThing::Get(map, it.TileIndex - 1))     && pDoor->Type == sThing::THING_DOOR) pDoor->Active = DoorActive;
						if ((pDoor = sThing::Get(map, it.TileIndex + MAP_W)) && pDoor->Type == sThing::THING_DOOR) pDoor->Active = DoorActive;
						if ((pDoor = sThing::Get(map, it.TileIndex - MAP_W)) && pDoor->Type == sThing::THING_DOOR) pDoor->Active = DoorActive;
						if (NewActive) imcSfxDoor.Play(true);
					}
					it.Active = NewActive;
				}
			}
			else if (it.Type == sThing::THING_KEY)
			{
				if (it.Active) continue;
				srfItems.SetTilesetIndex(0).DrawTo(it.Pos.x+.1f, it.Pos.y-.1f, it.Pos.x+1.1f, it.Pos.y+0.9f, ZL_Color::DarkGray);
				srfItems.SetTilesetIndex(0).DrawTo(it.Pos.x+0, it.Pos.y+0, it.Pos.x+1, it.Pos.y+1, colKeyLocks[it.Col]);
			}
			else if (it.Type == sThing::THING_LOCK)
			{
				if (it.Active && it.Active != 1) it.Active = ZL_Math::Min(1, it.Active + ZLELAPSEDF(2));
				srfTiles.SetTilesetIndex(3).DrawTo(it.Pos.x+0, it.Pos.y+0, it.Pos.x+1, it.Pos.y+(1 - it.Active * .9f), colKeyLocks[it.Col]);
			}
			else if (it.Type == sThing::THING_FINISH)
			{
				srfItems.SetTilesetIndex(1).DrawTo(it.Pos.x+0, it.Pos.y+0, it.Pos.x+1, it.Pos.y+1);
			}
		}

		#if defined(ZILLALOG) && !defined(__SMARTPHONE__)
		if (ZL_Input::Held(ZLK_LALT))
		{
			char* pMap = MapMain[map];
			char tmpstr[2] = {0, 0};
			for (int i = 0; i != MAP_W*MAP_H; i++)
				if (pMap[i] != TILE_EMPTY && pMap[i] != TILE_WALL)
					tmpstr[0] = pMap[i], fntBig.Draw(MAPPOSFROMTILE(0, i)+ZLV(.3f,.1f), tmpstr, .01f);
		}
		#endif

		ZL_Display::PopMatrix();
	}

	if (!Player.OperatingPrism)
	{
		//Draw player
		srfItems.SetTilesetIndex(7).DrawTo(ZL_Rectf(Player.Pos, PlayerSize * Player.Scale * 1.3f), ZLLUMA(0,.3));
		srfItems.SetTilesetIndex(7).DrawTo(ZL_Rectf(Player.Pos, PlayerSize * Player.Scale));
	}

	#if defined(ZILLALOG) //MAP EDITING MAP EDITING MAP EDITING MAP EDITING MAP EDITING MAP EDITING
	ZL_Vector mp = ZL_Display::ScreenToWorld(ZL_Input::Pointer());
	for (int map = 4; map >= 0; map--)
	{
		if (!recMaps[map].Contains(mp)) continue;
		mp = recMaps[0].LerpPos(recMaps[map].InverseLerpPos(mp));
		int mx = (int)mp.x, my = MAP_H-1-(int)mp.y;
		#ifndef __SMARTPHONE__
		char& MMapTile = MapMain[map][my * MAP_W + mx];
		static bool mRemoving = false;
		bool Chg = false;
		if (ZL_Input::Down()) mRemoving = (MMapTile == TILE_WALL), Chg = true;
		if (ZL_Input::Held()) { MMapTile = (mRemoving ? TILE_EMPTY : TILE_WALL); sThing::Remove(&MMapTile); Chg = true; }
		if (ZL_Input::Held(ZLK_LCTRL) || ZL_Input::Held(ZLK_RCTRL))
		{
			for (char tc = 'A'; tc <= 'Z'; tc++) if (ZL_Input::Down((ZL_Key)(ZLK_A+tc-'A'))) { MMapTile = tc; sThing::Setup(&MMapTile); Chg = true; }
			static const char SpecialChars[] = "/*-+";
			for (int tc = ZLK_KP_DIVIDE; tc <= ZLK_KP_PLUS; tc++) if (ZL_Input::Down((ZL_Key)tc)) { MMapTile = SpecialChars[tc-ZLK_KP_DIVIDE]; sThing::Setup(&MMapTile); Chg = true; }
		}
		if (ZL_Input::Clicked(ZL_BUTTON_MIDDLE)) { MMapTile = (MMapTile < 'a' ? MMapTile|0x20 : MMapTile&~0x20); sThing::Setup(&MMapTile); Chg = true; }
		if (Chg) FillMapBackgroundTextures();
		ZL_Display::FillRect(ZL_Rectf::ByCorners(recMaps[map].LerpPos(recMaps[0].InverseLerpPos(ZLV(mx, MAP_H-my))), recMaps[map].LerpPos(recMaps[0].InverseLerpPos(ZLV(mx+1, MAP_H-my-1)))), ZL_Color::Yellow);
		#endif
		if (ZL_Input::Down(ZLK_F1)) Player.Pos = recMaps[map].LerpPos(recMaps[0].InverseLerpPos(ZLV(mx+.5f, MAP_H-my-.5f)));
		break;
	}
	if (ZL_Input::Down(ZLK_F2))
		for (int map = 0; map <= 4; map++)
			for (int y = 0; y != MAP_H; y++)
			{
				FILE *f; if (map == 0 && y == 0) f = fopen("map.inl", "w");
				fprintf(f, map == 0 && y == 0 ? "static char MapMain[5][MAP_W*MAP_H+1] =\n{\n    \"" : "    \"");
				for (int x = 0; x != MAP_W; x++) fprintf(f, "%c", (MapMain[map][y * MAP_W + x] ? MapMain[map][y * MAP_W + x] : TILE_EMPTY));
				fprintf(f, y == MAP_H-1 ? (map == 4 ? "\"\n};\n\n" : "\",\n\n") : "\"\n");
				if (y == MAP_H-1 && map == 4) fclose(f);
			}
	#endif //MAP EDITING MAP EDITING MAP EDITING MAP EDITING MAP EDITING MAP EDITING

	ZL_Display::PopOrtho();

	//Draw inventory
	float InvX = ZLHALFW - 56;
	ZL_Display::DrawRect(InvX    , -1, InvX + 112, 27    , ZL_Color::Gray, ZLLUMA(0, .5));
	ZL_Display::DrawLine(InvX + 28, 3, InvX +  28, 27 - 4, ZL_Color::Gray);
	ZL_Display::DrawLine(InvX + 56, 3, InvX +  56 ,27 - 4, ZL_Color::Gray);
	ZL_Display::DrawLine(InvX + 84, 3, InvX +  84, 27 - 4, ZL_Color::Gray);
	if (Player.KeyRing & 0x1) srfItems.SetTilesetIndex(0).Draw(InvX +  16, 14, PIHALF*.5f, .5, .5, colKeyLocks[0]);
	if (Player.KeyRing & 0x2) srfItems.SetTilesetIndex(0).Draw(InvX +  44, 14, PIHALF*.5f, .5, .5, colKeyLocks[1]);
	if (Player.KeyRing & 0x4) srfItems.SetTilesetIndex(0).Draw(InvX +  72, 14, PIHALF*.5f, .5, .5, colKeyLocks[2]);
	if (Player.KeyRing & 0x8) srfItems.SetTilesetIndex(0).Draw(InvX + 100, 14, PIHALF*.5f, .5, .5, colKeyLocks[3]);

	float DistToStartSq = Player.Pos.GetDistanceSq(PlayerStartPos);
	if (Phase == PHASE_ASKQUIT)
	{
		ZL_Display::FillRect(0, 0, ZLWIDTH, ZLHEIGHT, ZLLUMA(0, .7));
		DrawTextBordered(ZL_Display::Center() + ZLV(0,  200), "PAUSE", 1.25f, ZL_Color::White, ZL_Color::Black, fntBig, 4, ZL_Origin::Center);
		if (showTouchUI)
		{
			DrawTextBordered(ZL_Display::Center() + ZLV(0, -200), "Press [BACK] again to quit", .5f, ZL_Color::White, ZL_Color::Black, fntBig, 1, ZL_Origin::Center);
			DrawTextBordered(ZL_Display::Center() + ZLV(0, -300), "Tap screen to continue", .5f, ZL_Color::White, ZL_Color::Black, fntBig, 1, ZL_Origin::Center);
		}
		else
		{
			#ifndef __WEBAPP__
			DrawTextBordered(ZL_Display::Center() + ZLV(0, -200), "Press [ESC] again to quit", .5f, ZL_Color::White, ZL_Color::Black, fntBig, 1, ZL_Origin::Center);
			#endif
			DrawTextBordered(ZL_Display::Center() + ZLV(0, -300), "Press [SPACE] to continue", .5f, ZL_Color::White, ZL_Color::Black, fntBig, 1, ZL_Origin::Center);
		}
	}
	else if (Player.OperatingPrism)
	{
		ZL_Vector TextOffset = ZLV(0, Player.Pos.y < recMaps[Player.Map].MidY() ? 240 : -300);
		if (showTouchUI)
		{
			DrawTextBordered(ZL_Display::Center() + TextOffset + ZLV(0, 75), "Drag left/right to rotate prism", .75f, ZL_Color::Black, ZL_Color::White, fntBig, 2, ZL_Origin::Center);
			DrawTextBordered(ZL_Display::Center() + TextOffset, "Tap prism to stop operation", .75f, ZL_Color::Black, ZL_Color::White, fntBig, 2, ZL_Origin::Center);
		}
		else
		{
			DrawTextBordered(ZL_Display::Center() + TextOffset + ZLV(0, 75), "Use [A] / [D] to rotate prism", .75f, ZL_Color::Black, ZL_Color::White, fntBig, 2, ZL_Origin::Center);
			DrawTextBordered(ZL_Display::Center() + TextOffset, "Press [SPACE] to stop operation", .75f, ZL_Color::Black, ZL_Color::White, fntBig, 2, ZL_Origin::Center);
		}
	}
	else if (Player.TileTouchingPrism)
	{
		ZL_Vector TextOffset = ZLV(0, Player.Pos.y < recMaps[Player.Map].MidY() ? 250 : -250);
		if (showTouchUI)
		{
			DrawTextBordered(ZL_Display::Center() + TextOffset, "Tap prism to operate it", .75f, ZL_Color::Black, ZL_Color::White, fntBig, 2, ZL_Origin::Center);
		}
		else
		{
			DrawTextBordered(ZL_Display::Center() + TextOffset, "Press [SPACE] to operate prism", .75f, ZL_Color::Black, ZL_Color::White, fntBig, 2, ZL_Origin::Center);
		}
	}
	else if (DistToStartSq < 1)
	{
		float a = 1.f-ssqrt(DistToStartSq);
		ZL_Color TexWhite = ZLLUMA(1, a), TexBlack = ZLLUMA(0, a);
		DrawTextBordered(ZL_Display::Center() + ZLV(0,  200), "Welcome to the Lazer Maze!", 1.25f, TexWhite, TexBlack, fntBig, 4, ZL_Origin::Center);
		if (Phase == PHASE_PLAY)
		{
			DrawTextBordered(ZL_Display::Center() + ZLV(0, -120), "Get to the goal!", 1.0f, TexBlack, TexWhite, fntBig, 2, ZL_Origin::Center);
			if (showTouchUI)
			{
				DrawTextBordered(ZL_Display::Center() + ZLV(0, -220), "Drag up/down/left/right to move", 1.0f, TexBlack, TexWhite, fntBig, 2, ZL_Origin::Center);
			}
			else
			{
				DrawTextBordered(ZL_Display::Center() + ZLV(0, -220), "Use [WASD] or [ARROWS] to move", 1.0f, TexBlack, TexWhite, fntBig, 2, ZL_Origin::Center);
				DrawTextBordered(ZL_Display::Center() + ZLV(0, -285), "[ESC] to Pause / [ALT]+[Enter] for fullscreen toggle", .25f, TexBlack, TexWhite, fntBig, 1, ZL_Origin::Center);
			}
			DrawTextBordered(ZL_Display::Center() + ZLV(0, -320), "(C) 2018-2019 Bernhard Schelling", .25f, TexBlack, TexWhite, fntBig, 1, ZL_Origin::Center);
		}
	}

	if (Phase == PHASE_FADEIN)
	{
		if (PhaseTicks == (ticks_t)-1) PhaseTicks = ZLTICKS;
		ZL_Display::FillRect(0, 0, ZLWIDTH, ZLHEIGHT, ZLLUMA(0, 1.25f-ZLSINCESECONDS(PhaseTicks)));
	}
	if (Phase == PHASE_FINISH)
	{
		float a = ZLSINCESECONDS(PhaseTicks) * .25f;
		ZL_Display::FillRect(0, 0, ZLWIDTH, ZLHEIGHT, ZLLUMA(0, ZL_Math::Min(a * .5f, .5f)));
		ZL_Color TexWhite = ZLLUMA(1, a), TexBlack = ZLLUMA(0, a);
		DrawTextBordered(ZL_Display::Center() + ZLV(0,    0), "You defeated the Lazer Maze!", 1.25f, TexWhite, TexBlack, fntBig, 4, ZL_Origin::Center);
		DrawTextBordered(ZL_Display::Center() + ZLV(0,  200), "Congratulations!", 1.25f, TexWhite, TexBlack, fntBig, 4, ZL_Origin::Center);
		#if !defined(__WEBAPP__) && !defined(__SMARTPHONE__)
		DrawTextBordered(ZL_Display::Center() + ZLV(0, -300), "Press ESC to Quit", .5f, TexWhite, TexBlack, fntBig, 1, ZL_Origin::Center);
		#endif
	}
}

static struct sLazerMaze : public ZL_Application
{
	sLazerMaze() : ZL_Application() { }

	virtual void Load(int argc, char *argv[])
	{
		if (!ZL_Application::LoadReleaseDesktopDataBundle()) return;
		if (!ZL_Display::Init("Lazer Maze", 1280, 720, ZL_DISPLAY_ALLOWRESIZEHORIZONTAL)) return;
		ZL_Input::Init();
		ZL_Audio::Init();
		fntBig = ZL_Font("Data/typomoderno.ttf.zip", 100).SetDrawAtBaseline(false);
		Init();
	}

	virtual void AfterFrame()
	{
		Update();
		Draw();
		//Show FPS
		#ifdef ZILLALOG
		fntBig.Draw(ZLFROMW(50), ZLFROMH(30), ZL_String::format("%d", FPS), ZL_Origin::TopRight);
		#endif
	}
} LazerMaze;
