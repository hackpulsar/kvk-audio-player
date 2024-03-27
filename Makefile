all: dirs libs app

dirs:
	mkdir -p bin

libs:
	cd extern/openal-soft && cmake -B./lib && cd lib && make
	cd extern/fmt && cmake -B./lib && cd lib && make

app:
	cmake -B./bin -DCMAKE_BUILD_TYPE=Release && cd bin && make
