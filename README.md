# RayMarching

Welcome to the beautiful world of sphere tracing.

![Alt text](Readme/FractalsSmooth.gif)

## How does raymarching work in the context of rendering?

In raytracing you shoot a ray into the scene from the camera pov and calculate the intersection point of a ray. In sphere tracing you still shoot a ray from the camera, but you calculate the distance from the current point (initially the camera) to the closest object. Once you know the minimum distance to the closest object you know that you can travel along the ray to that minimum distance and get a new current point. And you repeat these steps until the minimum distance to the closest object is smaller than an arbitrary value (0.001) and you know you have hit something.
