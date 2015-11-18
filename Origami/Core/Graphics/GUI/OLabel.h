#pragma once

#include <Core/Graphics/2D/OSprite.h>
#include "OFontManager.h"

namespace Origami {

	class OLabel : public OSprite
	{
	private:
		OFont* m_Font;
		std::string m_text;
		float m_x, m_y;
	public:
		OLabel(const std::string& text, float x, float y, uint color);
		OLabel(const std::string& text, float x, float y, OFont* font, uint color);
		OLabel(const std::string& text, float x, float y, const std::string& font, uint color);
		OLabel(const std::string& text, float x, float y, const std::string& font, uint size, uint color);
		void Submit(ORenderer2D* renderer) const override;
		void ValidateFont(const std::string& name, int size = -1);
        
        inline void setText(const std::string& nt) {m_text = nt;}
	};

}