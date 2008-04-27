for i in 0 1 2 3  
do
./nachos -m $i -s &
done
echo "Press Ctrl-C to kill all nachos process"
echo ""
./nachos -m 4 -s 
kill -9 $(ps -e|grep nachos|awk '{print $1}')
