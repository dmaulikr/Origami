#include "OFont.h"

#include <Core/Utils/OLog.h>
#include <Core/Utils/ResourcesUtils.h>

namespace Origami {
    
	OFont::OFont(const std::string& name, const std::string& filename, float size)
		: m_Name(name), m_Filename(filename), m_Size(size), m_Scale(maths::vec2(1.0f, 1.0f))
	{
		m_FTAtlas = ftgl::texture_atlas_new(512, 512, 1);
        m_FTFont = ftgl::texture_font_new_from_file(m_FTAtlas, size, ResourcesUtils::getResourcePathforFile(filename.c_str()));
//        if(!m_FTFont) 
//			OErrLog("Failed to load font '" <<  filename.c_str() << "'!");
	}

//	OFont::OFont(const std::string& name, const unsigned char* data, unsigned int datasize, float size)
//		: m_Name(name), m_Filename("NULL"), m_Size(size), m_Scale(maths::vec2(1.0f, 1.0f))
//	{
//		m_FTAtlas = ftgl::texture_atlas_new(512, 512, 1);
//		m_FTFont = ftgl::texture_font_new_from_memory(m_FTAtlas, size, data, datasize);
//		if(!m_FTFont) OErrLog("Failed to load font from data!");
//	}

	void OFont::SetScale(float x, float y)
	{
		m_Scale = maths::vec2(x, y);
	}

}