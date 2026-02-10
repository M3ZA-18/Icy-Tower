#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <iomanip>
using namespace std;
using namespace sf;



int frame = 0, move = 0, point = 0, menuIndex = 0, settingOpen = 0, paupoint = 0,highstscore;
float value = 100.f;


//credit background
Texture creditback;
Sprite creditbackground;
//creadit text
Text credittext[5];
string creditlist[5]{ "~Stephano","Y/Ehab","Mo3az","Gharib","Flip Back" };

//the finger
Texture fingerTex;
Sprite finger;

//main menu words
Font font;
Text maintext[3];
string mainlist[3]{ "Go To Jump","setting","Goodbye" };
//main menu backgroundTexture bg
Texture mainback;
Sprite mainbackground;
//settings menu words
Text settingtext[2];
string settinglist[2]{ "Sound","Flip Back" };
//setting menu backgroundTexture
Texture settingback;
Sprite settingbackground;
//tips menu words
Text tipstext[2];
String tipsstring[2]{ "jump or Die \n the hell coming to you","Back to home" };

//tips menu backgroundTexture
Texture tipsback;
Sprite tipsbackground;
//pause menu words
Text pausetext[3];
string pauselist[3]{ "Keep Jumping","Credits","Back to home" };
//pause menu backgroundTexture
Texture pauseback;
Sprite pausebackground;
//sound
SoundBuffer buffer;
Sound moveSound;
SoundBuffer buffer1;
Sound enter;
SoundBuffer theme;
Sound theme_music;
SoundBuffer in_game;
Sound game_mu;
SoundBuffer creditmu;
Sound credit_music;
SoundBuffer amazing_so;
Sound ama;
SoundBuffer extreme;
Sound ext;
SoundBuffer good;
Sound go;
SoundBuffer cheer;
Sound che;
SoundBuffer great;
Sound gre;
SoundBuffer super;
Sound su;
SoundBuffer wazup;
Sound waz;
//gameover menu words
Text gameoverstext[2];
String gameoverstring[2]{ "Retry","Back to home" };
//gameover menu backgroundTexture
Texture gameoversback;
Sprite gameoverbackground;


//sounds && jump
SoundBuffer jumpBuffer, walkBuffer, fallBuffer, scoreBuffer, gameoverBuffer, pauseBuffer;
Sound jumpSound; Sound walkSound, fallSound, scoreSound, gameoverSound, pauseSound;
//background 
Texture backgroundTexture;
Sprite backgroundSprite[3];



//platform
Texture plat;
// walls
Texture leftw;
Texture rightw;

// GLOBAL VARIABLES
Event event;
RenderWindow window = { VideoMode(1800,900) , "Icy Towers" };
Vector2u windowSize = window.getSize();
View gameCamera = window.getDefaultView();

Clock timeClock;
Clock dtClock;
float dt = 0;

// Game variables
float playerSpeed = 80.0f;
float playerAcceleration = 25.0f;
float playerJump = 120.0f;
float wallJump = 0.01f;
float gravity = 35.8f;
bool wantsToJump = false;
float highestPlayerY;
float camY;
float camHeight = 100.0f;
float camLerpSpeed = 10.0f;
float camVerticalSpeed = 10.0f;
float camAcceleration = 0.01f;
float highestplat;
int platformSpacing = 155;
bool gameOver = false;
bool showGameOver = false;
Clock gameOverClock;
Text gameOverText;
bool isPlayerFlipping = false;
float playerFlipSpeed = 200.0f;  // degrees per second
float playerFlipThreshold = 75.0f; // speed required to start flipping
bool mu = true;
float lastCamY = 0.f;
float wallsSpeed = -0.8f;
float leftWallX;
float rightWallX;
int cnt = 0;


// runtime
Texture playerTexture;
Sprite player;

Texture groundTexture;
Sprite ground;

Texture wallTexture;
Sprite leftWall[4];
Sprite rightWall[4];

Sprite platform[10];

float velocityX = 0;
float velocityY = 0;
bool isGrounded = false;
bool isTouchingWall = false;

// Score
int score = 0;
Font gameFont;
Text scoreText;
Texture amazingTexture;
Sprite phrase;
Texture goodtex;
Sprite good_ph;
Texture greattex;
Sprite great_ph;
Texture extremetex;
Sprite extre_ph;

bool showPhrase = false;
bool showgood = false;
bool showgreat = false;
bool showextreme = false;
Clock phraseClock;
Clock goodClock;
Clock greatClock;
Clock extremeClock;

// Random helpers
int RandomRange(int start, int end) {
    return rand() % (end + 1 - start) + start;
}

float RandomRangeF(float start, float end) {
    float random = rand() % (int)(end * 10 + 1 - start * 10) + start * 10;
    return random / 10.0f;
}
float Clamp(float value, float minLimit, float maxLimit) {
    value = max(value, minLimit);
    value = min(value, maxLimit);
    return value;
}
float Lerp(float start, float end, float factor) {
    return (end - start) * Clamp(factor, 0, 1) + start;
}
void SetPlayerTexture(int frame, float startOffset) {
    player.setTextureRect(IntRect(frame * 37 + startOffset, -10, 42, 83));
}
void ResetWalls()
{
    for (size_t i = 0; i < 4; i++)
    {
        leftWall[i].setPosition(
            leftWallX,
            windowSize.y - i * leftWall[i].getGlobalBounds().height
        );

        rightWall[i].setPosition(
            rightWallX,
            windowSize.y - i * rightWall[i].getGlobalBounds().height
        );
    }
}
//particles 
struct Particle
{
    CircleShape shape;
    float velocityX = 0;
    float velocityY = 0;
    float spawnTime;

    CircleShape MakeRandomParticle() {
        CircleShape shape(RandomRangeF(2.0f, 5.0f));
        shape.setFillColor(Color(
            RandomRange(100, 255),
            RandomRange(100, 255),
            RandomRange(100, 255)
        ));
        return shape;
    }

    Particle(float x, float y, float velX, float velY)
    {
        velocityX = velX;
        velocityY = velY;

        shape = MakeRandomParticle();
        shape.setPosition(x, y);
        spawnTime = timeClock.getElapsedTime().asSeconds();
    }

    void Update(int i, vector<Particle>& particles) {
        velocityY += gravity * dt * 0.5f;
        shape.move(velocityX * dt, velocityY * dt);

        if (timeClock.getElapsedTime().asSeconds() - spawnTime > 5.0f)
        {
            particles[i] = particles.back();
            particles.pop_back();
        }
    }
};
vector<Particle> particles;
Clock particlesClock;

void SpawnParticles(int amount, float velMultiplier = 1.0f) {
    for (int i = 0; i < amount; i++)
    {
        particles.push_back(Particle(player.getPosition().x, player.getPosition().y - 50,
            RandomRangeF(-50, 50) * velMultiplier, RandomRangeF(-50, 50) * velMultiplier));
    }
}

bool zoom = false;

// START GAME
void Start() {

    if (!zoom)
    {
        zoom = true;
        gameCamera.zoom(0.55);

    }

    srand(timeClock.getElapsedTime().asMicroseconds());
    //sounds && jump
    jumpBuffer.loadFromFile("jump_hi.ogg");
    scoreBuffer.loadFromFile("amazing.ogg");
    walkBuffer.loadFromFile("step.ogg");
    jumpSound.setBuffer(jumpBuffer);
    jumpSound.setVolume(50);
    walkSound.setBuffer(walkBuffer);
    pauseBuffer.loadFromFile("wazup.ogg");
    gameoverBuffer.loadFromFile("gameover.ogg");
    fallBuffer.loadFromFile("falling.ogg");
    pauseSound.setBuffer(pauseBuffer);
    gameoverSound.setBuffer(gameoverBuffer);
    fallSound.setBuffer(fallBuffer);
    in_game.loadFromFile("in_game_music.mp3");
    game_mu.setBuffer(in_game);
    creditmu.loadFromFile("credit_music.mp3");
    credit_music.setBuffer(creditmu);
    amazing_so.loadFromFile("amazing.ogg");
    ama.setBuffer(amazing_so);
    extreme.loadFromFile("extreme.ogg");
    ext.setBuffer(extreme);
    good.loadFromFile("good.ogg");
    go.setBuffer(good);
    cheer.loadFromFile("cheer.ogg");
    che.setBuffer(cheer);
    great.loadFromFile("great.ogg");
    gre.setBuffer(great);
    super.loadFromFile("super.ogg");
    su.setBuffer(super);
    wazup.loadFromFile("wazup.ogg");
    waz.setBuffer(wazup);


    // walls + back
    backgroundTexture.loadFromFile("background.png");
    leftw.loadFromFile("LeftWall.png");
    rightw.loadFromFile("RightWall.png");


    // platform
    plat.loadFromFile("log.png");

    //load
    font.loadFromFile("font.ttf");
    mainback.loadFromFile("bg.png");
    settingback.loadFromFile("bgs.png");
    pauseback.loadFromFile("bgpa.png");
    tipsback.loadFromFile("bgti.png");
    creditback.loadFromFile("credit.png");
    creditbackground.setTexture(creditback);
    //========================================Text===================================
    //main menu
    for (int i = 0; i < 3; i++) {
        maintext[i].setString(mainlist[i]);
        maintext[i].setFont(font);
        maintext[i].setCharacterSize(80);
        maintext[i].setFillColor(Color::White);
        maintext[i].setOrigin(maintext[i].getLocalBounds().width / 2.0f, maintext[i].getLocalBounds().height / 2.0f);
        maintext[i].setPosition(890 + ((windowSize.x - 1800) / 2), 520 + ((windowSize.y - 900) / 2) + i * 100);
    }
    //credit text
    for (int i = 0; i < 2; i++) {
        credittext[i].setString(creditlist[i]);
        credittext[i].setFont(font);
        credittext[i].setCharacterSize(90);
        credittext[i].setFillColor(Color::Black);
        credittext[i].setOrigin(credittext[i].getLocalBounds().width / 2.0f, credittext[i].getLocalBounds().height / 2.0f);
        credittext[i].setPosition(500 + i * 700, 500);
    }
    for (int i = 2; i < 4; i++) {
        credittext[i].setString(creditlist[i]);
        credittext[i].setFont(font);
        credittext[i].setCharacterSize(90);
        credittext[i].setFillColor(Color::Black);
        credittext[i].setOrigin(credittext[i].getLocalBounds().width / 2.0f, credittext[i].getLocalBounds().height / 2.0f);
        credittext[i].setPosition(500 + (i - 2) * 700, 600);
    }
    credittext[4].setString(creditlist[4]);
    credittext[4].setFont(font);
    credittext[4].setCharacterSize(80);
    credittext[4].setFillColor(Color::Black);
    credittext[4].setOrigin(credittext[4].getLocalBounds().width / 2.0f, credittext[4].getLocalBounds().height / 2.0f);
    credittext[4].setPosition(900, 700);


    //background
    for (size_t i = 0; i < 3; i++)
    {
        backgroundSprite[i].setTexture(backgroundTexture);
        backgroundSprite[i].setOrigin(backgroundSprite[i].getLocalBounds().width / 2.0f, backgroundSprite[i].getLocalBounds().height / 2.0f);
        backgroundSprite[i].setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f - i * (backgroundSprite[i].getGlobalBounds().height - 50));
    }
    

    //wall
    leftWallX = 460;
    rightWallX = windowSize.x - 460;

    for (size_t i = 0; i < 4; i++)
    {
        leftWall[i].setTexture(leftw);
        leftWall[i].setOrigin(
            leftWall[i].getLocalBounds().width / 2.0f,
            leftWall[i].getLocalBounds().height / 2.0f
        );
        leftWall[i].setScale(1, 1);
        leftWall[i].setPosition(
            leftWallX,
            windowSize.y - i * leftWall[i].getGlobalBounds().height
        );

        rightWall[i].setTexture(rightw);
        rightWall[i].setOrigin(
            rightWall[i].getLocalBounds().width / 2.0f,
            rightWall[i].getLocalBounds().height / 2.0f
        );
        rightWall[i].setScale(1, 1);
        rightWall[i].setPosition(
            rightWallX,
            windowSize.y - i * rightWall[i].getGlobalBounds().height
        );
    }



    //setting menu
    for (int i = 0; i < 2; i++) {
        settingtext[i].setString(settinglist[i]);
        settingtext[i].setFont(font);
        settingtext[i].setCharacterSize(80);
        settingtext[i].setFillColor(Color::White);
        settingtext[i].setOrigin(settingtext[i].getLocalBounds().width / 2.0f, settingtext[i].getLocalBounds().height / 2.0f);
        settingtext[i].setPosition(890 + ((windowSize.x - 1800) / 2), 580 + ((windowSize.y - 900) / 2) + i * 100);
    }
    //pause menu
    for (int i = 0; i < 3; i++) {
        pausetext[i].setString(pauselist[i]);
        pausetext[i].setFont(font);
        pausetext[i].setCharacterSize(80);
        pausetext[i].setFillColor(Color::White);
        pausetext[i].setOrigin(pausetext[i].getLocalBounds().width / 2.0f, pausetext[i].getLocalBounds().height / 2.0f);
        pausetext[i].setPosition(890 + ((windowSize.x - 1800) / 2), 550 + ((windowSize.y - 900) / 2) + i * 100);
    }
    // game over menu

    for (int i = 0; i < 2; i++) {
        gameoverstext[i].setString(gameoverstring[i]);
        gameoverstext[i].setFont(font);
        gameoverstext[i].setCharacterSize(80);
        gameoverstext[i].setFillColor(Color::White);
        gameoverstext[i].setOrigin(gameoverstext[i].getLocalBounds().width / 2.0f, gameoverstext[i].getLocalBounds().height / 2.0f);
        gameoverstext[i].setPosition(890 + ((windowSize.x - 1800) / 2), 400 + ((windowSize.y - 900) / 2) + 50 + i * 100);
    }


    //==========================================Background=================================
    //main background
    mainbackground.setTexture(mainback);
    mainbackground.setTextureRect(IntRect(0, 0, windowSize.x, windowSize.y));
    mainbackground.setPosition(0, 0);
    mainbackground.setScale(1 + 1 - mainbackground.getGlobalBounds().width / windowSize.x, 1 + 1 - mainbackground.getGlobalBounds().height / windowSize.y)
        ;
    //setting background
    settingbackground.setTexture(settingback);
    settingbackground.setTextureRect(IntRect(0, 0, 1800, 900));
    settingbackground.setScale(1 + 1 - settingbackground.getGlobalBounds().width / windowSize.x, 1 + 1 - settingbackground.getGlobalBounds().height / windowSize.y);
    settingbackground.setPosition(0, 0);
    //pause background
    pausebackground.setTexture(pauseback);
    pausebackground.setTextureRect(IntRect(0, 0, 1800, 900));
    pausebackground.setScale(1 + 1 - pausebackground.getGlobalBounds().width / windowSize.x, 1 + 1 - pausebackground.getGlobalBounds().height / windowSize.y);
    pausebackground.setPosition(0, 0);
    //game over
    gameoversback.loadFromFile("game_over.png");
    //gameover background
    gameoverbackground.setTexture(gameoversback);
    gameoverbackground.setTextureRect(IntRect(0, 0, 1800, 900));
    gameoverbackground.setScale(1 + 1 - gameoverbackground.getGlobalBounds().width / windowSize.x, 1 + 1 - gameoverbackground.getGlobalBounds().height / windowSize.y);
    gameoverbackground.setPosition(0, 0);
    //==============================================sound=======================================
    buffer.loadFromFile("move sound.mp3");
    moveSound.setBuffer(buffer);
    moveSound.setVolume(float(value));
    buffer1.loadFromFile("enter.mp3");
    enter.setBuffer(buffer1);
    enter.setVolume(value);
    theme.loadFromFile("theme_mu.wav");
    theme_music.setBuffer(theme);
    //the finger
    //=======================================================================finger========================================================================
    fingerTex.loadFromFile("finger.png");
    finger.setTexture(fingerTex);
    finger.setTextureRect(IntRect(300, 170, 55, 55));
    finger.setOrigin(finger.getLocalBounds().width / 2.0f, finger.getLocalBounds().height / 2.0f);
    finger.setScale(2.0f, 2.0f);
    finger.scale(.5f, .5f);


    // Load font
    if (!gameFont.loadFromFile("font.ttf")) {
        cout << "Font not loaded!" << endl;
    }
    scoreText.setFont(gameFont);
    scoreText.setCharacterSize(65);
    scoreText.setFillColor(Color::White);
    scoreText.setString("Score: 0");

    // Load score phrase
    if (!amazingTexture.loadFromFile("amazing2.png")) {
        cout << "Phrase image not loaded!" << endl;
    }
    if (!goodtex.loadFromFile("good_pho.png")) {
        cout << "Phrase image not loaded!" << endl;
    }
    if (!greattex.loadFromFile("great_pho.png")) {
        cout << "Phrase image not loaded!" << endl;
    }
    if (!extremetex.loadFromFile("extreme_ph.png")) {
        cout << "Phrase image not loaded!" << endl;
    }
    phrase.setTexture(amazingTexture);
    phrase.setScale(0.5, 0.5);
    good_ph.setTexture(goodtex);
    good_ph.setScale(.8, .8);
    great_ph.setTexture(greattex);
    great_ph.setScale(.8, .8);
    extre_ph.setTexture(extremetex);
    extre_ph.setScale(1, 1);

    // Ground
    groundTexture.loadFromFile("log.png");
    ground.setTexture(groundTexture);
    ground.setOrigin(ground.getLocalBounds().width / 2.0f, ground.getLocalBounds().height / 2.0f);
    ground.setPosition(windowSize.x / 2.0f, windowSize.y - 200.0f);
    ground.setScale(4, 1);

    // Player
    playerTexture.loadFromFile("player.png");
    player.setTexture(playerTexture);
    player.setTextureRect(IntRect(0, 0, 48, 56));
    player.setOrigin(player.getLocalBounds().width / 2.0f, player.getLocalBounds().height);
    player.setPosition(windowSize.x / 2.0f, ground.getGlobalBounds().top);
    highestPlayerY = player.getPosition().y;
    camY = highestPlayerY - camHeight;



    // game over
    gameOverText.setFont(gameFont);
    gameOverText.setCharacterSize(120);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setString("GAME OVER");


    // Platforms initial
    for (int i = 0; i < 10; i++) {
        platform[i].setTexture(plat);
        platform[i].setPosition(RandomRange(-300, 300) + windowSize.x / 2, ground.getGlobalBounds().top - (i + 1) * platformSpacing);
        platform[i].setOrigin(platform[i].getLocalBounds().width / 2.0f, platform[i].getLocalBounds().height / 2.0f);
        platform[i].setScale(RandomRangeF(0.8, 1.4), 1.2f);
    }

    highestplat = platform[0].getPosition().y;
    for (int i = 1; i < 10; i++)
        highestplat = min(highestplat, platform[i].getPosition().y);



    // RESET VARIABLES

    velocityX = 0;
    velocityY = 0;

    isGrounded = false;
    isTouchingWall = false;
    wantsToJump = false;

    score = 0;
    showPhrase = false;

    gameOver = false;
    showGameOver = false;

    phraseClock.restart();
    gameOverClock.restart();

    // IMPORTANT: reset clocks
    dtClock.restart();
    timeClock.restart();
    particlesClock.restart();


    camY = highestPlayerY - camHeight;
    lastCamY = camY;
    ResetWalls();

}

// Player jump
void Jump(float jumpForce) {
    velocityY = -jumpForce;
    isGrounded = false;
    wantsToJump = false;
    jumpSound.play();
}

// Collision with platforms and ground
bool PlayerCollision(Sprite other) {
    bool isFalling = velocityY > 0.1f;
    bool isInOtherX = player.getPosition().x > other.getGlobalBounds().left && player.getPosition().x < other.getGlobalBounds().left + other.getGlobalBounds().width;
    float deltaY = player.getPosition().y - other.getGlobalBounds().top;
    bool isInOtherY = deltaY <= 50.0f && deltaY >= 0;

    if (isFalling && isInOtherY && isInOtherX) {
        player.setPosition(player.getPosition().x, other.getGlobalBounds().top);
        velocityY = 0;

        return true;
    }
    else
        return false;
}

// Event loop
void GameEventLoop(Event event) {
    if (event.type == Event::KeyPressed && (event.key.code == Keyboard::Space || event.key.code == Keyboard::Up)) {
        wantsToJump = true;
    }
    if (event.type == Event::KeyPressed && (event.key.code == Keyboard::P || event.key.code == Keyboard::Escape)) {
        menuIndex = 2;
        waz.play();
    }
}

// Camera follow + Score
void gamecam() {
    if (player.getPosition().y < highestPlayerY)
        highestPlayerY = min(player.getPosition().y, highestPlayerY);

    float targetCamY = highestPlayerY - camHeight;

    if (targetCamY < camY)
        camY = Lerp(camY, targetCamY, dt * dt * camLerpSpeed);

    camY -= camVerticalSpeed * dt + (score * camAcceleration);

    gameCamera.setCenter(windowSize.x / 2.0f, camY);
    window.setView(gameCamera);

    // Update score
    score = max(score, int(ground.getGlobalBounds().top - highestPlayerY) / platformSpacing);
    scoreText.setString("Score: " + to_string(score));
    scoreText.setPosition(gameCamera.getCenter().x - windowSize.x / 2 + +400, camY - windowSize.y / 2 + 180);

    // Show amazing phrase every 20 points
    
    if (score > 0 ) {
        
            if (score == 20) {
                ama.play();
                showPhrase = true;
                phraseClock.restart();
            }
            else if (score == 40) {
                go.play();
                showgood = true;
                goodClock.restart();
            }
            else if (score == 60) {
                gre.play();
                showgreat = true;
                greatClock.restart();
            }
            else if (score==80) {
                ext.play();
                showextreme = true;
                extremeClock.restart();
            }
            else if (score == 200){
                su.play();
            }
            
        
        

        

    }

    if (showPhrase && phraseClock.getElapsedTime().asSeconds() > 1.0f) {
        showPhrase = false;
    }
    if (showgood && goodClock.getElapsedTime().asSeconds() > 1.0f) {
        showgood = false;
    }
    if (showgreat && greatClock.getElapsedTime().asSeconds() > 1.0f) {
        showgreat = false;
    }
    if (showextreme && extremeClock.getElapsedTime().asSeconds() > 1.0f) {
        showextreme = false;
    }

    phrase.setPosition(gameCamera.getCenter().x - phrase.getGlobalBounds().width / 2,
        camY - windowSize.y / 2 + 300);
    good_ph.setPosition(gameCamera.getCenter().x - good_ph.getGlobalBounds().width / 2,
        camY - windowSize.y / 2 + 300);
    great_ph.setPosition(gameCamera.getCenter().x - great_ph.getGlobalBounds().width / 2,
        camY - windowSize.y / 2 + 300);
    extre_ph.setPosition(gameCamera.getCenter().x - extre_ph.getGlobalBounds().width / 2,
        camY - windowSize.y / 2 + 300);
    
}
//
void MoveSprite(Sprite* sprite, float speed, bool moveToTop) {
    sprite->move(0, (camY - lastCamY) * speed);

    if (sprite->getGlobalBounds().top > windowSize.y / 2.0f + camY)
    {
        if (moveToTop)
            sprite->setPosition(sprite->getPosition().x, camY - windowSize.y / 2 - sprite->getGlobalBounds().height / 2);
        else
            sprite->setPosition(sprite->getPosition().x, sprite->getGlobalBounds().top - sprite->getGlobalBounds().height * 2);
    }
}




// Platforms respawn
void platforms() {
    for (int i = 0; i < 10; i++) {
        if (platform[i].getPosition().y > camY + windowSize.y / 2 + 120) {
            highestplat -= platformSpacing + RandomRange(-20, 20);
            platform[i].setPosition(RandomRange(-300, 300) + windowSize.x / 2, highestplat);
            platform[i].setScale(RandomRangeF(0.8, 1.4), 1.5f);
        }
    }
}

void StartFlipping() {
    player.setRotation(0);
    isPlayerFlipping = true;
    player.setOrigin(player.getLocalBounds().width / 2.0f, player.getLocalBounds().height / 2.0f);
}

void StopFlipping() {
    player.setRotation(0);
    isPlayerFlipping = false;
    player.setOrigin(player.getLocalBounds().width / 2.0f, player.getLocalBounds().height);
}


// Wall Collision + Wall Jump
void WallCollision(Sprite& player) {

    FloatRect p = player.getGlobalBounds();
    FloatRect left = leftWall[0].getGlobalBounds();
    FloatRect right = rightWall[0].getGlobalBounds();

    bool touching = false;

    if (p.left <= left.left + left.width) {
        player.setPosition(left.left + left.width + p.width / 2, player.getPosition().y);
        touching = true;
    }

    else if (p.left + p.width >= right.left) {
        player.setPosition(right.left - p.width / 2, player.getPosition().y);
        touching = true;
    }

    if (touching && !isTouchingWall) {
        if (!isGrounded)
        {
            velocityX *= -1;
            Jump(playerJump * wallJump * abs(velocityX));
            SpawnParticles(RandomRange(10, 30));
            if (abs(velocityX) > playerFlipThreshold) StartFlipping();
        }
        else
            velocityX *= -0.4f;
    }

    isTouchingWall = touching;
}

void PlayerAnimation() {

    float midAnimation = 30.0f;
    bool goingUpwards = velocityY < -midAnimation;
    bool isFalling = velocityY > midAnimation;
    bool isMoving = abs(velocityX) > 10.0f;


    if (isMoving)
        if (velocityX > 0)
            player.setScale(1.2, 1.2);
        else
            player.setScale(-1.2, 1.2);

    if (isGrounded) StopFlipping();

    if (isMoving) {
        if (isGrounded) // move animation
            SetPlayerTexture((int)(timeClock.getElapsedTime().asSeconds() * 5) % 4 + 3, 2);
        else if (isPlayerFlipping)
        {
            SetPlayerTexture(12, -10);
            player.rotate(playerFlipSpeed * dt);
        }
        else if (goingUpwards) // up animation
            SetPlayerTexture(9, -25);
        else if (isFalling)    // down animation
            SetPlayerTexture(11, -20);
        else // mid air animation
            SetPlayerTexture(10, -20);
    }
    else
        if (isGrounded)
            // idle animation
            SetPlayerTexture((int)(timeClock.getElapsedTime().asSeconds() * 2) % 3, 1);
        else if (isPlayerFlipping)
        {
            SetPlayerTexture(12, -10);
            player.rotate(playerFlipSpeed * dt);
        }
        else if (goingUpwards) // up animation
            SetPlayerTexture(8, -25);
        else if (isFalling)   // down animation
            SetPlayerTexture(16, -10);
        else // mid air animation
            SetPlayerTexture(12, -10);
}
//credit
void Credit() {
    Event e;
    point = 5;
    while (window.pollEvent(e)) {
        point = 4;
        if (e.type == Event::Closed) {
            window.close();
        }
        if (e.type == Event::KeyPressed)
        {
            if (e.key.code == Keyboard::Enter) {
                menuIndex = 2;
            }
        }
        finger.setPosition(credittext[point].getPosition().x - credittext[point].getLocalBounds().width / 2.0f - 40, credittext[point].getPosition().y + 20);
        window.clear();
        window.draw(creditbackground);
        window.draw(finger);
        credittext[point].setFillColor(Color::White);
        for (int i = 0; i < 5; i++) {
            window.draw(credittext[i]);
        }
        window.display();

    }
}







// Update game logic
void UpdateGame() {
    game_mu.setLoop(true);
    game_mu.setVolume(40);

    //leftWall.setPosition(458, camY);
    //rightWall.setPosition((windowSize.x - rightWall.getLocalBounds().width) - 345, camY);

    //backgroundSprite.setPosition(gameCamera.getCenter().x, gameCamera.getCenter().y);

    if (Keyboard::isKeyPressed(Keyboard::Right))
        velocityX += playerAcceleration * dt;
    else if (Keyboard::isKeyPressed(Keyboard::Left))
        velocityX -= playerAcceleration * dt;
    else if (velocityX != 0)
        velocityX += (velocityX > 0 ? -1 : 1) * playerAcceleration * dt;


    velocityY += gravity * dt;
    velocityX = Clamp(velocityX, -playerSpeed, playerSpeed);

    // Ground and platforms collision
    isGrounded = false;
    isGrounded |= PlayerCollision(ground);

    for (int i = 0; i < 10; i++)
    {
        isGrounded |= PlayerCollision(platform[i]);
    }

    if (wantsToJump && isGrounded) Jump(playerJump);

    // Move player
    player.move(velocityX * dt, velocityY * dt);

    // Wall collision
    WallCollision(player);

    // Game Over check: 
    if (player.getPosition().y > camY + windowSize.y / 2 + 150) {
        gameOver = true;
        showGameOver = true;
        gameOverClock.restart();
        gameoverSound.play();
        fallSound.play();

    }

    gamecam();


    for (size_t i = 0; i < 4; i++)
    {
        MoveSprite(&leftWall[i], wallsSpeed, true);
        MoveSprite(&rightWall[i], wallsSpeed, true);
    }

    for (size_t i = 0; i < 3; i++)
        MoveSprite(&backgroundSprite[i], 0.5f, false);

    lastCamY = camY;
    platforms();
    PlayerAnimation();
    if (isPlayerFlipping && particlesClock.getElapsedTime().asSeconds() > 0.1f)
    {
        SpawnParticles(RandomRange(3, 5));
        particlesClock.restart();
    }
    else if (abs(velocityX) > 1.0f && particlesClock.getElapsedTime().asSeconds() > 0.5f) {
        SpawnParticles(RandomRange(3, 5), 0.1f);
        particlesClock.restart();
    }


    for (size_t i = 0; i < particles.size(); i++)
        particles[i].Update(i, particles);
}

// Draw game
void DrawGame() {

    for (size_t i = 0; i < 3; i++)
        window.draw(backgroundSprite[i]);
    if (showPhrase)
        window.draw(phrase);
    if (showgood)
        window.draw(good_ph);
    if (showgreat)
        window.draw(great_ph);
    if (showextreme)
        window.draw(extre_ph);
    for (int i = 0; i < 10; i++)
        window.draw(platform[i]);
    window.draw(ground);
    for (size_t i = 0; i < 4; i++)
    {
        window.draw(leftWall[i]);
        window.draw(rightWall[i]);
    }
    for (size_t i = 0; i < particles.size(); i++)
        window.draw(particles[i].shape);
    window.draw(player);

    window.draw(scoreText);
}

// Main Menu
Event e;
void MainMenu() {
    game_mu.setLoop(true);
    game_mu.setVolume(40);
    credit_music.stop();
    window.setView(window.getDefaultView());
    while (window.pollEvent(e)) {

        if (e.type == Event::Closed) {
            window.close();
        }
        maintext[point].setFillColor(Color::White);
        if (e.type == Event::KeyPressed)
        {

            if (e.key.code == Keyboard::Down) {
                point++; point = point % 3;
                moveSound.play();
            }
            else if (e.key.code == Keyboard::Up)
            {
                point--;
                if (point < 0) { point = 2; }
                moveSound.play();
            }
            if (e.key.code == Keyboard::Enter) {
                if (point == 0) {
                    menuIndex = 1;
                    if (mu) {
                        game_mu.play();
                    }
                }
                if (point == 1) { menuIndex = 3; }
                if (point == 2) { window.close(); }
                enter.play();
                maintext[point].setFillColor(Color::White);
                point = 0;
            }
        }
    }
    finger.setPosition(maintext[point].getPosition().x - maintext[point].getLocalBounds().width / 2.0f - 40, maintext[point].getPosition().y + 20);
    window.clear(Color::White);
    window.draw(mainbackground);
    window.draw(finger);
    maintext[point].setFillColor(Color::Yellow);
    for (int i = 0; i < 3; i++) {
        window.draw(maintext[i]);
    }
    window.display();
}
// settings
void Setting() {
    credit_music.stop();
    game_mu.stop();
    window.setView(window.getDefaultView());

    while (window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            window.close();
        }
        if (e.type == Event::KeyPressed)
        {
            settingtext[point].setFillColor(Color::White);
            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Down) {
                    point++; point = point % 2;
                    moveSound.play();
                }
                else if (e.key.code == Keyboard::Up)
                {
                    point--;
                    if (point < 0) { point = 1; }
                    moveSound.play();
                }
                if (e.key.code == Keyboard::Enter) {
                    if (point == 0) { mu = !mu; }
                    if (point == 1) { menuIndex = 0; }
                    enter.play();
                    settingtext[point].setFillColor(Color::White);
                    point = 0;
                }
            }
        }
        finger.setPosition(settingtext[point].getPosition().x - settingtext[point].getLocalBounds().width / 2.0f - 40, settingtext[point].getPosition().y + 20);
        window.clear();
        window.draw(settingbackground);
        window.draw(finger);
        settingtext[point].setFillColor(Color::Yellow);
        for (int i = 0; i < 2; i++) {
            window.draw(settingtext[i]);
        }
        window.display();

    }
}

void GameOver() {
    game_mu.stop();
    window.setView(window.getDefaultView());
    while (window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            window.close();
        }
        gameoverstext[point].setFillColor(Color::White);
        if (e.type == Event::KeyPressed)
        {

            if (e.key.code == Keyboard::Down) {
                point++; point = point % 3;
                moveSound.play();
            }
            else if (e.key.code == Keyboard::Up)
            {
                point--;
                if (point < 0) { point = 2; }
                moveSound.play();
            }
            if (e.key.code == Keyboard::Enter) {
                if (point == 0) {
                    menuIndex = 1; game_mu.play();
                }
                if (point == 1) { menuIndex = 0; theme_music.play(); }
                enter.play();
                gameoverstext[point].setFillColor(Color::White);
                point = 0;

            }
        }
    }
    finger.setPosition(gameoverstext[point].getPosition().x - gameoverstext[point].getLocalBounds().width / 2.0f - 40, gameoverstext[point].getPosition().y + 20);
    window.clear(Color::White);
    window.draw(gameoverbackground);
    window.draw(finger);
    gameoverstext[point].setFillColor(Color::Yellow);
    for (int i = 0; i < 2; i++) {
        window.draw(gameoverstext[i]);
    }
    window.display();
}


// Game loop
void Game() {
    theme_music.stop();
    credit_music.stop();

    while (window.pollEvent(event)) {
        if (event.type == Event::Closed)
            window.close();
        GameEventLoop(event);
    }

    if (!showGameOver)
        UpdateGame();

    window.clear();
    DrawGame();

    if (showGameOver) {
        gameOverText.setPosition(
            gameCamera.getCenter().x - gameOverText.getGlobalBounds().width / 2,
            gameCamera.getCenter().y - 100
        );
        window.draw(gameOverText);

        if (gameOverClock.getElapsedTime().asSeconds() > 2.5f) {
            Start();   // Restart Game
            menuIndex = 4;
        }
    }

    window.display();
}


// Pause Menu
void PauseMenu() {
    game_mu.stop();
    credit_music.stop();
    window.setView(window.getDefaultView());
    pausetext[0].setFillColor(Color::White);
    pausetext[1].setFillColor(Color::White);
    pausetext[2].setFillColor(Color::White);

    while (window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            window.close();
        }
        pausetext[paupoint].setFillColor(Color::White);
        if (e.type == Event::KeyPressed)
        {
            if (e.type == Event::KeyPressed)
            {
                if (e.key.code == Keyboard::Down) {
                    paupoint++; paupoint = paupoint % 3;
                    moveSound.play();
                }
                else if (e.key.code == Keyboard::Up)
                {
                    paupoint--;
                    if (paupoint < 0) { paupoint = 1; }
                    moveSound.play();
                }
                if (e.key.code == Keyboard::Enter) {
                    if (paupoint == 0) {
                        menuIndex = 1;
                        if (mu) { game_mu.play(); }
                    }
                    if (paupoint == 1) {
                        menuIndex = 5;
                        if (mu) { credit_music.play(); }
                    }
                    if (paupoint == 2) {
                        menuIndex = 0;
                        if (mu) { theme_music.play(); }
                    }
                    enter.play();
                    pausetext[paupoint].setFillColor(Color::White);
                    paupoint = 0;
                    pausetext[paupoint].setFillColor(Color::White);
                    point = 0;
                }
            }
        }
    }
    finger.setPosition(pausetext[paupoint].getPosition().x - pausetext[paupoint].getLocalBounds().width / 2.0f - 40, pausetext[paupoint].getPosition().y + 20);
    window.clear(Color::White);
    window.draw(pausebackground);
    window.draw(finger);
    pausetext[paupoint].setFillColor(Color::Yellow);
    for (int i = 0; i < 3; i++) {
        window.draw(pausetext[i]);
    }
    window.display();
}
// Main
int main() {
    Start();
    menuIndex = 0;
    if (!mu) { theme_music.stop(); }
    else { theme_music.play(); }

    theme_music.setVolume(40);
    while (window.isOpen()) {
        dt = dtClock.restart().asSeconds() * 10;
        switch (menuIndex) {
        case 0: MainMenu();
            break;
        case 1: Game(); break;
        case 2: PauseMenu(); break;
        case 3: Setting(); break;
        case 4: GameOver(); break;
        case 5:Credit(); break;
        default:
            cout << "ERROR: undefined menu index" << endl;
            break;
        }
    }
    return 0;
}