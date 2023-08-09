# web-lightning

Web ligthning is work in progress web framework that aims to replace the classical DOM element creation and manipulation
with JS and CSS in order to create web GUI. Instead everything should be rendered on a single canvas. This should in principle
avoid the bloated DOM manipulation, give the ability to do complex effects cheaply on the GPU, and rasterize all the fonts on
the GPU as well (having an advantage when zooming over the CPU rasterization used by browsers).

## Building
    emcmake -B build && cmake --build build -j `nproc`

## Usage

## Progress
### General
- [ ] Create general WebGL rendering interface
- [ ] Enable font rendering
- [ ] Enable general 2D vector rendering on the GPU
- [ ] API for 3D rendering
### Widgets
- [ ] Containers
- [ ] Buttons
- [ ] Sliders
- [ ] Menus
- [ ] Images
- [ ] Canvases
- [ ] Input Fields
