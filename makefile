CC=g++
LIBS=-lglfw -lGL -lGLEW
BNAME=Program
OBJDIR=obj
BDIR=build

$(BNAME): Main.o Window.o GlslProcessor.o Shutdown.o Batch.o Camera3D.o KeyProcessing.o ObjLoader.o
	$(CC) $(OBJDIR)/*.o -o $(BDIR)/$(BNAME) $(LIBS)

Main.o: Main.cpp Main.h Window.h GlslProcessor.h Camera3D.h KeyProcessing.h ObjLoader.h Geometry.h
	$(CC) -c $< -o $(OBJDIR)/$@

Window.o: Window.cpp Window.h
	$(CC) -c $< -o $(OBJDIR)/$@

GlslProcessor.o: GlslProcessor.cpp GlslProcessor.h Shutdown.h
	$(CC) -c $< -o $(OBJDIR)/$@

Shutdown.o: Shutdown.cpp Shutdown.h
	$(CC) -c $< -o $(OBJDIR)/$@

Batch.o: Batch.cpp Batch.h Camera3D.h Geometry.h
	$(CC) -c $< -o $(OBJDIR)/$@

Camera3D.o: Camera3D.cpp Camera3D.h
	$(CC) -c $< -o $(OBJDIR)/$@

KeyProcessing.o: KeyProcessing.cpp KeyProcessing.h
	$(CC) -c $< -o $(OBJDIR)/$@

ObjLoader.o: ObjLoader.cpp ObjLoader.h
	$(CC) -c $< -o $(OBJDIR)/$@
