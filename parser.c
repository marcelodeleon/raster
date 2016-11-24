#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "obj.h"
#include "framework/mm.h"


char **tokenize(const char *input, const char *delimiter) {
    char *str = strdup(input);
    int count = 0;
    int capacity = 10;
    char **result = cg_malloc(capacity * sizeof(*result));
    char *tok = strtok(str, delimiter);
    while (1) {
        if (count >= capacity)
            result = realloc(result, (capacity *= 2) * sizeof(*result));

        result[count++] = tok ? strdup(tok) : tok;

        if (!tok) break;

        tok = strtok(NULL, delimiter);
    }
    free(str);
    return result;
}

Point3D *make_point3D_from_tokens(char **tokens) {
    Point3D *p = (Point3D *) cg_malloc(sizeof(Point3D));
    p->x = atof(*(tokens + 1));
    p->y = atof(*(tokens + 2));
    p->z = atof(*(tokens + 3));
    return p;
}

Point2D *make_point2D_from_tokens(char **tokens) {
    Point2D *p = (Point2D *) cg_malloc(sizeof(Point2D));
    p->x = atof(*(tokens + 1));
    p->y = atof(*(tokens + 2));
    return p;
}

Face *make_face_from_tokens(char **tokens) {
    Face *facePtr = (Face *) cg_malloc(sizeof(Face));

    char **group0 = tokenize(*(tokens + 1), "/");
    char **group1 = tokenize(*(tokens + 2), "/");
    char **group2 = tokenize(*(tokens + 3), "/");

    facePtr->vertexes[0] = atoi(group0[0]);
    facePtr->normals[0] = atoi(group0[1]);
    facePtr->textures[0] = atoi(group0[2]);

    facePtr->vertexes[1] = atoi(group1[0]);
    facePtr->normals[1] = atoi(group1[1]);
    facePtr->textures[1] = atoi(group1[2]);

    facePtr->vertexes[2] = atoi(group2[0]);
    facePtr->normals[2] = atoi(group2[1]);
    facePtr->textures[2] = atoi(group2[2]);

    return facePtr;
}

void print_head(List *l) {
    Block *block = (Block *) l->head;
    if (block != NULL) {
        Point3D *headVertex = (Point3D *) block->data;
        point3D_print(headVertex);
    } else {
        puts("NULL\n");
    }
}

Obj *parse_obj(char* filename) {
    int KB = 1024;
    FILE *pFile;
    char line[1 * KB];

    pFile = fopen(filename, "r");
    if (pFile == NULL) perror("Error opening file");
    else {
        Obj *obj = (Obj *) obj_new();
        while (fgets(line, 1 * KB, pFile) != NULL) {
            char **tokens = tokenize(line, " ");
            if (strcmp(*tokens, "o") == 0) {
                char *objData = *(tokens + 1);
                char **objTokens = tokenize(objData, "/");
                obj->name = *(objTokens + 1);
            } else if (strcmp(*tokens, "v") == 0) {
                Point3D *vertexPtr = make_point3D_from_tokens(tokens);
                list_add(obj->vertexes, vertexPtr);
            } else if (strcmp(*tokens, "vn") == 0) {
                Point3D *normalPtr = make_point3D_from_tokens(tokens);
                list_add(obj->normals, normalPtr);
            } else if (strcmp(*tokens, "vt") == 0) {
                Point2D *texturePtr = make_point2D_from_tokens(tokens);
                list_add(obj->textures, texturePtr);
            } else if (strcmp(*tokens, "f") == 0) {
                Face *facePtr = make_face_from_tokens(tokens);
                list_add(obj->faces, facePtr);
            } else {
                // Ignore comments and blank and invalid lines.
            }
        }
        fclose(pFile);
        return obj;
    }
}
