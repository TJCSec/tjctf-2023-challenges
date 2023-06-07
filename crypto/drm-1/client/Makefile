all: clean drm.pyc drm_exe

.PHONY: clean

drm.pyc:
	python3.8 -m py_compile src/drm.py
	mv src/__pycache__/*.pyc drm.pyc
	rm -rf __pycache__
	
drm_exe:
	g++ src/main.cpp -o drm_exe -lopenal -lm 
	strip drm_exe

clean:
	rm -f drm.pyc
	rm -f drm_exe
	rm -rf src/__pycache__
