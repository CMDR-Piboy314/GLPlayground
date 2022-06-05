all:
	gcc -O3 -o main.out src/main.c lib/glad/src/glad.c -Llib/cglm/libcglm.so -Ilib/cglm/headers -Ilib/nuklear -Ilib/stb -Ilib/glad/include/glad -Ilib/glad/include/KHR -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lrt -lm -lpthread -std=c11 && ./main.out

debug:
	gcc -O0 -o main.out src/main.c lib/glad/src/glad.c -Llib/cglm/libcglm.so -Ilib/cglm/headers -Ilib/nuklear -Ilib/stb -Ilib/glad/include/glad -Ilib/glad/include/KHR -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lrt -lm -lpthread -std=c11 && ./main.out

compileonly:
	gcc -O3 -o main.out src/main.c lib/glad/src/glad.c -Llib/cglm/libcglm.so -Ilib/cglm/headers -Ilib/nuklear -Ilib/stb -Ilib/glad/include/glad -Ilib/glad/include/KHR -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lrt -lm -lpthread -std=c11