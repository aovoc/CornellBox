# CornellBox

Using monte carlo path tracing method to render the famous CornellBox.

## Features
 - Mesh Rendering
 - Obj Model Import
 - Fast-BVH for Ray/Object Intersection Testing.
 - Diffuse, specular, and emission material types
 
## Compiling and Running
Requires cmake and OpenMP for multithreading.

Compile:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## Obj model material info
  describe in corresponding .mtl file in 'res' folder.
  
  the information contains:
  - material type(mtl_type): REFR, DIFF, SPEC
  - material color(color)
  - emitted factor(Ke)
  - ambient factor(Ka)
  - diffuse factor(Kd)
  - specular factor(Ks)
