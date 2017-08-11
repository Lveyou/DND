#include "DNDCanvas_imp.h"

namespace DND
{
	Canvas* Canvas::Create(UINT32 order)
	{
		return new Canvas_imp();
	}

}