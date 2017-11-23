#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

typedef struct
{
    float* vertexData;
    float* normalsData;
    float* texData;
} VertexArray;

VertexArray* vertex_array_new(int facesCount);

void vertex_array_free(VertexArray *vertexArray);

#endif //VERTEX_ARRAY_H
