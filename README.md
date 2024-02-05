# web-lightning

Web ligthning is work in progress WASM/WegGL web framework that aims to replace the classical DOM element creation and manipulation
with JS and CSS in order to create web GUI. Instead everything should be rendered on a single WebGL canvas. This should in principle
avoid the bloated DOM manipulation, give the ability to do complex effects cheaply on the GPU, and rasterize all the fonts on
the GPU as well (having an advantage when zooming over the CPU rasterization used by browsers).

## Building

For WebAssembly/WebGL builds:

    emcmake cmake -B build && cmake --build build -j `nproc`
    
For Desktop/WebGL

     cmake -B build && cmake --build build -j `nproc`

    
## Usage

## Progress
### General
- [x] General WebGL rendering interface.
- [x] CPU font rendering.
- [ ] GPU font rendering.
- [ ] GPU bezier curve rendering. 
- [x] CPU General 2D vector rendering.
- [ ] GPU 2D vector rendering.
- [ ] API for 3D rendering
### Widgets
- [x] Boxes
- [x] Containers
- [x] Grids
- [x] Buttons
- [x] Progress Indicators 
- [ ] Sliders
- [ ] Menus
- [x] Images
- [ ] Canvases
- [ ] Input Fields
### Animations
- [x] Per-widget per-frame arbitrary property animation chains.
- [ ] Bezier curve particle traversal.


## Showcase
