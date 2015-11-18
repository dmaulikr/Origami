
#include "ResourcesUtils.h"
#include <Core/Utils/OLog.h>
#include <sstream>

#ifdef __APPLE__
#import <Foundation/Foundation.h>
#endif

namespace Origami {

		const std::string RESOURCES_FOLDER = "AppData";

		const char* ResourcesUtils::getResourcePathforFile(const char * file)
		{
#ifdef __APPLE__
			NSBundle* mainBundle = [NSBundle mainBundle];

			if (mainBundle != nil)
			{
				NSString *resourcesFolder = [NSString stringWithCString : RESOURCES_FOLDER.c_str() encoding : NSUTF8StringEncoding];
				NSString *fullPathS = [NSString stringWithFormat : @"%@/%@",resourcesFolder, [NSString stringWithUTF8String : file]];

				const char* fullPath = [[mainBundle pathForResource : fullPathS ofType : nil] UTF8String];
//                OLog(fullPath);
				return fullPath;
			}
#elif __ANDROID__
			std::stringstream ss;
			std::string fol = RESOURCES_FOLDER;
			ss << OWindow::s_ExternalStorage << "/REGULUS/" << OWindow::s_PackageName << "/" << fol << "/" << file;
			std::string s = ss.str();
			return s.c_str();
#endif
            
            std::string outp = RESOURCES_FOLDER + '\\' + file;
			OLog(outp);
			return  file;
		}

		FILE * ResourcesUtils::openFile(const char * file)
		{
			FILE *f;
			f = fopen(getResourcePathforFile(file), "rb");
			return f;
		}

		int ResourcesUtils::fileLength(FILE* f, unsigned char *& buffer, size_t &buffer_length)
		{
			size_t bytes_read;

			if (NULL == f)
			{
				/*	the file doesn't seem to exist (or be open-able)	*/
				//        result_string_pointer = "Can not find DDS file";
				return 0;
			}
			fseek(f, 0, SEEK_END);
			buffer_length = ftell(f);
			fseek(f, 0, SEEK_SET);
			buffer = (unsigned char *)malloc(buffer_length);
			if (NULL == buffer)
			{
				//        result_string_pointer = "malloc failed";
				fclose(f);
				return 0;
			}
			bytes_read = fread((void*)buffer, 1, buffer_length, f);
			fclose(f);
			if (bytes_read < buffer_length)
			{
				/*	huh?	*/
				buffer_length = bytes_read;
			}
			return 1;
		}

}