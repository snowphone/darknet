#include <stdlib.h>
#include <string.h>

//mjo: 스트링을 역순으로 검색한다.
int rfind(const char* str, const char key)
{
	size_t length = strlen(str);
	for (int i = length - 1; i >= 0; --i)
	{
		if (str[i] == key)
			return i;
	}
	return -1;
}

//mjo: 파일_epoch_<epoch 숫자> 형식의 이름을 만든다.
char* get_output_name(char* dst, char const* src)
{
	strcpy(dst, src);
	int dot = rfind(src, '.');
	dst[dot] = 0;
	strcat(dst, "_labeled");
	strcat(dst, ".avi");
	return dst;
}
