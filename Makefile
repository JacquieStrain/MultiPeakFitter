CC=g++
FLAGS = `root-config --cflags --glibs`

all: main

main: SingleMinFunc.o GlobalMinFunc.o FormOfFunction.o DeclareFitter.o MultiPeakFitter.o PrintResults.o main.o
	${CC} SingleMinFunc.o GlobalMinFunc.o FormOfFunction.o DeclareFitter.o MultiPeakFitter.o PrintResults.o main.o -o main ${FLAGS}
	
SingleMinFunc.o: SingleMinFunc.cc
	${CC} -c SingleMinFunc.cc ${FLAGS}

GlobalMinFunc.o: GlobalMinFunc.cc
	${CC} -c GlobalMinFunc.cc ${FLAGS}

FormOfFunction.o: FormOfFunction.cc
	${CC} -c FormOfFunction.cc ${FLAGS}
	
DeclareFitter.o: DeclareFitter.cc
	${CC} -c DeclareFitter.cc ${FLAGS}

#io.o: io.cc
#	${CC} -c io.cc ${FLAGS}

main.o: main.cc
	${CC} -c main.cc ${FLAGS}

MultiPeakFitter.o: MultiPeakFitter.cc
	${CC} -c MultiPeakFitter.cc ${FLAGS}

PrintResults.o: PrintResults.cc
	${CC} -c PrintResults.cc ${FLAGS}
	
clean:
	rm -rf *o main