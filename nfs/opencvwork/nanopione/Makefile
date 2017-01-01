onepiece: main.o usart.o sonar.o
	g++  main.o usart.o sonar.o -o onepiece

main.o: main.c usart.h sonar.h
	g++ -c main.c usart.h sonar.h

usart.o: usart.c usart.h
	g++ usart.o -c usart.c usart.h
	
sonar.o: sonar.c sonar.h GPIO.h 
	g++ sonar.o -c sonar.c sonar.h GPIO.h 

clean: 
	rm onepiece mian.o usart.o sonar.o




