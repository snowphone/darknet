#include <stdlib.h>
#include <string.h>

//mjo: ��Ʈ���� �������� �˻��Ѵ�.
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

//mjo: ����_epoch_<epoch ����> ������ �̸��� �����.
char* get_output_name(char* dst, char const* src)
{
	strcpy(dst, src);
	int dot = rfind(src, '.');
	dst[dot] = 0;
	strcat(dst, "_labeled");
	strcat(dst, ".avi");
	return dst;
}
