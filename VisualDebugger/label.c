#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HAVE_STRUCT_TIMESPEC
#include "pthread.h"

#include "opencv2\core\core_c.h"
#include "opencv2\highgui\highgui_c.h"
#include "opencv2\videoio\videoio_c.h"

#define ESC (27)

int rfind(const char* str, const char key);
char* get_output_name(char* dst, char const* src);

typedef struct object
{
	int left, right, top, bot, obj_id, obj_name;
}object;

typedef struct _data
{
	IplImage* frame;
	CvSize* frame_size;
	CvFont* font;
	object* obj;
} Data;


static void draw_and_release_data(void* ptr)
{
	Data* data = (Data*)ptr;
	IplImage* frame = data->frame;
	CvSize frame_size = *data->frame_size;
	CvFont font = *data->font;
	object* obj = data->obj;

	const static CvScalar black_color = { 0, 0, 0 }, label_color = { 255, 0, 255 };

	CvPoint leftTop = { obj->left, obj->top },
		rightBottom = { obj->right, obj->bot },
		text_pt = { obj->left, obj->top - 12 };
	const float width = frame->height / 1000.f;
	cvRectangle(frame, leftTop, rightBottom, black_color, width, 8, 0);

	char obj_str_name[256];
	ultoa(obj->obj_name, obj_str_name, 10);

	cvPutText(frame, obj_str_name, text_pt, &font, label_color);

	//release data
	free(obj);
	free(data);
}

void label_video(CvCapture* capture, const char* labelPath)
{
	char c;

	char outputName[256] = { 0, };
	get_output_name(outputName, labelPath);
	printf("%s\n", outputName);

	IplImage* frame = cvQueryFrame(capture);
	CvSize frame_size = { .width = frame->width,.height = frame->height };	//cf) c99 standard ^^

	CvFont font;
	const float font_size = frame->height / 500.f;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, font_size, font_size, 0, font_size * 2, 8);

	CvVideoWriter* writer = NULL;
	writer = cvCreateVideoWriter(outputName, CV_FOURCC('D', 'I', 'V', 'X'), 25, frame_size, 1);
	if (!writer)
	{
		puts("Can't make the video file");
		exit(1);
	}
	FILE* fp = fopen(labelPath, "r");

	do 
	{
		//start of frame
		//initialize pthread_t array 
		pthread_t threads[100];
		size_t number_of_boxes = 0;
		while (!feof(fp)) 
		{
			char buf[1024];
			fgets(buf, 1024, fp);

			object obj;
			if (sscanf(buf, "left=%d, right=%d, top=%d, bottom=%d, obj_id=%d, obj=%d\n",
				&obj.left, &obj.right, &obj.top, &obj.bot, &obj.obj_id, &obj.obj_name))
			{
				Data* data = (Data*)malloc(sizeof(Data));
				data->frame = frame;
				data->frame_size = &frame_size;
				data->font = &font;
				data->obj = (object*)malloc(sizeof(object));
				memcpy(data->obj, &obj, sizeof(object));

				int pid = pthread_create(threads + number_of_boxes, NULL, draw_and_release_data, (void*)data);
				++number_of_boxes;
			}
			else
			{
				//classifier 및 maker에 end of frame 표시가 필요함.
				//empty line means the new frame begins
				break;
			}

		}
		//end of frame
		//join each treads
		for (size_t i = 0; i < number_of_boxes; i++)
		{
			pthread_join(threads[i], NULL);
		}

		cvShowImage("Example2", frame);
		cvWriteFrame(writer, frame);

		c = cvWaitKey(33);
		if (c == ESC)
			break;
	} while (frame = cvQueryFrame(capture));

	fclose(fp);
	cvReleaseCapture(&writer);
}

