#pragma once


#define ASSERT(x) if(!(x)) __debugbreak();
#define GlCall(x) GlClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GlClearError();
bool GLLogCall(const char* function, const char* file, int line);
