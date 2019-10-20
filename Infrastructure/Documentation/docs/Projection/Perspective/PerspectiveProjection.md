# Perspective projection

**Perspective projection** simulates the visual effect of real-world perspective, due to which
distant objects appear smaller than closer objects of the same size, and parallel lines approach
each other as they extend far from the observer:

<div style="text-align:center">
    <img width="40%" src="Images/Photos/Railroad.jpg" />
</div>

<br>

Perspective projection works by mapping the observer's camera's visible *frustum* into a cube
centered at the origin with sides of length 2 (so that each coordinate ranges from -1 to +1).
The perspective frustum (see the picture below) is defined by four parameters:

1. The vertical *field of view*: an angle which defines the visible portion of the scene above and
below the observer;
2. The *aspect ratio* (AR): the proportion between the width and the height of the surface onto
which the scene is projected. Together with the FOV, the AR specifies the *horizontal* visible
portion of the scene;
3. The *near plane distance* (d<sub>near</sub>): the *minimum* distance from the camera (along the
direction the camera is looking at) that an object must have in order to be visible. The near plane
can be thought of as the plane on which the projected image is formed;
4. The *far plane distance* (d<sub>far</sub>): the *maximum* distance from the camera (along the
direction the camera is looking at) that an object must have in order to be visible.

<div style="text-align:center">
    <img width="40%" src="Images/Diagrams/PerspectiveFrustum.png"/>
</div>

<br>

Points that lie within the perspective frustrum defined above will be projected on the rendering
surface; points that lie outside the frustum will be clipped and not visible.

It is important to stress that in OpenGL's right-handed view system the camera looks down the
**negative** Z axis. Therefore, the Z coordinate of the near and far plane will be equal to
(respectively) **-d<sub>near</sub>** and **-d<sub>far</sub>** rather than to d<sub>near</sub> and
d<sub>far</sub>.

In order to realize the visual effect of real-world perspective, the X and Y coordinate of a
projected point needs to be scaled based on that point's distance from the camera. The picture
below illustrates the geometric relationship between the Y coordinate y<sub>v</sub> of a point
p<sub>v</sub> in view space and the Y coordinate y<sub>near</sub> of the same point projected on
the near plane:

<div style="text-align:center">
    <img width="50%" src="Images/Diagrams/PerspectiveY.png"/>
</div>

<br>

Once again, keep in mind that z<sub>v</sub>, the Z coordinate of point **p<sub>v</sub>** in view
space, is a *negative* value. Triangle similarity allows to determine the value of
y<sub>near</sub>:

<div style="text-align:center">
    <img width="30%" src="Images/Equations/PerspectiveYNear.png"/>
</div>

<br>

These Z-weighed coordinates are then normalized so that the visible coordinates conveniently fall
within the size-independent range [-1, +1]. To perform this normalization we must obtain the
maximum and minimum projected Y coordinates of the visible points. The picture below shows how the
projected visible Y range is defined by the intersection between the near plane and the angular
section defined by the FOV (denoted as 𝛼 in the picture below):

<div style="text-align:center">
    <img width="50%" src="Images/Diagrams/PerspectiveYNormalization.png"/>
</div>

<br>

The y<sub>max</sub> value can be computed from the FOV and the d<sub>near</sub> distance through
simple trigonometry:

<div style="text-align:center">
    <img width="30%" src="Images/Equations/PerspectiveYMax.png"/>
</div>

<br>

Since the vertical view range is symmetrical, y<sub>min</sub> is equal to -y<sub>max</sub>.
Equations (1) and (2) can be combined to obtain the *normalized device coordinate* (NDC)
y<sub>min</sub>:

<div style="text-align:center">
    <img width="65%" src="Images/Equations/PerspectiveYNdc.png"/>
</div>

<br>

The computation of x<sub>near</sub> is similar to the computation of y<sub>near</sub> in equation
(1), yielding the following result:

<div style="text-align:center">
    <img width="30%" src="Images/Equations/PerspectiveXNear.png"/>
</div>

Normalization of the X coordinate slightly differs from normalization of the Y coordinate though,
because the X range is not defined by means of an angular range, but rather through a stretching
factor relative to the Y range - the *aspect ratio*, denoted as 𝜌 in formula (5) below:

<div style="text-align:center">
    <img width="30%" src="Images/Equations/PerspectiveXMax.png"/>
</div>

The value of x<sub>ndc</sub> can then be obtained by normalizing x<sub>near</sub> just like we did
for y<sub>near</sub> in equation (6):

<div style="text-align:center">
    <img width="65%" src="Images/Equations/PerspectiveXNdc.png"/>
</div>

<br>

Affine coordinate system transformations (i.e. combinations of scaling, rotation, and translation
transformations) are realized through 4x4 matrix multiplication in linear algebra.
We should therefore be able to express the transformation that maps x<sub>v</sub> into
x<sub>ndc</sub> and y<sub>v</sub> into y<sub>ndc</sub> as a 4x4 matrix.
Unfortunately, the transformations in equation (3) and (6) are **not linear**, and cannot be
expressed as an affine matrix: there is no way to obtain the right side of (3) and (6) as the dot
product of some row vector **r** and **v<sub>p</sub>** (in particular, there is no way to get
-z<sub>v</sub> as a denominator).

We can, however, get close enough to our initial goal of expressing (3) and (6) as an affine
transformation matrix (ignore the placeholder question marks for the moment):

<div style="text-align:center">
    <img width="22%" src="Images/Equations/PerspectiveXYMatrix.png"/>
</div>

<br>

Pre-multiplication of view space point **p<sub>v</sub>** by matrix P above will give us the right
side of equations (3) and (6) for X and Y coordinates, but *without the division by
-z<sub>v</sub>*:

<div style="text-align:center">
    <img width="30%" src="Images/Equations/PerspectiveXYClipSpace.png"/>
</div>

<br>

Vector **p<sub>c</sub>**=(x<sub>c</sub>, y<sub>c</sub>, z<sub>c</sub>, w<sub>c</sub>) is called the
**clip space** transformation of **p<sub>v</sub>**. Notice that even though the division by
-z<sub>v</sub> has not been performed, the value -z<sub>v</sub> has been stored in the
w<sub>c</sub> component of the output clip space vector.

So how do we get to perform that missing division and convert clip space coordinates into NDC
coordinates?
OpenGL's answer consists in letting the GPU perform it *implicitly*.
When the fragment shader is fed with (interpolated) vertex coordinates emitted by the vertex
shader, the GPU preliminarily performs an implicit division of such coordinates by the **w**
component of the corresponding vertex. This implicit division is called the **perspective divide**.
Compare the x<sub>ndc</sub> and y<sub>ndc</sub> components from equation (9) below with the right
side of equations (3) and (6) to confirm that matrix P from (7) encodes the correct transformation:

<div style="text-align:center">
    <img width="35%" src="Images/Equations/PerspectiveXYNdc.png"/>
</div>

<br>

Encoding the perspective divisor in the w<sub>c</sub> component is not just a convenient trick: it
is the way [homogenous coordinates](https://en.wikipedia.org/wiki/Homogeneous_coordinates) are used
to represent points in projective geometry.

Our matrix (7) is not complete though - and as a consequence, neither the right sides of equations
(8) and (9) are: what we still miss are the matrix coefficients that allow computing the
z<sub>ndc</sub> coordinate.

Although z<sub>ndc</sub> is not used for rendering purposes (after all, points are rendered on a
2D surface) it is required for
[Z-buffering and depth testing](https://en.wikipedia.org/wiki/Z-buffering). Our goal is then to
replace the placeholder question marks in matrix (7) with coefficients that would yield the desired
value of z<sub>ndc</sub> after matrix multiplication and perspective divide are performed. But what
is the desired value of z<sub>ndc</sub> and how can we determine the right values for the missing
coefficients?

First of all, let's start by givin this coefficients the symbolic names 𝛾 and 𝛿:

<div style="text-align:center">
    <img width="22%" src="Images/Equations/PerspectiveZMatrix.png"/>
</div>

<br>

The question mark on the right side of equation (8) can now be replaced by an expression in terms
of 𝛾 and 𝛿 to represent the clip space coordinate z<sub>c</sub>:

<div style="text-align:center">
    <img width="30%" src="Images/Equations/PerspectiveParametricZClipSpace.png"/>
</div>

<br>

The corresponding NDC coordinate resulting from the perspective divide is then given by:

<div style="text-align:center">
    <img width="30%" src="Images/Equations/PerspectiveParametricZNdc.png"/>
</div>

<br>

Our goal is to determine the values of 𝛾 and 𝛿 that cause the visible Z portion of the frustum
(i.e. the view space coordinate range [-d<sub>near</sub>, -d<sub>far</sub>]) to be mapped to the
NDC range [-1,+1]. Doing so amounts to resolving the linear system (13) below, where the notation
z<sub>ndc</sub>(-z<sub>near</sub>) is used to denote the right side of equation (12) when the value
of z<sub>v</sub> is replaced by -z<sub>near</sub> (similarly for -z<sub>far</sub>):

<div style="text-align:center">
    <img width="33%" src="Images/Equations/PerspectiveZNdcLinearSystem.png"/>
</div>

<br>

