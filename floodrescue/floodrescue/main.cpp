#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace sf;
using namespace std;

string curusername = "";


struct GameSettings {
	int musicvolume = 50;
	int sfxvolume = 50;
	int location = 1;
};

GameSettings settings;

static string s(double n) {
	stringstream ss;
	ss << n;
	return ss.str();
}


void saveUser(string username, string password) {


	ofstream file("users.txt", ios::app);
	file << username << " " << password << endl;
	file.close();
}

void leaderboardscore(string username, int gamescore) {
	ofstream file("scoring.txt", ios::app);
	file << username << " " << gamescore << endl;
	file.close();
}

bool checkuser(string username, string password) {
	ifstream file("users.txt");
	string u, p;
	while (file >> u >> p) {
		if (u == username && p == password) {
			file.close();
			return true;
		}
	}
	file.close();
	return false;
}

bool userexists(string username) {
	ifstream file("users.txt");
	string u, p;
	while (file >> u >> p) {
		if (u == username) {
			file.close();
			return true;
		}
	}
	file.close();
	return false;
}


bool loginscreen() {

	 Texture loginTexture;
	 loginTexture.loadFromFile("LOGIN.png");
	 Sprite loginbg(loginTexture);

	RenderWindow login(VideoMode(1000, 800), "Rescue River");

	Font font;
	if (!font.loadFromFile("pxfont.ttf")) {
		return false;
	}

	string username = "";
	string password = "";
	bool enterusername = true;
	bool isregister = false;

	Text title("Login", font, 80);
	title.setPosition(80, 80);
	title.setFillColor(Color(255, 189, 89));
	title.setOutlineColor(Color::Black);
	title.setOutlineThickness(8);

	Text userlabel("Username:", font, 30);
	userlabel.setPosition(80, 215);
	userlabel.setOutlineColor(Color::Black);
	userlabel.setOutlineThickness(4);

	Text passlabel("Password:", font, 30);
	passlabel.setPosition(80, 315);
	passlabel.setOutlineColor(Color::Black);
	passlabel.setOutlineThickness(4);

	Text userinput("", font, 30);
	userinput.setPosition(360, 215);
	userinput.setOutlineColor(Color::Black);
	userinput.setOutlineThickness(4);

	Text passinput("", font, 30);
	passinput.setPosition(360, 320);
	passinput.setOutlineColor(Color::Black);
	passinput.setOutlineThickness(4);

	Text message("", font, 30);
	message.setPosition(80, 580);
	message.setFillColor(Color::Yellow);
	message.setOutlineColor(Color::Black);
	message.setOutlineThickness(2);

	RectangleShape userBox(Vector2f(300, 60));
	userBox.setPosition(350, 210);
	userBox.setFillColor(Color::Transparent);
	userBox.setOutlineColor(Color::Green);
	userBox.setOutlineThickness(4);

	RectangleShape passBox(Vector2f(300, 60));
	passBox.setPosition(350, 310);
	passBox.setFillColor(Color::Transparent);
	passBox.setOutlineColor(Color::Black);
	passBox.setOutlineThickness(4);

	while (login.isOpen()) {
		Event e;
		while (login.pollEvent(e)) {
			if (e.type == Event::Closed) {
				login.close();
				return false;
			}

			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Down) {
					enterusername = !enterusername;
					userBox.setOutlineColor(enterusername ? Color::Green : Color::Black);
					passBox.setOutlineColor(enterusername ? Color::Black : Color::Green);
				}

				if (e.key.code == Keyboard::LControl || e.key.code == Keyboard::RControl) {
					isregister = !isregister;
					title.setString(isregister ? "REGISTER" : "LOGIN");
					message.setString("");
				}

				if (e.key.code == Keyboard::Return) {
					if (username.empty() || password.empty()) {
						message.setString("Please fill in all fields!");
						message.setFillColor(Color::Red);
					}
					else if (isregister) {
						if (userexists(username)) {
							message.setString("Username already exists!");
							message.setFillColor(Color::Red);
						}
						else {
							saveUser(username, password);
							message.setString("Registration successful!.");
							message.setFillColor(Color::Green);
							isregister = false;
							title.setString("LOGIN");
						}
					}
					else {
						if (checkuser(username, password)) {

							curusername = username;
							login.close();
							return true;
						}
						else {
							message.setString("Invalid credentials!");
							message.setFillColor(Color::Red);
						}
					}
				}

				if (e.key.code == Keyboard::BackSpace) {
					if (enterusername && !username.empty()) {
						username.pop_back();
					}
					else if (!enterusername && !password.empty()) {
						password.pop_back();
					}
				}
			}

			if (e.type == Event::TextEntered) {
				if (e.text.unicode < 128 && e.text.unicode != 8 && e.text.unicode != 13 && e.text.unicode != 9) {
					if (enterusername && username.length() < 15) {
						username += static_cast<char>(e.text.unicode);
					}
					else if (!enterusername && password.length() < 15) {
						password += static_cast<char>(e.text.unicode);
					}
				}
			}
		}

		userinput.setString(username);

		string passdisplay = "";
		for (int i = 0; i < password.length(); i++) {
			passdisplay += "*";
		}
		passinput.setString(passdisplay);


		login.clear();
		login.draw(loginbg);
		login.draw(title);
		login.draw(userlabel);
		login.draw(passlabel);
		login.draw(userBox);
		login.draw(passBox);
		login.draw(userinput);
		login.draw(passinput);
		login.draw(message);
		login.display();
	}

	return false;
}

void showleaderboard() {
	RenderWindow leaderboard(VideoMode(800, 600), "Leaderboard");
	Font font;
	font.loadFromFile("pxfont.ttf");

	Text title("LEADERBOARD", font, 40);
	title.setPosition(250, 30);
	title.setFillColor(Color::Yellow);

	vector<pair<int, string>> scores;
	ifstream file("scoring.txt");
	string username;
	int gamescore;

	while (file >> username >> gamescore) {
		scores.push_back(make_pair(gamescore, username));
	}
	file.close();

	sort(scores.begin(), scores.end(), greater<pair<int, string>>());


	Text entries[10];
	int ypos = 120;
	int count = 0;

	for (int i = 0; i < min(10, (int)scores.size()); i++) {
		string line = s(i + 1) + ". " + scores[i].second + " - " + s(scores[i].first);
		entries[i].setFont(font);
		entries[i].setString(line);
		entries[i].setCharacterSize(22);
		entries[i].setPosition(250, ypos);

		if (i < 3) {
			entries[i].setFillColor(Color::Green);
		}
		else {
			entries[i].setFillColor(Color::White);
		}

		ypos += 38;
		count++;
	}

	Text noscores("No scores", font, 25);
	noscores.setPosition(280, 250);
	noscores.setFillColor(Color::Red);

	while (leaderboard.isOpen()) {
		Event e;
		while (leaderboard.pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				leaderboard.close();
			}
		}

		leaderboard.clear(Color(20, 20, 40));
		leaderboard.draw(title);

		if (count == 0) {
			leaderboard.draw(noscores);
		}
		else {
			for (int i = 0; i < count; i++) {
				leaderboard.draw(entries[i]);
			}
		}

		leaderboard.display();
	}
}


void optionmenu() {
	RenderWindow options(VideoMode(800, 600), "Options");

	Font font;
	font.loadFromFile("pxfont.ttf");

	Text title("OPTIONS", font, 50);
	title.setPosition(280, 50);
	title.setFillColor(Color::White);

	Text musiclabel("Music Volume: ", font, 25);
	musiclabel.setPosition(150, 180);

	Text sfxlabel("SFX Volume: ", font, 25);
	sfxlabel.setPosition(150, 250);

	Text difflabel("Location: ", font, 25);
	difflabel.setPosition(150, 320);

	Text musicVal(s(settings.musicvolume), font, 25);
	musicVal.setPosition(500, 180);
	musicVal.setFillColor(Color::Yellow);

	Text sfxVal(s(settings.sfxvolume), font, 25);
	sfxVal.setPosition(500, 250);
	sfxVal.setFillColor(Color::Yellow);

	Text diffVal(settings.location == 1 ? "Sundarbans" : "Cox's Bazar", font, 25);
	diffVal.setPosition(500, 320);
	diffVal.setFillColor(Color::Yellow);

	Text instruction("Use UP/DOWN to adjust| LEFT/RIGHT to change | ESC to return", font, 15);
	instruction.setPosition(100, 500);
	instruction.setFillColor(Color::Cyan);

	int selectedoption = 0;

	while (options.isOpen()) {
		Event e;
		while (options.pollEvent(e)) {
			if (e.type == Event::Closed || (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape)) {
				options.close();
			}

			if (e.type == Event::KeyPressed) {
				if (e.key.code == Keyboard::Tab || e.key.code == Keyboard::Down) {
					selectedoption = (selectedoption + 1) % 3;
				}
				if (e.key.code == Keyboard::Up) {
					selectedoption = (selectedoption - 1 + 3) % 3;
				}

				if (e.key.code == Keyboard::Left) {
					if (selectedoption == 0 && settings.musicvolume > 0) settings.musicvolume -= 10;
					if (selectedoption == 1 && settings.sfxvolume > 0) settings.sfxvolume -= 10;
					if (selectedoption == 2 && settings.location > 1) settings.location--;
				}

				if (e.key.code == Keyboard::Right) {
					if (selectedoption == 0 && settings.musicvolume < 100) settings.musicvolume += 10;
					if (selectedoption == 1 && settings.sfxvolume < 100) settings.sfxvolume += 10;
					if (selectedoption == 2 && settings.location < 2) settings.location++;
				}
			}
		}

		musiclabel.setFillColor(selectedoption == 0 ? Color::Green : Color::White);
		sfxlabel.setFillColor(selectedoption == 1 ? Color::Green : Color::White);
		difflabel.setFillColor(selectedoption == 2 ? Color::Green : Color::White);

		musicVal.setString(s(settings.musicvolume));
		sfxVal.setString(s(settings.sfxvolume));
		diffVal.setString(settings.location == 1 ? "Sundarbans" : "Cox's Bazar");

		options.clear(Color(20, 20, 40));
		options.draw(title);
		options.draw(musiclabel);
		options.draw(sfxlabel);
		options.draw(difflabel);
		options.draw(musicVal);
		options.draw(sfxVal);
		options.draw(diffVal);
		options.draw(instruction);
		options.display();
	}
}


struct Obstacle {
	sf::Sprite sprite;
};

bool collides(const sf::Sprite& a, const sf::Sprite& b) {
	return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

int main()
{	
	if (!loginscreen()) {
		return 0;
	}

	b:
	RenderWindow start(VideoMode(1000, 800), "start");

	Texture st;
	st.loadFromFile("stscreen2.png");

	Sprite stscreen(st);


	while (start.isOpen()) {

		Event event;
		while (start.pollEvent(event)) {
			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
				start.close();
			}

			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
				start.close();
				return 0;
			}

			if (Keyboard::isKeyPressed(Keyboard::O)) {
				optionmenu();
			}
			///////
			if (Keyboard::isKeyPressed(Keyboard::L))
			{
				showleaderboard();
			}
		}

		start.clear();
		start.draw(stscreen);
		start.display();

	}

	double live = 3;
	double totalscore = 0;
	double score = 0;

a:

	srand(time(NULL));

	int loc = settings.location;

	int shipmovex = 4, shipmovey = 4;


	Font myfont; 
	Text myscore;
	Text sscore;

	myfont.loadFromFile("pxfont.ttf");

	SoundBuffer g;
	g.loadFromFile("failure.mp3");

	RenderWindow maingame(VideoMode(1000, 800), "maingame");

	if (live == 0) {

		Texture over;
		over.loadFromFile("gameover2.png");
		Sprite gameover(over);

		string scoreText = "Score: " + s(totalscore);
		Text sctext(scoreText, myfont, 40);
	
		sctext.setPosition(65, 30);
		sctext.setFillColor(Color::White);
		sctext.setOutlineColor(Color::Black);
		sctext.setOutlineThickness(3);

		maingame.clear();
		maingame.draw(gameover);
		maingame.draw(sctext);
		maingame.display();

		Sound go;
		go.setBuffer(g);
		go.play();
		leaderboardscore(curusername, (int)totalscore);

		Sleep(3000);

		totalscore = 0;

		goto b;

	}

	Texture h;
	h.loadFromFile("heart.png");

	Sprite heart[3];

	heart[0].setTexture(h);
	heart[0].setPosition(790, 70);

	heart[1].setTexture(h);
	heart[1].setPosition(850, 70);

	heart[2].setTexture(h);
	heart[2].setPosition(910, 70);


	Texture bgtex;
	Texture bgtex2;

	if (settings.location == 1) {
		bgtex.loadFromFile("sundarban.png");
		bgtex2.loadFromFile("sundarban2.png");
	}
	else {
		bgtex.loadFromFile("coxs.png");
		bgtex2.loadFromFile("coxs2.png");
	}
	bgtex.setRepeated(true);

	
	

	Sprite bg1(bgtex);
	Sprite bg2(bgtex2);

	bg1.setPosition(0, 0);
	bg2.setPosition(1000, 0);

	float scrollspeed = 0.5f;


	int manposrand = 0;
	manposrand = rand() % 5 + 1;

	Texture m;
	m.loadFromFile("child.png");

	int totalframe = 4;
	int currentframe = 0;
	float frameduration = 0.2f;
	float timer = 0.0f;

	const int FRAME_WIDTH = m.getSize().x / 4;  
	const int FRAME_HEIGHT = m.getSize().y;

	Sprite man(m);
	man.setTextureRect(IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
	man.setPosition(500, 430);



	Texture sh;
	sh.loadFromFile(settings.location == 1 ? "ship1.png" : "ship2.png");
	Sprite ship(sh);
	ship.setPosition( 20, 480);


	Texture bmb;
	bmb.loadFromFile("bomb2.png");
	Sprite bomb(bmb);

	Music music;
	music.openFromFile("bgm.mp3");
	music.setVolume(settings.musicvolume);
	music.play();
	music.setLoop(true);

	SoundBuffer buffer;
	buffer.loadFromFile("crash.wav");

	SoundBuffer bonus;
	bonus.loadFromFile("point.mp3");

	Sound point;
	point.setBuffer(bonus);
	point.setVolume(settings.sfxvolume);




	Texture logtex, croctex;

	logtex.loadFromFile(settings.location == 1 ? "laga.png" : "barrel.png");
	croctex.loadFromFile(settings.location == 1 ? "crocodile.png" : "shark.png");

	vector <Obstacle> obstacles;
	
	Clock clock;

	float spawntime = 0;
	float spawninterval = 1;

	RectangleShape black;
	black.setSize({ 1000, 800 });
	black.setFillColor(Color(0, 0, 0, 0));
	float cur = 0.f;
	float max = 128.f;

	Texture cloud;
	cloud.loadFromFile("cloud.png");

	Texture rain1, rain2, rain3;
	rain1.loadFromFile("rain1.png");
	rain2.loadFromFile("rain2.png");
	rain2.loadFromFile("rain3.png");

	Sprite rains1(rain1);
	Sprite rains2(rain2);
	Sprite rains3(rain3);
	rains1.setColor(Color(255, 255, 255, 120));
	rains2.setColor(Color(255, 255, 255, 120));
	rains3.setColor(Color(255, 255, 255, 120));

	Sprite rains[3];
	rains[0] = rains1;
	rains[1] = rains2;
	rains[2] = rains3;
	float index = 0.f;

	Sprite clouds(cloud);
	clouds.setColor(Color(255, 255, 255, 0));
	float cloudCur = 0.f;
	float cloudMax = 255.f;






	while (maingame.isOpen()) {

		score += 0.01;

		myscore.setFont(myfont);
		myscore.setString(s(score));

		myscore.setCharacterSize(20);
		myscore.setFillColor(sf::Color::Yellow);
		myscore.setOutlineColor(sf::Color::Black);
		myscore.setOutlineThickness(3);
		myscore.setPosition(215, 70);
		
		
		sscore.setFont(myfont);
		sscore.setString("SCORE: ");

		sscore.setCharacterSize(20);
		sscore.setFillColor(sf::Color::Yellow);
		sscore.setOutlineColor(sf::Color::Black);
		sscore.setOutlineThickness(3);

		sscore.setPosition(30, 70);

		maingame.setFramerateLimit(110);

		Event e;
		while (maingame.pollEvent(e)) {
			if (e.type == Event::Closed) {
				maingame.close();
			}
		}	

		bg1.move(-scrollspeed, 0);
		bg2.move(-scrollspeed, 0);

		if (bg1.getPosition().x <= -1000) {
			bg1.setPosition(bg2.getPosition().x + 1000, 0);
		}

		if (bg2.getPosition().x <= -1000) {
			bg2.setPosition(bg1.getPosition().x + 1000, 0);
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			goto b;
		}

			if (Keyboard::isKeyPressed(Keyboard::Up) && ship.getPosition().y > 280) {
				ship.move(0, -shipmovey);
			}

			if (Keyboard::isKeyPressed(Keyboard::Down) && ship.getPosition().y < 680) {
				ship.move(0, shipmovey);
			}
			if (Keyboard::isKeyPressed(Keyboard::Left) && ship.getPosition().x > 50) {
				ship.move(-shipmovex, 0);
			}
			if (Keyboard::isKeyPressed(Keyboard::Right) && ship.getPosition().x < 1000) {
				ship.move(shipmovex, 0);
			}

			if (score >= 500 && settings.location == 1) {
				if (cur < max) {
					cur += 1.f;
					black.setFillColor(Color(0, 0, 0, cur));
				}
				else if (cur == max) {
					if (cloudCur < cloudMax) {
						cloudCur += 2.f;
						if (cloudCur >= 255.f) {
							cloudCur = 255.f;
						}
						clouds.setColor(Color(255, 255, 255, cloudCur));
					}
				}
			}

			if (score > 500 && settings.location == 2) {
				if (cur < max) {
					cur += 1.f;
					black.setFillColor(Color(0, 0, 0, cur));
				}
			}

			float dt = clock.restart().asSeconds();

			spawntime += dt;


			if (spawntime >= spawninterval) {

				spawntime = 0;

				int type = rand() % 2;

				Sprite newspr;

				newspr.setTexture(type == 0 ? logtex : croctex);

				float ystart = (rand() % 5) * 100 + 340;

				newspr.setPosition(1000, ystart);

				bool overlap = false;

				for (auto& obj : obstacles) {

					if (collides(newspr, obj.sprite)) {
						overlap = true;
						break;
					}
				}

				if (!overlap) {
					Obstacle obs;
					obs.sprite = newspr;
					obstacles.push_back(obs);
				}

			}

			for (auto& obs : obstacles) {
				obs.sprite.move(-(150 * dt), 0);
			}

			for (int i = 0; i < obstacles.size(); i++) {
				float x = obstacles[i].sprite.getPosition().x;

				if (x < -100) {
					obstacles.erase(obstacles.begin() + i);
					i--;
				}
			}

			

			sf::FloatRect shipCollisionRect(
				ship.getPosition().x, // x offset from current posi
				ship.getPosition().y + 60, // y offset from current position
				105,                        // custom width
				40                         // custom height
			);

			for (int i = 0; i < obstacles.size(); i++) {

				if (shipCollisionRect.intersects(obstacles[i].sprite.getGlobalBounds())) {

					music.pause();


					Sound crash;
					crash.setBuffer(buffer);
					crash.setVolume(settings.sfxvolume);
					crash.play();
					bomb.setPosition(ship.getPosition().x + 25, ship.getPosition().y + 25);
					maingame.draw(bomb);
					maingame.display();
					Sleep(500);

					totalscore = score;

					live--;

					goto a;
				}

				if (man.getGlobalBounds().intersects(obstacles[i].sprite.getGlobalBounds())) {
					if (manposrand == 1) man.setPosition(1000, 330);
					if (manposrand == 2) man.setPosition(1000, 430);
					if (manposrand == 3) man.setPosition(1000, 530);
					if (manposrand == 4) man.setPosition(1000, 630);
					if (manposrand == 5) man.setPosition(1000, 730);

					score = score - 50;

					if (score < 0) {
						score = 0;
					}
				}


			}

			timer += dt;

			if (timer >= frameduration) {
				timer = 0;
				currentframe = (currentframe + 1)%totalframe;

				IntRect rect(FRAME_WIDTH * currentframe, 0, FRAME_WIDTH, FRAME_HEIGHT);

				man.setTextureRect(rect);

			}
			
			bool hit = false;

			if (ship.getGlobalBounds().intersects(man.getGlobalBounds())) {

				if (!hit) {

					hit = true;

					point.play();
				}

				if (manposrand == 1) man.setPosition(1000, 330);
				if (manposrand == 2) man.setPosition(1000, 430);
				if (manposrand == 3) man.setPosition(1000, 530);
				if (manposrand == 4) man.setPosition(1000, 630);
				if (manposrand == 5) man.setPosition(1000, 730);

				score = score + 100;
				
			}

			if (man.getPosition().x <= 20) {

				if (manposrand == 1) man.setPosition(1000, 330);
				if (manposrand == 2) man.setPosition(1000, 430);
				if (manposrand == 3) man.setPosition(1000, 530);
				if (manposrand == 4) man.setPosition(1000, 630);
				if (manposrand == 5) man.setPosition(1000, 730);

				manposrand = rand() % 5 + 1;
			}

			man.move(-1, 0);
			manposrand = rand() % 5 + 1;

			maingame.clear();

			maingame.draw(bg1);
			maingame.draw(bg2);
			maingame.draw(black);
			
			for (auto& obs : obstacles) {
				maingame.draw(obs.sprite);
			}
			maingame.draw(man);

			maingame.draw(ship);

			if (cloudCur == 255.f && settings.location == 1) {
				maingame.draw(rains[static_cast<int>(index)]);
				index += 0.05;
				if (index >= 2.f) {
					index = 0.f;
				}
			}
			if (settings.location == 1) maingame.draw(clouds);
			maingame.draw(myscore);
			maingame.draw(sscore);

			for (int i = 0; i < live; i++) {
				maingame.draw(heart[i]);
			}

			maingame.display();

		}

		return 0;

}

