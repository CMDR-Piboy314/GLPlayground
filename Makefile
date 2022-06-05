all:
	gcc -Wall -O3 -o main.out src/main.c lib/glad/src/glad.c -Ilib/nuklear/ -Ilib/stb -Ilib/glad/include/glad/ -Ilib/glad/include/KHR/ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lrt -lm -lpthread -lcglm -std=c11 && ./main.out

debug:
	gcc -Wall -O0 -o main.out src/main.c lib/glad/src/glad.c -Ilib/nuklear/ -Ilib/stb -Ilib/glad/include/glad/ -Ilib/glad/include/KHR/ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lrt -lm -lpthread -lcglm -std=c11 && ./main.out

compileonly:
	gcc -Wall -O3 -o main.out src/main.c lib/glad/src/glad.c -Ilib/nuklear/ -Ilib/stb -Ilib/glad/include/glad/ -Ilib/glad/include/KHR/ -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lrt -lm -lpthread -lcglm -std=c11