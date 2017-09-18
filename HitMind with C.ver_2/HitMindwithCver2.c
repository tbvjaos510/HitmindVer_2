﻿/*
HitMind with C.ver_2 프로젝트를 시작합니다.
조원: 장민석, 배수한, 신상호, 서상희

해당 프로젝트를 git을 연동하여 공동으로 작업을 하며 각각의 파트가아닌 전부가 참여합니다.

먼저 함수의 선언은 include.h에 선언, 함수의 정의는 각각의 헤더파일(console,socket,mysql,graphic)에 작성합니다.

공동으로 작업:
	Ⅰ. 프로젝트 올리기
		1. 팀 탐색기 - 홈(팀 탐색기 제일 위에 집모양을 클릭) 에 들어가 변경 내용을 클릭합니다. 
		2. 자신의 작업한 내용을 커밋 메시지에 입력합니다.
		3. 모두 커밋(I)를 클릭합니다.
		4. 프로젝트를 저장합니다.
		5. 변경 내용 아래에 "커밋 ********을(를) 로컬에서 만들었습니다.... 라고 내용이 나오면 
		"내용을 공유하여 동기화합니다"에서 동기화를 클릭합니다.
		6. 동기화로 넘어가면 "분기: master" 아래에 동기화를 클릭합니다.
		7. 끝
	Ⅱ. 프로젝트 받기
		1. 팀 탐색기 - 홈(팀 탐색기 제일 위에 집모양을 클릭) 에 들어가 동기화를 클릭합니다.
		2. "분기: master"아래에 페치를 클릭합니다.
		3. "분기: master"아래에 끌어오기를 클릭합니다.
		4. 끝
*/

#include "include.h"

int main(int argc, char *argv[])
{
	
	MYSQL *cons = Mysql_Connect("10.80.162.92");
	SDL_Window * Window = NULL;		//SDL 관련
	SDL_Renderer *renderer;
	SDL_Event event;
	TTF_Init();
	char version[] = "1.0.1 - Beta";
	TTF_Font *font = TTF_OpenFont(".\\font\\NanumGothic.ttf", 30);
	if (font == 0)
	{
		printf("error");
		getchar();
	}
	SDL_Init(SDL_INIT_EVERYTHING);
	Window = SDL_CreateWindow("Orbit or Beat with C", 300, 200, Display_X, Display_Y, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN);
	renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture * WaitBar = LoadTexture(renderer, ".\\maintema\\touch.png");
	if (WaitBar == 0)
		getchar();
	SDL_Texture * TitleImage = LoadTextureEx(renderer, ".\\mainicon\\main_wallpaper.jpg", 255, 255, 255);
	SDL_Texture * TitleText = LoadTexture(renderer, ".\\mainicon\\MainText.png"); 
	
	int quit = false;
	while (!quit)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderClear(renderer);
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_TEXTINPUT: // 채팅 입력 이벤트
				
					break;
				case SDL_KEYDOWN: // 키보드 입력 이벤트
					
					break;
				case SDL_QUIT :
					quit = true;
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_CLOSE:// 다수 창에서의 닫기이벤트가 발생할경우
							quit = true;// quit를 true로 변경
							Sleep(100);
							break;// 브레이크
						case SDL_WINDOWEVENT_ENTER:// 윈도우
							SDL_RaiseWindow(SDL_GetWindowFromID(event.window.windowID));//포커스 이동시킴
							break;
						case SDL_WINDOWEVENT_LEAVE:
						//	drag = false;//마우스가 창에서 나갔으므로 드래그 기능을 중지시킴
							break;
						case SDL_WINDOWEVENT_FOCUS_GAINED:
							break;
					}
			}
		}
		
		RenderTextureXYWH(renderer, TitleImage, 0, 0, Display_X, Display_Y);
		PutText(renderer, version, 20, (Display_Y / 20) * 19, Display_X / 48, 255, 255, 255);
		RenderTextureXYWH(renderer, TitleText, Display_X / 3, Display_Y / 10, Display_X / 3, Display_Y / 3);
		RenderTextureXYWH(renderer, WaitBar, 0, Display_Y / 1.3, Display_X, Display_Y / 15);
		SDL_RenderPresent(renderer);
		
	}
	SDL_DestroyTexture(WaitBar);
	SDL_DestroyTexture(TitleImage);
	SDL_DestroyTexture(TitleText);
	TTF_CloseFont(font);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
	return 0;
}

