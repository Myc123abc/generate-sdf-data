A simple program to generate sdf bitmap data to a unsigned char array with width and height and its unicode in a specific file.  
It can be easy used in c/cpp project to pre-load some sdf glyphs, such as a missing glyph bitmap.

Genrating way use freetype's sdf rendering bitmap. The pixel value is 0~255, in vulkan, you can use `VK_FORMAT_R8_UNORM` image type for it, 
to use in fragment shader.

The detail about sdf bitmap data look at: [freetype-render-sdf](https://freetype.org/freetype2/docs/reference/ft2-glyph_retrieval.html#ft_render_mode_sdf)

```
usage: generate-sdf-data <font path> <unicode> <pixel size> <output file>
```

##### example
```
generate-sdf-data.exe NotoSansJP-Regular.ttf 0xfffd 32 output.h
```
