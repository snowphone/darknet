/* 빌드 옵션: x64 release (debug 빌드는 cpp namespace를 사용하므로 c에서는 사용 불가하다.)
 * 실행 파일에는 opencv dll과 동영상 코덱 지원을 위한 ffmpeg dll이 필요하다. 
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

