#include "framework/mm.h"
#include "vertexarray.h"

VertexArray* vertex_array_new(int facesCount)
{
    VertexArray *vertexArray = (VertexArray *) cg_malloc(sizeof(VertexArray));

    // Asinga la memoria necesaria para guardar todos los vertices. Hay Faces*3*3 vertices.
    vertexArray->vertexData = (float *)cg_malloc(sizeof(float)*facesCount*3*3);
    // Asinga la memoria necesaria para guardar todas los normales. Hay Faces*3*3 normales.
    vertexArray->normalsData = (float *)cg_malloc(sizeof(float)*facesCount*3*3);
    // Asigna la memoria necesaria para guardar las texturas. Hay Faces*3*2 texturas.
    vertexArray->texData = (float *)cg_malloc(sizeof(float)*facesCount*3*2);

    return vertexArray;
}

void vertex_array_free(VertexArray* vertexArray)
{
    cg_free(vertexArray->vertexData);
    cg_free(vertexArray->normalsData);
    cg_free(vertexArray->texData);
    cg_free(vertexArray);
}
