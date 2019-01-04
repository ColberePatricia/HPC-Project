
output: *.cpp *.h
	mpicc *.cpp -o outputHPC

clean:
	rm outputHPC
