#include "ByteBuffer.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "ByteUtil.h"

using namespace std;

ByteBuffer::ByteBuffer(const string &sPath, const char *sMode) :
	m_iPosition(0)
{
	// Load the file
	Load(sPath, sMode);
}

ByteBuffer::ByteBuffer(const uint8_t *sBuf, uint32_t iLength) :
	m_iPosition(0)
{
	// Create the new buffer
	m_vBuffer.resize(iLength, 0);

	// Put the data in
	PutRaw(sBuf, iLength);

	// Reset the position
	m_iPosition = 0;
}

ByteBuffer::ByteBuffer(uint32_t iLength) :
	m_iPosition(0)
{
	// Create the new buffer
	m_vBuffer.resize(iLength, 0);
}

ByteBuffer::ByteBuffer(const ByteBuffer &rhs) :
	m_vBuffer(rhs.m_vBuffer),
	m_iPosition(rhs.m_iPosition)
{
	unsigned iLeft = rhs.GetLength() - rhs.GetPosition();
	PutRaw(&rhs.GetBuffer()[GetPosition()], iLeft);
	m_iPosition = 0;
}

ByteBuffer::~ByteBuffer()
{
	m_vBuffer.clear();
}

void ByteBuffer::Load(const string &sPath, const char *sMode)
{
	// Open a file handle
	FILE *pFile = fopen(sPath.c_str(), sMode);

	// Ensure it works
	if (pFile == NULL)
	{
		m_vBuffer.clear();
		return;
	}

	// Get the file data
	fseek(pFile, 0, SEEK_END);
	unsigned iLength = ftell(pFile);
	rewind(pFile);

	// Create our buffer
	m_vBuffer.resize(iLength, 0);

	// Read the file into our buffer
	uint32_t iRead = fread(&m_vBuffer[0], 1, iLength, pFile);

	// Close the file
	fclose(pFile);

	// Ensure the full length read
	if (iRead != iLength)
	{
		m_vBuffer.clear();
		return;
	}
}

void ByteBuffer::Save(const string &sPath)
{
	// Open a file
	FILE *pFile = fopen(sPath.c_str(), "wb");

	// Test for a null file
	if (pFile == NULL)
		return;

	// Start writing
	size_t iWrote = fwrite(GetBuffer(), 1, GetLength(), pFile);

	// Test it
	if (iWrote != GetLength())
		printf("ByteBuffer Save: Error writing all data\n");

	// Close it up
	fclose (pFile);
}

void ByteBuffer::Reinit(uint32_t iLength)
{
	m_vBuffer.clear();
	m_vBuffer.resize(iLength, 0);
	m_iPosition = 0;
}

void ByteBuffer::Reinit(const uint8_t *sBuf, uint32_t iLength)
{
	m_vBuffer.clear();
	m_vBuffer.resize(iLength, 0);

	m_iPosition = 0;
	PutRaw(sBuf, iLength);

	m_iPosition = 0;
}

void ByteBuffer::Reinit(const ByteBuffer &Buffer)
{
	m_vBuffer.clear();
	m_vBuffer.resize(Buffer.GetLength(), 0);

	m_iPosition = 0;
	PutRaw(Buffer.GetBuffer(), Buffer.GetLength());

	m_iPosition = 0;
}

void ByteBuffer::Resize(uint32_t iLength)
{
	// Just change the capacity
	if (iLength > m_vBuffer.size())
		m_vBuffer.resize(iLength);
}

uint8_t ByteBuffer::GetByte()
{
	return m_vBuffer[m_iPosition++];
}

uint16_t ByteBuffer::GetShort()
{
	m_iPosition += 2;
	return Make16(&m_vBuffer[m_iPosition - 2]);
}

uint32_t ByteBuffer::GetTriByte()
{
	m_iPosition += 3;
	return Make24(&m_vBuffer[m_iPosition - 3]);
}

uint32_t ByteBuffer::GetInt()
{
	m_iPosition += 4;
	return Make32(&m_vBuffer[m_iPosition - 4]);
}

uint64_t ByteBuffer::GetLong()
{
	m_iPosition += 8;
	return Make64(&m_vBuffer[m_iPosition - 8]);
}

string ByteBuffer::GetString()
{
	char c;
	string sReturn;

	// safeguard against infinite looping on end position
	while (m_iPosition < GetLength())
		if ((c = GetByte()) == '\0')
			break;
		else
			sReturn.push_back(c);
	return sReturn;
}

string ByteBuffer::GetLine()
{
	char c;
	string sReturn;

	// safeguard against infinite looping on end position
	while (m_iPosition < GetLength())
	{
		if ((c = GetByte()) == '\n')
			break;
		else if (c == '\r')
			continue;
		else
			sReturn.push_back(c);
	}
	return sReturn;
}

unsigned ByteBuffer::GetRaw(uint8_t* buf, unsigned iBytes)
{
	unsigned iPossible = m_vBuffer.size() - m_iPosition;
	unsigned iToCopy = iPossible > iBytes ? iBytes : iPossible;
	memcpy(buf, &m_vBuffer[m_iPosition], iToCopy);

	m_iPosition += iToCopy;
	return iToCopy;
}

void ByteBuffer::PutByte(const uint8_t &iByte)
{
	m_vBuffer[m_iPosition++] = iByte;
}

void ByteBuffer::PutShort(const uint16_t &iShort)
{
	Split16(iShort, &m_vBuffer[m_iPosition]);
	m_iPosition += 2;
}

void ByteBuffer::PutTriByte(const uint32_t &iTriByte)
{
	Split24(iTriByte, &m_vBuffer[m_iPosition]);
	m_iPosition += 3;
}

void ByteBuffer::PutInt(const uint32_t &iInt)
{
	Split32(iInt, &m_vBuffer[m_iPosition]);
	m_iPosition += 4;
}

void ByteBuffer::PutLong(const uint64_t &iLong)
{
	Split64(iLong, &m_vBuffer[m_iPosition]);
	m_iPosition += 8;
}

void ByteBuffer::PutString(const std::string &sString)
{
	PutRaw((uint8_t *)sString.c_str(), sString.length());
	PutByte('\n');
}

unsigned ByteBuffer::PutRaw(const uint8_t *data, unsigned iLen)
{
	uint32_t iLeft = m_vBuffer.size() - m_iPosition;
	iLen = (iLen < iLeft) ? iLen : iLeft;

	memcpy(&m_vBuffer[m_iPosition], data, iLen);
	m_iPosition += iLen;
	return iLen;
}

