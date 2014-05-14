#include <stdio.h>
#include <oxygarum.h>

using namespace oxygarum;

int main(void) {
	printf("Hello Candy-World!\n");

	SDLWindow *window = new SDLWindow("CandyCourier", 800,600);

	while(1) {
		
		window->update();
	}

	return 0;
}

