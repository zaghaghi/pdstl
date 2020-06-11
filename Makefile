setup: FORCE
	meson build

build: FORCE
	ninja -C build -j 4

clean: FORCE
	ninja -C build -j 4 -t clean

docs: FORCE
	cd docs && python3 /Users/hamedzaghaghi/anaconda2/bin/sphinx-build -M html . _build/

FORCE: