INC= -I../../../libev-4.11 -I./ -I../../include

LIB=-luuid -lpthread -lrt -lcrypto
BASELIB = ../../../lib/libev.pa ../../../lib/libs2sclient.so.3.0.0
#BASELIB = ../../../lib/libev.pa ../../../lib/libs2sclient.so.2.0.2
#BASELIB = ../../../lib/libev.pa ../../../lib/64bit_libs2sclient.so.2.3.0

demo = demo_d
demo1 = demo1_d
demo2 = demo2_d
demo3 = demo3_d
demo4 = demo4_d
demo5 = demo5_d
demo6 = demo6_d
demo7 = demo7_d
demo8 = demo8_d

FLAG=-g -Wno-deprecated -Wl,-rpath,../bin:./
	
all:$(demo) $(demo1) $(demo2) $(demo3) $(demo4) $(demo5) $(demo6) $(demo7) $(demo8)


$(demo):demo.cpp
	g++ $(FLAG) demo.cpp -o ./$@ $(OBJECT)  $(INC)  $(LIB) $(BASELIB)

$(demo1):demo1.cpp
	g++ $(FLAG) demo1.cpp -o ./$@ $(OBJECT)  $(INC)  $(LIB) $(BASELIB)
$(demo2):demo2.cpp
	g++ $(FLAG) demo2.cpp -o ./$@ $(OBJECT)  $(INC)  $(LIB) $(BASELIB)
$(demo3):demo3.cpp
	g++ $(FLAG) demo3.cpp -o ./$@ $(OBJECT)  $(INC)  $(LIB) $(BASELIB)
$(demo4):demo4.cpp
	g++ $(FLAG) demo4.cpp -o ./$@ $(OBJECT)  $(INC)  $(LIB) $(BASELIB)
$(demo5):demo5.cpp
	g++ $(FLAG) demo5.cpp -o ./$@ $(OBJECT)  $(INC)  $(LIB) $(BASELIB)
$(demo6):demo6.cpp
	g++ $(FLAG) demo6.cpp -o ./$@ $(OBJECT)  $(INC)  $(LIB) $(BASELIB)
$(demo7):demo7.cpp
	g++ $(FLAG) demo7.cpp -o ./$@ $(OBJECT)  $(INC)  $(LIB) $(BASELIB)
$(demo8):demo8.cpp
	g++ $(FLAG) demo8.cpp -o ./$@ $(OBJECT)  $(INC)  $(LIB) $(BASELIB)

clean:
	rm *_d








