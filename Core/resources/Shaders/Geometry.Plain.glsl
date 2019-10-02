#version 430 core

layout(triangles) in;

layout(triangle_strip, max_vertices = 256) out;

float lineWidth = 0.05;

int connectorResolution = 16;

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

void emitThickLine(vec4 source, vec4 target, vec2 normal)
{
    vec2 offset = normal * lineWidth;

    gl_Position = (source - vec4(offset, 0.0, 0.0));

    EmitVertex();

    gl_Position = (source + vec4(offset, 0.0, 0.0));

    EmitVertex();

    gl_Position = (target - vec4(offset, 0.0, 0.0));

    EmitVertex();

    gl_Position = (target + vec4(offset, 0.0, 0.0));

    EmitVertex();
}

void emitTriangleFan(
    vec4 center,
    vec2 sourceDirection,
    vec2 targetDirection,
    float amplitude,
    int numOfTriangles)
{
    float istep = 1.0 / numOfTriangles;

    for (float i = 0; i <= 1.0 + istep * 2.0; i += istep * 2.0)
    {
        gl_Position = center;

        EmitVertex();

        vec2 n1 = normalize(mix(sourceDirection, targetDirection, i)) * amplitude;

        gl_Position = center + vec4(n1, 0.0, 0.0);

        EmitVertex();

        vec2 n2 = normalize(mix(sourceDirection, targetDirection, i + istep)) * amplitude;

        gl_Position = center + vec4(n2, 0.0, 0.0);

        EmitVertex();
    }
}

void emitRoundConnector(vec4 vertex, vec2 sourceNormal, vec2 targetNormal)
{
    emitTriangleFan(vertex, sourceNormal, targetNormal, lineWidth, connectorResolution);

    emitTriangleFan(vertex, -targetNormal, -sourceNormal, lineWidth, connectorResolution);
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
