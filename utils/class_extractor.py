import sys
import os

def getLabel():
	with open("obj.names") as f:
		ret = [line.strip() for line in f.readlines()]
	return ret

def extract_class(path, text_list, label):
	for text_file in text_list:
		with open(path + '/' + text_file, 'r') as reader:
			new_text = [line for line in reader.readlines() 
                            if int(line.split()[0]) == int(label)]
		with open(path + '/' + text_file, 'w') as writer:
			writer.writelines(new_text)
	return

def main():
	if len(sys.argv) == 1:
		print("Usage: <program> <extract-class-index> <images location> = ./img")
		print("Make sure you must backup your text file!")
		for i, label in enumerate(getLabel()):
			print("{}: {}".format(i, label))
		return 
	elif len(sys.argv) == 2:
		imgPath = "./img/"
	else:
		imgPath = sys.argv[2]
	label = sys.argv[1]

	text_list = filter(lambda f: f.rfind(".txt") != -1, os.listdir(imgPath))
	extract_class(imgPath, text_list, label)

	return


if __name__ == "__main__":
	main()
