#include "PixelCompressor.h"

#include <iostream>
#include <chrono>

#include "lz4.h"

PixelCompressor::PixelCompressor()
	: m_bufferSize(0)
{
}

PixelCompressor::~PixelCompressor()
{
}

PixelCompressor::CompressionOutput PixelCompressor::Compress(Texture* apTexture)
{
	CompressionOutput output;

	auto start = std::chrono::steady_clock::now();

	//if (m_buffer.empty())
	//{
	m_bufferSize = apTexture->GetWidth() * apTexture->GetHeight() * sizeof(Leadwerks::Texture::Pixel);

	m_buffer.resize(m_bufferSize);
	m_pixelBuffer.resize(m_bufferSize / sizeof(PixelColor));
	m_dest.resize(m_bufferSize);
	//}

	apTexture->GetPixels(m_buffer.c_str());
	memcpy(&m_pixelBuffer[0], &m_buffer[0], m_bufferSize);

	PixelProtocol protocol;
	//protocol.data = 0; // Left 4 bits: Count | Right 4 bits: Pixel ID
	protocol.count = 0;
	protocol.pixelID = 0;

	std::vector<PixelProtocol> result;

	uint8_t countCap = UINT8_MAX;
	uint8_t pixelID = 0;
	uint8_t count = 0;

	for (const auto& col : m_pixelBuffer)
	{
		uint8_t id = GetPixelID(col);

		if (count == 0)
		{
			count++;
			pixelID = id;
		}
		else
		{
			if (count < countCap && pixelID == id)
			{
				count++;

				if (count == countCap)
				{
					protocol.count = count;
					protocol.pixelID = pixelID;

					result.push_back(protocol);
					count = 0;
				}
			}
			else
			{
				protocol.count = count;
				protocol.pixelID = pixelID;

				result.push_back(protocol);
				count = 1;
				pixelID = id;
			}
		}
	}

	if (count > 0)
	{
		protocol.count = count;
		protocol.pixelID = pixelID;

		result.push_back(protocol);
	}

	//std::cout << "Done encoding! Current size: " << (result.size() * sizeof(PixelProtocol)) << " bytes (" << result.size() << " entries, " << m_registeredPixels.size() << " unique pixels)" << std::endl;

	std::string rawBuffer;
	rawBuffer.resize(result.size() * sizeof(PixelProtocol));
	memcpy(&rawBuffer[0], &result[0], rawBuffer.size());

	m_encodedBuffer = rawBuffer;

	auto newSize = LZ4_compress_default(&rawBuffer[0], &m_dest[0], rawBuffer.size(), m_bufferSize);
	m_dest.resize(newSize);

	auto end = std::chrono::steady_clock::now();
	//std::cout << "Compressed to " << newSize << " bytes (from " << m_buffer.size() << ") in " << (std::chrono::duration_cast<chrono::microseconds>(end - start).count()) << " microseconds" << std::endl;

	output.buffer = m_dest;
	output.protocolCount = result.size();

	return output;
}

void PixelCompressor::Decompress(Texture* apTexture, const std::string& acBuffer, uint32_t aProtocolCount)
{
	if (m_buffer.empty())
	{
		m_buffer.resize(apTexture->GetWidth() * apTexture->GetHeight() * sizeof(Leadwerks::Texture::Pixel));
	}

	m_bufferSize = apTexture->GetWidth() * apTexture->GetHeight() * sizeof(Leadwerks::Texture::Pixel);
	m_dest.resize(m_bufferSize);

	int32_t newSize = LZ4_decompress_safe(&acBuffer[0], &m_buffer[0], acBuffer.size(), m_bufferSize);

	uint32_t writeIt = 0;
	for (int32_t i = 0; i < newSize; i += sizeof(PixelProtocol))
	{
		PixelProtocol protocol;
		memcpy(&protocol, &m_buffer[i], sizeof(PixelProtocol));

		PixelColor color = GetPixelColor(protocol.pixelID);
		color.a = UINT8_MAX;

		for (auto j = 0; j < protocol.count; ++j)
		{
			if (writeIt >= m_bufferSize)
			{
				break;
			}

			memcpy(&m_dest[writeIt], &color, sizeof(PixelColor));
			writeIt += sizeof(PixelColor);
		}
	}

	apTexture->SetPixels(&m_dest[0]);
}

void PixelCompressor::RegisterPixelID(uint8_t aID, uint8_t aRed, uint8_t aGreen, uint8_t aBlue, uint8_t aAlpha)
{
	std::cout << "Pixel color ID " << uint32_t(aID) << " is now R" << uint32_t(aRed) << " G" << uint32_t(aGreen) << " B" << uint32_t(aBlue) << std::endl;

	if (m_registeredPixels.size() == aID)
	{
		PixelColor color;
		color.r = aRed;
		color.g = aGreen;
		color.b = aBlue;
		color.a = aAlpha;
		m_registeredPixels.push_back(color);
		return;
	}

	while (m_registeredPixels.size() < aID + 1)
	{
		PixelColor color;
		color.r = 127;
		color.g = 0;
		color.b = 55;
		color.a = UINT8_MAX;
		m_registeredPixels.push_back(color);

		if (m_registeredPixels.size() == aID + 1)
		{
			m_registeredPixels[aID].r = aRed;
			m_registeredPixels[aID].r = aGreen;
			m_registeredPixels[aID].r = aBlue;
			m_registeredPixels[aID].r = aAlpha;
		}
	};
}

uint8_t PixelCompressor::GetPixelID(const PixelColor& acPixelColor)
{
	auto hash = GetPixelHash(acPixelColor);

	auto pos = m_pixelHashes.find(hash);
	if (pos != std::string::npos)
	{
		return pos;
	}

	// New color!
	std::cout << "Registered color: R" << uint32_t(acPixelColor.r) << " G" << uint32_t(acPixelColor.g) << " B" << uint32_t(acPixelColor.b) << " as ID " << m_registeredPixels.size() << "!" << std::endl;
	m_registeredPixels.push_back(acPixelColor);

	m_pixelHashes.push_back(hash);

	return uint8_t(m_registeredPixels.size() - 1);
}

const std::vector<PixelColor>& PixelCompressor::GetPixelColors() const
{
	return m_registeredPixels;
}

const std::string& PixelCompressor::GetPixelHashes() const
{
	return m_pixelHashes;
}

char PixelCompressor::GetPixelHash(const PixelColor& acColor)
{
	return (
		(acColor.r + acColor.g + acColor.b) ^
		(acColor.r * acColor.g * acColor.b) ^
		(acColor.r ^ (acColor.b + acColor.g)) ^
		(acColor.g ^ (acColor.r + acColor.b)) ^
		(acColor.b ^ (acColor.r + acColor.g)) ^
		acColor.r ^ acColor.g ^ acColor.b
		);
}

PixelColor PixelCompressor::GetPixelColor(uint8_t aPixelID)
{
	if (aPixelID >= m_registeredPixels.size())
	{
		// Default error-color
		return PixelColor{ 127U, 0U, 55U, 255U };
	}
	else
	{
		return m_registeredPixels[aPixelID];
	}
}
