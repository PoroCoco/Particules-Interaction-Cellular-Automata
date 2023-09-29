
main : main.c 
	gcc -g3 -O0 -fsanitize=address,undefined main.c -lraylib -o main -I./raylib-4.5.0/src/ -L./raylib-4.5.0/src -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

clean:
	rm *.o
	rm main