# RayMarching

Welcome to the beautiful world of sphere tracing.

![Alt text](Readme/FractalsSmooth.gif)

## How does raymarching work in the context of rendering?

In raytracing you shoot a ray into the scene from the camera pov and calculate the intersection point of a ray. In sphere tracing you still shoot a ray from the camera, but you calculate the distance from the current point (initially the camera) to the closest object. Once you know the minimum distance to the closest object you know that you can travel along the ray to that minimum distance and get a new current point. And you repeat these steps until the minimum distance to the closest object is smaller than an arbitrary value (0.001) and you know you have hit something.

## My implementations

### Smooth blending

![Alt text](Readme/CubeSphereSmooth.gif)

Here you'll find my first attempt figuring some basic formula's out. It uses the signed distance formula's for a sphere, a cube, and a plane.
It might look like all objects are lit and use normals, but it actually cleverly remaps the value of how far the ray travelled in the scene to a black and white colour.
When the shpere or cube overlaps with the plane it takes whatever is closest. However when they overlap with eachother a smooth minimum of the objects is taken. Resulting in this cool slimey effect.

### Fractals

![Alt text](Readme/FractalsRotation.gif)

### MandelBulb

![Alt text](Readme/MandelBulb.gif)
