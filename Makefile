setup: FORCE
	meson build

build: FORCE
	ninja -C build -j 4

clean: FORCE
	ninja -C build -j 4 -t clean

docs: FORCE
	cd docs && make html

FORCE: