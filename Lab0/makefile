all :  lab0

COMMON_DIR = ../common

lab0: lab0.c $(COMMON_DIR)/GL_utilities.c $(COMMON_DIR)/VectorUtils3.c $(COMMON_DIR)/LoadTGA.c $(COMMON_DIR)/loadobj.c $(COMMON_DIR)/Linux/MicroGlut.c
	gcc -Wall -o lab0 -DGL_GLEXT_PROTOTYPES lab0.c $(COMMON_DIR)/GL_utilities.c $(COMMON_DIR)/VectorUtils3.c $(COMMON_DIR)/LoadTGA.c $(COMMON_DIR)/loadobj.c $(COMMON_DIR)/Linux/MicroGlut.c -I$(COMMON_DIR) -I$(COMMON_DIR)/Linux -lm -lGL -lX11

clean :
	rm lab0

