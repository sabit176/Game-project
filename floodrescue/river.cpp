#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <Windows.h>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace sf;
using namespace std;

/// 
string curusername = "";

//gamesettings
struct GameSettings {
    int musicvolume = 50;
    int sfxvolume = 50;
    int difficulty = 1;
};

GameSettings settings;

static string s(double n) {
    stringstream ss;
    ss << n;
    return ss.str();
}

static string s(int n) {
    stringstream ss;
    ss << n;
    return ss.str();
}

void savescore(string username, string password) {


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

    /* Texture bgTexture;
     bgTexture.loadFromFile("stscreen2.png");
     Sprite bgSprite(bgTexture);*/

    RenderWindow login(VideoMode(800, 600), "River Rescue");

    Font font;
    if (!font.loadFromFile("pxfont.ttf")) {
        return false;
    }

    string username = "";
    string password = "";
    bool enterusername = true;
    bool isregister = false;

    Text title("Login", font, 50);//text title
    title.setPosition(320, 50);
    title.setFillColor(Color::White);

    Text userlabel("Username:", font, 25);
    userlabel.setPosition(150, 200);

    Text passlabel("Password:", font, 25);
    passlabel.setPosition(150, 280);

    Text userinput("", font, 25);
    userinput.setPosition(400, 200);

    Text passinput("", font, 25);
    passinput.setPosition(400, 280);

    Text message("", font, 20);
    message.setPosition(200, 380);
    message.setFillColor(Color::Yellow);

    Text instruction("TAB=Switch Control=Register", font, 15);
    instruction.setPosition(120, 500);
    instruction.setFillColor(Color::Cyan);

    RectangleShape userBox(Vector2f(300, 40));
    userBox.setPosition(390, 195);
    userBox.setFillColor(Color::Transparent);
    userBox.setOutlineColor(Color::Green);
    userBox.setOutlineThickness(2);

    RectangleShape passBox(Vector2f(300, 40));
    passBox.setPosition(390, 275);
    passBox.setFillColor(Color::Transparent);
    passBox.setOutlineColor(Color::White);
    passBox.setOutlineThickness(2);

    while (login.isOpen()) {
        Event e;
        while (login.pollEvent(e)) {
            if (e.type == Event::Closed) {
                login.close();
                return false;
            }

            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Tab) {
                    enterusername = !enterusername;
                    userBox.setOutlineColor(enterusername ? Color::Green : Color::White);
                    passBox.setOutlineColor(enterusername ? Color::White : Color::Green);
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

        login.clear(Color(20, 20, 40));
        //login.clear();
        //login.draw(bgSprite);
        login.draw(title);
        login.draw(userlabel);
        login.draw(passlabel);
        login.draw(userBox);
        login.draw(passBox);
        login.draw(userinput);
        login.draw(passinput);
        login.draw(message);
        login.draw(instruction);
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

    Text difflabel("Difficulty: ", font, 25);
    difflabel.setPosition(150, 320);

    Text musicVal(s(settings.musicvolume), font, 25);
    musicVal.setPosition(500, 180);
    musicVal.setFillColor(Color::Yellow);

    Text sfxVal(s(settings.sfxvolume), font, 25);
    sfxVal.setPosition(500, 250);
    sfxVal.setFillColor(Color::Yellow);

    Text diffVal(settings.difficulty == 1 ? "Easy" : (settings.difficulty == 2 ? "Medium" : "Hard"), font, 25);
    diffVal.setPosition(500, 320);
    diffVal.setFillColor(Color::Yellow);

    Text instruction("Use UP/DOWN to adjust|LEFT/RIGHT to change difficulty | ESC to return", font, 15);
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
                    if (selectedoption == 2 && settings.difficulty > 1) settings.difficulty--;
                }

                if (e.key.code == Keyboard::Right) {
                    if (selectedoption == 0 && settings.musicvolume < 100) settings.musicvolume += 10;
                    if (selectedoption == 1 && settings.sfxvolume < 100) settings.sfxvolume += 10;
                    if (selectedoption == 2 && settings.difficulty < 3) settings.difficulty++;
                }
            }
        }

        musiclabel.setFillColor(selectedoption == 0 ? Color::Green : Color::White);
        sfxlabel.setFillColor(selectedoption == 1 ? Color::Green : Color::White);
        difflabel.setFillColor(selectedoption == 2 ? Color::Green : Color::White);

        musicVal.setString(s(settings.musicvolume));
        sfxVal.setString(s(settings.sfxvolume));
        diffVal.setString(settings.difficulty == 1 ? "Easy" : (settings.difficulty == 2 ? "Medium" : "Hard"));

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

    Font menuFont;
    menuFont.loadFromFile("pxfont.ttf");


    Text playtext("Press ENTER to Play", menuFont, 30);
    playtext.setPosition(320, 450);
    playtext.setFillColor(Color::Green);
    ///////
    Text optext("Press O for Options", menuFont, 25);
    optext.setPosition(330, 550);
    optext.setFillColor(Color::Green);
    ///////
    Text leadtext("Press L to show leaderboard", menuFont, 25);
    leadtext.setPosition(230, 650);
    leadtext.setFillColor(Color::Green);

    while (start.isOpen()) {
        Event event;
        while (start.pollEvent(event)) {
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                start.close();
            }

            if (Keyboard::isKeyPressed(Keyboard::O)) {
                optionmenu();
            }
            ///////
            if (Keyboard::isKeyPressed(Keyboard::L))
            {
                showleaderboard();
            }

            if (event.type == Event::Closed) {
                start.close();
                return 0;
            }
        }

        start.clear();
        start.draw(stscreen);
        start.draw(playtext);
        start.draw(optext);
        start.draw(leadtext);
        start.display();
    }

    double live = 3;
    ////
    double totalscore = 0;

a:
    srand(time(NULL));

    int speed1 = 0, speed2 = 0, speed3 = 0, speed4 = 0, speed5 = 0;

    //////
    int diffmult = settings.difficulty;

    speed1 = rand() % 5 + diffmult;
    speed2 = rand() % 5 + diffmult;
    speed3 = rand() % 5 + diffmult;
    speed4 = rand() % 5 + diffmult;
    speed5 = rand() % 5 + diffmult;

    int manposrand = 0;
    manposrand = rand() % 5 + 1;

    int logp1 = 340, logp2 = 440, logp3 = 540, logp4 = 640, logp5 = 740;

    int bframe = 0;

    int x = 4, y = 4;

    double score = 0;

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

        //////
        string scoreText = "Score: " + s(totalscore);
        Text sctext(scoreText, menuFont, 50);
        //Text sctext("Score: " + s(score), menuFont, 50);
        sctext.setPosition(320, 750);
        sctext.setFillColor(Color::Red);

        maingame.clear();
        maingame.draw(gameover);
        /////
        maingame.draw(sctext);

        maingame.display();
        Sound go;
        go.setBuffer(g);
        go.setVolume(settings.sfxvolume);
        go.play();

        ////
        leaderboardscore(curusername, (int)totalscore);



        Sleep(3000);

        // //////
        totalscore = 0;
        live = 3;

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

    ////////
    Texture b;
    if (settings.difficulty == 1) {
        b.loadFromFile("gamebg.png");
    }
    else if (settings.difficulty == 2) {
        b.loadFromFile("gamebg2.jpeg");
    }
    else {
        b.loadFromFile("gamebg3.jpeg");
    }
    Sprite bg(b);

    Texture m;
    m.loadFromFile("man.png");
    Sprite man(m);
    man.setPosition(500, 430);

    Texture sh;
    sh.loadFromFile("ship1.png");
    Sprite ship(sh);
    ship.setPosition(20, 480);

    Texture l1;
    l1.loadFromFile("laga.png");

    Sprite log1(l1);
    log1.setPosition(1000, 340);
    Sprite log2(l1);
    log2.setPosition(1000, 440);
    Sprite log3(l1);
    log3.setPosition(1000, 540);
    Sprite log4(l1);
    log4.setPosition(1000, 640);
    Sprite log5(l1);
    log5.setPosition(1000, 740);

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
    bonus.loadFromFile("bonus.wav");

    while (maingame.isOpen()) {
        score += 0.01;

        myscore.setFont(myfont);
        myscore.setString(s(score));
        myscore.setCharacterSize(20);
        myscore.setFillColor(sf::Color::White);
        myscore.setOutlineColor(sf::Color::Black);
        myscore.setPosition(215, 70);

        sscore.setFont(myfont);
        sscore.setString("SCORE: ");
        sscore.setCharacterSize(20);
        sscore.setFillColor(sf::Color::White);
        sscore.setOutlineColor(sf::Color::Black);
        sscore.setPosition(30, 70);

        maingame.setFramerateLimit(110);

        Event e;
        while (maingame.pollEvent(e)) {
            if (e.type == Event::Closed) {
                maingame.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Up) && ship.getPosition().y > 280) {
            ship.move(0, -y);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down) && ship.getPosition().y < 680) {
            ship.move(0, y);
        }
        if (Keyboard::isKeyPressed(Keyboard::Left) && ship.getPosition().x > 50) {
            ship.move(-x, 0);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right) && ship.getPosition().x < 1000) {
            ship.move(x, 0);
        }
        /////////////////////
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            goto b;
        }


        if (log1.getPosition().x <= 20) { log1.setPosition(1000, logp1); speed1 = rand() % 5 + diffmult; }
        if (log2.getPosition().x <= 20) { log2.setPosition(1000, logp2); speed2 = rand() % 5 + diffmult; }
        if (log3.getPosition().x <= 20) { log3.setPosition(1000, logp3); speed3 = rand() % 5 + diffmult; }
        if (log4.getPosition().x <= 20) { log4.setPosition(1000, logp4); speed4 = rand() % 5 + diffmult; }
        if (log5.getPosition().x <= 20) { log5.setPosition(1000, logp5); speed5 = rand() % 5 + diffmult; }

        log1.move(-(speed1 / 2 + 1), 0);
        log2.move(-(speed2 / 2 + 1), 0);
        log3.move(-(speed3 / 2 + 1), 0);
        log4.move(-(speed4 / 2 + 1), 0);
        log5.move(-(speed5 / 2 + 1), 0);

        if (ship.getGlobalBounds().intersects(log1.getGlobalBounds()) ||
            ship.getGlobalBounds().intersects(log2.getGlobalBounds()) ||
            ship.getGlobalBounds().intersects(log3.getGlobalBounds()) ||
            ship.getGlobalBounds().intersects(log4.getGlobalBounds()) ||
            ship.getGlobalBounds().intersects(log5.getGlobalBounds())) {

            music.pause();
            Sound crash;
            crash.setBuffer(buffer);
            crash.setVolume(settings.sfxvolume);
            crash.play();
            bomb.setPosition(ship.getPosition().x + 25, ship.getPosition().y + 25);
            maingame.draw(bomb);
            maingame.display();
            Sleep(500);

            ////////
            totalscore += score;

            live--;
            goto a;
        }

        if (ship.getGlobalBounds().intersects(man.getGlobalBounds())) {
            music.pause();
            Sound ptup;
            ptup.setBuffer(bonus);
            ptup.setVolume(settings.sfxvolume);
            ptup.play();

            if (manposrand == 1) man.setPosition(1000, 330);
            if (manposrand == 2) man.setPosition(1000, 430);
            if (manposrand == 3) man.setPosition(1000, 530);
            if (manposrand == 4) man.setPosition(1000, 630);
            if (manposrand == 5) man.setPosition(1000, 730);

            score = score + 100;
            music.play();
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
        maingame.clear();
        maingame.draw(bg);
        maingame.draw(ship);
        maingame.draw(myscore);
        maingame.draw(sscore);
        maingame.draw(log1);
        maingame.draw(log2);
        maingame.draw(log3);
        maingame.draw(log4);
        maingame.draw(log5);
        maingame.draw(man);

        for (int i = 0; i < live; i++) {
            maingame.draw(heart[i]);
        }

        maingame.display();
    }

    return 0;
}