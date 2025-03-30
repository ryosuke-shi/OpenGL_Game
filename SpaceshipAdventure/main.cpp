#include <GL/glew.h>               // GLEW（OpenGL拡張機能ライブラリ）
#include <GLFW/glfw3.h>            // GLFW（ウィンドウや入力処理のライブラリ）
#include <glm/glm.hpp>             // GLM（数学ライブラリ）
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include <GL/glut.h>
#include <string>
using namespace std;

// ウィンドウサイズ
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// エンティティ（プレイヤ、隕石など）の構造体
struct Entity {
    glm::vec2 position;   // 位置
    glm::vec2 size;       // サイズ
};

// プレイヤと隕石のリスト
Entity player;
std::vector<Entity> meteors;
int playerSize = 50;

// 隕石の生成に関する変数
float lastMeteorTime = 0.0f;   // 最後に隕石を生成した時間
float spawnInterval = 1.0f;    // 隕石生成の間隔
float spawnSpeedUpInterval = 5.0f;     // 次に生成速度がアップする間隔
float nextSpawnSpeedUpTime = spawnSpeedUpInterval; // 次に生成速度がアップする時間
float speedUpInterval = 10.0f; // 隕石のスピードアップの間隔
float nextSpeedUpTime = speedUpInterval; // 次に隕石がスピードアップする時間
int meteorSpeed = 200; // 隕石のスピード

// ゲームの状態
float survivalTime = 0.0f;     // 生存時間
bool gameOver = false;         // ゲームオーバー状態

// 衝突判定：2つのエンティティが重なっているかどうか
bool checkCollision(const Entity& a, const Entity& b) {
    return (a.position.x < b.position.x + b.size.x &&
        a.position.x + a.size.x > b.position.x &&
        a.position.y < b.position.y + b.size.y &&
        a.position.y + a.size.y > b.position.y);
}

// 隕石の生成関数
void spawnMeteor() {
    // ランダムなX座標で画面上部に隕石を追加
    float x = rand() % (SCREEN_WIDTH - 50);
    meteors.push_back({ glm::vec2(x, SCREEN_HEIGHT), glm::vec2(50, 50) });
}

void renderText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
}

// ゲームの状態更新（毎フレーム）
void update(float deltaTime) {
    if (gameOver) return;  // ゲームオーバー時は処理をスキップ

    survivalTime += deltaTime;  // 生存時間を加算

    // 一定間隔で隕石のスピードアップ
    if (survivalTime > nextSpeedUpTime) {
        meteorSpeed += 50;
        nextSpeedUpTime += speedUpInterval;
        std::cout << "meteorSpeed: " + to_string(meteorSpeed) << endl;
    }

    // 一定間隔で隕石の生成間隔アップ
    if (survivalTime > nextSpawnSpeedUpTime) {
        spawnInterval *= 0.85f;
        nextSpawnSpeedUpTime += spawnSpeedUpInterval;
        std::cout << "spawnInterval: " + to_string(spawnInterval) << endl;
    }

    // 一定時間で隕石を生成
    if (glfwGetTime() - lastMeteorTime > spawnInterval) {
        spawnMeteor();
        lastMeteorTime = glfwGetTime();
    }

    // 隕石を下方向に移動させる
    for (auto& m : meteors) {
        m.position.y -= meteorSpeed * deltaTime;
    }

    // プレイヤと各隕石との衝突をチェック
    for (const auto& m : meteors) {
        if (checkCollision(player, m)) {
            gameOver = true;  // 衝突したらゲームオーバー
        }
    }
}

// プレイヤの操作
void processInput(GLFWwindow* window, float deltaTime) {
    float speed = 300.0f * deltaTime;  // 移動速度

    // ウィンドウの外側に行けないようにする処理
    if (player.position.x < 0) {
        player.position.x = 0;
    }
    if (player.position.x + player.size.x > SCREEN_WIDTH) {
        player.position.x = SCREEN_WIDTH - player.size.x;
    }
    if (player.position.y < 0) {
        player.position.y = 0;
    }
    if (player.position.y + player.size.y > SCREEN_HEIGHT) {
        player.position.y = SCREEN_HEIGHT - player.size.y;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) player.position.y += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) player.position.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) player.position.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) player.position.x += speed;
}

// 描画処理：プレイヤと隕石を画面に描画
void render() {
    glClear(GL_COLOR_BUFFER_BIT);  // 画面をクリア

    glBegin(GL_QUADS);  // 四角形描画の開始

    // 宇宙船を緑色で描画
    glColor3f(0, 1, 0);
    glVertex2f(player.position.x, player.position.y);
    glVertex2f(player.position.x + player.size.x, player.position.y);
    glVertex2f(player.position.x + player.size.x, player.position.y + player.size.y);
    glVertex2f(player.position.x, player.position.y + player.size.y);

    // 隕石を赤色で描画
    glColor3f(1, 0, 0);
    for (const auto& m : meteors) {
        glVertex2f(m.position.x, m.position.y);
        glVertex2f(m.position.x + m.size.x, m.position.y);
        glVertex2f(m.position.x + m.size.x, m.position.y + m.size.y);
        glVertex2f(m.position.x, m.position.y + m.size.y);
    }

    glEnd();  // キャラ描画終了

    // スコア表示
    glColor3f(1.0f, 1.0f, 1.0f);  // 白色
    std::string timeText = "Time: " + std::to_string((int)survivalTime) + "s";
    renderText(10.0f, SCREEN_HEIGHT - 30.0f, timeText);  // 左上に表示
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0)));  // 乱数初期化
    glutInit(&argc, argv); // GLUT初期化
    glfwInit();  // GLFW初期化

    // ウィンドウの作成
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Spaceship Adventure", nullptr, nullptr);
    glfwMakeContextCurrent(window);  // コンテキスト設定
    glewInit();  // GLEW初期化

    // 左下が(0,0)、右上が(SCREEN_WIDTH, SCREEN_HEIGHT)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);

    // プレイヤ初期位置とサイズの設定
    player.position = glm::vec2((SCREEN_WIDTH/2)-(playerSize/2), 50);
    player.size = glm::vec2(playerSize, playerSize);

    float lastFrame = glfwGetTime();  // 前フレームの時間記録

    // メインループ
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();               // 現在の時間
        float deltaTime = currentFrame - lastFrame;       // 経過時間（フレーム間隔）
        lastFrame = currentFrame;

        processInput(window, deltaTime);  // 入力処理
        update(deltaTime);                // 状態更新
        render();                         // 描画処理

        glfwSwapBuffers(window);          // バッファの入れ替え（画面更新）
        glfwPollEvents();                 // イベント処理（キー入力など）

        // ゲームオーバー時の処理
        if (gameOver) {
            std::cout << "Game Over! Survival Time: " << survivalTime << " seconds" << std::endl;
            break;
        }
    }

    glfwTerminate();  // GLFW終了処理
    return 0;
}
