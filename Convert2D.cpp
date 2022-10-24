#include "Convert2D.h"

Vector2 Convert2D(Vector3 pos)
{
	Vector2 convertPos;
	//3DÀ•W‚ğ2D‚É•ÏŠ·
	convertPos.x = (pos.x + 32) * 20;
	convertPos.y = (pos.y - 18) * -20;

    return convertPos;
}
