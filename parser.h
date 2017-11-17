#ifndef MAIN_H
#define MAIN_H

#include "obj.h"

char **tokenize(const char *input, const char *delimiter);
Point3D *make_point3D_from_tokens(char **tokens);
Point2D *make_point2D_from_tokens(char **tokens);
Face *make_face_from_tokens(char **tokens);
Block *make_point_block_from_tokens(char **tokens);
Obj *parse_obj(char* filename);

#endif //MAIN_H
