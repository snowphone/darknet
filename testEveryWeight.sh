for weight in $(ls crackBackup/*.weights)
do
	./darknet detector test crackData/obj.data crackData/yolo-obj.cfg ${weight} crackData/test.txt -dont_show
done
