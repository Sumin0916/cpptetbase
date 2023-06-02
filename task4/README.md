강생 여러분께,

다음 코딩 과제를 드립니다. 기한 내에 제출하기 바랍니다.

- 제출기한:  6월 17일(토) 오후 5시 (기한 초과 후 6시간 이내 제출시 만점의 25% 감점, 12시간 이내 제출시 50% 감점, 18시간 이내 제출시 75% 감점, 18시간 이후 초과시 100% 감점)

- 제출처: 스마트캠퍼스

- 제출물: cpptetbase_v12hw.zip 파일을 이용하고, 그 안에서 Main.cpp 파일만 "학번.zip" 파일로 압축해서 제출할 것

      (참고사항1: v12를 Ubuntu에서 사용하려면, "sudo apt install libncursesw5-dev" 라는 명령으로 ncurses library 설치가 필요함)

      (참고사항2: v12를 MacOS에서 사용하려면, "brew install ncurses" 라는 명령으로 ncurses library 를 설치하고,

      Windows.h 파일 안에 #include <ncursesw/curses.h> ==> #include <curses.h> 로 변경하고,

     Makefile 안에서 -lncursesw ==> -lncurses 로 변경해야 함)

- 요구사항: 

  1) cpptetbase_v12.zip 파일 안에 있는 Main.cpp를 수정하여 사용할 것.
  2) "./Main.exe record" 명령 사용시, key값을 10개씩 그룹지어, 그룹 단위로 keyseq.txt 파일에 저장할 것.
  2-1) key 그룹은 string 객체로 저장하고 이 과정에서 string class의 push_back 함수를 사용할 것
  2-2) string 클래스의 c_str() 함수를 통해서 문자 배열의 시작 주소를 꺼내고 ofstream class의 write 함수를 사용하여 파일에 기록할 것
  2-3) 프로그램 종료 직전에 10개가 채워지지 않은 key 그룹도 파일에 기록될 수 있도록 조치할 것

  3) "./Main.exe replay" 명령 사용시, keyseq.txt 파일로부터 key값을 10개씩 읽어들여 버퍼링할 것.
  3-1) istream class의 read 함수를 이용하여 key값을 10개씩 읽어들이고, string 객체로 저장할 것
  3-2) getTetrisKey 함수는 버퍼링 여부에 상관 없이 key값을 1개씩 return해야 함 (즉, string 객체를 버퍼링되고 있는 key들이 있으면 그 중에 첫번째 것을 리턴하고, 없으면 3-1을 통해서 다시 읽어들이고, 그 중에 첫번째 것을 리턴함)
  3-3) keyseq.txt 파일의 모든 키가 읽혀지고 replay에 사용되어야 함


이상입니다.

문의사항은 언제든지 알려주기 바랍니다.

수고하세요.

김강희 드림