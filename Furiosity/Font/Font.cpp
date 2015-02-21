////////////////////////////////////////////////////////////////////////////////
//  Font.cpp
//  Furiosity
//
//  Created by Gerard Meier on 4/15/13.
//  Copyright (c) 2013 Game Oven. All rights reserved.
////////////////////////////////////////////////////////////////////////////////

#include "Font.h"
#include "FileIO.h"


using namespace Furiosity;

FT_Library Font::library = 0;

Font::Font(const std::string& filename)
: Resource(RESOURCE_TYPE_FONT)
{
    // We'll start basic by caching just the raw font blob,
    // at some point we could also cache the FT_Face instance.
    fontdata = Furiosity::ReadFile(filename);
    
    if(fontdata.empty())
    {
        LOG("Font(%s) unable to open font file.", filename.c_str());
        assert(0);
    }
    
    // Update for statistics:
    size = (int)fontdata.length();
    
    FT_Error error;
    
    // Singleton, create one library instance:
    if(Font::library == 0) {
        error = FT_Init_FreeType(&Font::library);
        if(error != 0) {
            LOG("Label::Reload() Unable to initialize the freetype library. The error code is: %i", error);
            assert(false);
        }
    }
    
    error = FT_New_Memory_Face(Font::library,
                               reinterpret_cast<const FT_Byte*>(fontdata.c_str()),
                               fontdata.length(),
                               0,
                               &face);
    
    if ( error == FT_Err_Unknown_File_Format )
    {
        LOG("Label::Reload() The font file could be opened and read, but it "
            "appears that its font format is unsupported. Was freetype "
            "compiled with support for '%s'?", filename.c_str());
    }
    else if ( error )
    {
        LOG("Label::Reload() Another error code means that the font file could "
            "not be opened  or read, or simply that it is broken. "
            "Font: '%s'", filename.c_str());
    }
}

Font::~Font() {
    FT_Done_Face(face);
}