touch list.txt
dir=$(pwd)
for name in *.jpg
do
	printf "%s/%s\n" "$dir" "$name" >> list.txt
done
