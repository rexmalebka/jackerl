
../priv/jackerl.so: clean
	mkdir -p ../priv
	gcc -fPIC -shared -o ../priv/jackerl.so `pkg-config --cflags --libs jack` jackerl.c  -I $ERL_ROOT/usr/include/
	 
clean: 
	rm -f ../priv/jackerl.so
