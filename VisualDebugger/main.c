/* 빌드 옵션: x64 release (debug 빌드는 cpp namespace를 사용하므로 c에서는 사용 불가하다.)
 * 실행 파일에는 opencv dll과 동영상 코덱 지원을 위한 ffmpeg dll이 필요하다. 
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Windows.h>

#include "opencv2\core\core_c.h"
#include "opencv2\highgui\highgui_c.h"
#include "opencv2\videoio\videoio_c.h"

#define ESC (27)
char* make_output_name(char* dst, const char* src);
void label_video(CvCapture* capture, const char* labelPath);


int main(int argc, const char* argv[]) 
{
	if (argc != 3)
	{
		puts("Usage: VisualDebugger.exe <video> <text>");
		return 0;
	}
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

char* make_output_name(char* dst, const char* src)
{
	size_t index = strlen(src) - 4;		//4 means dot(1) + extention(3)
	memcpy(dst, src, index);		//copy without extension
	strcat(dst, "_label");
	strcpy(dst, src + index);		//copy extension
	return dst;
}

void label_video(CvCapture* capture, const char* labelPath)
{
	char c;


	int frame_counter = 0, debug_counter = 0;


	char outputName[256] = { 0, };
	make_output_name(outputName, labelPath);

	IplImage* frame = cvQueryFrame(capture);
	CvSize frame_size = { .width = frame->width,.height = frame->height };	//cf) c99 standard ^^

	CvFont font;
	const float font_size = frame->height / 500.f;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, font_size, font_size, 0, font_size * 6, 8);

	CvVideoWriter* writer = cvCreateVideoWriter(outputName, CV_FOURCC('D', 'I', 'V', 'X'), 30, frame_size, 1);
	FILE* fp = fopen(labelPath, "r");

	do {
		while (!feof(fp)) 
		{
			char buf[1024];
			fgets(buf, 1024, fp);

			int left, right, top, bot, obj_id, obj_name;
			if(sscanf(buf, "left=%d, right=%d, top=%d, bottom=%d, obj_id=%d, obj=%d\n", 
				&left, &right, &top, &bot, &obj_id, &obj_name)) 
			{
				const static CvScalar black_color = { 0, 0, 0 }, red_color = { 0, 0, 256 };

				CvPoint leftTop = { left, top }, rightBottom = { right, bot }, text_pt = { left, top - 12 };
				const float width = frame->height / 1000.f;
				cvRectangle(frame, leftTop, rightBottom, black_color, width, 8, 0);

				char obj_str_name[256];
				ultoa(obj_name, obj_str_name, 10);

				cvPutText(frame, obj_str_name, text_pt, &font, red_color);
			}
			else
			{
				//go to next frame
				break;
			}

		}


		cvShowImage("Example2", frame);
		cvWriteFrame(writer, frame);
		++frame_counter;

		c = cvWaitKey(33);
		if (c == ESC)
			break;
	} while (frame = cvQueryFrame(capture));

	fclose(fp);
	cvReleaseCapture(&writer);
}