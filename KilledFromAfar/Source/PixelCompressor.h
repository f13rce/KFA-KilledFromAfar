#pragma once

#include <string>
#include <vector>

#include "PixelColor.h"
#include "PixelProtocol.h"

#include "Leadwerks.h"
using namespace Leadwerks;

class PixelCompressor
{
public:
	PixelCompressor();
	~PixelCompressor();

	struct CompressionOutput
	{
		std::string buffer;
		uint32_t protocolCount;
	};
	CompressionOutput Compress(Texture* apTexture);
	void Decompress(Texture* apTexture, const std::string& acBuffer, uint32_t aOriginalProtocolSize);

	uint8_t GetPixelID(const PixelColor& acPixelColor);

	const std::vector<PixelColor>& GetPixelColors() const;
	const std::string& GetPixelHashes() const;

private:
	char GetPixelHash(const PixelColor& acColor);
	PixelColor GetPixelColor(uint8_t aPixelID);

	size_t m_bufferSize;
	std::string m_buffer;
	std::vector<PixelColor> m_pixelBuffer;
	std::string m_dest;

	std::string m_encodedBuffer;

	static constexpr uint8_t s_cMaxPixels = 32;
	std::vector<PixelColor> m_registeredPixels;
	std::string m_pixelHashes;

};