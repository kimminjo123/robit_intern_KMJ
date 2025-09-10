#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateBall();             // 내 공 움직임
    void updateEnemyBall();        // 상대 공 움직임
    void readPendingDatagrams();   // UDP 수신 처리
    void fireBall();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    QGraphicsEllipseItem *ball;       // 빨간 공 (내 공)
    QGraphicsEllipseItem *enemyBall;  // 초록 공 (상대 공)
    QTimer *timer;        // 내 공 타이머
    QTimer *enemyTimer;   // 상대 공 타이머

    QUdpSocket *udpSocket; // UDP 소켓
    QTimer *powerTimer;    // 파워 충전용 타이머
    bool spacePressed;

    // 내 공 변수
    double angle1, angle2, power;
    double vx, vy, vz;
    double x, y, z;
    double t;

    // 상대 공 변수
    double enemyAngle1, enemyAngle2, enemyPower;
    double evx, evy, evz;
    double ex, ey, ez;
    double et;

    // 환경 변수
    double g;
    double wallZ;

    // 시작 위치
    double startX, startY;     // 빨간 공 시작 위치
    double enemyX, enemyY;     // 초록 공 시작 위치

    // 벽 셀
    QGraphicsRectItem* wallCells[3][3];

    int board[3][3];        // 0=미점유, 1=내(빨강), 2=상대(초록)
    bool gameOver;          // 게임 종료 플래그

    void markCell(int row, int col, int owner);
    bool checkBingo(int owner) const;
    void endGame(int winner); // 1=빨강 승, 2=초록 승
};

#endif // MAINWINDOW_H
