#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <vector>
#include <stdexcept>
#include <cstdarg>
#include <sstream>

class FTLibraryContainer
{
public:
	FTLibraryContainer();
	~FTLibraryContainer();

	FT_Library& getLibrary();

private:
	static FT_Library m_library;
};

class Font
{
public:
	Font();
	~Font();

	void Load(const std::string& filename, unsigned int size);
	void release();
	
	bool isValid() const;

	void drawText(float x, float y, const char *str, ...) const;
	void drawText(float x, float y, const std::string& str) const;
	
	std::ostream& beginDraw(float x, float y);
	void endDraw();
	
	unsigned int calcStringWidth(const std::string& str) const;
	unsigned int getHeight() const;

private:
	// leave copy constructor and operator= undefined to make noncopyable
	Font(const Font&);
	const Font& operator=(const Font&);
	
private:
	// font data
	unsigned int texID_;
	unsigned int listBase_;
	std::vector<unsigned char> widths_;
	unsigned char height_;

	// stream drawing stuff
	std::ostringstream m_ss;
	float m_X;
	float m_Y;

	static const unsigned int SPACE = 32;
	static const unsigned int NUM_CHARS = 96;

	static FTLibraryContainer m_library;
};
