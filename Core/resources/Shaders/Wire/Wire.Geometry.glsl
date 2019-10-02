#version 430 core

/*
Vertex generation procedure:
 
 - The input is an array of 3 vertices/sides. For each vertex/side:
  - Emit a thick line (produces 4 vertices)
  - Emit a round connector made of 2 triangle fans. For each fan:
   - Emit "numOfConnectorTriangles" triangles, each with 3 vertices.

Number of generated vertices:

 => 3 x (4 + 2 * numOfConnectorTriangles * 3) = 12 + 18 * numOfConnectorTriangles

Examples:

 16 triangles per connector => 12 + 18 * 16 = 300 vertices
 10 triangles per connector => 12 + 18 * 10 = 192 vertices
  8 triangles per connector => 12 + 18 * 8  = 156 vertices
*/

layout(triangles) in;

layout(triangle_strip, max_vertices = 192) out;

uniform float lineWidth = 0.05;

const int numOfConnectorTriangles = 10;

vec4 getClipSpaceVertex(int i)
{
    return gl_in[i].gl_Position;
}

vec2 getClipSpaceNormal(vec4 source, vec4 target)
{
    vec2 clipSpaceV1 = (source / source.w).xy;

    vec2 clipSpaceV2 = (target / target.w).xy;

    vec2 edge = (clipSpaceV2 - clipSpaceV1);

    return normalize(vec2(edge.y, -edge.x));
}

vec4[3] getClipSpaceVertices()
{
    return vec4[3](getClipSpaceVertex(0), getClipSpaceVertex(1), getClipSpaceVertex(2));
}

vec2[3] getClipSpaceNormals(vec4 vertices[3])
{
    return vec2[3](
        getClipSpaceNormal(vertices[0], vertices[1]),
        getClipSpaceNormal(vertices[1], vertices[2]),
        getClipSpaceNormal(vertices[2], vertices[0]));
}

void emitVertex(vec4 position)
{
    gl_Position = position;

    EmitVertex();
}

void emitThickLine(vec4 source, vec4 target, vec2 normal)
{
    vec2 offset = normal * lineWidth;

    emitVertex(source - vec4(offset, 0.0, 0.0));

    emitVertex(source + vec4(offset, 0.0, 0.0));

    emitVertex(target - vec4(offset, 0.0, 0.0));

    emitVertex(target + vec4(offset, 0.0, 0.0));
}

vec2 getTriangleFanVertex(vec2 source, vec2 target, float interpolator, float amplitude)
{
    return normalize(mix(source, target, interpolator)) * amplitude;
}

void emitTriangleFan(vec4 center, vec2 source, vec2 target, float amplitude, int numOfTriangles)
{
    float interpolationStep = (1.0 / numOfTriangles);

    for (int i = 0; i < numOfTriangles; ++i)
    {
        emitVertex(center);

        vec2 v1 = getTriangleFanVertex(source, target, interpolationStep * i, amplitude);

        emitVertex(center + vec4(v1, 0.0, 0.0));

        vec2 v2 = getTriangleFanVertex(source, target, interpolationStep * (i + 1), amplitude);

        emitVertex(center + vec4(v2, 0.0, 0.0));
    }
}

void emitRoundConnector(vec4 vertex, vec2 sourceNormal, vec2 targetNormal)
{
    emitTriangleFan(vertex, sourceNormal, targetNormal, lineWidth, numOfConnectorTriangles);

    emitTriangleFan(vertex, -targetNormal, -sourceNormal, lineWidth, numOfConnectorTriangles);
}

void main()
{
    vec4 vertices[] = getClipSpaceVertices();

    vec2 normals[] = getClipSpaceNormals(vertices);

    for (int i = 0; i < 3; ++i)
    {
        vec4 thisPosition = vertices[i];

        vec4 nextPosition = vertices[(i + 1) % 3];

        vec2 thisNormal = normals[i];

        vec2 nextNormal = normals[(i + 1) % 3];

        emitThickLine(thisPosition, nextPosition, thisNormal);
    
        emitRoundConnector(nextPosition, thisNormal, nextNormal);
    }

    EndPrimitive();
}
