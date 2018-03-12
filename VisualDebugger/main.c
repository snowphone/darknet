/* ���� �ɼ�: x64 release (debug ����� cpp namespace�� ����ϹǷ� c������ ��� �Ұ��ϴ�.)
 * ���� ���Ͽ��� opencv dll�� ������ �ڵ� ������ ���� ffmpeg dll�� �ʿ��ϴ�. 
 */
#include <stdio.h>

#include <Windows.h>

#include "opencv2\core\core_c.h"
#include "opencv2\highgui\highgui_c.h"
#include "opencv2\videoio\videoio_c.h"


void draw_and_release_data(void* ptr);
void label_video(CvCapture* capture, const char* labelPath);


int main(int argc, const char* argv[]) 
{
	if (argc != 3)
	{
		puts("Usage: VisualDebugger.exe <video> <text>");
		return 0;
	}
	puts("press ESC to terminate the program");
	cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);

	CvCapture* capture = cvCreateFileCapture(argv[1]);
	if (!capture)
	{
		perror("couldn't load the video");
		return 1;
	}

	label_video(capture, argv[2]);

	cvReleaseCapture(&capture);
	cvDestroyWindow("Example2");

	return 0;
}

