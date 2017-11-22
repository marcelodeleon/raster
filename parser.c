#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "obj.h"
#include "point2D.h"
#include "point3D.h"
#include "framework/mm.h"


char **tokenize(const char *input, const char *delimiter)
{
    char *str = strdup(input);
    int count = 0;
    int capacity = 10;
    char **result = cg_malloc(capacity * sizeof(*result));
    char *tok = strtok(str, delimiter);
    while (1)
    {
        if (count >= capacity)
            result = realloc(result, (capacity *= 2) * sizeof(*result));

        result[count++] = tok ? strdup(tok) : tok;

        if (!tok) break;

        tok = strtok(NULL, delimiter);
    }
    cg_free(str);
    return result;
}

Point3D *make_point3D_from_tokens(char **tokens)
{
    float x = atof(*(tokens + 1));
    float y = atof(*(tokens + 2));
    float z = atof(*(tokens + 3));

    Point3D *p = point3D_new(x, y, z);
    return p;
}

Point2D *make_point2D_from_tokens(char **tokens)
{
    float x = atof(*(tokens + 1));
    float y = atof(*(tokens + 2));

    Point2D *p = point2D_new(x, y);
    return p;
}

Face *make_face_from_tokens(char **tokens)
{
    char **group0 = tokenize(*(tokens + 1), "/");
    char **group1 = tokenize(*(tokens + 2), "/");
    char **group2 = tokenize(*(tokens + 3), "/");

    int v1 = atoi(group0[0]);
    int n1 = atoi(group0[1]);
    int t1 = atoi(group0[2]);

    int v2 = atoi(group1[0]);
    int n2 = atoi(group1[1]);
    int t2 = atoi(group1[2]);

    int v3 = atoi(group2[0]);
    int n3 = atoi(group2[1]);
    int t3 = atoi(group2[2]);

    Face *facePtr = face_new(v1, n1, t1, v2, n2, t2, v3, n3, t3);

    // Liberar recursos.
    cg_free(group0);
    cg_free(group1);
    cg_free(group2);

    return facePtr;
}

void print_head(List *l)
{
    Block *block = (Block *) l->head;
    if (block != NULL)
    {
        Point3D *headVertex = (Point3D *) block->data;
        point3D_print(headVertex);
    }
    else
    {
        puts("NULL\n");
    }
}

Obj *parse_obj(char* filename)
{
    int KB = 1024;
    FILE *pFile;
    char line[1 * KB];

    pFile = fopen(filename, "r");
    if (pFile == NULL)
    {
        perror("Error opening file");
        return 0;
    }
    else
    {
        Obj *obj = (Obj *) obj_new();
        while (fgets(line, 1 * KB, pFile) != NULL)
        {
            char **tokens = tokenize(line, " ");
            if (strcmp(*tokens, "o") == 0)
            {
                char *objData = *(tokens + 1);
                char **objTokens = tokenize(objData, "/");
                /* obj->name = *(objTokens + 1); */

                cg_free(objData);
                cg_free(objTokens);
            }
            else if (strcmp(*tokens, "v") == 0)
            {
                Point3D *vertexPtr = make_point3D_from_tokens(tokens);
                list_add(obj->vertexes, vertexPtr);
            }
            else if (strcmp(*tokens, "vn") == 0)
            {
                Point3D *normalPtr = make_point3D_from_tokens(tokens);
                list_add(obj->normals, normalPtr);
            }
            else if (strcmp(*tokens, "vt") == 0)
            {
                Point2D *texturePtr = make_point2D_from_tokens(tokens);
                list_add(obj->textures, texturePtr);
            }
            else if (strcmp(*tokens, "f") == 0)
            {
                Face *facePtr = make_face_from_tokens(tokens);
                list_add(obj->faces, facePtr);
            }
            else
            {
                // Ignorar comentarios, lineas en blanco y lineas invalidas.
            }

            // Liberar recursos.
            cg_free(tokens);
        }

        fclose(pFile);
        
        return obj;
    }
}
