build:
	emcc main.cpp -s EXPORTED_RUNTIME_METHODS=stringToUTF8,UTF8ToString,writeArrayToMemory -s USE_SDL=2 -o ../server/static/index.js 

clean:
	rm -f ../server/static/index.js ../server/static/index.wasm
