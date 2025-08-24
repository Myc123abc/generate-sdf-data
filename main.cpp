#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <fstream>
#include <filesystem>

std::string program_name;

void print_help()
{
  std::cout << "usage: " << program_name << " <font path> <unicode> <pixel size>" << std::endl;
}

void check(bool b, char const* msg)
{
  if (b)
  {
    std::cout << msg << std::endl;
    exit(1);
  }
}

void store_sdf_bitmap(FT_Bitmap bitmap)
{
  std::ofstream file("output.hpp");
  file << "#pragma once\n"
       << "\n"
       << "constexpr auto Missing_Glyph_Width = " << bitmap.width << ";\n"
       << "constexpr auto Missing_Glyph_Height = " << bitmap.rows << ";\n"
       << "constexpr unsigned char Missing_Glyph_Bitmap[] = {\n";

  for (auto i = 0; i < bitmap.rows; ++i)
  {
    for (auto j = 0; j < bitmap.width; ++j)
    {
      auto v = bitmap.buffer[i * bitmap.pitch + j];
      file << static_cast<int>(v) << ",";
    }
    file << "\n";
  }
  file << "};\n";
}

int main(int argc, char** argv)
{
  program_name = std::filesystem::path(argv[0]).stem().string();
  
  if (argc != 4)
  {
    print_help();
    exit(1);
  }

  auto font_path  = argv[1];
  auto unicode    = strtol(argv[2], nullptr, 0);
  auto pixel_size = atoi(argv[3]);

  FT_Library ft;
  FT_Face face;
  FT_Init_FreeType(&ft);

  check(FT_New_Face(ft, font_path, 0, &face), "invalid font path.");

  FT_Set_Pixel_Sizes(face, 0, pixel_size);

  auto glyph_index = FT_Get_Char_Index(face, unicode);
  check(glyph_index == 0, "not found this unicode");

  FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
  FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF);

  store_sdf_bitmap(face->glyph->bitmap);
  
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}