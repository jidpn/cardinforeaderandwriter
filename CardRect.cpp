#include "CardRect.h"

bool compCardRect(const CardRect& a, const CardRect& b) {
	return a.priority < b.priority;
}
