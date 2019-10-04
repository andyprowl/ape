#version 430 core

layout(triangles) in;

layout(triangle_strip, max_vertices = 16) out;

uniform float lineWidth = 0.03;

vec4 getClipSpaceVertex(int i)
{
    return gl_in[i].gl_Position;
}

vec4[3] getClipSpaceVertices()
{
    return vec4[3](getClipSpaceVertex(0), getClipSpaceVertex(1), getClipSpaceVertex(2));
}

vec2 getClipSpaceNormal(vec4 source, vec4 target)
{
    vec2 clipSpaceV1 = (source / source.w).xy;

    vec2 clipSpaceV2 = (target / target.w).xy;

    vec2 edge = (clipSpaceV2 - clipSpaceV1);

    return normalize(vec2(edge.y, -edge.x));
}

vec2[3] getClipSpaceNormals(vec4 clipSpaceVertices[3])
{
    return vec2[3](
        getClipSpaceNormal(clipSpaceVertices[0], clipSpaceVertices[1]),
        getClipSpaceNormal(clipSpaceVertices[1], clipSpaceVertices[2]),
        getClipSpaceNormal(clipSpaceVertices[2], clipSpaceVertices[0]));
}

void emitVertex(vec4 position)
{
    gl_Position = position;

    EmitVertex();
}

void emitThickLine(vec4 source, vec4 target, vec2 normal)
{
    vec4 offset = vec4(normal * lineWidth, 0.0, 0.0);

    emitVertex(source - offset);

    emitVertex(source + offset);

    emitVertex(target - offset);

    emitVertex(target + offset);
}

void main()
{
    vec4 vertices[] = getClipSpaceVertices();

    vec2 normals[] = getClipSpaceNormals(vertices);

    for (int i = 0; i < 4; ++i)
    {
        vec4 thisVertex = vertices[i % 3];

        vec4 nextVertex = vertices[(i + 1) % 3];

        vec2 thisNormal = normals[i % 3];

        emitThickLine(thisVertex, nextVertex, thisNormal);
    }
}
