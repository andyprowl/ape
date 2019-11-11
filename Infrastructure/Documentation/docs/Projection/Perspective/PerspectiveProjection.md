# Perspective projection

This tutorial provides a math-rich explanation of perspective projection and its realization in
the OpenGL rendering pipeline.
The projection matrix derived here is consistent with the one produced by the 
[`perspective()`](https://github.com/inviwo/glm/blob/master/glm/ext/matrix_clip_space.inl#L238-L252)
function of the [GLM library](https://github.com/inviwo/glm) when using the default configuration
settings (right-handed coordinate system, [-1,+1] normalization range).

The tutorial is organized into the following sections:

- [Introduction](#Introduction)
- [Projection transformation](#ProjectionTransformation)
- [Projection matrix](#ProjectionMatrix)
- [Clipping](#Clipping)
- [Perspective divide](#PerspectiveDivide)
- [Depth component](#DepthComponent)
- [Depth precision](#DepthPrecision)
- [Summary](#Summary)
- [Related links](#RelatedLinks)

<hr/>

<a name="Introduction"></a>
### Introduction

**Perspective projection** simulates the visual effect of real-world perspective, due to which
distant objects appear smaller than closer objects of the same size, and parallel lines approach
each other as they extend far away from the observer:

<div style="text-align:center; font-size:4pt">
    <img width="60%" src="Images/Photos/Railroad.jpg" />
    <br/>
    <a href="https://fspy.io/basics/">(source)</a>
</div>

<br>

Perspective projection works by mapping the observer camera's visible *frustum* (see the picture
below) into a cube centered at the origin. The **perspective frustum** is a pyramidal frustum
defined by four parameters:

1. The vertical *field of view*: an angle which defines the visible portion of the scene above and
below the observer;
2. The *aspect ratio* (AR): the proportion between the width and the height of the surface onto
which the scene is projected. Together with the FOV, the AR specifies the *horizontal* visible
portion of the scene;
3. The *near plane distance* (d<sub>near</sub>): the *minimum* distance from the camera (along the
direction the camera is looking at) that an object must have in order to be visible. The near plane
is also the plane on which the projected image is formed;
4. The *far plane distance* (d<sub>far</sub>): the *maximum* distance from the camera (along the
direction the camera is looking at) that an object must have in order to be visible.

<div style="text-align:center">
    <img width="40%" src="Images/Diagrams/PerspectiveFrustum.png"/>
</div>

<br>

Points that lie within the perspective frustrum defined above will be projected on the rendering
surface; points that lie outside the frustum will be clipped and not visible.

It is worth stressing that in OpenGL's right-handed coordinate system the camera looks down the
**negative** Z axis. Therefore, the Z coordinate of the near and far planes will be equal to
(respectively) **-d<sub>near</sub>** and **-d<sub>far</sub>** rather than d<sub>near</sub> and
d<sub>far</sub>.

<a name="ProjectionTransformation"></a>
### Projection transformation

In order to accomplish the visual effect of real-world perspective, the X and Y coordinates of a
projected point need to be scaled based on that point's distance from the camera. The following
diagram illustrates the geometric relationship between the Y coordinate y<sub>v</sub> of a 
view-space point **p<sub>v</sub>**=(x<sub>v</sub>,y<sub>v</sub>,z<sub>v</sub>) and the Y coordinate
y<sub>near</sub> of **p<sub>v</sub>**'s projection *on the near plane*, which is the plane on which
the projected image is formed:

<div style="text-align:center">
    <img width="50%" src="Images/Diagrams/PerspectiveY.png"/>
</div>

<br>

Once again, keep in mind that z<sub>v</sub>, the Z coordinate of view-space point
**p<sub>v</sub>**, is a *negative* value. Triangle similarity allows determining the value of
y<sub>near</sub>:

<div style="text-align:center">
    <img width="30%" src="Images/Equations/PerspectiveYNear.png"/>
</div>

<br>

<a name="PositiveZValues"></a>
The expression on the right side of equation (1) is well-defined if and only if z<sub>v</sub> is
different from 0 - notice that positive z<sub>v</sub> coordinates would be projected by (1) to
*negated* values of y<sub>near</sub>, thus forming an upside-down image.

In order for OpenGL to deal with projected coordinates in a way that is independent of the size of
the rendering surface, the value obtained in (1) is **normalized** in such a way that visible
coordinates get mapped to the range [-1,+1].
To perform this normalization we must obtain the minimum and maximum Y coordinates of *visible*
points projected on the near plane.
These values are given by the intersection of the near plane and the angular section defined by the
FOV (denoted as 𝛼 in the picture below):

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
Equations (1) and (2) can be combined to obtain the **normalized device coordinate** (NDC)
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

<br/>

Normalization of the X coordinate slightly differs from normalization of the Y coordinate though,
because the X range is not defined by means of an angular range, but rather through a stretching
factor relative to the Y range - the *aspect ratio* (AR), denoted as 𝜌 in formula (5) below:

<div style="text-align:center">
    <img width="30%" src="Images/Equations/PerspectiveXMax.png"/>
</div>

<br/>

The value of x<sub>ndc</sub> can then be obtained by normalizing x<sub>near</sub> just like we did
for y<sub>ndc</sub> in equation (6):

<div style="text-align:center">
    <img width="65%" src="Images/Equations/PerspectiveXNdc.png"/>
</div>

<br>

<a name="ProjectionMatrix"></a>
### Projection matrix

The transformations in equations (3) and (6) are **non-linear**, and as such they cannot be applied
through multiplication by some matrix P; in particular, there is no way to obtain -z<sub>v</sub> as
a divisor for any arbitrary vector **p<sub>v</sub>** through multiplication by the *same* matrix P.
This is unfortunate, because representing linear transformations as matrices allows for efficient
and convenient processing.

We can, however, define a matrix P that *almost* represents transformations (3) and (6) - ignore
the placeholder question marks for the time being, they will be replaced later on:

<div style="text-align:center">
    <img width="22%" src="Images/Equations/PerspectiveXYMatrix.png"/>
</div>

<br>

If we multiply a view-space point **p<sub>v</sub>** (with a fourth component set to 1 for reasons
that will become apparent later on) by P we will get the right side of equations (3) and (6) for X
and Y coordinates *except* for the division by -z<sub>v</sub> (something we will fix
[in a later section](#PerspectiveDivide)):

<div style="text-align:center">
    <img width="30%" src="Images/Equations/PerspectiveXYClipSpace.png"/>
</div>

<br>

Notice that even though the division by -z<sub>v</sub> was not performed, the value -z<sub>v</sub>
has been stored in the fourth component (w<sub>c</sub>) of the resulting clip-space vector.
Because of this, and because of equations (3) and (6), the following fundamental equivalence holds: 

<div style="text-align:center">
    <img width="35%" src="Images/Equations/PerspectiveNdcClipSpaceRelation.png"/>
</div>

<br>

Vector **p<sub>c</sub>**=(x<sub>c</sub>, y<sub>c</sub>, z<sub>c</sub>, w<sub>c</sub>) is called the
**clip space** projection of **p<sub>v</sub>**.
The reason why it is called *clip* space is that these coordinates are used by the GPU to perform
*clipping*.

<a name="Clipping"></a>
### Clipping

Clipping is the operation that determines which portions of a (projected) primitive lie within the
visible NDC unit cube and which do not, and excludes the latter from rendering.
The picture below illustrates the concept for the XY plane, but it is important to keep in mind
that clipping is performed against a 3D *volume*:

<div style="text-align:center">
    <img width="60%" src="Images/Diagrams/Clipping.png"/>
</div>

<br>

It is interesting to observe that clipping can actually cause *new* primitives to be generated, as
is the case for the triangle near the bottom-right corner of the clipping area in the picture
above.

Clipping is an optimization that can significantly speed up the rendering pipeline, but it is not
*just* an optimization.
[As previously discussed](#PositiveZValues), the transformation realized by projection matrix (7)
will project vertices with positive z<sub>v</sub> coordinates (i.e. vertices lying *behind* the
camera) on the near plane with *mirrored* XY coordinates.
Since we do not want objects behind the camera to be displayed on the the screen - let alone
mirrored -, we need to
prevent such points from being further processed by the rendering pipeline - which is exactly what
clipping is for.

As we can see from equation (8), the *negated* value of z<sub>v</sub> is stored in w<sub>c</sub>,
the W component of clip-space point **p<sub>c</sub>**.
We want to clip points with a positive z<sub>v</sub>, i.e. points with a *negative* w<sub>c</sub>;
and since the projection transformation is not defined for w<sub>c</sub>=0, only points with a
*positive* w<sub>c</sub> clip-space coordinate shall eventually be rendered.

Because we constructed matrix (7) with the intent of mapping visible points to the origin-centered
unit cube *in NDC coordinates*, and because - as shown in equation (9) NDC coordinates are obtained
by dividing clip space coordinates by their w<sub>c</sub> component, the following conditions must
be satisfied in order for a point *not* to be clipped:

<div style="text-align:center">
    <img width="57%" src="Images/Equations/PerspectiveClipConditions.png"/>
</div>

<br>

We can see from equation (10) that the clipping volume for a clip-space point is a cube whose size
is equal to the distance from the camera of the corresponding view-space point.

<a name="PerspectiveDivide"></a>
### Perspective divide

When we introduced clip-space coordinates in equation (8) we mentioned that even though the
division by -z<sub>v</sub> was not performed when multiplying the view-space point
**p<sub>v</sub>** by the projection matrix (7), the value -z<sub>v</sub> happened to be stored in
the fourth component (w<sub>c</sub>) of the clip-space projected vector.
Of course this is not by accident: the value of the third element of the last row of matrix (7) has
been set to -1 precisely for this purpose.

The rationale behind this is that with OpenGL the GPU performs the division by w<sub>c</sub>
*implicitly* whenever the fragment shader is fed with (interpolated) vertex coordinates emitted by
the vertex shader.[<sup>1</sup>](#HomogenousCoordinates)
This implicit division by w<sub>c</sub> is called the **perspective divide** and it is only
performed for vertices that have not been clipped.

When combined with matrix P from (7), the perspective divide completes the transformation of
view-space points into their NDC projection:

<div style="text-align:center">
    <img width="35%" src="Images/Equations/PerspectiveXYNdc.png"/>
</div>

<br>

It is easy to verify that the X and Y components on right side of equation (11) do indeed coincide
with the right sides of equations (3) and (6).

Storing the perspective divisor in the w<sub>c</sub> component is something more than just a
convenient trick for realizing the perspective divide: it is a well-known and elegant way of
generalizing Cartesian coordinates to 
<a href="https://en.wikipedia.org/wiki/Homogeneous_coordinates"><i>homogenous coordinates</i></a>.

Our projection matrix (7) is still incomplete though, and so are (as a consequence) the right sides
of equations (8) and (11): we are missing the matrix coefficients that allow computing the
z<sub>ndc</sub> coordinate, also known as the *depth* component.

</br>

<a name="DepthComponent"></a>
### Depth component

Although the z<sub>ndc</sub> coordinate is not used for rendering purposes (after all, points are
rendered on a 2D surface) it is required for
[Z-buffering and depth testing](https://en.wikipedia.org/wiki/Z-buffering).
The purpose of depth testing is to correctly handle the rendering of overlapping primitives that
get projected onto the same XY coordinates on the screen.

After the clip-space coordinates emitted by the vertex shader have been converted to NDC
coordinates through the perspective divide, and assuming those coordinates fall within the unit
cubical range of visible coordinates, the GPU needs to determine whether or not the corresponding
fragment ought to be rendered on the screen.
To make this decision the GPU keeps track, for every fragment, of the NDC Z coordinate of the last
vertex that was rendered on that fragment.
A special buffer with the same width and height of the screen - the **depth buffer** - is used to
store these coordinate values.
If the vertex to be rendered on a given fragment has an NDC Z coordinate which is greater than the
value stored in the depth buffer for that fragment (meaning that it is farther from the camera than
the vertex that was last rendered on the same fragment), the vertex is discarded and does not get
rendered.<sup>[1](#EarlyDepthTesting)</sup>

To obtain the NDC Z coordinate of a projected point, the placeholder question marks in our
perspective matrix (7) need to be replaced with coefficients that would yield the desired values
of z<sub>ndc</sub> after matrix multiplication and perspective divide.
But what are the desired values of z<sub>ndc</sub>, and how to determine the coefficients that
yield them?

First of all, let's start by giving these coefficients a symbolic name. We will use 𝛾 and 𝛿:

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

Our goal is to find out the values of 𝛾 and 𝛿 that cause the visible Z portion of the frustum (i.e.
the view-space coordinate range [-d<sub>near</sub>, -d<sub>far</sub>]) to be mapped to the NDC
range [-1,+1]. Doing so amounts to resolving the linear system (15) below, where the notation
z<sub>ndc</sub>(-z<sub>near</sub>) is used to denote the right side of equation (14) with the value
of z<sub>v</sub> replaced by -z<sub>near</sub> (similarly for z<sub>v</sub>=-z<sub>far</sub>):

<div style="text-align:center">
    <img width="33%" src="Images/Equations/PerspectiveZNdcLinearSystem.png"/>
</div>

<br>

To solve system (15) we first simplify the signs in the two equations, then we solve the upper
equation for 𝛾 as a function of 𝛿:

<div style="text-align:center">
    <img width="68%" src="Images/Equations/GammaDeltaDerivation1.png"/>
</div>

<br>

We can then replace the value of 𝛾 from (16) in the lower equation and determine the value of 𝛿 as
in the following derivation:

<div style="text-align:center">
    <img width="73%" src="Images/Equations/GammaDeltaDerivation2.png"/>
</div>

<br>

By replacing the value of 𝛿 from (17) in the upper equation of (16) we can determine the value of
𝛾 as well:

<div style="text-align:center">
    <img width="75%" src="Images/Equations/GammaDeltaDerivation3.png"/>
</div>

<br>

Now that both coefficients 𝛾 and 𝛿 are known we can provide the complete definition of the
parametric projection matrix P introduced in (12):

<div style="text-align:center">
    <img width="32%" src="Images/Equations/PerspectiveProjectionMatrix.png"/>
</div>

<br>

<a name="EarlyDepthTesting"></a>
<div style="font-size:4pt">
<sup>1</sup> On modern GPUs the depth testing can be performed <i>before</i> the fragment shader is
run. This potentially allows skipping time-consuming lightSystem calculations if the GPU can deduce
that the fragment won't eventually be visible due to occlusion by another vertex.
This process is called *early depth testing*.
In order for early depth testing to be possible, the fragment shader shall not modify the NDC Z
coordinate.
</div>

</br>

### Depth precision

It is worth stressing that the mapping of view-space Z coordinates to NDC Z coordinates is *not*
linear: the view-space Z coordinate which lies halfway between -z<sub>near</sub> and 
-z<sub>far</sub> does *not* get mapped to the NDC coordinate 0.
The non-linearity of the mapping stems from the fact that z<sub>ndc</sub> (just like all NDC
coordinates) is *inversely* proportional to z<sub>v</sub>.

The graph below depicts the relationship between a view-space point's distance from the camera
(on the horizontal axis) and its projected z<sub>ndc</sub> (on the vertical axis) when
d<sub>near</sub>=0.3 and d<sub>far</sub>=2.5.

<div style="text-align:center">
    <img width="60%" src="Images/Diagrams/PerspectiveZNdcGraph.png"/>
</div>

<br>

As the diagram shows, the distance range that gets mapped to the z<sub>ndc</sub> range [-1,0] is
significantly narrower than the one that gets mapped to [0,+1] - even more so when the value of
d<sub>far</sub> - d<sub>near</sub> is larger than in the example above, as is usually the case in
video games.
In practice, this means that more floating-point precision is available for the depth value of
objects that are close to the camera than for those that are far away.
This is convenient, because closer objects are generally what the viewer focuses on, and a higher
depth precision helps reducing artifacts and contributes to a more realistic rendering of the most
relevant portions of the scene.

</br>

<a name="Summary"></a>
### Summary

When we multiply a view-space point vector
**p<sub>v</sub>**=(x<sub>v</sub>, y<sub>v</sub>, z<sub>v</sub>, w<sub>v</sub>) by projection matrix
(19) we obtain its *clip space* transformation
**c**=(x<sub>c</sub>, y<sub>c</sub>, z<sub>c</sub>, w<sub>c</sub>):

<div style="text-align:center">
    <img width="43%" src="Images/Equations/PerspectiveClipSpace.png"/>
</div>

<br>

The GPU uses these coordinates to perform clipping. If the vertex coordinates fall within the
clip volume - which is defined as a cube centered at the origin with side length equal to the value
of the clip-space w<sub>c</sub> component, see equation (10) - the perspective divide is performed.
From the perspective divide we obtain the NDC coordinates:

<div style="text-align:center">
    <img width="50%" src="Images/Equations/PerspectiveNdc.png"/>
</div>

<br>

It is not difficult to verify that the z<sub>ndc</sub> component of the resulting vector in
equation (21) above assumes the value -1 when z<sub>v</sub>=-z<sub>near</sub>, and the value +1
when z<sub>v</sub>=-z<sub>far</sub>.

If the Z component of the NDC coordinate obtained from the perspective divide is larger than the
value currently stored in the depth buffer for the target fragment, the vertex is discarded (and if
early depth testing<sup>[1](#EarlyDepthTesting)</sup> is availble, the fragment shader is not run).

After obtaining the NDC coordinates, the rendering pipeline will have to transform them into screen
coordinates.
This transformation consists of a trivial offsetting and scaling operation; however, since it is
not very closely related to the projection process, it will not be discussed here.

<a name="RelatedLinks"></a>
### Related links

- [OpenGL Projection Matrix](http://www.songho.ca/opengl/gl_projectionmatrix.html)
- [The Perspective and Orthographic Projection Matrix](https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix)
- [Homogeneous Coordinates](http://www.songho.ca/math/homogeneous/homogeneous.html)