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
