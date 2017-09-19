﻿/*
HitMind with C.ver_2 프로젝트를 시작합니다.
조원: 장민석, 배수한, 신상호, 서상희

해당 프로젝트를 git을 연동하여 공동으로 작업을 하며 각각의 파트가아닌 전부가 참여합니다.

먼저 함수의 선언은 include.h에 선언, 함수의 정의는 각각의 헤더파일(console,socket,mysql,graphic)에 작성합니다.

공동으로 작업:
	Ⅰ. 프로젝트 올리기
		1. 팀 탐색기 - 홈(팀 탐색기 제일 위에 집모양을 클릭) 에 들어가 변경 내용을 클릭합니다.
		2. 자신의 작업한 내용을 커밋 메시지에 입력합니다.
		3. 모두 커밋(I)를 클릭합니다.  혹은 모두커밋 옆에 화살표를 눌러 모두 커밋 후 동기화를 눌러도 됨
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
	Connect_status status;	//MySQL이 연결된 상태를 저장하는 구조체
	MYSQL *cons = 0;		//MySQL선언
	status.arg = cons;		//status에 mysql의 주소를 저장한다
	status.ishappen = false;//연결이 안된 상태
	SDL_Window * Window = NULL;		//SDL 관련
	SDL_Renderer *renderer;			//SDL - 렌더러로 그리기에 씀
	SDL_Event event;				//SDL 이벤트를 저장함
	TTF_Init();		//TTF 초기화
	char version[] = "1.0.1 - Beta";		//현제 버전
	TTF_Font *font = TTF_OpenFont(".\\font\\NanumGothic.ttf", 30);	//나눔고딕 폰트를 불러옴
	if (font == 0)
	{
		printf("error");
		getchar();
	}
	SDL_Init(SDL_INIT_EVERYTHING);						//SDL 초기화
	Window = SDL_CreateWindow("HitMind_2", 300, 200, Display_X, Display_Y, SDL_WINDOW_ALLOW_HIGHDPI);		//해당 해상도로 Window를 생성함
	renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture * WaitBar = LoadTexture(renderer, ".\\maintema\\touch.png");		//계속하려면 클릭해주세요... 이미지
	SDL_Texture * TitleText = LoadTexture(renderer, ".\\mainicon\\MainText.png");	//HitMind 글씨 이미지
	SDL_Texture * TitleImage = LoadTexture(renderer, ".\\mainicon\\main_wallpaper.jpg");
	SDL_Texture * LoadingBar = LoadTexture(renderer, ".\\maintema\\loading.png");
	int quit = false; // while문 조건문에 쓰이는 불 변수
	int loginpopup = false; //로그인 팝업 반복문 상태

	int hanyeong = false; // 한영키상태에 쓰이는 불 변수
	int hangeul = false; // 현재 입력하고 있는 글자가 한글인지 아닌지 식별해주는 불 변수
	int textinput = true; // 글자가 하나 더 입력되었는지 알려주는 불 변수
	int enter = false; // 엔터가 입력되었는지 알려주는 불 변수
	wchar_t wchar[2] = L""; // 한글자 한글자 입력에 쓰이는 배열
	wchar_t wstr[256] = L"";// 지금까지 입력한 텍스트를 저장하는 배열
	char euckr[512] = ""; // euckr 변환에 필요한 배열
	char utf8[768] = ""; // utf8 변환에 필요한 배열
	wchar_t unicode[256] = L""; // unicode 변환에 필요한 배열
	int slice = 0;
	SDL_Color color = { 0,0,0 ,0 };
	_beginthreadex(NULL, 0, (_beginthreadex_proc_type)Thread_MySQL, (void *)&status, 0, 0);
	while (!quit)
	{
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				/*	case SDL_TEXTINPUT: // 채팅 입력 이벤트
						if (hanyeong == true && (event.text.text[0] == -29 || event.text.text[0] + 256 >= 234 && event.text.text[0] + 256 <= 237))// 한글일 경우
						{
							wcscpy(wchar, L"");
							int sum = (event.text.text[0] + 22) * 64 * 64 + (event.text.text[1] + 128) * 64 + event.text.text[2] + 41088;
							wchar[0] = sum;
							wcscat(wstr, wchar);
							if (event.text.text[0] == -29)
								slice = 1;
							else
								slice = 1+!((wchar[0]-0xac00)%28);
						}
						else if (!((event.text.text[0] == 'c' || event.text.text[0] == 'C') && (event.text.text[0] == 'v' || event.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL)) {// 영어 입력 시
							wcscpy(wchar, L"");
							swprintf(wchar, sizeof(wchar) / sizeof(wchar_t), L"%hs", event.text.text);// event.text.text 문자열 그냥 연결시켜버림
							wcscat(wstr, wchar);// 문자열 연결
							hangeul = false;
							slice = 0;
						}
						textinput = true;
						break;
					case SDL_KEYDOWN:
						if (event.key.keysym.sym == SDLK_RETURN) {
							if (hangeul == true && enter == false)
								enter = true;
							else {
								strcpy(utf8, UNICODE2UTF8(wstr, wcslen(wstr)));
								UTF82EUCKR(euckr, 512, utf8, 768);
								euckr[strlen(euckr)] = '\0';
								han2unicode(euckr, unicode);
								if (hannum(unicode, wcslen(unicode)) != hannum(wstr, wcslen(wstr))) {
									strcpy(euckr, "[Error] invalid conversion");
								}
								// euckr을 DB에 올리면 됨
								strcpy(utf8, "");
								wcscpy(wstr, L"");
								wcscpy(unicode, L"");
								strcpy(euckr, "");
								enter = false;
								textinput = true;
							}
						}
						else if (event.key.keysym.sym == SDLK_RALT)
							hanyeong = !(hanyeong);
						else if (event.key.keysym.sym == SDLK_BACKSPACE && wcslen(wstr) > 0)// 키보드 백스페이스고 배열의 길이가 1이상일때
						{
							if (slice == 0) {
								wstr[wcslen(wstr) - 1] = '\0';// 마지막문자를 널문자로 바꿈
								textinput = true;
							}
							else {
								slice--;
							}
						}
						else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {// 컨트롤 모드이고 c를 눌렀다면
							strcpy(utf8, UNICODE2UTF8(wstr, wcslen(wstr)));
							SDL_SetClipboardText(utf8);// 클립보드에 넣음
						}
						else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {// 컨트롤 모드이고 v를 눌렀다면
							wcscat(wstr, UTF82UNICODE(SDL_GetClipboardText(), strlen(SDL_GetClipboardText())));// 클립보드에서 가져옴
							hangeul = false;
							textinput = true;
						}
						else {
							hangeul = true;
							slice++;
						}
						break;*/
			case SDL_QUIT:
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
		RenderTextureXYWH(renderer, TitleText, Display_X / 3, Display_Y / 10, Display_X / 3, Display_Y / 3);
		PutText(renderer, version, 20, (Display_Y / 20) * 19, Display_X / 48, 255, 255, 255);
		int ID_INPUT = 0;
		int PASSWORD_INPUT = 0;
		if (status.ishappen == false)
			RenderTextureXYWH(renderer, LoadingBar, 0, Display_Y / 1.3, Display_X, Display_Y / 15);
		else
			if (PutButtonImage(renderer, WaitBar, WaitBar, 0, Display_Y / 1.3, Display_X, Display_Y / 15, &event))	//계속하려면 클릭하세요 버튼을 누를때
			{
				loginpopup = true;
				int autologin_checking = 0;
				SDL_Texture * autologin_noclick = LoadTexture(renderer, ".\\login\\autologin1.png");
				SDL_Texture * autologin_click = LoadTexture(renderer, ".\\login\\autologin2.png");
				SDL_Texture * login_base = LoadTexture(renderer, ".\\login\\base.png");
				SDL_Texture * login_close_noclick = LoadTexture(renderer, ".\\login\\close1.png");
				SDL_Texture * login_close_click = LoadTexture(renderer, ".\\login\\close2.png");
				SDL_Texture * login_input_id_noclick = LoadTexture(renderer, ".\\login\\ID1.png");
				SDL_Texture * login_input_id_click = LoadTexture(renderer, ".\\login\\ID2.png");
				SDL_Texture * login_button_id_noclick = LoadTexture(renderer, ".\\login\\loginbutton1.png");
				SDL_Texture * login_button_id_click = LoadTexture(renderer, ".\\login\\loginbutton2.png");
				SDL_Texture * autologin_check = LoadTexture(renderer, ".\\login\\loginbuttonup.png");
				SDL_Texture * login_findpassword_noclick = LoadTexture(renderer, ".\\login\\searchPW1.png");
				SDL_Texture * login_findpassword_click = LoadTexture(renderer, ".\\login\\searchPW2.png");
				SDL_Texture * login_signup_noclick = LoadTexture(renderer, ".\\login\\signup1.png");
				SDL_Texture * login_signup_click = LoadTexture(renderer, ".\\login\\signup2.png");

				while (loginpopup)
				{
					if (SDL_PollEvent(&event))
					{
						switch (event.type)
						{
						case SDL_QUIT:
							quit = true;
							break;
						case SDL_WINDOWEVENT:
							switch (event.window.event) {
							case SDL_WINDOWEVENT_CLOSE:// 다수 창에서의 닫기이벤트가 발생할경우
								loginpopup = false;// 팝업창을 닫음
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
						RenderTextureXYWH(renderer, login_base, Display_X / 3, Display_Y / 3, 666, 411);
						if (PutButtonImage(renderer, login_close_noclick, login_close_click, Display_X / 3 + 565, Display_Y / 3, 101, 83, &event))
							loginpopup = false;


						if (ID_INPUT == 0){	//아이디 입력창
							if (PutButtonImage(renderer, login_input_id_noclick, login_input_id_noclick, Display_X / 3 + 22, Display_Y / 3 + 110, 617, 63, &event))
							{
								ID_INPUT = 1;
								PASSWORD_INPUT = 0;
							}
						}
						else
							RenderTextureXYWH(renderer, login_input_id_click, Display_X / 3 + 22, Display_Y / 3 + 110, 617, 63);



						if (PASSWORD_INPUT == 0) {	//비밀번호 입력창
							if (PutButtonImage(renderer, login_input_id_noclick, login_input_id_noclick, Display_X / 3 + 22, Display_Y / 3 + 208, 617, 63, &event))
							{
								ID_INPUT = 0;
								PASSWORD_INPUT = 1;
							}
						}
						else
							RenderTextureXYWH(renderer, login_input_id_click, Display_X / 3 + 22, Display_Y / 3 + 208, 617, 63);

						if (PutButtonImage(renderer, login_button_id_noclick, login_button_id_click, Display_X / 3 + 489, Display_Y / 3+ 273, 147, 71,&event))	//로그인 버튼
						{
							
						}

						if (PutButtonImage(renderer, autologin_noclick, autologin_click, Display_X / 3 + 40, Display_Y / 3 + 280, 194, 47, &event))	//자동로그인 체크박스
						{
							MouseUP_Wait;
							if (autologin_checking == 0)
								autologin_checking = 1;
							else
								autologin_checking = 0;
						}	
						if (autologin_checking == 1)		//로그인 체크
						{
							RenderTextureXYWH(renderer, autologin_check, Display_X / 3 + 23, Display_Y / 3 + 255, 211, 80);
						}
						if (PutButtonImage(renderer, login_signup_noclick, login_signup_click, Display_X / 3 + 85, Display_Y / 3 + 335, 182, 71, &event))
						{

						}
						if (PutButtonImage(renderer, login_findpassword_noclick, login_findpassword_click, Display_X / 3 + 305, Display_Y / 3 + 335, 269, 71, &event))
						{

						}

						SDL_RenderPresent(renderer);
					//	SDL_WaitEvent(&event);
					}
				}
				SDL_DestroyTexture(autologin_check);
				SDL_DestroyTexture(autologin_click);
				SDL_DestroyTexture(autologin_noclick);
				SDL_DestroyTexture(login_base);
				SDL_DestroyTexture(login_button_id_click);
				SDL_DestroyTexture(login_button_id_noclick);
				SDL_DestroyTexture(login_close_click);
				SDL_DestroyTexture(login_close_noclick);
				SDL_DestroyTexture(login_findpassword_click);
				SDL_DestroyTexture(login_findpassword_noclick);
				SDL_DestroyTexture(login_input_id_click);
				SDL_DestroyTexture(login_input_id_noclick);
				SDL_DestroyTexture(login_signup_click);
				SDL_DestroyTexture(login_signup_noclick);


			}
			//	PutButtonImage(renderer, WaitBar, LoadingBar, 0, Display_Y / 1.3, Display_X, Display_Y / 15, &event);
			/*	if (textinput == true) {
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
					SDL_RenderClear(renderer);
					PutText_Unicode(renderer,wstr,0,0,30,color);
					PutText(renderer, version, 20, (Display_Y / 20) * 19, Display_X / 48, 255, 255, 255);
					RenderTextureXYWH(renderer, TitleText, Display_X / 3, Display_Y / 10, Display_X / 3, Display_Y / 3);
					RenderTextureXYWH(renderer, WaitBar, 0, Display_Y / 1.3, Display_X, Display_Y / 15);


					textinput = false;
				}*/

			SDL_RenderPresent(renderer);
			SDL_WaitEvent(&event);
		}
		SDL_DestroyTexture(WaitBar);
		SDL_DestroyTexture(TitleText);
		TTF_CloseFont(font);
		TTF_Quit();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(Window);
		SDL_Quit();
		return 0;
	}

