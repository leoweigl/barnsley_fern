# Barnsley Fern

A C implementation of the [Barnsley Fern](https://en.wikipedia.org/wiki/Barnsley_fern) — a fractal generated with an Iterated Function System (IFS) that produces a shape strikingly similar to a real fern leaf, from just four simple linear transformations.

## How it works

Starting from a single point at the origin, the algorithm repeatedly applies one of four affine transformations to the current point, chosen at random according to fixed probabilities:

| Transformation | Probability | Role |
|---|---|---|
| f1 | 1% | Stem |
| f2 | 85% | Main frond (self-similar body of the fern) |
| f3 | 7% | Left leaflet |
| f4 | 7% | Right leaflet |

Each transformation has the form:

```
x' = a*x + b*y + e
y' = c*x + d*y + f
```

After hundreds of thousands of iterations, the resulting set of points converges onto the fern shape — a consequence of each transformation being a contraction mapping toward its own sub-region of the overall attractor.

## Features

- Weighted random selection of transformations based on cumulative probability
- Double-precision iteration to preserve fractal detail before mapping to pixel space
- Linear coordinate mapping from fern-space (x ∈ [-3, 3], y ∈ [0, 10]) to pixel space, with y-axis flip to match image row order
- Bounds-checked grid writes
- Per-transformation color coding: each of the four transformations is rendered in a distinct color, visually revealing which part of the fern each one is responsible for — including the self-similar miniature copies of the whole fern produced by the leaflet transformations
- PPM image export

## Building & running

Requires a C compiler (tested with GCC/Clang) and CMake.

```
cmake -B build
cmake --build build
./build/barnsley_farn
```

This generates `fern.ppm` in the working directory. Convert it to a more common format if needed, e.g.:

```
ffmpeg -i fern.ppm fern.png
```

## Notes

- Grid resolution and the fern's coordinate bounds are defined near the top of `main.c`.
- The stem transformation only ever produces points in the bottom ~16% of the fern's height range — it does not run the full height of the plant. The vertical "spine" visible for the full height of the fern is actually produced by the main frond transformation, not the stem.
