#include "renderer.h"

/*
Header File 설명
renderer.h: opengl 초기화, 윈도우 생성, 키보드 입력 처리
shader.h: shader 컴파일 및 링킹, uniform 변수 설정(Set* 함수)
renderfunction.h: quad(정사각형), rectangle(직사각형), cube(정육면체) 렌더링
textureWithClass.h: texture 로딩 및 초기화(normal map, albedo map을 로딩할 때 사용)
*/

int main()
{
	Renderer renderer;
	renderer.Render();
}