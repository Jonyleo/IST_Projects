if [ ! $# -eq 4 ]
then	
	echo "Usage: ./runTests inputdir outputdir maxThreads numberBuckets"
	exit 1
fi

inputdir=$1
outputdir=$2
maxthreads=$3
numbuckets=$4

if [ ! -d $inputdir ] 
then
	echo "Input Directory Non Existant"
	exit 1
fi

if [ ! -d $outputdir ] 
then
	echo "Output Directory Non Existant"
	exit 1
fi


if [ $maxthreads -lt 1 ] 
then
	echo "Invalid Number of Threads"
	exit 1
fi

if [ $numbuckets -lt 1 ] 
then
	echo "Invalid Number of Buckets"
	exit 1
fi


for file in $inputdir/* ; do

	fileEnd=$(basename $file .txt)

	echo InputFile=​ $(basename $file) ​ NumThreads=​ 1
	./tecnicofs-nosync $file $outputdir/$fileEnd-1.txt 1 $numbuckets | tail -1
	echo

	for ((x = 2; x <= maxthreads; x++)) ; do
		echo InputFile=​ $(basename $file) ​ NumThreads=​ $x
		./tecnicofs-mutex $file $outputdir/$fileEnd-$x.txt $x $numbuckets | tail -1
		echo
	done

done
