echo Compiling
# g++ -std=c++17 Prac2_brut.cpp -o brut -O3
g++ -std=c++17 Prac5.cpp -o wzo -O3
g++ -std=c++17 Prac5_brut.cpp -o brut -O3
g++ -std=c++17 Prac5_gen.cpp -o gen -O3

for i in {1..20000}
do
	echo generating $i
	./gen $i > test.txt
    ./brut < test.txt > bodp.txt
    ./wzo < test.txt > wodp.txt
    diff -w bodp.txt wodp.txt || cat - 
    # solve task test-generation-add --generator cia8-gentest.cpp --arguments 0$i --name alm0$i.in
	# ./testMaker 0$i > ../tests/cia0$i.in
done