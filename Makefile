CC=g++
CPPFLAGS = `root-config --cflags`
LDFLAGS = `root-config --cflags --libs`

all: main

main: SingleMinFunc.o GlobalMinFunc.o FormOfFunction.o DeclareFitter.o MultiPeakFitter.o PrintResults.o main.o
	${CC} SingleMinFunc.o GlobalMinFunc.o FormOfFunction.o DeclareFitter.o MultiPeakFitter.o PrintResults.o main.o -o main ${LDFLAGS}
	
SingleMinFunc.o: SingleMinFunc.cc
	${CC} -c SingleMinFunc.cc ${CPPFLAGS}

GlobalMinFunc.o: GlobalMinFunc.cc
	${CC} -c GlobalMinFunc.cc ${CPPFLAGS}

FormOfFunction.o: FormOfFunction.cc
	${CC} -c FormOfFunction.cc ${CPPFLAGS}
	
DeclareFitter.o: DeclareFitter.cc
	${CC} -c DeclareFitter.cc ${CPPFLAGS}

main.o: main.cc
	${CC} -c main.cc ${CPPFLAGS}

MultiPeakFitter.o: MultiPeakFitter.cc
	${CC} -c MultiPeakFitter.cc ${CPPFLAGS}

PrintResults.o: PrintResults.cc
	${CC} -c PrintResults.cc ${CPPFLAGS}
	
clean:
	rm -rf *o main