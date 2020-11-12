// UserDef.h
//

#pragma once

#include "UserType.h"
#include "UserGdiplus.h"

#if !defined(SAFE_FREE)
#define SAFE_FREE(P) if (P){free(P); P = NULL;}
#endif

#if !defined(SAFE_DELETE)
#define SAFE_DELETE(P) if (P){delete P; P = NULL;}
#endif

#if !defined(SAFE_DELETE_ARY)
#define SAFE_DELETE_ARY(P) if (P){delete [] P; P = NULL;}
#endif

#if !defined(DELETE_EXCEPTION)
#define DELETE_EXCEPTION(e) do { e->Delete(); } while (0)
#endif
