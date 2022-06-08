#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <math.h>

#include "defs.h"
#include "text.h"
#include "vector.h"
#include "object.h"
#include "target.h"
#include "shapes.h"

int INIT(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("Initialization failed\n");
		return -1;
	}
	if(TTF_Init() != 0){
		printf("TTF Initialization failed");
		return -1;
	}
	if(IMG_Init(IMG_INIT_JPG) < 0){
		printf("SDL Image error: %s\n", IMG_GetError());
	}
	return 0;
}

int main(){
	if(INIT() < 0){
		printf("Initialization failed.\n");
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("window", 
										   SDL_WINDOWPOS_CENTERED,
										   SDL_WINDOWPOS_CENTERED,
										   SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if(!window) printf("failed to create window");

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	initFont(1, "Graph35PixRegular-vj1A.ttf", renderer);

	SDL_Event event;
	int keep_window_open = 1;

	int num_obj = 1;
	struct object** objects = (struct object **)malloc(sizeof(struct object*) * num_obj);
	//objects[0] = new_object(5, 15.0, 100.0f, 200.0f, 10.0f, SCREEN_HEIGHT/2);
	objects[0] = new_object(8E24, 50.0, 0, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	//objects[2] = new_object(8E16, 30.0, 0, 200, SCREEN_WIDTH/2 + 100, 10);
	SDL_Texture * gravity_field = generate_gravity_field(renderer, objects[0]);
	SDL_SetTextureBlendMode(gravity_field, SDL_BLENDMODE_BLEND);
	
	int CurrentTime = 0;
	int LastTimeMove = 0;
	int LastTimeUI = 0;

	SDL_Texture * Earth = IMG_LoadTexture(renderer, "earth.png");
	objects[0]->txtr = Earth;
	SDL_Texture * Harold = IMG_LoadTexture(renderer, "./2i8ld7.jpg");

	double launch_v = 0;
	double launch_a = 0;
	char * buffer = (char*)malloc(sizeof(char) * 64);
	int score = 0;
	int s_counter = 0;
	int limit = 5;

	struct target * target = new_target(positions[0][0], positions[0][1], 10); 
	//update_objects(objects, num_obj);
	int gravity_flag = 0;
	int win_flag = 0;
	while(keep_window_open == 1){
		const Uint8 * state = SDL_GetKeyboardState(NULL);
		CurrentTime = SDL_GetTicks();
		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//draw gravity field
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		if(gravity_flag) SDL_RenderCopy(renderer, gravity_field, NULL, NULL);
		drawThickCircle(renderer, black, objects[0]->p->x, objects[0]->p->y, 45, 45);

		//draw_gravity_field(renderer, objects, num_obj);
		draw_objects(renderer, objects, num_obj);

		//draw score
		sprintf(buffer, "score: %d", score);
		drawText(buffer, renderer, SCREEN_WIDTH - 200, 10, 255, 255, 255, 1);

		//draw target
		drawThickCircle(renderer, white, target->p->x, SCREEN_HEIGHT - target->p->y, target->radius, 3);
		drawThickCircle(renderer, blue, target->p->x, SCREEN_HEIGHT - target->p->y, target->radius - 3, 3);
		drawThickCircle(renderer, red, target->p->x, SCREEN_HEIGHT - target->p->y, target->radius - 5, 3);
		drawThickCircle(renderer, yellow, target->p->x, SCREEN_HEIGHT - target->p->y, target->radius - 8, 3);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		//draw launch area
		SDL_Rect launch = {SCREEN_WIDTH/2 - 20, SCREEN_HEIGHT/2 + 60, 40, 290};
		SDL_RenderDrawRect(renderer, &launch);

		
		if(CurrentTime - LastTimeMove > 10){
			update_objects(objects, &num_obj);
			LastTimeMove = CurrentTime;
			//printf("%f\n", (180.0f/M_PI)*atan(objects[0]->v->y/objects[0]->v->x));
			//printf("(%f, %f)\n", objects[0]->p->x, objects[0]->p->x); 
		}

		int mouse_x, mouse_y;
		SDL_GetMouseState(&mouse_x, &mouse_y);
		
		//Draw reset button
		SDL_Rect button = {SCREEN_WIDTH-125, SCREEN_HEIGHT-55, 105, 35};
		//printf("%d, %d\n", mouse_x, mouse_y);
		if(mouse_x > button.x && mouse_x < button.x + button.w && 
		mouse_y > button.y && mouse_y < button.y + button.h){
			SDL_SetRenderDrawColor(renderer, green.r + 50, green.g + 50, green.b + 50, green.a);
		}
		else{
			SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, green.a);
		}
		SDL_RenderFillRect(renderer, &button);
		drawText("reset", renderer, button.x + 5, button.y + 5, 255, 255, 255, 255);

		//Draw toggle gravity field button
		SDL_Rect button2 = {SCREEN_WIDTH - 270, 50, 265, 35};
		if(mouse_x > button2.x && mouse_x < button2.x + button2.w && 
		mouse_y > button2.y && mouse_y < button2.y + button2.h){
			SDL_SetRenderDrawColor(renderer, green.r + 50, green.g + 50, green.b + 50, green.a);
		}
		else{
			SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, green.a);
		}		
		SDL_RenderFillRect(renderer, &button2);
		drawText("gravity field", renderer, button2.x + 5, button2.y + 5, 255, 255, 255, 255);

		
		if(SDL_PollEvent(&event) > 0){
			switch(event.type){
				case SDL_QUIT:
					keep_window_open = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					if(win_flag){
						win_flag = 0;
						break;
					}
					//printf("%d\n", win_flag);
					//printf("");
					double x = event.button.x;
					double y = event.button.y;
					if(x > launch.x && x < launch.x + launch.w && y > launch.y && y < launch.y + launch.h){
						num_obj++;	
						objects = (struct object **)realloc(objects, sizeof(struct object *) * num_obj);
						objects[num_obj - 1] = new_object(5, 15.0, 
								cos(launch_a * (M_PI/180)) * launch_v * 1000, 
								sin(launch_a * (M_PI/180)) * launch_v * 1000, x, SCREEN_HEIGHT - y);
					}
					if(x > button.x && x < button.x + button.w && y > button.y && y < button.y + button.h){
						if(num_obj > 1){
							while(objects[1] != objects[num_obj - 1]){
								remove_object(objects, 1, &num_obj);
							}
							remove_object(objects, 1, &num_obj);
							score = 0;
						}
						else{
							score = 0;
						}
					}
					if(x > button2.x && x < button2.x + button2.w && y > button2.y && y < button2.y + button2.h){
						gravity_flag = !gravity_flag;
					}		
 			}
		}
		if(CurrentTime - LastTimeUI >= 10){
			if(state[SDL_SCANCODE_UP] && launch_v < 128){
				launch_v += 0.5;
			}
			if(state[SDL_SCANCODE_DOWN] && launch_v > 0){
				launch_v -= 0.5;
			}
			if(state[SDL_SCANCODE_LEFT]){
				launch_a += 1;
				if(launch_a > 360) launch_a -= 360;
			}
			if(state[SDL_SCANCODE_RIGHT]){
				launch_a -= 1;
				if(launch_a < 0) launch_a += 360;
			}
			LastTimeUI = CurrentTime;
		}
		sprintf(buffer, "%d deg.", (int)launch_a);
		drawText(buffer, renderer, 27, SCREEN_HEIGHT - 220, 255, 255, 255, 1);
		
		//Draw angle selector
		struct vector arrowp = {100, 100};
		drawThickCircle(renderer, gray, 100, SCREEN_HEIGHT - 100, 80, 80);
		drawCircle(renderer, white, 100, SCREEN_HEIGHT - 100, 80);
		drawThickCircle(renderer, white, 100, SCREEN_HEIGHT - 100, 15, 15);
		drawArrow(renderer, white, &arrowp, 70, launch_a * (M_PI/180));

		//Draw velocity slider
		SDL_Rect s_back = {30, 100, 40, 350};
		SDL_SetRenderDrawColor(renderer, gray.r, gray.g, gray.b, gray.a);
		SDL_RenderFillRect(renderer, &s_back);
		SDL_Rect s_button = {20, 410 - 310 * (launch_v / 128), 60, 40};
		SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, green.a);
		SDL_RenderFillRect(renderer, &s_button);
		sprintf(buffer, "%0.1f km/s", (launch_v));
		drawText(buffer, renderer, 85, s_button.y + 8, 255, 255, 255, 1);

		//Draw victory screen

		if(s_counter == limit){
			win_flag = 1;
			s_counter = 0;
			limit = limit * 2;
		}
		if(win_flag){
			SDL_Rect window = {100, 100, SCREEN_WIDTH - 200, SCREEN_HEIGHT - 200};
			SDL_RenderCopy(renderer, Harold, NULL, &window);
			SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
			SDL_RenderDrawRect(renderer, &window);
			drawText("Congratulations", renderer, SCREEN_WIDTH/2 - 150, 110, 0, 0, 0, 1);
			drawText("you've hit:", renderer, SCREEN_WIDTH/2 - 130, 150, 0, 0, 0, 1);
			sprintf(buffer, "%d", score);
			drawText(buffer, renderer, SCREEN_WIDTH/2 - 20, 190, 0, 0, 0, 1);
			drawText("targets", renderer, SCREEN_WIDTH/2 - 80, 230, 0, 0, 0, 1);
			drawText("click to continue", renderer, SCREEN_WIDTH/2 - 170, SCREEN_HEIGHT- 140, 0, 0, 0, 1);
		}
		if(check_targets(objects, target, num_obj)){
			score++;
			s_counter++;
		}

		


		
		
		
	}
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
