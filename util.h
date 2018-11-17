#ifndef _INCLUDE_UTIL_H_
#define _INCLUDE_UTIL_H_


#include "extension.h"


inline Vector CellsToVector(const cell_t *cells)
{
	return Vector(sp_ctof(cells[0]), sp_ctof(cells[1]), sp_ctof(cells[2]));
}

inline void VectorToCells(const Vector& vec, cell_t *cells)
{
	cells[0] = sp_ftoc(vec.x);
	cells[1] = sp_ftoc(vec.y);
	cells[2] = sp_ftoc(vec.z);
}


inline Vector GetVector(cell_t cell, IPluginContext *pCtx)
{
	cell_t *cells;
	pCtx->LocalToPhysAddr(cell, &cells);
	
	return Vector(sp_ctof(cells[0]), sp_ctof(cells[1]), sp_ctof(cells[2]));
}

inline void SetVector(cell_t cell, IPluginContext *pCtx, const Vector& vec)
{
	cell_t *cells;
	pCtx->LocalToPhysAddr(cell, &cells);
	
	cells[0] = sp_ftoc(vec.x);
	cells[1] = sp_ftoc(vec.y);
	cells[2] = sp_ftoc(vec.z);
}

#endif // _INCLUDE_UTIL_H_
