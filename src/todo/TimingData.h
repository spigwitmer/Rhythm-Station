#ifndef _TIMING_DATA_H_
#define _TIMING_DATA_H_

// please edit this until you think it is sane.

// this should probably be a bitmask instead!
enum NoteType
{
	Note_Tap = 0,
	Note_Hold,	// hold "heads" = first hold, tail is the second.
	Note_Roll,	// same rule as holds.
	Note_Mine
};

enum NoteAttribute
{
	NoteAttribute_None = 0,
	NoteAttribute_Fake,
	NoteAttribute_Invisible
};

struct Note
{
	// Generic stuff.
	NoteType type;
	std::vector<NoteAttribute> flags;
	
	unsigned char column;	// For games which use columns (e.g. StepMania)
	
	bool hit;
};

struct NoteRow
{
	std::vector<Note> notes;	// Notes on this row
	unsigned int time;			// Time in milliseconds.
	
	float scroll_speed;			// A stop would just be setting this to 0.
};

struct NoteRegion
{
	unsigned int combo_rate; // aka tickcount
	
	struct TimeSignature
	{
		int numerator;
		int denominator;
	} time_sig;
	
	unsigned int time;
};

struct TimingData
{
	std::vector<NoteRow> note_rows;
	std::vector<NoteRegion> note_regions;
	
	int timing_offset;
};

#endif

/**
 * @file
 * @author Colby Klein (c) 2011
 * @section LICENSE
 * 
 * All rights reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
