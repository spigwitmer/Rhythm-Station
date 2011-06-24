#ifndef BYTE_BUFFER_H
#define BYTE_BUFFER_H

#include <stdint.h>
#include <string>
#include <vector>

class ByteBuffer
{
public:
	// Create empty buffer
	ByteBuffer() : m_iPosition(0) { }
	ByteBuffer(const std::string &sPath, const char *sMode = "rb");
	ByteBuffer(const uint8_t *sBuf, uint32_t iLength);
	ByteBuffer(uint32_t iLength);
	ByteBuffer(const ByteBuffer &rhs);
	~ByteBuffer(); // Cleanup

	// Load all data from a file
	void Load(const std::string &sPath, const char *sMode = "rb");

	// Save data into a file
	void Save(const std::string &sPath);

	// Re-initialization stuff
	void Reinit(uint32_t iLength);
	void Reinit(const uint8_t *sBuf, uint32_t iLength);
	void Reinit(const ByteBuffer &Buffer);
	void Resize(uint32_t iLength);

	void SetPosition(uint32_t i)		{ m_iPosition = i; }
	void MovePosition(int i)			{ m_iPosition += i; }

	const uint8_t *GetBuffer() const	{ return &m_vBuffer[0]; }
	uint32_t GetPosition() const		{ return m_iPosition; }
	uint32_t GetLength() const			{ return m_vBuffer.size(); }

	/* Check for if we have more data */
	bool HasMoreData() const			{ return m_iPosition < GetLength(); }

	/* Different types of byte readers */
	uint8_t GetByte();
	uint16_t GetShort();
	uint32_t GetTriByte();
	uint32_t GetInt();
	uint64_t GetLong();
	std::string GetString();
	std::string GetLine();

	/* Different types of byte writers */
	void PutByte(const uint8_t &iByte);
	void PutShort(const uint16_t &iShort);
	void PutTriByte(const uint32_t &iTriByte); // what on earth is 3 bytes?
	void PutInt(const uint32_t &iInt);
	void PutLong(const uint64_t &iLong);
	void PutString(const std::string &sString);

	/* Reads and puts raw data into the buffer */
	unsigned GetRaw(uint8_t *buf, unsigned iBytes);
	unsigned PutRaw(const uint8_t *data, unsigned iLen);

private:
	// buffer data
	std::vector<uint8_t> m_vBuffer;

	uint32_t m_iPosition;
};

#endif // BYTE_BUFFER_H

