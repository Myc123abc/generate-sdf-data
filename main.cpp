#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <fstream>
#include <filesystem>

std::string program_name;
std::string output_file;
std::string unicode_str;
uint32_t    pixel_size;

void print_help()
{
  std::cout << "usage: " << program_name << " <font path> <unicode> <pixel size> <output file>" << std::endl;
}

void check(bool b, char const* msg)
{
  if (b)
  {
    std::cout << msg << std::endl;
    exit(1);
  }
}

void store_sdf_bitmap(FT_GlyphSlot glyph_slot)
{
  auto advance_x = glyph_slot->advance.x >> 6;
  auto advance_y = glyph_slot->advance.y >> 6;

  auto bitmap = glyph_slot->bitmap;

  std::ofstream file(output_file);
  file << "#ifndef GLYPH_SDF_BITMAP_H\n"
       << "#define GLYPH_SDF_BITMAP_H\n"
       << "\n"
       << "#define Glyph_Unicode "     << unicode_str             << "\n"
       << "#define Glyph_Width  "      << bitmap.width            << "\n"
       << "#define Glyph_Height "      << bitmap.rows             << "\n"
       << "#define Glyph_Left_Offset " << glyph_slot->bitmap_left << "\n"
       << "#define Glyph_Up_Offset "   << -glyph_slot->bitmap_top << "\n"
       << "#define Glyph_Size "        << pixel_size              << "\n"
       << "#define Glyph_Advance_X "   << advance_x               << "\n"
       << "#define Glyph_Advance_Y "   << advance_y               << "\n"
       << "static const unsigned char Glyph_SDF_Bitmap[] = {\n";

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
  file << "\n#endif";
}

int main(int argc, char** argv)
{
  program_name = std::filesystem::path(argv[0]).stem().string();

  if (argc != 5)
  {
    print_help();
    exit(1);
  }

  auto font_path  = argv[1];
  unicode_str     = argv[2];
  auto unicode    = strtol(argv[2], nullptr, 0);
  pixel_size      = atoi(argv[3]);
  output_file     = argv[4];

  FT_Library ft;
  FT_Face face;
  FT_Init_FreeType(&ft);

  check(FT_New_Face(ft, font_path, 0, &face), "invalid font path.");

  FT_Set_Pixel_Sizes(face, 0, pixel_size);

  auto glyph_index = FT_Get_Char_Index(face, unicode);
  check(glyph_index == 0, "not found this unicode");

  FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
  FT_Render_Glyph(face->glyph, FT_RENDER_MODE_SDF);

  store_sdf_bitmap(face->glyph);
  
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}