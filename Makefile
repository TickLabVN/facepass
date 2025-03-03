package:
	rm -rf dist
	mkdir dist
	# libpam_facepass.so, libface_detection.so
	cp build/*/libfacepass_*.so dist/
	cp external/libtorch/lib/*.so dist/