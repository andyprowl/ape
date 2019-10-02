#version 430 core

layout(triangles) in;

layout(triangle_strip, max_vertices = 256) out;

float lineWidth = 0.05;

int connectorResolution = 16;

vec4 getCameraPosition(int i)
{
    vec4 pos = gl_in[i].gl_Position;

    return pos;
}

vec2 getXYNormal(vec4 source, vec4 target)
{
    vec2 clipSpaceV1 = (source / source.w).xy;

    vec2 clipSpaceV2 = (target / target.w).xy;

    vec2 edge = (clipSpaceV2 - clipSpaceV1);

    return normalize(vec2(edge.y, -edge.x));
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

void emitConnector(vec4 pos, vec2 normal01, vec2 normal12, int connectorResolution)
{
    float istep = 1.0 / connectorResolution;

    for (float i = 0; i <= 1.0 + istep * 2.0; i += istep * 2.0)
    {
        gl_Position = pos;

        EmitVertex();

        vec2 n1 = normalize(mix(normal01, normal12, i)) * lineWidth;

        gl_Position = pos + vec4(n1, 0.0, 0.0);

        EmitVertex();

        vec2 n2 = normalize(mix(normal01, normal12, i + istep)) * lineWidth;

        gl_Position = pos + vec4(n2, 0.0, 0.0);

        EmitVertex();
    }
}

vec4[3] getVertices()
{
    return vec4[3](getCameraPosition(0), getCameraPosition(1), getCameraPosition(2));
}

vec2[3] getNormals(vec4 vertices[3])
{
    return vec2[3](
        getXYNormal(vertices[0], vertices[1]),
        getXYNormal(vertices[1], vertices[2]),
        getXYNormal(vertices[2], vertices[0]));
}

void main()
{

    vec4 positions[] = getVertices();

    vec2 normals[] = getNormals(positions);



/* 
    for (int i = 0; i < gl_in.length(); i++)
    {
        vec4 source = getCameraPosition(i);

        vec4 target = getCameraPosition((i + 1) % gl_in.length());

        vec2 normal = getXYNormal(source, target);

        emitThickLine(source, target, normal);

        vec4 nextTarget = getCameraPosition((i + 2) % gl_in.length());

        vec2 nextNormal = getXYNormal(target, nextTarget);

        emitConnector(pos2, normal, nextNormal, connectorResolution);

        emitConnector(pos2, -nextNormal, -normal, connectorResolution);
    }
    

    vec4 source = getCameraPosition(0);

    vec4 target = getCameraPosition(1);

    vec2 normal = getXYNormal(source, target);

    emitThickLine(source, target, normal);
*/



    emitThickLine(positions[0], positions[1], normals[0]);
    
    emitConnector(positions[1], normals[0], normals[1], connectorResolution);

    emitConnector(positions[1], -normals[1], -normals[0], connectorResolution);


    emitThickLine(positions[1], positions[2], normals[1]);

    emitConnector(positions[2], normals[1], normals[2], connectorResolution);

    emitConnector(positions[2], -normals[2], -normals[1], connectorResolution);

    
    emitThickLine(positions[2], positions[0], normals[2]);

    emitConnector(positions[0], normals[2], normals[0], connectorResolution);

    emitConnector(positions[0], -normals[0], -normals[2], connectorResolution);


    emitThickLine(positions[0], positions[1], normals[0]);



    EndPrimitive();
}
