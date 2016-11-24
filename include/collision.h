#pragma once

#include "circ.h"
#include "lines.h"
#include "poly.h"
#include "rect.h"

bool engulfs_circ(Circ outer, Circ inner);
bool engulfs_rect(Rect outer, Rect inner);
bool engulfs_rect_circ(Rect outer, Circ inner);
bool engulfs_circ_rect(Circ outer, Rect inner);
bool overlap_circ(Circ a, Circ b);
bool overlap_poly(Polygon a, Polygon b);
bool overlap_rect(Rect a, Rect b);
bool overlap_rect_circ(Rect a, Circ b);
bool overlap_poly_rect(Polygon a, Rect b);
bool overlap_poly_circ(Polygon a, Circ b);
