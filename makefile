all:
	(cd comm/src; make)
	(cd utils/src; make)
	(cd ndmp/src; make)
	
clean:
	(cd comm/src; make clean)
	(cd utils/src; make clean)
	(cd ndmp/src; make clean)
	
