![](https://github.com/marwac-9/terrain-editor/blob/master/terraineditor.png?raw=true)

# terrain-editor
Terrain Editor project for Nebula Trifid Game Engine

This project is only part of the terrain editor containing mostly resources and input handling. As the editor is integrated with the engine, the rest of the code is available in my [fork](https://github.com/NebulaTerrain/nebula-trifid/tree/terrain-editor) of the nebula-trifid engine.

Features:
* Brushes implemented as alpha pictures which are easily added to editor.
* Easy pipeline for adding new types(function) of brushes like standard, smooth or flat.
* Tileable multi-texturing implemented as splat-maps.
* Very natural way of painting textures thanks to implemented additional blending with on-the-fly normalization instead of typical linear interpolation.

Brush functions:
* Add
* Subtract
* Smooth

Other functionality:
* Set max sculpting height, good for sculpting hard features like clifs or layered canyons
* Smooth entire channel
* One-click set height for entire terrain, good if you want to start sculpting high and dig down
* Load existing height-map or create a new one
* Load stored blend-map
* Load stored terrain
* Set tile-size for each texture channel
* Adjustable brush strength, brush radius and smooth strength
