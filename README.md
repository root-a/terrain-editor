# terrain-editor
Terrain Editor project for Nebula Trifid Game Engine

This project is only part of the terrain editor containing mostly resources and input handling. As the editor is integrated with the engine, the rest of the code is available in my [fork](https://github.com/NebulaTerrain/nebula-trifid/tree/terrain-editor) of the nebula-trifid engine.

Features:
* Brushes implemented as alpha pictures which can be easily added to editor.
* Easy pipeline for adding new types(function) of brushes like standard, smooth or flat.
* Tileable multi-texturing implemented as splat-maps.
* Very natural way of painting textures thanks to implemented additional blending with on-the-fly normalization instead of typical linear interpolation.
