all:
	tools/gyp/gyp --depth=. -Goutput_dir=./ -Icommon.gypi --generator-output=out -Dlibrary=static_library -Duv_library=static_library -f make
	make -C out
	cp out/Release/webserver ./

clean:
	rm -rf out
	rm webserver