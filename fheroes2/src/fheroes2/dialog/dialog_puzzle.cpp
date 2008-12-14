/***************************************************************************
 *   Copyright (C) 2008 by Andrey Afletdinov <afletdinov@mail.dc.baikal.ru>*
 *                         Josh Matthews <josh@joshmatthews.net>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <string>
#include "agg.h"
#include "button.h"
#include "cursor.h"
#include "world.h"
#include "settings.h"
#include "dialog.h"
#include "gamearea.h"
#include "radar.h"
#include "kingdom.h"


#define PUZZLE_WIDTH 6
#define PUZZLE_HEIGHT 8

void InitPuzzle(std::vector<Point> & pieceOffsets, std::vector<Point> & revealOrder)
{
    pieceOffsets.push_back(Point(-13, 0));
    pieceOffsets.push_back(Point(-20, 0));
    pieceOffsets.push_back(Point(-15, 0));
    pieceOffsets.push_back(Point(-23, 0));
    pieceOffsets.push_back(Point(-29, 0));
    
    pieceOffsets.push_back(Point(0, -25));
    pieceOffsets.push_back(Point(-16, 5));
    pieceOffsets.push_back(Point(-13, -9));
    pieceOffsets.push_back(Point(-19, 14));
    pieceOffsets.push_back(Point(-22, 0));
    pieceOffsets.push_back(Point(-18, -5));
    
    pieceOffsets.push_back(Point(0, -22));
    pieceOffsets.push_back(Point(-27, -1));
    pieceOffsets.push_back(Point(-18, -8));
    pieceOffsets.push_back(Point(-19, 18));
    pieceOffsets.push_back(Point(-18, 0));
    pieceOffsets.push_back(Point(-19, -3));
    
    pieceOffsets.push_back(Point(0, -28));
    pieceOffsets.push_back(Point(-20, 8));
    pieceOffsets.push_back(Point(-13, 0));
    pieceOffsets.push_back(Point(-20, 5));
    pieceOffsets.push_back(Point(-23, -11));
    pieceOffsets.push_back(Point(-16, 3));
    
    pieceOffsets.push_back(Point(0, -20));
    pieceOffsets.push_back(Point(-16, -1));
    pieceOffsets.push_back(Point(-23, -7));
    pieceOffsets.push_back(Point(-12, 13));
    pieceOffsets.push_back(Point(-13, -3));
    pieceOffsets.push_back(Point(-20, -1));
    
    pieceOffsets.push_back(Point(0, -28));
    pieceOffsets.push_back(Point(-19, 6));
    pieceOffsets.push_back(Point(-18, -7));
    pieceOffsets.push_back(Point(-19, 0));
    pieceOffsets.push_back(Point(-18, 2));
    pieceOffsets.push_back(Point(-27, 1));
    
    pieceOffsets.push_back(Point(0, -22));
    pieceOffsets.push_back(Point(-18, -4));
    pieceOffsets.push_back(Point(-22, -12));
    pieceOffsets.push_back(Point(-19, 26));
    pieceOffsets.push_back(Point(-13, -9));
    pieceOffsets.push_back(Point(-16, 2));
    
    pieceOffsets.push_back(Point(0, -25));
    pieceOffsets.push_back(Point(-29, 2));
    pieceOffsets.push_back(Point(-23, -8));
    pieceOffsets.push_back(Point(-15, 6));
    pieceOffsets.push_back(Point(-20, 4));
    pieceOffsets.push_back(Point(-13, 0));

    revealOrder.push_back(Point(5, 3));
    revealOrder.push_back(Point(5, 0));
    revealOrder.push_back(Point(0, 4));
    revealOrder.push_back(Point(1, 0));
    revealOrder.push_back(Point(2, 7));
    revealOrder.push_back(Point(0, 6));
    revealOrder.push_back(Point(5, 1));
    revealOrder.push_back(Point(1, 1));
    revealOrder.push_back(Point(4, 2));
    revealOrder.push_back(Point(5, 7));
    revealOrder.push_back(Point(0, 5));
    revealOrder.push_back(Point(3, 6));
    revealOrder.push_back(Point(3, 7));
    revealOrder.push_back(Point(4, 0));
    revealOrder.push_back(Point(1, 5));
    revealOrder.push_back(Point(2, 0));
    revealOrder.push_back(Point(1, 7));
    revealOrder.push_back(Point(4, 6));
    revealOrder.push_back(Point(3, 0));
    revealOrder.push_back(Point(1, 6));
    revealOrder.push_back(Point(4, 7));
    revealOrder.push_back(Point(5, 6));
    revealOrder.push_back(Point(0, 2));
    revealOrder.push_back(Point(3, 2));
    revealOrder.push_back(Point(2, 2));
    revealOrder.push_back(Point(2, 6));
    revealOrder.push_back(Point(7, 7));
    revealOrder.push_back(Point(5, 2));
    revealOrder.push_back(Point(4, 5));
    revealOrder.push_back(Point(0, 0));
    revealOrder.push_back(Point(0, 7));
    revealOrder.push_back(Point(5, 6));
    revealOrder.push_back(Point(6, 7));
    revealOrder.push_back(Point(4, 4));
    revealOrder.push_back(Point(5, 5));
    revealOrder.push_back(Point(0, 1));
    revealOrder.push_back(Point(1, 3));
    revealOrder.push_back(Point(1, 4));
    revealOrder.push_back(Point(4, 7));
    revealOrder.push_back(Point(4, 3));
    revealOrder.push_back(Point(1, 2));
    revealOrder.push_back(Point(2, 3));
    revealOrder.push_back(Point(5, 4));
    revealOrder.push_back(Point(3, 1));
    revealOrder.push_back(Point(4, 1));
    revealOrder.push_back(Point(2, 5));
    revealOrder.push_back(Point(3, 4));
    // ?????
}

void Dialog::PuzzleMaps(void)
{
    std::vector<Point> pieceOffsets;
    std::vector<Point> revealOrder;

    pieceOffsets.reserve(50);
    revealOrder.reserve(50);

    InitPuzzle(pieceOffsets, revealOrder);

    Display & display = Display::Get();

    Cursor & cursor = Cursor::Get();
    cursor.Hide();
    cursor.SetThemes(Cursor::POINTER);
    
    AGG::PlayMusic(MUS::PUZZLE);

    const Surface & sf = world.GetUltimateArtifactArea();
    u8 open_puzzle = world.GetKingdom(Settings::Get().MyColor()).CountVisitedObjects(MP2::OBJ_OBELISK);
    const u8 max_obelisk = world.CountObeliskOnMaps();

    Dialog::FrameBorder background(false, sf.w(), sf.h());

    const Point cur_pt(background.GetArea().x, background.GetArea().y);
    Point dst_pt(cur_pt);

    const Rect &radar = Radar::Get().GetRect();
    const Sprite &puzzleTitle = AGG::GetICN(ICN::VIEWPUZL, 0);
    display.Blit(puzzleTitle, radar);

    if(display.w() == 640)
        dst_pt.x += BORDERWIDTH;
    if(display.h() == 480)
        dst_pt.y += BORDERWIDTH;
    display.Blit(sf, dst_pt);
    
    // determine which puzzle pieces are visible
    char pieces[PUZZLE_HEIGHT][PUZZLE_WIDTH];
    memset(pieces, 1, sizeof(pieces));
    const u16 revealPerObelisk = PUZZLE_WIDTH * PUZZLE_HEIGHT / max_obelisk;
    //const u16 extraPieces = PUZZLE_WIDTH * PUZZLE_HEIGHT % max_obelisk;
    u16 revealIndex = 0;
    while(open_puzzle--)
    {
        u16 toReveal = revealPerObelisk;
        while(toReveal--)
        {
            if(revealIndex >= revealOrder.size())
                continue;
            
            Point revealPt = revealOrder[revealIndex++];
            pieces[revealPt.y][revealPt.x] = 0;
        }
    }
    
    // draw pieces
    Point drawPt(dst_pt.x, dst_pt.y);
    for(u16 y = 0; y < PUZZLE_HEIGHT; y++)
    {
        for(u16 x = 0; x < PUZZLE_WIDTH; x++)
        {
            const Sprite &piece = AGG::GetICN(ICN::PUZZLE, y * PUZZLE_WIDTH + x);
            if(pieces[y][x]) //FIXME: Temporary until pieces can fade out
                display.Blit(piece, drawPt);
            drawPt.x += piece.w() + pieceOffsets[y * PUZZLE_WIDTH + x].x;
            drawPt.y += pieceOffsets[y * PUZZLE_WIDTH + x].y;
            if(x == PUZZLE_WIDTH - 1)
                drawPt.y += piece.h();
        }
        drawPt.x = dst_pt.x;
    }

    //TODO: Make non-visible pieces fade out

    // The exit button ICNs for the exit button are screwed up (about 5x as much vertical space
    // as it needed, so the size is hardcoded here)
    ICN::icn_t buttonICN = Settings::Get().EvilInterface() ? ICN::LGNDXTRE : ICN::LGNDXTRA;
    const Sprite &button = AGG::GetICN(buttonICN, 4);
    dst_pt.x = radar.x + (radar.w - button.w()) / 2;
    dst_pt.y = radar.y + radar.h - 27 - 10;
    Button buttonExit(dst_pt, buttonICN, 4, 5);

    buttonExit.Draw();

    cursor.Show();
    display.Flip();

    LocalEvent & le = LocalEvent::GetLocalEvent();
   
    // message loop
    while(le.HandleEvents())
    {
        le.MousePressLeft(buttonExit) ? buttonExit.PressDraw() : buttonExit.ReleaseDraw();

        if(le.MouseClickLeft(buttonExit) || le.KeyPress(KEY_RETURN) || le.KeyPress(KEY_ESCAPE)) break;
    }

    // In higher resolutions, the radar is not included in the FrameBorder,
    // so we need to make sure that it is explicitly redrawn.
    Radar::Get().RedrawArea();
}