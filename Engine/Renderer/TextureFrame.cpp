#include "pch.h"
#include "TextureFrame.h"
#include "Serialization/Json.h"
#include "Renderer/Texture.h"


namespace nu {
	bool TextureFrame::Load(const std::string& filename, Renderer& renderer) {
		json::document_t document;
		if (!json::Load(filename, document)) return false;

		std::string texture_name;
		JSON_READ_REQ(document, texture_name);
		if (texture_name.empty()) return false;

		m_texture = Resources().Get<Texture>(texture_name, renderer);
		if (!m_texture) {
			std::cerr << "Could not load Texture Frame texture: " << texture_name << std::endl;
			return false;
		}

		JSON_READ_NAME(document, "columns", m_numColumns);
		JSON_READ_NAME(document, "rows", m_numRows);
		JSON_READ_NAME(document, "start_frame", m_startFrame);
		JSON_READ_NAME(document, "total_frames", m_totalFrames);

		if (m_numColumns <= 0) {
			std::cerr << "columns is 0 or less" << std::endl;
		}
		else if (m_numRows <= 0) {
			std::cerr << "rows is 0 or less" << std::endl;
		}

		Vector2 textureSize = m_texture->GetSize();

		m_frameSize = textureSize / Vector2{ static_cast<float>(m_numColumns), static_cast<float>(m_numRows) };

		return true;
	}

	Rect TextureFrame::GetFrameRect(unsigned int frame) {
		if (frame >= GetTotalFrames()) {
			std::cerr << "Texture Frame out of bounds: " << frame << std::endl;
			frame = 0;
		}

		int currentFrame = m_startFrame + frame;
		int column = currentFrame % m_numColumns;
		int rows = currentFrame / m_numColumns;

		float x = column * m_frameSize.x;
		float y = rows * m_frameSize.y;

		return Rect{ x, y, m_frameSize.x, m_frameSize.y };
	}
}